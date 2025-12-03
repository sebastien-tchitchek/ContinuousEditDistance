/// \ingroup base
/// \class ttk::TimeVaryingPersistenceDiagramDistanceMatrix
/// \author Sebastien Tchitchek <sebastien.tchitchek@lip6.fr>
/// \author Julien Tierny <julien.tierny@lip6.fr>
/// \date June 2023
///
/// \b Related \b publication \n
///
///
///
/// 
///
///
///
///
///
///
///   

#include <algorithm>
#include <limits>
#include <cmath>
#include <deque>

#include <TimeVaryingPersistenceDiagramDistanceMatrix.h>
#include <DTWDistance.h>
#include <FrechetDistance.h>
#include <CGEDDistanceMatrix.h>
#include <TWEDDistanceParallel.h>
#include <L2.h>

using namespace ttk;

typedef std::pair<ttk::DiagramType,double> TemporalPersistenceDiagram;
typedef std::vector<TemporalPersistenceDiagram> TemporalPersistenceDiagramTimeSeries;

typedef std::vector<ttk::DiagramType> PersistenceDiagramTimeSeries;

double costMatrixComputation(int &k, int &l,double &weight, std::vector<ttk::DiagramType>& Geodesic1, std::vector<ttk::DiagramType>& Geodesic2,  std::vector<double>& GeodesicTime1, std::vector<double>& GeodesicTime2) 
{
    
                std::vector<ttk::DiagramType> vec2(2);
                vec2[0]=Geodesic1[k];
                vec2[1]=Geodesic2[l];
                ttk::PersistenceDiagramDistanceMatrix MatrixCalculator2;
                std::array<size_t, 2> nInputs{2, 0};
                MatrixCalculator2.setDos(true, true, true); 
                MatrixCalculator2.setThreadNumber(1); 
                std::vector<std::vector<double>> distMatrix = MatrixCalculator2.execute(vec2, nInputs);
                
                double TemporalDistanceBetweenKAndL;

                TemporalDistanceBetweenKAndL = (1-weight)*distMatrix[0][1]+weight*std::abs(GeodesicTime1[k]-GeodesicTime2[l]);
    
                return TemporalDistanceBetweenKAndL;
}

std::vector< std::vector< double > > ttk::TimeVaryingPersistenceDiagramDistanceMatrix::execute(const std::vector< std::vector< std::pair< ttk::DiagramType, double > > >& TemporalPersistenceDiagramTimeSeriesSet, double step, double weight, int chosenDistance, double &beta) const
{
    
    if(beta<0){
        beta=std::abs(beta);
    }
    
    if(beta>1){
        beta = 1;
    }
    
    if(beta==0){
        beta = 0.00001;
    }
    
    //We recover the number of temporal persistence diagram time series in the considered set
    int cardinalOfTemporalPersistenceDiagramTimeSeriesSet = TemporalPersistenceDiagramTimeSeriesSet.size();
        
    std::vector<std::vector<double>> TVDistanceMatrix (cardinalOfTemporalPersistenceDiagramTimeSeriesSet,std::vector<double>(cardinalOfTemporalPersistenceDiagramTimeSeriesSet));    
    
    //We create an instance to contain the set of the approximated geodesics, we create separately another instance to contain all the temporal indices of the approximated geodesics
    std::vector<PersistenceDiagramTimeSeries> GeodesicSet(cardinalOfTemporalPersistenceDiagramTimeSeriesSet);
    std::vector<std::vector<double>> GeodesicTimesSet(cardinalOfTemporalPersistenceDiagramTimeSeriesSet);
        
    if(chosenDistance == 1){
    
        ttk::PersistenceDiagramClustering persistenceDiagramClustering;
        
        //In this loop we will, for each of the temporal diagrams time series of the set, calculate the approximated geodesic according to precision parameter "step"
        #ifdef TTK_ENABLE_OPENMP
        #pragma omp parallel for schedule(dynamic) num_threads(threadNumber_)
        #endif // TTK_ENABLE_OPENMP
        for(int i = 0; i < cardinalOfTemporalPersistenceDiagramTimeSeriesSet; i++) {

            //We consider the i-th temporal persistence diagram time series of the temporal persistence diagram time series set
            TemporalPersistenceDiagramTimeSeries TemporalPersistenceDiagramTimeSeriesNumberi = TemporalPersistenceDiagramTimeSeriesSet[i];

            //We recover the cardinal of the temporal persistence diagram time series considered
            int cardinalOfTemporalPersistenceDiagramTimeSeries = TemporalPersistenceDiagramTimeSeriesNumberi.size();

            //We recover the first temporal persistence diagram of the temporal persistence diagram time series considered
            TemporalPersistenceDiagram TemporarySaveFirstTemporalPersistenceDiagram = TemporalPersistenceDiagramTimeSeriesNumberi.front();

            //We recover the last temporal persistence diagram of the temporal persistence diagram time series considered
            TemporalPersistenceDiagram TemporarySaveLastTemporalPersistenceDiagram = TemporalPersistenceDiagramTimeSeriesNumberi.back();

            //We recover the temporal index of TemporarySaveFirstTemporalPersistenceDiagram
            double depart = TemporarySaveFirstTemporalPersistenceDiagram.second;

            //We recover the temporal index of TemporarySaveLastTemporalPersistenceDiagram
            double fin = TemporarySaveLastTemporalPersistenceDiagram.second;

            // From here_1,...
            std::vector<double> temporalSampling(1,depart);

            int t = 1;

            while(depart+t*step<=fin) {
                
                temporalSampling.push_back(depart+t*step);
                t++;
            }
            // ...to here_1, we calculate and save the temporal sampling of the approximated geodesics for the calculation precision desired by the user (precision chosen thanks to "step")

            //We create an instance to contain the approximated geodesic of the i-th temporal persistence diagram time series, and separately its time indices
            PersistenceDiagramTimeSeries Geodesic;
            std::vector<double> GeodesicTimes;

            //In this loop we calculate the partial approximated geodesic between each consecutive j-th pair of temporal persistence diagram of the i-th time series
            for(int j = 0; j < cardinalOfTemporalPersistenceDiagramTimeSeries-1; j++) {  

                //We create instances to contain the j-th and j+1-th temporal persistence diagrams of the i-th temporal persistence diagram time series
                TemporalPersistenceDiagram Pairj = TemporalPersistenceDiagramTimeSeriesNumberi[j];
                TemporalPersistenceDiagram Pairjplus1 = TemporalPersistenceDiagramTimeSeriesNumberi[j+1];

                //We create a variable to store only the sampling steps, of our i-th approximated geodesic, restricted to the portion between the j-th temporal persistence diagram and the j+1-th
                std::vector<double> concernedTimes;

                // From here_2,...
                int limite = 0;
                
                
                if(j < cardinalOfTemporalPersistenceDiagramTimeSeries-2){
                    
                    for(int k = 0; k<temporalSampling.size(); k++) {

                        if(Pairj.second<=temporalSampling[k] && temporalSampling[k]<Pairjplus1.second) {

                            limite++;
                            concernedTimes.push_back(temporalSampling[k]);

                        }

                    }    
                }
                
                else {
                    
                    for(int k = 0; k<temporalSampling.size(); k++) {

                        if(Pairj.second<=temporalSampling[k] && temporalSampling[k]<=Pairjplus1.second) {

                            limite++;
                            concernedTimes.push_back(temporalSampling[k]);
                        
                        }

                    }
                    
                }
                
                // ... to here_2, we create a positive integer variable, which will be strictly greater than 0 if the portion between the j-th temporal persistence diagram and the j+1-th includes points of the desired temporal sampling chosen by the user


                // If the portion between the j-th and and j+1-th contain one or more temporal sampling of the desired approximated geodesic, we calculate the corresponding persistence diagram at this time points
                if(limite>0) {

                    std::vector<ttk::DiagramType> centroids;
                    std::vector<ttk::DiagramType> intermediateDiagrams{Pairj.first,Pairjplus1.first};
                    std::vector<std::vector<std::vector<ttk::MatchingType>>> allMatchings;

                    std::vector<int> clusterIds = persistenceDiagramClustering.execute(intermediateDiagrams, centroids, allMatchings);

                    std::vector<ttk::MatchingType> branch1 = allMatchings[0][0];

                    ttk::DiagramType ka;
                    ka = centroids.front();
                    
                    for(int k = 0; k < concernedTimes.size(); k++) {

                        ttk::DiagramType BarycenterDiag;

                        double concernedTime = concernedTimes[k];

                        double Time1 = Pairj.second;
                        double Time2 = Pairjplus1.second;

                        double coefficient = 2*std::abs(concernedTime-Time1)/std::abs(Time2-Time1);

                        for(int t = 0; t < branch1.size(); t++) {

                        std::tuple<int, int, double > toto = branch1[t];
                        int a = std::get<0>(toto);
                        int b = std::get<1>(toto);
                       
                        if(a!=-1 && b!=-1) {

                           
                            ttk::PersistencePair ConcernedTimePersistencePair;
                           
                            ttk::CriticalVertex px_b=Pairj.first[a].birth;
                            ttk::CriticalVertex px_d=Pairj.first[a].death;
                            ttk::CriticalVertex pc_b=ka[b].birth;
                            ttk::CriticalVertex pc_d=ka[b].death;

                            const double bx = px_b.sfValue * (1.0 - coefficient) + pc_b.sfValue * coefficient;
                            const double by = px_d.sfValue * (1.0 - coefficient) + pc_d.sfValue * coefficient;
                                                   
                            ttk::CriticalVertex vb = px_b; vb.sfValue = bx;
                            ttk::CriticalVertex vd = px_d; vd.sfValue = by;
                           
                            ConcernedTimePersistencePair.birth = vb;
                            ConcernedTimePersistencePair.death = vd;
                            ConcernedTimePersistencePair.dim   = Pairj.first[a].dim;

                            ConcernedTimePersistencePair.isFinite = (Pairj.first[a].isFinite && ka[b].isFinite);
                           
                            BarycenterDiag.push_back(ConcernedTimePersistencePair);
                           
                        }

                        else if(a==-1 && b!=-1) {

                           
                           
                        ttk::CriticalVertex pc_b=ka[b].birth;
                        ttk::CriticalVertex pc_d=ka[b].death;

                        const double m = 0.5 * (pc_b.sfValue + pc_d.sfValue);
                        const double bx = m * (1.0 - coefficient) + pc_b.sfValue * coefficient;
                        const double by = m * (1.0 - coefficient) + pc_d.sfValue * coefficient;

                        ttk::PersistencePair p{};

                        ttk::CriticalVertex vb = pc_b; vb.sfValue = bx;
                        ttk::CriticalVertex vd = pc_d; vd.sfValue = by;

                        p.birth = vb;
                        p.death = vd;
                        p.dim   = ka[b].dim;

                        p.isFinite = ka[b].isFinite;
                           
                            if(bx != by){
                                BarycenterDiag.push_back(p);
                            }

                        }

                    }

                        GeodesicTimes.push_back(concernedTime);
                        Geodesic.push_back(BarycenterDiag);

                    }

                }

            }

            GeodesicTimesSet[i]=GeodesicTimes;
            GeodesicSet[i]=Geodesic;

        }
        
        
        for(int i =0; i < GeodesicSet.size(); i++) {
            GeodesicSet[i].pop_back();
            GeodesicTimesSet[i].pop_back();
        }
        
    }

    //We calculate in this loop, for each input pair, the chosen distance
    for(int i =0; i < cardinalOfTemporalPersistenceDiagramTimeSeriesSet; i++) {

        for(int j = i+1; j<cardinalOfTemporalPersistenceDiagramTimeSeriesSet; j++) {
            
            if(chosenDistance == 1) {

                std::vector<std::vector<double>> costMatrix(GeodesicSet[i].size(), std::vector<double>(GeodesicSet[j].size()));
                    
                std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[i];
                std::vector<double> TimesOfjthGeodesic = GeodesicTimesSet[j];
                    
                std::vector<std::vector<int>> parameterList;
                
                for(int k = 0; k<GeodesicSet[i].size(); k++) {

                    for(int l = 0; l<GeodesicSet[j].size(); l++) { 

                        std::vector<int> toFillParameterList(2);
                                    
                        toFillParameterList[0] = k;
                        toFillParameterList[1] = l;
                                    
                        parameterList.push_back(toFillParameterList);

                    }

                }

                #ifdef TTK_ENABLE_OPENMP
                #pragma omp parallel for schedule(dynamic) num_threads(threadNumber_)
                #endif
                for(size_t t = 0; t < parameterList.size(); t++){
                                
                    int k = -1;
                    int l = -1;
                                
                    k = parameterList[t][0];
                    l = parameterList[t][1];
                                
                    costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[i], GeodesicSet[j], TimesOfithGeodesic, TimesOfjthGeodesic);
                                
                }

                std::vector<std::vector<int>> unusedMatchings(0);
                std::vector<double> unusedMatchingsCost(0);
                double distanceBetweenCurves = -1;
                std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                 
                CGEDDistanceMatrix(weight, step, costMatrix, GeodesicSet[i], GeodesicSet[j], false, false,  unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded, beta);                
             
                TVDistanceMatrix[i][j]= distanceBetweenCurves;
                TVDistanceMatrix[j][i]= TVDistanceMatrix[i][j];

            }
    

            else if(chosenDistance == 2) {
 
                   
                TemporalPersistenceDiagramTimeSeries TemporaryTemporalPersistenceDiagramTimeSeriesNumberi = TemporalPersistenceDiagramTimeSeriesSet[i];
                TemporalPersistenceDiagramTimeSeries TemporaryTemporalPersistenceDiagramTimeSeriesNumberj = TemporalPersistenceDiagramTimeSeriesSet[j];
                    
                std::vector<ttk::DiagramType> TemporaryPersistenceDiagramTimeSeriesNumberi(0);
                std::vector<ttk::DiagramType> TemporaryPersistenceDiagramTimeSeriesNumberj(0);
                    
                std::vector<double> TimesOfithGeodesic(0); 
                std::vector<double> TimesOfjthGeodesic(0); 
                    
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberi.size();k++){
                    
                    TemporaryPersistenceDiagramTimeSeriesNumberi.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberi[k].first);
                        
                }
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberi.size();k++){
                    
                    TimesOfithGeodesic.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberi[k].second);
                        
                }
                
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberj.size();k++){
                        
                    TemporaryPersistenceDiagramTimeSeriesNumberj.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberj[k].first);
                }
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberj.size();k++){
                        
                    TimesOfjthGeodesic.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberj[k].second);
                }
                   
                std::vector<std::vector<double>> costMatrix(TemporaryPersistenceDiagramTimeSeriesNumberi.size(), std::vector<double>(TemporaryPersistenceDiagramTimeSeriesNumberj.size()));
                        
                std::vector<std::vector<int>> parameterList;
                    
                for(int k = 0; k<TemporaryPersistenceDiagramTimeSeriesNumberi.size(); k++) {

                    for(int l = 0; l<TemporaryPersistenceDiagramTimeSeriesNumberj.size(); l++) { 

                        std::vector<int> toFillParameterList(2);
                                        
                        toFillParameterList[0] = k;
                        toFillParameterList[1] = l;
                                        
                        parameterList.push_back(toFillParameterList);

                    }

                }
                        
                #ifdef TTK_ENABLE_OPENMP
                #pragma omp parallel for schedule(dynamic) num_threads(threadNumber_)
                #endif
                for(size_t t = 0; t < parameterList.size(); t++){
                                    
                        int k = -1;
                        int l = -1;
                                    
                        k = parameterList[t][0];
                        l = parameterList[t][1];
                                    
                        costMatrix[k][l] = costMatrixComputation(k, l, weight, TemporaryPersistenceDiagramTimeSeriesNumberi, TemporaryPersistenceDiagramTimeSeriesNumberj, TimesOfithGeodesic, TimesOfjthGeodesic);
                                    
                }

                double resultDTW(-1);
                    
                DTWDistance(TemporaryPersistenceDiagramTimeSeriesNumberi.size(), TemporaryPersistenceDiagramTimeSeriesNumberj.size(),costMatrix, resultDTW);
                    
                TVDistanceMatrix[i][j]= resultDTW;
                TVDistanceMatrix[j][i]= TVDistanceMatrix[i][j];
                    
            }
             
        
            else if(chosenDistance == 3) {
                    
                TemporalPersistenceDiagramTimeSeries TemporaryTemporalPersistenceDiagramTimeSeriesNumberi = TemporalPersistenceDiagramTimeSeriesSet[i];
                TemporalPersistenceDiagramTimeSeries TemporaryTemporalPersistenceDiagramTimeSeriesNumberj = TemporalPersistenceDiagramTimeSeriesSet[j];
                    
                std::vector<ttk::DiagramType> TemporaryPersistenceDiagramTimeSeriesNumberi(0);
                std::vector<ttk::DiagramType> TemporaryPersistenceDiagramTimeSeriesNumberj(0);
                    
                std::vector<double> TimesOfithGeodesic(0); 
                std::vector<double> TimesOfjthGeodesic(0); 
                    
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberi.size();k++){
                    
                    TemporaryPersistenceDiagramTimeSeriesNumberi.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberi[k].first);
                        
                }
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberi.size();k++){
                    
                    TimesOfithGeodesic.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberi[k].second);
                        
                }
                    
                    
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberj.size();k++){
                        
                    TemporaryPersistenceDiagramTimeSeriesNumberj.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberj[k].first);
                }
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberj.size();k++){
                        
                    TimesOfjthGeodesic.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberj[k].second);
                }
                    
                   
                std::vector<std::vector<double>> costMatrix(TemporaryPersistenceDiagramTimeSeriesNumberi.size(), std::vector<double>(TemporaryPersistenceDiagramTimeSeriesNumberj.size()));
                        
                std::vector<std::vector<int>> parameterList;
                    
                for(int k = 0; k<TemporaryPersistenceDiagramTimeSeriesNumberi.size(); k++) {

                    for(int l = 0; l<TemporaryPersistenceDiagramTimeSeriesNumberj.size(); l++) { 

                        std::vector<int> toFillParameterList(2);
                                        
                        toFillParameterList[0] = k;
                        toFillParameterList[1] = l;
                                        
                        parameterList.push_back(toFillParameterList);

                    }

                }
                        
                        
                #ifdef TTK_ENABLE_OPENMP
                #pragma omp parallel for schedule(dynamic) num_threads(threadNumber_)
                #endif
                for(size_t t = 0; t < parameterList.size(); t++){
                                    
                        int k = -1;
                        int l = -1;
                                    
                        k = parameterList[t][0];
                        l = parameterList[t][1];
                                    
                        costMatrix[k][l] = costMatrixComputation(k, l, weight, TemporaryPersistenceDiagramTimeSeriesNumberi, TemporaryPersistenceDiagramTimeSeriesNumberj, TimesOfithGeodesic, TimesOfjthGeodesic);
                                    
                }

                double resultFrechet(-1);
                    
                FrechetDistance(TemporaryPersistenceDiagramTimeSeriesNumberi.size(), TemporaryPersistenceDiagramTimeSeriesNumberj.size(),costMatrix, resultFrechet);
                    
                TVDistanceMatrix[i][j]= resultFrechet;
                TVDistanceMatrix[j][i]= TVDistanceMatrix[i][j];

            }


            else if(chosenDistance == 4) {
                
                
                TemporalPersistenceDiagramTimeSeries TemporaryTemporalPersistenceDiagramTimeSeriesNumberi = TemporalPersistenceDiagramTimeSeriesSet[i];
                TemporalPersistenceDiagramTimeSeries TemporaryTemporalPersistenceDiagramTimeSeriesNumberj = TemporalPersistenceDiagramTimeSeriesSet[j];
                    
                std::vector<ttk::DiagramType> TemporaryPersistenceDiagramTimeSeriesNumberi(0);
                std::vector<ttk::DiagramType> TemporaryPersistenceDiagramTimeSeriesNumberj(0);
                    
                std::vector<double> TimesOfithGeodesic(0); 
                std::vector<double> TimesOfjthGeodesic(0); 
                    
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberi.size();k++){
                    
                    TemporaryPersistenceDiagramTimeSeriesNumberi.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberi[k].first);
                        
                }
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberi.size();k++){
                    
                    TimesOfithGeodesic.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberi[k].second);
                        
                }
                
                    
                
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberj.size();k++){
                        
                    TemporaryPersistenceDiagramTimeSeriesNumberj.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberj[k].first);
                }
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberj.size();k++){
                        
                    TimesOfjthGeodesic.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberj[k].second);
                }
                
                   
                std::vector<std::vector<double>> costMatrix(TemporaryPersistenceDiagramTimeSeriesNumberi.size(), std::vector<double>(TemporaryPersistenceDiagramTimeSeriesNumberj.size()));
                        
                std::vector<std::vector<int>> parameterList;
                    
                for(int k = 0; k<TemporaryPersistenceDiagramTimeSeriesNumberi.size(); k++) {

                    for(int l = 0; l<TemporaryPersistenceDiagramTimeSeriesNumberj.size(); l++) { 

                        std::vector<int> toFillParameterList(2);
                                        
                        toFillParameterList[0] = k;
                        toFillParameterList[1] = l;
                                        
                        parameterList.push_back(toFillParameterList);

                    }

                }
                        
                
                #ifdef TTK_ENABLE_OPENMP
                #pragma omp parallel for schedule(dynamic) num_threads(threadNumber_)
                #endif
                for(size_t t = 0; t < parameterList.size(); t++){
                                    
                        int k = -1;
                        int l = -1;
                                    
                        k = parameterList[t][0];
                        l = parameterList[t][1];
                                    
                        costMatrix[k][l] = costMatrixComputation(k, l, weight, TemporaryPersistenceDiagramTimeSeriesNumberi, TemporaryPersistenceDiagramTimeSeriesNumberj, TimesOfithGeodesic, TimesOfjthGeodesic);
                                    
                }
                
                double resultTWED(-1);
                
                TWEDDistance2(weight, step, costMatrix, TemporaryPersistenceDiagramTimeSeriesNumberi, TemporaryPersistenceDiagramTimeSeriesNumberj, TimesOfithGeodesic, TimesOfjthGeodesic, resultTWED, threadNumber_);
                
                TVDistanceMatrix[i][j]=resultTWED;
                TVDistanceMatrix[j][i]=TVDistanceMatrix[i][j];
                
            }
            
            else if(chosenDistance == 5) {
                
                TemporalPersistenceDiagramTimeSeries TemporaryTemporalPersistenceDiagramTimeSeriesNumberi = TemporalPersistenceDiagramTimeSeriesSet[i];
                TemporalPersistenceDiagramTimeSeries TemporaryTemporalPersistenceDiagramTimeSeriesNumberj = TemporalPersistenceDiagramTimeSeriesSet[j];
                    
                std::vector<ttk::DiagramType> TemporaryPersistenceDiagramTimeSeriesNumberi(0);
                std::vector<ttk::DiagramType> TemporaryPersistenceDiagramTimeSeriesNumberj(0);
                    
                std::vector<double> TimesOfithGeodesic(0); 
                std::vector<double> TimesOfjthGeodesic(0); 
                    
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberi.size();k++){
                    
                    TemporaryPersistenceDiagramTimeSeriesNumberi.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberi[k].first);
                        
                }
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberi.size();k++){
                    
                    TimesOfithGeodesic.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberi[k].second);
                        
                }
                    
                    
                
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberj.size();k++){
                        
                    TemporaryPersistenceDiagramTimeSeriesNumberj.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberj[k].first);
                }
                    
                for(int k =0;k<TemporaryTemporalPersistenceDiagramTimeSeriesNumberj.size();k++){
                        
                    TimesOfjthGeodesic.push_back(TemporaryTemporalPersistenceDiagramTimeSeriesNumberj[k].second);
                }
                    
                   
                std::vector<std::vector<double>> costMatrix(TemporaryPersistenceDiagramTimeSeriesNumberi.size(), std::vector<double>(TemporaryPersistenceDiagramTimeSeriesNumberj.size()));
                        
                std::vector<std::vector<int>> parameterList;
                    
                for(int k = 0; k<TemporaryPersistenceDiagramTimeSeriesNumberi.size(); k++) {

                    for(int l = 0; l<TemporaryPersistenceDiagramTimeSeriesNumberj.size(); l++) { 

                        std::vector<int> toFillParameterList(2);
                                        
                        toFillParameterList[0] = k;
                        toFillParameterList[1] = l;
                                        
                        parameterList.push_back(toFillParameterList);

                    }

                }
                        
                        
                #ifdef TTK_ENABLE_OPENMP
                #pragma omp parallel for schedule(dynamic) num_threads(threadNumber_)
                #endif
                for(size_t t = 0; t < parameterList.size(); t++){
                                    
                        int k = -1;
                        int l = -1;
                                    
                        k = parameterList[t][0];
                        l = parameterList[t][1];
                                    
                        costMatrix[k][l] = costMatrixComputation(k, l, weight, TemporaryPersistenceDiagramTimeSeriesNumberi, TemporaryPersistenceDiagramTimeSeriesNumberj, TimesOfithGeodesic, TimesOfjthGeodesic);
                                    
                }

                double resultL2(-1);
                    
                if(TemporaryPersistenceDiagramTimeSeriesNumberi.size() != TemporaryPersistenceDiagramTimeSeriesNumberj.size()){
                    std::cout<<"For a L2 distance the sizes of the different vineyards must be identical, this is not the case here."<<std::endl;
                }
                
                
                L2(TemporaryPersistenceDiagramTimeSeriesNumberi.size(), TemporaryPersistenceDiagramTimeSeriesNumberj.size(),costMatrix, resultL2);
                    
                TVDistanceMatrix[i][j]= resultL2;
                TVDistanceMatrix[j][i]= TVDistanceMatrix[i][j];
                    


                
            }
    
        }
        
    }
    



    
    return TVDistanceMatrix;
}
