/// \ingroup base
/// \class ttk::TimeVaryingPersistenceDiagramClustering
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
#include <iterator>
#include <random>
#include <chrono>
#include <iostream>
#include <TimeVaryingPersistenceDiagramClustering.h>
#include <CGEDDistance.h>
#include <CGEDDistanceWithBeta.h>
#include <ExplicitTriangulation.h>
#include <ScalarFieldSmoother.h>
#include <Triangulation.h>
using namespace ttk;

typedef std::pair<ttk::DiagramType,double> TemporalPersistenceDiagram;
typedef std::vector<TemporalPersistenceDiagram> TemporalPersistenceDiagramTimeSeries;

typedef std::vector<ttk::DiagramType> PersistenceDiagramTimeSeries;

inline std::uint32_t &globalSeedTVPD() {
  static std::uint32_t seed{std::random_device{}()};
  return seed;
}

inline std::mt19937 &globalRngTVPD() {
  static std::mt19937 engine{globalSeedTVPD()};
  return engine;
}

inline void setGlobalSeedTVPD(std::uint32_t seed) {
  globalSeedTVPD()  = seed;
  globalRngTVPD().seed(seed);
}

static int farthestPoint(const std::vector<double> &nearestDist2) {
  return static_cast<int>(std::distance(
      nearestDist2.begin(),
      std::max_element(nearestDist2.begin(), nearestDist2.end())));
}

void swapPairsInPlace(std::vector<std::vector<int>> &vv) {
  for (auto &p : vv) {
    assert(p.size() == 2);
    std::swap(p[0], p[1]);
  }
}

double costMatrixComputation(int k, int l,double &weight, std::vector<ttk::DiagramType>& Geodesic1, std::vector<ttk::DiagramType>& Geodesic2,  std::vector<double>& GeodesicTime1, std::vector<double>& GeodesicTime2)
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

void distanceOfTargetFromSet(double &step, double &weight, std::vector<PersistenceDiagramTimeSeries> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, PersistenceDiagramTimeSeries &TargetGreedyBarycenter, std::vector<double> &TargetGreedyTimeBarycenter, int &threadNumber, int &closerIndexInSet, double &distanceToTheNearest, std::vector<std::vector<int>> &matchingsWithTheNearest, std::vector<double> &matchingsCostWithTheNearest, bool &withMatchingsOrNot, std::vector<double> &matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest)
{
     
   
    std::vector<std::vector<int>> bestMatchings(0);
    std::vector<double> bestMatchingsCost(0);
    std::vector<double> bestMatchingsCostsWithDiagonalCostsNotIncluded(0);
   
    double bestMin(-1);
    int indiceOfBestMin(-1);
     
    for(int z =0; z < GeodesicSet.size(); z++) {

        std::vector<std::vector<double>> costMatrix(GeodesicSet[z].size(), std::vector<double>(TargetGreedyBarycenter.size()));
                               
        std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[z];
        std::vector<double> TimesOfjthGeodesic = TargetGreedyTimeBarycenter;
                               
        double lengthOftheGeodesic(0);
                               
        std::vector<std::vector<int>> parameterList;
       
        for(int k = 0; k<GeodesicSet[z].size(); k++) {

            for(int l = 0; l<TargetGreedyBarycenter.size(); l++) {

                std::vector<int> toFillParameterList(2);
                                       
                toFillParameterList[0] = k;
                toFillParameterList[1] = l;
                                       
                parameterList.push_back(toFillParameterList);

            }

        }
                                   
        #ifdef TTK_ENABLE_OPENMP
        #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
        #endif
        for(size_t t = 0; t < parameterList.size(); t++){
                                   
            int k = -1;
            int l = -1;
                                   
            k = parameterList[t][0];
            l = parameterList[t][1];
                                   
            costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[z], TargetGreedyBarycenter, TimesOfithGeodesic, TimesOfjthGeodesic);
                                   
        }
                               
        std::vector<std::vector<int>> matchings(0);
        std::vector<double> matchingsCost(0);
        double distanceBetweenCurves = -1;
        std::vector<double> matchingsCostsWithDiagonalCostsNotIncluded(0);
               
        if(withMatchingsOrNot == true){
            CGEDDistance(weight, step, costMatrix, GeodesicSet[z], TargetGreedyBarycenter, true, true, matchings, matchingsCost, distanceBetweenCurves, matchingsCostsWithDiagonalCostsNotIncluded);
        }
        else{
            CGEDDistance(weight, step, costMatrix, GeodesicSet[z], TargetGreedyBarycenter, false, false, matchings, matchingsCost, distanceBetweenCurves, matchingsCostsWithDiagonalCostsNotIncluded);
        }
                               
        lengthOftheGeodesic = distanceBetweenCurves;
                               
                               
        if(z==0){
                                   
            bestMin = lengthOftheGeodesic;
            indiceOfBestMin = 0;
           
            if(withMatchingsOrNot == true){
                bestMatchings = matchings;
                bestMatchingsCost = matchingsCost;
                bestMatchingsCostsWithDiagonalCostsNotIncluded = matchingsCostsWithDiagonalCostsNotIncluded;
            }
        }
        else if(lengthOftheGeodesic<bestMin){
                                   
            bestMin = lengthOftheGeodesic;
            indiceOfBestMin = z;
           
            if(withMatchingsOrNot == true){
                bestMatchings = matchings;
                bestMatchingsCost = matchingsCost;
                bestMatchingsCostsWithDiagonalCostsNotIncluded = matchingsCostsWithDiagonalCostsNotIncluded;
            }

        }
                               
    }
   
    closerIndexInSet = indiceOfBestMin;
    distanceToTheNearest = bestMin;
   
    if(withMatchingsOrNot == true){
        matchingsWithTheNearest = bestMatchings;
        matchingsCostWithTheNearest = bestMatchingsCost;
        matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest = bestMatchingsCostsWithDiagonalCostsNotIncluded;
    }
   
}


void distanceOfIndicesiFromIndicesj(double &step, double &weight, std::vector<PersistenceDiagramTimeSeries> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, std::vector<double> &scoreVector, std::vector<int> &indicesOfFirstSet, std::vector<int> &indicesOfSecondSet, int &threadNumber)
{
   
   
    for(int i = 0; i < indicesOfSecondSet.size(); i++){
       
                double minimaOfi(0);
                std::vector<double> distancesOfi(0);
                for(int z = 0; z < indicesOfFirstSet.size(); z++) {


                    std::vector<std::vector<double>> costMatrix(GeodesicSet[indicesOfFirstSet[z]].size(), std::vector<double>(GeodesicSet[indicesOfSecondSet[i]].size()));
                   
                    std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[indicesOfFirstSet[z]];
                    std::vector<double> TimesOfjthGeodesic = GeodesicTimesSet[indicesOfSecondSet[i]];
                    double lengthOftheGeodesic(0);
                   
                   
                    std::vector<std::vector<int>> parameterList;
       
                    for(int k = 0; k<GeodesicSet[indicesOfFirstSet[z]].size(); k++) {

                        for(int l = 0; l<GeodesicSet[indicesOfSecondSet[i]].size(); l++) {

                            std::vector<int> toFillParameterList(2);
                           
                            toFillParameterList[0] = k;
                            toFillParameterList[1] = l;
                           
                            parameterList.push_back(toFillParameterList);

                        }

                    }
                       
                    #ifdef TTK_ENABLE_OPENMP
                    #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
                    #endif
                    for(size_t t = 0; t < parameterList.size(); t++){
                       
                        int k = -1;
                        int l = -1;
                       
                        k = parameterList[t][0];
                        l = parameterList[t][1];
                       
                        costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[indicesOfFirstSet[z]], GeodesicSet[indicesOfSecondSet[i]], TimesOfithGeodesic, TimesOfjthGeodesic);
                       
                    }
                   
                    std::vector<std::vector<int>> unusedMatchings(0);
                    std::vector<double> unusedMatchingsCost(0);
                    double distanceBetweenCurves = -1;
                    std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                   
               
                    CGEDDistance(weight, step, costMatrix, GeodesicSet[indicesOfFirstSet[z]], GeodesicSet[indicesOfSecondSet[i]], false, false, unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);
                   
                    lengthOftheGeodesic = distanceBetweenCurves;
                   
                    distancesOfi.push_back(lengthOftheGeodesic);
                   

                   
            }
    minimaOfi = *std::min_element(distancesOfi.begin(), distancesOfi.end());
    scoreVector.push_back(minimaOfi);
   
    }
   
}


void intraSampleDistanceOrder(double &step, double &weight, std::vector<PersistenceDiagramTimeSeries> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, std::vector<double> &scoreVector)
{
   
   
    for(int i = 0; i < GeodesicSet.size(); i++){
       
                double sumActualMinimum(0);
                for(int z = 0; z < GeodesicSet.size(); z++) {// verifier boucle correcte


                    std::vector<std::vector<double>> costMatrix(GeodesicSet[z].size(), std::vector<double>(GeodesicSet[i].size()));
                   
                    std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[z];
                    std::vector<double> TimesOfjthGeodesic = GeodesicTimesSet[i];
                    double lengthOftheGeodesic(0);
                   
                    for(int k = 0; k<GeodesicSet[z].size(); k++) {

                        for(int l = 0; l<GeodesicSet[i].size(); l++) { //We also calculate on the diagonal for verification

                            std::vector<ttk::DiagramType> vec2(2);
                            vec2[0]=GeodesicSet[z][k];
                            vec2[1]=GeodesicSet[i][l];
                            ttk::PersistenceDiagramDistanceMatrix MatrixCalculator2;
                            std::array<size_t, 2> nInputs{2, 0};
                            MatrixCalculator2.setDos(true, true, true);
                            MatrixCalculator2.setThreadNumber(3);
                            std::vector<std::vector<double>> distMatrix = MatrixCalculator2.execute(vec2, nInputs);

                            costMatrix[k][l] = (1-weight)*distMatrix[0][1]+weight*std::abs(TimesOfithGeodesic[k]-TimesOfjthGeodesic[l]);

                        }

                    }
                   
                    std::vector<std::vector<int>> unusedMatchings(0);
                    std::vector<double> unusedMatchingsCost(0);
                    double distanceBetweenCurves = -1;
                    std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                   
                    CGEDDistance(weight, step, costMatrix, GeodesicSet[z], GeodesicSet[i], false, false, unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);
                   
                    lengthOftheGeodesic = distanceBetweenCurves;
                    sumActualMinimum = sumActualMinimum+(lengthOftheGeodesic*lengthOftheGeodesic);
                   

                   
            }
           
    scoreVector[i]=sumActualMinimum;
   
    }
   
}

void geodesicComputation(double step, double weight, std::vector<ttk::DiagramType>& TargetGeodesic, std::vector<double>& TargetTimeGeodesic, double geodesicCoefficient, std::vector<ttk::DiagramType>& Geodesic1, std::vector<ttk::DiagramType>& Geodesic2, std::vector<double>& GeodesicTime1, std::vector<double>& GeodesicTime2, int &threadNumber)
{
   
        std::vector<PersistenceDiagramTimeSeries> GeodesicSet;
        std::vector<std::vector<double>> GeodesicTimesSet;
           
        GeodesicTimesSet.push_back(GeodesicTime1);
        GeodesicSet.push_back(Geodesic1);

        GeodesicTimesSet.push_back(GeodesicTime2);
        GeodesicSet.push_back(Geodesic2);
       
        double lengthOftheGeodesic;
           
        std::vector<std::vector<double>> costMatrix(GeodesicSet[0].size(), std::vector<double>(GeodesicSet[1].size()));
   
        std::vector<std::vector<int>> parameterList;
       
        for(int k = 0; k<GeodesicSet[0].size(); k++) {

            for(int l = 0; l<GeodesicSet[1].size(); l++) {

                std::vector<int> toFillParameterList(2);
               
                toFillParameterList[0] = k;
                toFillParameterList[1] = l;
               
                parameterList.push_back(toFillParameterList);

            }

        }
           
        #ifdef TTK_ENABLE_OPENMP
        #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
        #endif
        for(size_t t = 0; t < parameterList.size(); t++){
           
            int k = -1;
            int l = -1;
           
            k = parameterList[t][0];
            l = parameterList[t][1];
           
            costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[0], GeodesicSet[1], GeodesicTimesSet[0], GeodesicTimesSet[1]);
           
        }
           
    std::vector<std::vector<int>> Matching;
    std::vector<double> unusedMatchingsCost(0);
    double distanceBetweenCurves = -1;
    std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                   
    CGEDDistance(weight, step, costMatrix, GeodesicSet[0], GeodesicSet[1], true, true, Matching, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);
           
    lengthOftheGeodesic = distanceBetweenCurves;
   
   
    std::vector<ttk::DiagramType> GeodesicStep1;
    std::vector<double> GeodesicTimeStep1;
   
    for(int i = 0; i<Matching.size();i++){
   
       
                if(Matching[i][0] != -1 && Matching[i][1] != -1){
                   
                    GeodesicStep1.push_back(GeodesicSet[0][Matching[i][0]-1]);
                    GeodesicTimeStep1.push_back(GeodesicTimesSet[0][Matching[i][0]-1]);
                }
       
    }
   
   
    double distanceGeo0Step1;
   
    std::vector<std::vector<double>> costMatrixGeo0Step1(GeodesicSet[0].size(), std::vector<double>(GeodesicStep1.size()));
   
    std::vector<std::vector<int>> parameterList2;
       
    for(int k = 0; k<GeodesicSet[0].size(); k++) {

        for(int l = 0; l<GeodesicStep1.size(); l++) {

            std::vector<int> toFillParameterList(2);
               
            toFillParameterList[0] = k;
            toFillParameterList[1] = l;
               
            parameterList2.push_back(toFillParameterList);

        }

    }
   
   
    #ifdef TTK_ENABLE_OPENMP
    #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
    #endif
    for(size_t t = 0; t < parameterList2.size(); t++){
           
        int k = -1;
        int l = -1;
           
        k = parameterList2[t][0];
        l = parameterList2[t][1];
   
        costMatrixGeo0Step1[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[0], GeodesicStep1, GeodesicTimesSet[0], GeodesicTimeStep1);
           
    }    
   
    std::vector<std::vector<int>> unusedMatchings2(0);
    std::vector<double> unusedMatchingsCost2(0);
    double distanceBetweenCurves2 = -1;
    std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded2(0);
   
    CGEDDistance(weight, step, costMatrixGeo0Step1, GeodesicSet[0], GeodesicStep1, false, false, unusedMatchings2, unusedMatchingsCost2, distanceBetweenCurves2, unusedMatchingsCostsWithDiagonalCostsNotIncluded2);
   
    distanceGeo0Step1=distanceBetweenCurves2;
   
    if(geodesicCoefficient*lengthOftheGeodesic<=distanceGeo0Step1){
       
        double coefficientGeodesicStep1 = geodesicCoefficient*lengthOftheGeodesic/distanceGeo0Step1;
       
        for(int i = 0; i<Matching.size();i++){  
               

                        if(Matching[i][0] != -1 && Matching[i][1] != -1){
                           
                            TargetGeodesic.push_back(GeodesicSet[0][Matching[i][0]-1]);
                            TargetTimeGeodesic.push_back(GeodesicTimesSet[0][Matching[i][0]-1]);
                           
                           
                        }else if(Matching[i][0] != -1 && Matching[i][1] == -1){
                           
                            ttk::PersistenceDiagramClustering persistenceDiagramClustering2;
                           
                            std::vector<ttk::DiagramType> centroids;
           
                            ttk::DiagramType neutre;
           
                            std::vector<ttk::DiagramType> intermediateDiagrams{GeodesicSet[0][Matching[i][0]-1],neutre};
                            std::vector<std::vector<std::vector<ttk::MatchingType>>> allMatchings;

                            std::pair<ttk::DiagramType,double> Pairj;
                            std::pair<ttk::DiagramType,double> Pairjplus1;
           
                            Pairj.first=GeodesicSet[0][Matching[i][0]-1];
                            Pairjplus1.first=neutre;
                           
                            Pairj.second=GeodesicTimesSet[0][Matching[i][0]-1];
                            Pairjplus1.second=GeodesicTimesSet[0][Matching[i][0]-1];
                           
                            TargetTimeGeodesic.push_back(Pairj.second);
                           

                            std::vector<int> clusterIds = persistenceDiagramClustering2.execute(intermediateDiagrams, centroids, allMatchings);
                            ttk::DiagramType ka;
                            ka = centroids.front();
                            std::vector<ttk::MatchingType> branch1 = allMatchings[0][0];
                   
                            ttk::DiagramType BarycenterDiag;

                            double coefficient = 2*coefficientGeodesicStep1;

                            for(int t = 0; t < branch1.size(); t++) {

                                std::tuple<int, int, double > toto = branch1[t];
                                int a = std::get<0>(toto);
                                int b = std::get<1>(toto);

                                if(a!=-1 && b!=-1) {
                                    
                                    ttk::CriticalVertex px_b = Pairj.first[a].birth;
                                    ttk::CriticalVertex px_d = Pairj.first[a].death;
                                    ttk::CriticalVertex pc_b = ka[b].birth;
                                    ttk::CriticalVertex pc_d = ka[b].death;

                                    const double bx = px_b.sfValue * (1.0 - coefficient) + pc_b.sfValue * coefficient;
                                    const double by = px_d.sfValue * (1.0 - coefficient) + pc_d.sfValue * coefficient;
                                    
                                    ttk::PersistencePair p{};

                                    auto vb = px_b; vb.sfValue = bx;
                                    auto vd = px_d; vd.sfValue = by;
                                    
                                    p.birth = vb;
                                    p.death = vd;
                                    p.dim   = Pairj.first[a].dim;

                                    p.isFinite = (Pairj.first[a].isFinite && ka[b].isFinite);
                                   
                                    if(bx != by){
                                        BarycenterDiag.push_back(p);
                                    }

                                }else if(a==-1 && b!=-1) { //cannot enter in this condition normally
                                
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

                            TargetGeodesic.push_back(BarycenterDiag);

                        }                
            }
           
    }else{
       
       
       
        std::vector<ttk::DiagramType> GeodesicStep2;
        std::vector<double> GeodesicTimeStep2;
   
        for(int i = 0; i<Matching.size();i++){
   
       
                if(Matching[i][0] != -1 && Matching[i][1] != -1){
                   
                    GeodesicStep2.push_back(GeodesicSet[1][Matching[i][1]-1]);
                    GeodesicTimeStep2.push_back(GeodesicTimesSet[1][Matching[i][1]-1]);
                   
                }
       
        }
       
       
           
        double distanceStep1Step2;
       
        std::vector<std::vector<double>> costMatrixStep1Step2(GeodesicStep1.size(), std::vector<double>(GeodesicStep2.size()));
           
        std::vector<std::vector<int>> parameterList3;
           
        for(int k = 0; k<GeodesicStep1.size(); k++) {

            for(int l = 0; l<GeodesicStep2.size(); l++) {

                std::vector<int> toFillParameterList(2);
                   
                toFillParameterList[0] = k;
                toFillParameterList[1] = l;
                   
                parameterList3.push_back(toFillParameterList);

            }

        }
   
       
        #ifdef TTK_ENABLE_OPENMP
        #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
        #endif
        for(size_t t = 0; t < parameterList3.size(); t++){
               
            int k = -1;
            int l = -1;
               
            k = parameterList3[t][0];
            l = parameterList3[t][1];
       
            costMatrixStep1Step2[k][l] = costMatrixComputation(k, l, weight, GeodesicStep1, GeodesicStep2, GeodesicTimeStep1, GeodesicTimeStep2);
               
        }
       
        std::vector<std::vector<int>> unusedMatchings2(0);
        std::vector<double> unusedMatchingsCost2(0);
        double distanceBetweenCurves2 = -1;
        std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded2(0);
       
        CGEDDistance(weight, step, costMatrixStep1Step2, GeodesicStep1, GeodesicStep2, false, false,  unusedMatchings2, unusedMatchingsCost2, distanceBetweenCurves2, unusedMatchingsCostsWithDiagonalCostsNotIncluded2);
       
        distanceStep1Step2 = distanceBetweenCurves2;
       
        if( geodesicCoefficient*lengthOftheGeodesic<=distanceGeo0Step1+distanceStep1Step2 ){
           
            double coefficientGeodesicStep2( (geodesicCoefficient*lengthOftheGeodesic-distanceGeo0Step1)/distanceStep1Step2 );
           
            for(int i = 0; i<Matching.size();i++){
               

                if(Matching[i][0] != -1 && Matching[i][1] != -1){
                           
                            ttk::PersistenceDiagramClustering persistenceDiagramClustering2;
                           
                            std::vector<ttk::DiagramType> centroids;
                           
                            std::vector<ttk::DiagramType> intermediateDiagrams{GeodesicSet[0][Matching[i][0]-1],GeodesicSet[1][Matching[i][1]-1]};
                            std::vector<std::vector<std::vector<ttk::MatchingType>>> allMatchings;

                            std::pair<ttk::DiagramType,double> Pairj;
                            std::pair<ttk::DiagramType,double> Pairjplus1;
           
                            Pairj.first=GeodesicSet[0][Matching[i][0]-1];
                            Pairjplus1.first=GeodesicSet[1][Matching[i][1]-1];
                           
                            Pairj.second=GeodesicTimesSet[0][Matching[i][0]-1];
                            Pairjplus1.second=GeodesicTimesSet[1][Matching[i][1]-1];
                           
                            TargetTimeGeodesic.push_back(Pairj.second*(1-coefficientGeodesicStep2)+Pairjplus1.second*coefficientGeodesicStep2);
                           

                            std::vector<int> clusterIds = persistenceDiagramClustering2.execute(intermediateDiagrams, centroids, allMatchings);
                            ttk::DiagramType ka;
                            ka = centroids.front();
                            std::vector<ttk::MatchingType> branch1 = allMatchings[0][0];
                   
                            ttk::DiagramType BarycenterDiag;

                            double coefficient = 2*coefficientGeodesicStep2;
                           
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
                                    
                                    if(bx != by){
                                        BarycenterDiag.push_back(ConcernedTimePersistencePair);
                                    }
                                    
                                    
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

                            TargetGeodesic.push_back(BarycenterDiag);
                }
               
            }
       
        }else{
           
       
         double coefficientGeodesicStep3 =  (geodesicCoefficient*lengthOftheGeodesic-distanceGeo0Step1-distanceStep1Step2)/ (lengthOftheGeodesic-distanceGeo0Step1-distanceStep1Step2);
           
           
         for(int i = 0; i<Matching.size();i++){
               

                if(Matching[i][0] != -1 && Matching[i][1] != -1){
               
                   
                    TargetGeodesic.push_back(GeodesicSet[1][Matching[i][1]-1]);
                    TargetTimeGeodesic.push_back(GeodesicTimesSet[1][Matching[i][1]-1]);
                   
                   
                }else if(Matching[i][0] == -1 && Matching[i][1] != -1){
                   
                           
                            ttk::PersistenceDiagramClustering persistenceDiagramClustering2;
                            ttk::DiagramType neutre;
           
                            std::vector<ttk::DiagramType> intermediateDiagrams{neutre,GeodesicSet[1][Matching[i][1]-1]};                
                            std::vector<ttk::DiagramType> centroids;
                            std::vector<std::vector<std::vector<ttk::MatchingType>>> allMatchings;

                            std::pair<ttk::DiagramType,double> Pairj;
                            std::pair<ttk::DiagramType,double> Pairjplus1;
           
                            Pairj.first=neutre;
                            Pairjplus1.first=GeodesicSet[1][Matching[i][1]-1];
                           
                            Pairj.second=GeodesicTimesSet[1][Matching[i][1]-1];
                            Pairjplus1.second=GeodesicTimesSet[1][Matching[i][1]-1];
                           
                            TargetTimeGeodesic.push_back(GeodesicTimesSet[1][Matching[i][1]-1]);
                           

                            std::vector<int> clusterIds = persistenceDiagramClustering2.execute(intermediateDiagrams, centroids, allMatchings);
                            ttk::DiagramType ka;
                            ka = centroids.front();
                            std::vector<ttk::MatchingType> branch1 = allMatchings[0][0];
                   
                            ttk::DiagramType BarycenterDiag;

                            double coefficient = 2*coefficientGeodesicStep3;
                           
                            for(int t = 0; t < branch1.size(); t++) {

                                std::tuple<int, int, double > toto = branch1[t];
                                int a = std::get<0>(toto);
                                int b = std::get<1>(toto);

                                if(a!=-1 && b!=-1) {//cannot enter in this condition normally

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
                                    
                                    if(bx != by){
                                        BarycenterDiag.push_back(ConcernedTimePersistencePair);
                                    }
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

                            TargetGeodesic.push_back(BarycenterDiag);
                           
                }
               
            }
           
        }
       
    }

}

void stochasticBarycenterComputation(int &tau, int &GeodesicSetSize, int &depart, double &step, double &weight, std::vector<PersistenceDiagramTimeSeries> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, std::vector<ttk::DiagramType> &stochBarycenterToBring, std::vector<double> &stochBarycenterTimeToBring, double &sumActualMinimum, double &pas, bool &criteria, int &threadNumber)
{
 
    if(GeodesicSetSize == 1){
       
        stochBarycenterToBring =GeodesicSet[0];
        stochBarycenterTimeToBring = GeodesicTimesSet[0];
       
    }
    else if(GeodesicSetSize > 1){

        int iteration = 20*tau;
        int halfIteration = iteration/2;
        int endIteration(halfIteration*0.9);
       
       
        std::vector<ttk::DiagramType> TargetStochBarycenter;
        std::vector<double> TargetStochTimeBarycenter;
       
        std::vector<ttk::DiagramType> TemporalStochTargetBarycenter;
        std::vector<double> TemporalStochTargetTimeBarycenter;
       
        std::vector<ttk::DiagramType> StochBarycenter;
        std::vector<double> StochTimeBarycenter;
       
        std::vector<double> iterationVector;
       
        bool convergence(false);
       
       
        for(int i = 0; i <= endIteration; i++){
           
            double toAdd(pas-(i*pas/halfIteration));
            iterationVector.push_back(toAdd);
           
            }
       
        for(int i = 0; i <= halfIteration; i++){
           
            double veryTemp = 0.1*pas;
            iterationVector.push_back(veryTemp);
           
        }
       
        int choice(depart);
       
        int veryTemp2=halfIteration+endIteration;
       
        for(int i =0;i<veryTemp2;i++){
           
            std::cout<< "i vaut " <<i<<std::endl;
       
            if(i==0){
               
                std::uniform_int_distribution<int> pick(0, GeodesicSetSize - 2);
                int offset = pick(globalRngTVPD());
                choice = (offset >= depart) ? offset + 1
                                            : offset;
               
                std::cout<<"choice = "<<choice<<std::endl;
                                            
                geodesicComputation(step, weight, TargetStochBarycenter, TargetStochTimeBarycenter, iterationVector[i], GeodesicSet[depart], GeodesicSet[choice], GeodesicTimesSet[depart], GeodesicTimesSet[choice], threadNumber);
               
                TemporalStochTargetBarycenter = TargetStochBarycenter;
                TemporalStochTargetTimeBarycenter = TargetStochTimeBarycenter;
               
                StochBarycenter = TargetStochBarycenter;
                StochTimeBarycenter = TargetStochTimeBarycenter;

                for(int z =0; z < GeodesicSet.size(); z++) {


                        std::vector<std::vector<double>> costMatrix(GeodesicSet[z].size(), std::vector<double>(TargetStochBarycenter.size()));
                       
                        std::vector<std::vector<int>> parameterList;
                       
                        for(int k = 0; k<GeodesicSet[z].size(); k++) {

                            for(int l = 0; l<TargetStochBarycenter.size(); l++) {

                                std::vector<int> toFillParameterList(2);
                               
                                toFillParameterList[0] = k;
                                toFillParameterList[1] = l;
                               
                                parameterList.push_back(toFillParameterList);

                            }

                        }
                       
                        std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[z];
                        std::vector<double> TimesOfjthGeodesic = TargetStochTimeBarycenter;
                           
                        #ifdef TTK_ENABLE_OPENMP
                        #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
                        #endif
                        for(size_t t = 0; t < parameterList.size(); t++){
                           
                            int k = -1;
                            int l = -1;
                           
                            k = parameterList[t][0];
                            l = parameterList[t][1];
                           
                            costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[z], TargetStochBarycenter, TimesOfithGeodesic, TimesOfjthGeodesic);
                           
                        }

                        double lengthOftheGeodesic(0);
                       
                        std::vector<std::vector<int>> unusedMatchings(0);
                        std::vector<double> unusedMatchingsCost(0);
                        double distanceBetweenCurves = -1;
                        std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                       
                        CGEDDistance(weight, step, costMatrix, GeodesicSet[z], TargetStochBarycenter, false, false,  unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);
                       
                        lengthOftheGeodesic = distanceBetweenCurves;
                        sumActualMinimum = sumActualMinimum+(lengthOftheGeodesic*lengthOftheGeodesic);

                }
               
                stochBarycenterToBring = StochBarycenter;
                stochBarycenterTimeToBring = StochTimeBarycenter;
                std::cout<<"sumActualMinimum = "<<sumActualMinimum<<std::endl;
            }
           
            else{
               
                std::uniform_int_distribution<int> pick(0, GeodesicSetSize - 1);
                choice = pick(globalRngTVPD());
               
                std::vector<ttk::DiagramType> TargetStochBarycenter;
                std::vector<double> TargetStochTimeBarycenter;
               
                geodesicComputation(step, weight, TargetStochBarycenter, TargetStochTimeBarycenter, iterationVector[i], TemporalStochTargetBarycenter, GeodesicSet[choice], TemporalStochTargetTimeBarycenter, GeodesicTimesSet[choice], threadNumber);
               
                TemporalStochTargetBarycenter = TargetStochBarycenter;
                TemporalStochTargetTimeBarycenter = TargetStochTimeBarycenter;
               
                double sumCandidate(0);
               

                for(int z =0; z < GeodesicSet.size(); z++) {


                        std::vector<std::vector<double>> costMatrix(GeodesicSet[z].size(), std::vector<double>(TargetStochBarycenter.size()));
                       
                        std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[z];
                        std::vector<double> TimesOfjthGeodesic = TargetStochTimeBarycenter;
                       
                        double lengthOftheGeodesic(0);
                       
                        std::vector<std::vector<int>> parameterList;
                       
                        for(int k = 0; k<GeodesicSet[z].size(); k++) {

                            for(int l = 0; l<TargetStochBarycenter.size(); l++) {

                                std::vector<int> toFillParameterList(2);
                               
                                toFillParameterList[0] = k;
                                toFillParameterList[1] = l;
                               
                                parameterList.push_back(toFillParameterList);

                            }

                        }
                           
                        #ifdef TTK_ENABLE_OPENMP
                        #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
                        #endif
                        for(size_t t = 0; t < parameterList.size(); t++){
                           
                            int k = -1;
                            int l = -1;
                           
                            k = parameterList[t][0];
                            l = parameterList[t][1];
                           
                            costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[z], TargetStochBarycenter, TimesOfithGeodesic, TimesOfjthGeodesic);
                           
                        }
               
                        std::vector<std::vector<int>> unusedMatchings(0);
                        std::vector<double> unusedMatchingsCost(0);
                        double distanceBetweenCurves = -1;
                        std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                       
                        CGEDDistance(weight, step, costMatrix, GeodesicSet[z], TargetStochBarycenter, false, false, unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);
                       
                        lengthOftheGeodesic = distanceBetweenCurves;
                        sumCandidate = sumCandidate+(lengthOftheGeodesic*lengthOftheGeodesic);

                       
                }
                           
                if(sumCandidate < sumActualMinimum){
                   
                    double improvement = sumActualMinimum-sumCandidate;
                    double sumActualMinimumFraction = sumActualMinimum/100;
                    StochBarycenter = TargetStochBarycenter;
                    StochTimeBarycenter = TargetStochTimeBarycenter;
                    sumActualMinimum = sumCandidate;
                   
                    stochBarycenterToBring = StochBarycenter;
                    stochBarycenterTimeToBring = StochTimeBarycenter;
                    std::cout<<"sumActualMinimum = "<<sumActualMinimum<<std::endl;
                    if(improvement<sumActualMinimumFraction){
                       
                        convergence = true;
                       
                    }
                   
                }
               
            }
           
            if(criteria == true && convergence == true){
               
                break;
            }
           
        }
       
    }

}

void greedyBarycenterComputation(int &tau, int &GeodesicSetSize, int &depart, double &greedySegmentation, double &step, double &weight, std::vector<PersistenceDiagramTimeSeries> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, std::vector<ttk::DiagramType> &greedBarycenterToBring, std::vector<double> &greedBarycenterTimeToBring, double &MySumBarycenter, int &threadNumber)
{
   
    if(GeodesicSetSize == 1){
       
        greedBarycenterToBring =GeodesicSet[0];
        greedBarycenterTimeToBring = GeodesicTimesSet[0];
       
    }
    else if(GeodesicSetSize > 1){
       
        int iteration = 20*tau;    
       
        std::vector<ttk::DiagramType> TargetGreedyBarycenter;
        std::vector<double> TargetGreedyTimeBarycenter;
       
        std::vector<ttk::DiagramType> TemporalGreedyTargetBarycenter;
        std::vector<double> TemporalGreedyTargetTimeBarycenter;
       
        std::vector<ttk::DiagramType> MyGreedyBarycenter;
        std::vector<double> MyGreedyTimeBarycenter;
           
        for(int i = 0; i <iteration; i++){  

            std::vector<ttk::DiagramType> GreedyBarycenter;
            std::vector<double> GreedyTimeBarycenter;
       
            if(i==0){
               
                double sumGreedyOnLoop(0);
                for(int j = 0;j<GeodesicSetSize;j++){
                   
                    std::vector<ttk::DiagramType> TargetGreedyBarycenter;
                    std::vector<double> TargetGreedyTimeBarycenter;
                   
                   
                    double sumGreedy(0);
               
                    if(depart !=j){
                       
                        std::vector<ttk::DiagramType> BestIntraGeodesicCandidate;
                        std::vector<double> BestIntraGeodesicTimeCandidate;
                        double BestsumGreedyIntraLoopA(0);
                       
                        for(int a=1;a<=greedySegmentation;a++){

                            std::vector<ttk::DiagramType> TargetGreedyBarycenter;
                            std::vector<double> TargetGreedyTimeBarycenter;
                            double coeffGeo = a*(1/greedySegmentation);
                            geodesicComputation(step, weight, TargetGreedyBarycenter, TargetGreedyTimeBarycenter, coeffGeo, GeodesicSet[depart], GeodesicSet[j], GeodesicTimesSet[depart], GeodesicTimesSet[j], threadNumber);
                                   
                            double sumGreedyIntraLoopA(0);

                            for(int z =0; z < GeodesicSet.size(); z++) {

                                    std::vector<std::vector<double>> costMatrix(GeodesicSet[z].size(), std::vector<double>(TargetGreedyBarycenter.size()));
                                   
                                    std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[z];
                                    std::vector<double> TimesOfjthGeodesic = TargetGreedyTimeBarycenter;
                                    double lengthOftheGeodesic(0);
                                   
                                    std::vector<std::vector<int>> parameterList;
                                   
                                    for(int k = 0; k<GeodesicSet[z].size(); k++) {

                                        for(int l = 0; l<TargetGreedyBarycenter.size(); l++) {

                                            std::vector<int> toFillParameterList(2);
                               
                                            toFillParameterList[0] = k;
                                            toFillParameterList[1] = l;
                               
                                            parameterList.push_back(toFillParameterList);

                                        }

                                    }
                                   
                                    #ifdef TTK_ENABLE_OPENMP
                                    #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
                                    #endif
                                    for(size_t t = 0; t < parameterList.size(); t++){
                           
                                        int k = -1;
                                        int l = -1;
                           
                                        k = parameterList[t][0];
                                        l = parameterList[t][1];
                           
                                        costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[z], TargetGreedyBarycenter, TimesOfithGeodesic, TimesOfjthGeodesic);
                           
                                    }
                                   
                                    std::vector<std::vector<int>> unusedMatchings(0);
                                    std::vector<double> unusedMatchingsCost(0);
                                    double distanceBetweenCurves = -1;
                                    std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                                   
                                    CGEDDistance(weight, step, costMatrix, GeodesicSet[z], TargetGreedyBarycenter, false, false, unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);

                                    lengthOftheGeodesic = distanceBetweenCurves;
                                    sumGreedyIntraLoopA = sumGreedyIntraLoopA+(lengthOftheGeodesic*lengthOftheGeodesic);

                                }
                               
                           
               
                                if(a==1){
                                    BestsumGreedyIntraLoopA = sumGreedyIntraLoopA;
                                    BestIntraGeodesicCandidate = TargetGreedyBarycenter;
                                    BestIntraGeodesicTimeCandidate = TargetGreedyTimeBarycenter;
                                }
                               
                                if(sumGreedyIntraLoopA<BestsumGreedyIntraLoopA){
                                    BestsumGreedyIntraLoopA = sumGreedyIntraLoopA;
                                    BestIntraGeodesicCandidate = TargetGreedyBarycenter;
                                    BestIntraGeodesicTimeCandidate = TargetGreedyTimeBarycenter;                
                                }
                           
                        }
                       
                        TargetGreedyBarycenter = BestIntraGeodesicCandidate;
                        TargetGreedyTimeBarycenter = BestIntraGeodesicTimeCandidate;
                    }
                   
                    else if(depart == j){
                        TargetGreedyBarycenter = GeodesicSet[depart];
                        TargetGreedyTimeBarycenter = GeodesicTimesSet[depart];
                       
                    }

                for(int z = 0; z < GeodesicSet.size(); z++) {


                        std::vector<std::vector<double>> costMatrix(GeodesicSet[z].size(), std::vector<double>(TargetGreedyBarycenter.size()));
                       
                        std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[z];
                        std::vector<double> TimesOfjthGeodesic = TargetGreedyTimeBarycenter;
                        double lengthOftheGeodesic(0);
                       
                       
                        std::vector<std::vector<int>> parameterList2;
                                   
                        for(int k = 0; k<GeodesicSet[z].size(); k++) {

                            for(int l = 0; l<TargetGreedyBarycenter.size(); l++) {

                                std::vector<int> toFillParameterList(2);
                               
                                toFillParameterList[0] = k;
                                toFillParameterList[1] = l;
                               
                                parameterList2.push_back(toFillParameterList);

                                }

                        }
                                   
                        #ifdef TTK_ENABLE_OPENMP
                        #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
                        #endif
                        for(size_t t = 0; t < parameterList2.size(); t++){
                           
                            int k = -1;
                            int l = -1;
                           
                            k = parameterList2[t][0];
                            l = parameterList2[t][1];
                           
                            costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[z], TargetGreedyBarycenter, TimesOfithGeodesic, TimesOfjthGeodesic);
                           
                        }
                       
                        std::vector<std::vector<int>> unusedMatchings(0);
                        std::vector<double> unusedMatchingsCost(0);
                        double distanceBetweenCurves = -1;
                        std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                       
                        CGEDDistance(weight, step, costMatrix, GeodesicSet[z], TargetGreedyBarycenter, false, false, unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);
                       
                        lengthOftheGeodesic = distanceBetweenCurves;
                        sumGreedy = sumGreedy+(lengthOftheGeodesic*lengthOftheGeodesic);
                       
                    }
                   
                    if(j==0){
                        sumGreedyOnLoop = sumGreedy;
                        TemporalGreedyTargetBarycenter=TargetGreedyBarycenter;
                        TemporalGreedyTargetTimeBarycenter=TargetGreedyTimeBarycenter;
                        MyGreedyBarycenter =TemporalGreedyTargetBarycenter;
                        MyGreedyTimeBarycenter=TemporalGreedyTargetTimeBarycenter;
                        MySumBarycenter = sumGreedyOnLoop;
                        greedBarycenterToBring = MyGreedyBarycenter;
                        greedBarycenterTimeToBring = MyGreedyTimeBarycenter;
                        std::cout<<"MyFirstSumBarycenter = "<<MySumBarycenter<<std::endl;
                    }
                   
                    if(sumGreedy<sumGreedyOnLoop){
                       
                        sumGreedyOnLoop = sumGreedy;
                        TemporalGreedyTargetBarycenter=TargetGreedyBarycenter;
                        TemporalGreedyTargetTimeBarycenter=TargetGreedyTimeBarycenter;
                        MyGreedyBarycenter =TemporalGreedyTargetBarycenter;
                        MyGreedyTimeBarycenter=TemporalGreedyTargetTimeBarycenter;
                        MySumBarycenter = sumGreedyOnLoop;
                        greedBarycenterToBring = MyGreedyBarycenter;
                        greedBarycenterTimeToBring = MyGreedyTimeBarycenter;
                        std::cout<<"MyFirstSumBarycenter = "<<MySumBarycenter<<std::endl;
                    }
                   
                }

            }
           
            else{
               
                std::vector<ttk::DiagramType> Sauvegarde = TemporalGreedyTargetBarycenter;
                std::vector<double> TimeSauvegarde = TemporalGreedyTargetTimeBarycenter;
               
                double sumGreedyOnLoop(0);
               
                for(int j = 0;j<GeodesicSetSize;j++){
               
                    std::vector<ttk::DiagramType> TargetGreedyBarycenter;
                    std::vector<double> TargetGreedyTimeBarycenter;
                    double sumGreedy(0);                
                                       
                        std::vector<ttk::DiagramType> BestIntraGeodesicCandidate;
                        std::vector<double> BestIntraGeodesicTimeCandidate;
                        double BestsumGreedyIntraLoopA(0);
                       
                        for(int a=1;a<=greedySegmentation;a++){
                           
                            std::vector<ttk::DiagramType> TargetGreedyBarycenter;
                            std::vector<double> TargetGreedyTimeBarycenter;
                           
                            geodesicComputation(step, weight, TargetGreedyBarycenter, TargetGreedyTimeBarycenter, a*(1/greedySegmentation), TemporalGreedyTargetBarycenter, GeodesicSet[j], TemporalGreedyTargetTimeBarycenter, GeodesicTimesSet[j], threadNumber);
                                   
                            double sumGreedyIntraLoopA(0);
                                       

                            for(int z =0; z < GeodesicSet.size(); z++) {

                                    std::vector<std::vector<double>> costMatrix(GeodesicSet[z].size(), std::vector<double>(TargetGreedyBarycenter.size()));
                                   
                                    std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[z];
                                    std::vector<double> TimesOfjthGeodesic = TargetGreedyTimeBarycenter;
                                    double lengthOftheGeodesic(0);
                                   
                                   
                                   
                                    std::vector<std::vector<int>> parameterList3;
                                   
                                    for(int k = 0; k<GeodesicSet[z].size(); k++) {

                                        for(int l = 0; l<TargetGreedyBarycenter.size(); l++) {

                                            std::vector<int> toFillParameterList(2);
                                           
                                            toFillParameterList[0] = k;
                                            toFillParameterList[1] = l;
                                           
                                            parameterList3.push_back(toFillParameterList);

                                        }

                                    }
                                       
                                    #ifdef TTK_ENABLE_OPENMP
                                    #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
                                    #endif
                                    for(size_t t = 0; t < parameterList3.size(); t++){
                                       
                                        int k = -1;
                                        int l = -1;
                                       
                                        k = parameterList3[t][0];
                                        l = parameterList3[t][1];
                                       
                                        costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[z], TargetGreedyBarycenter, TimesOfithGeodesic, TimesOfjthGeodesic);
                                       
                                    }
                                   
                                    std::vector<std::vector<int>> unusedMatchings(0);
                                    std::vector<double> unusedMatchingsCost(0);
                                    double distanceBetweenCurves = -1;
                                    std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                                   
                                    CGEDDistance(weight, step, costMatrix, GeodesicSet[z], TargetGreedyBarycenter, false, false, unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);
                                   
                                    lengthOftheGeodesic = distanceBetweenCurves;
                                    sumGreedyIntraLoopA = sumGreedyIntraLoopA+(lengthOftheGeodesic*lengthOftheGeodesic);
                                   
                                }
               
                                if(a==1){
                                    BestsumGreedyIntraLoopA = sumGreedyIntraLoopA;
                                    BestIntraGeodesicCandidate = TargetGreedyBarycenter;
                                    BestIntraGeodesicTimeCandidate = TargetGreedyTimeBarycenter;
                                   
                                }
                               
                                if(sumGreedyIntraLoopA<BestsumGreedyIntraLoopA){
                                   
                                    BestsumGreedyIntraLoopA = sumGreedyIntraLoopA;
                                    BestIntraGeodesicCandidate = TargetGreedyBarycenter;
                                    BestIntraGeodesicTimeCandidate = TargetGreedyTimeBarycenter;                
                                   
                                }
                        }
                       
                        TargetGreedyBarycenter = BestIntraGeodesicCandidate;
                        TargetGreedyTimeBarycenter = BestIntraGeodesicTimeCandidate;

                for(int z =0; z < GeodesicSet.size(); z++) {

                        std::vector<std::vector<double>> costMatrix(GeodesicSet[z].size(), std::vector<double>(TargetGreedyBarycenter.size()));
                       
                        std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[z];
                        std::vector<double> TimesOfjthGeodesic = TargetGreedyTimeBarycenter;
                        double lengthOftheGeodesic(0);
                       
                        std::vector<std::vector<int>> parameterList4;
                       
                        for(int k = 0; k<GeodesicSet[z].size(); k++) {

                            for(int l = 0; l<TargetGreedyBarycenter.size(); l++) {

                                std::vector<int> toFillParameterList(2);
                               
                                toFillParameterList[0] = k;
                                toFillParameterList[1] = l;
                               
                                parameterList4.push_back(toFillParameterList);

                            }

                        }
                           
                        #ifdef TTK_ENABLE_OPENMP
                        #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
                        #endif
                        for(size_t t = 0; t < parameterList4.size(); t++){
                           
                            int k = -1;
                            int l = -1;
                           
                            k = parameterList4[t][0];
                            l = parameterList4[t][1];
                           
                            costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[z], TargetGreedyBarycenter, TimesOfithGeodesic, TimesOfjthGeodesic);
                           
                        }
                       
                        std::vector<std::vector<int>> unusedMatchings(0);
                        std::vector<double> unusedMatchingsCost(0);
                        double distanceBetweenCurves = -1;
                        std::vector<double> unusedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                       
                        CGEDDistance(weight, step, costMatrix, GeodesicSet[z], TargetGreedyBarycenter, false, false, unusedMatchings, unusedMatchingsCost, distanceBetweenCurves, unusedMatchingsCostsWithDiagonalCostsNotIncluded);
                       
                        lengthOftheGeodesic = distanceBetweenCurves;
                        sumGreedy = sumGreedy+(lengthOftheGeodesic*lengthOftheGeodesic);

                       
                    }
                   
                    if(j==0){
                        sumGreedyOnLoop = sumGreedy;
                       
                        GreedyBarycenter=TargetGreedyBarycenter;
                        GreedyTimeBarycenter=TargetGreedyTimeBarycenter;
                    }
                   
                    if(sumGreedy<sumGreedyOnLoop){
                       
                        sumGreedyOnLoop = sumGreedy;
                       
                        GreedyBarycenter=TargetGreedyBarycenter;
                        GreedyTimeBarycenter=TargetGreedyTimeBarycenter;
                    }
                   
                }
               
               
                if(sumGreedyOnLoop<MySumBarycenter){
                   
                    MySumBarycenter = sumGreedyOnLoop;
                    MyGreedyBarycenter =GreedyBarycenter;
                    MyGreedyTimeBarycenter=GreedyTimeBarycenter;
                    TemporalGreedyTargetBarycenter = MyGreedyBarycenter;
                    TemporalGreedyTargetTimeBarycenter=MyGreedyTimeBarycenter;
                    greedBarycenterToBring = MyGreedyBarycenter;
                    greedBarycenterTimeToBring = MyGreedyTimeBarycenter;
                    std::cout<<"MySumBarycenter = "<<MySumBarycenter<<std::endl;
                }
                else{
                   
                    break;
                   
                }
               
            }
           
        }
       
    }
   
}

void totalStochasticBarycenterComputation(int &tau, int &GeodesicSetSize, double &step, double &weight, std::vector<PersistenceDiagramTimeSeries> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, std::vector<ttk::DiagramType> &stochBarycenterToBring, std::vector<double> &stochBarycenterTimeToBring, double &pas, bool &criteria, int &numberOfDeparturesStochasticBarycenterComputation, double &myBestStochasticSumBarycenter, int &threadNumber, int &geodesicForBarycenter)
{
    int numberOfDepartures = std::min(numberOfDeparturesStochasticBarycenterComputation, GeodesicSetSize);
   
    if(geodesicForBarycenter == 1){
        if(GeodesicSetSize == 1){
           
            stochBarycenterToBring =GeodesicSet[0];
            stochBarycenterTimeToBring = GeodesicTimesSet[0];
           
        }else if(GeodesicSetSize == 2){

        geodesicComputation(step, weight, stochBarycenterToBring, stochBarycenterTimeToBring, 0.5, GeodesicSet[0], GeodesicSet[1], GeodesicTimesSet[0], GeodesicTimesSet[1],threadNumber);

        }else if(GeodesicSetSize > 2){
           
            std::vector<double> initialStepVector(0);
           
            for(int i =0;i<4;i++){
               
                initialStepVector.push_back(pas-((pas/100)*20*i));
               
            }
               
            std::vector<int> v(0);
           
            for(int i =0;i<GeodesicSet.size();i++){
               
                v.push_back(i);
               
            }
       
            std::shuffle(v.begin(), v.end(), globalRngTVPD());
           
            myBestStochasticSumBarycenter = -1;
           
           
           
            for(int i = 0; i<numberOfDepartures; i++){

                for(int j =0; j<4; j++){
                   
                    std::vector<ttk::DiagramType> stochBarycenter;
                    std::vector<double> stochBarycenterTime;
                   
                    double myStochSumBarycenter(0);
                    std::cout<<"Passage stochastique"<<std::endl;
                    int departStochasticBarycenter(v[i]);

                    stochasticBarycenterComputation(tau, GeodesicSetSize, departStochasticBarycenter, step, weight, GeodesicSet, GeodesicTimesSet, stochBarycenter, stochBarycenterTime, myStochSumBarycenter, initialStepVector[j], criteria, threadNumber);
                   
                    if(i == 0 && j == 0){
                   
                        myBestStochasticSumBarycenter = myStochSumBarycenter;
                        stochBarycenterToBring = stochBarycenter;
                        stochBarycenterTimeToBring = stochBarycenterTime;
                       
                    }
                   
                    else if(myStochSumBarycenter<myBestStochasticSumBarycenter){
                       
                        myBestStochasticSumBarycenter = myStochSumBarycenter;
                        stochBarycenterToBring = stochBarycenter;
                        stochBarycenterTimeToBring = stochBarycenterTime;
                       
                    }
                   
                }

            }

        }
    }else if(geodesicForBarycenter == 0){
        if(GeodesicSetSize == 1){
           
            stochBarycenterToBring =GeodesicSet[0];
            stochBarycenterTimeToBring = GeodesicTimesSet[0];
           
        }else if(GeodesicSetSize > 1){
           
            std::vector<double> initialStepVector(0);
           
            for(int i =0;i<4;i++){
               
                initialStepVector.push_back(pas-((pas/100)*20*i));
               
            }
               
            std::vector<int> v(0);
           
            for(int i =0;i<GeodesicSet.size();i++){
               
                v.push_back(i);
               
            }
       
            std::shuffle(v.begin(), v.end(), globalRngTVPD());
           
            myBestStochasticSumBarycenter = -1;
           
           
           
            for(int i = 0; i<numberOfDepartures; i++){

                for(int j =0; j<4; j++){
                   
                    std::vector<ttk::DiagramType> stochBarycenter;
                    std::vector<double> stochBarycenterTime;
                   
                    double myStochSumBarycenter(0);
                    std::cout<<"Passage stochastique"<<std::endl;
                    int departStochasticBarycenter(v[i]);
                    
                    using clock = std::chrono::steady_clock;
                    auto t0 = clock::now();
                    
                    stochasticBarycenterComputation(tau, GeodesicSetSize, departStochasticBarycenter, step, weight, GeodesicSet, GeodesicTimesSet, stochBarycenter, stochBarycenterTime, myStochSumBarycenter, initialStepVector[j], criteria, threadNumber);
                    
                    auto t1 = clock::now();
                    double secs = std::chrono::duration<double>(t1 - t0).count();
                    std::cout << "Durée: " << secs << " s"<<std::endl;
                    
                    if(i == 0 && j == 0){
                   
                        myBestStochasticSumBarycenter = myStochSumBarycenter;
                        stochBarycenterToBring = stochBarycenter;
                        stochBarycenterTimeToBring = stochBarycenterTime;
                       
                    }
                   
                    else if(myStochSumBarycenter<myBestStochasticSumBarycenter){
                       
                        myBestStochasticSumBarycenter = myStochSumBarycenter;
                        stochBarycenterToBring = stochBarycenter;
                        stochBarycenterTimeToBring = stochBarycenterTime;
                       
                    }
                   
                }

            }

        }
   
    }
   
}

void totalGreedyBarycenterComputation(int &tau, int &GeodesicSetSize, double &greedySegmentation, double &step, double &weight, std::vector<PersistenceDiagramTimeSeries> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, std::vector<ttk::DiagramType> &greedyBarycenterToBring, std::vector<double> &greedyBarycenterTimeToBring, double &myBestGreedySumBarycenter, int &threadNumber, int &geodesicForBarycenter)
{
   
    double myBestGreedyIndice(0);
   
    if(geodesicForBarycenter == 1){
        if(GeodesicSetSize == 1){
           
            greedyBarycenterToBring =GeodesicSet[0];
            greedyBarycenterTimeToBring = GeodesicTimesSet[0];
           
        }else if(GeodesicSetSize == 2){
           
            geodesicComputation(step, weight, greedyBarycenterToBring, greedyBarycenterTimeToBring, 0.5, GeodesicSet[0], GeodesicSet[1], GeodesicTimesSet[0], GeodesicTimesSet[1],threadNumber);

        }else if(GeodesicSetSize >2){

            for(int i = 0; i<GeodesicSetSize; i++){
                   
                std::vector<ttk::DiagramType> greedyBarycenter;
                std::vector<double> greedyBarycenterTime;
                   
                double myGreedySumBarycenter(0);
               
                int departGreedyBarycenter(i);
                
                greedyBarycenterComputation(tau, GeodesicSetSize, departGreedyBarycenter, greedySegmentation, step, weight, GeodesicSet, GeodesicTimesSet, greedyBarycenter, greedyBarycenterTime, myGreedySumBarycenter, threadNumber);
                
                if(i == 0){
                   
                    myBestGreedySumBarycenter = myGreedySumBarycenter;
                    greedyBarycenterToBring = greedyBarycenter;
                    greedyBarycenterTimeToBring = greedyBarycenterTime;
                    myBestGreedyIndice = i;
                       
                }
                   
                else if(myGreedySumBarycenter<myBestGreedySumBarycenter){
                       
                    myBestGreedySumBarycenter = myGreedySumBarycenter;
                    greedyBarycenterToBring = greedyBarycenter;
                    greedyBarycenterTimeToBring = greedyBarycenterTime;
                    myBestGreedyIndice = i;
                       
                }
               
            }
                   
        }
    }else if(geodesicForBarycenter == 0){
        if(GeodesicSetSize == 1){
           
            greedyBarycenterToBring =GeodesicSet[0];
            greedyBarycenterTimeToBring = GeodesicTimesSet[0];
           
        }else if(GeodesicSetSize >1){

            for(int i = 0; i<GeodesicSetSize; i++){
                   
                std::vector<ttk::DiagramType> greedyBarycenter;
                std::vector<double> greedyBarycenterTime;
                
                double myGreedySumBarycenter(0);
               
                int departGreedyBarycenter(i);
                
                using clock = std::chrono::steady_clock;
                auto t0 = clock::now();
                
                greedyBarycenterComputation(tau, GeodesicSetSize, departGreedyBarycenter, greedySegmentation, step, weight, GeodesicSet, GeodesicTimesSet, greedyBarycenter, greedyBarycenterTime, myGreedySumBarycenter, threadNumber);
                
                auto t1 = clock::now();
                double secs = std::chrono::duration<double>(t1 - t0).count();
                std::cout << "Durée: " << secs << " s"<<std::endl;
                
                if(i == 0){
                   
                    myBestGreedySumBarycenter = myGreedySumBarycenter;
                    greedyBarycenterToBring = greedyBarycenter;
                    greedyBarycenterTimeToBring = greedyBarycenterTime;
                    myBestGreedyIndice = i;
                       
                }
                   
                else if(myGreedySumBarycenter<myBestGreedySumBarycenter){
                       
                    myBestGreedySumBarycenter = myGreedySumBarycenter;
                    greedyBarycenterToBring = greedyBarycenter;
                    greedyBarycenterTimeToBring = greedyBarycenterTime;
                    myBestGreedyIndice = i;
                       
                }
               
            }
                   
        }
    }
}

void kMeanComputationWithBuffer(int &tau, double &greedySegmentation, double &step, double &weight, std::vector<PersistenceDiagramTimeSeries> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, std::vector<PersistenceDiagramTimeSeries> &greedyClustersToBring, std::vector<std::vector<double>> &greedyClustersTimeToBring, int &numberOfClusters, bool &kMeanPlusPlus, int &iterationsNumberKMeans, int &methodChoice, int &numberOfDeparturesStochasticBarycenterComputation, double &pas, bool &criteria, int &threadNumber, double &wcssToBring, std::vector<int> &idsToBring, int &geodesicForBarycenter)
{
   
   std::vector<int> initialClusters(0);
   
   if(kMeanPlusPlus == false){
       
        std::vector<int> v(0);
       
        for(int i =0;i<GeodesicSet.size();i++){
           
            v.push_back(i);
           
        }
   
        std::shuffle(v.begin(), v.end(), globalRngTVPD());
       
        for(int i =0;i<numberOfClusters;i++){
           
            initialClusters.push_back(v[i]);

        }
       
       
    }
   else{
       
        std::vector<int> v(0);
       
        for(int i =0;i<GeodesicSet.size();i++){
           
            v.push_back(i);
           
        }
   
        std::shuffle(v.begin(), v.end(), globalRngTVPD());
                   
        initialClusters.push_back(v[0]);
               
        std::vector<int> vecFrom0ToGeodesicSetSizeMinusOne(0);

        for (int i = 0; i < GeodesicSet.size(); i++) {
           
            vecFrom0ToGeodesicSetSizeMinusOne.push_back(i);
           
        }
       
        for(int k = 0 ; k<numberOfClusters-1; k++){
           
            std::vector<double> scoreVector(0);
            std::vector<int> indicesOfSecondSet;

            for (int elem : vecFrom0ToGeodesicSetSizeMinusOne) {

                if (std::find(initialClusters.begin(), initialClusters.end(), elem) == initialClusters.end()) {

                    indicesOfSecondSet.push_back(elem);
                   
                }
            }
           
           
            distanceOfIndicesiFromIndicesj(step, weight, GeodesicSet, GeodesicTimesSet, scoreVector, initialClusters, indicesOfSecondSet, threadNumber);
           
            std::vector<double> scoreVectorSquared(0);
           
            for(int i =0;i<scoreVector.size();i++){
               
                scoreVectorSquared.push_back(scoreVector[i]*scoreVector[i]);
               
            }
                     
            std::discrete_distribution<> d(scoreVectorSquared.begin(), scoreVectorSquared.end());
           
            int p = d(globalRngTVPD());          
           
            initialClusters.push_back(indicesOfSecondSet[p]);
           
        }
    }
   
    for(int i =0;i<initialClusters.size();i++){
               
        std::cout<<" element "<<i<<" de initialClusters : "<<initialClusters[i]<<std::endl;
               
    }
   
    std::vector<PersistenceDiagramTimeSeries> geodesicSetClusters(0);
    std::vector<std::vector<double>> geodesicTimesSetClusters(0);
   
    double bestWCSS = 0.0;
    std::vector<PersistenceDiagramTimeSeries> bestCenters;
    std::vector<std::vector<double>> bestTimes;
    std::vector<int> bestIds;
   
    for(int i = 0;i<iterationsNumberKMeans+1;i++){
       
       if(i==0){
           
           std::vector<PersistenceDiagramTimeSeries> clusterGeodesicSet(0);
           std::vector<std::vector<double>> clusterGeodesicTimesSet(0);
           
           for(int z=0; z<initialClusters.size(); z++){
                             
                clusterGeodesicSet.push_back(GeodesicSet[initialClusters[z]]);
                clusterGeodesicTimesSet.push_back(GeodesicTimesSet[initialClusters[z]]);
               
            }
           
           std::vector<int> kmeansClusterIds(0);
           std::vector<double> nearestDist2(GeodesicSet.size(), 0.0);
           
           for(int z =0;z<GeodesicSet.size();z++){
               
                int closerIndexInSet = -1;
                double distanceToTheNearest = -1;
                std::vector<std::vector<int>> matchingsWithTheNearest(0);
                std::vector<double> matchingsCostWithTheNearest(0);
                bool withMatchingsOrNot = false;
                std::vector<double> matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest(0);
               
               
                distanceOfTargetFromSet(step, weight, clusterGeodesicSet, clusterGeodesicTimesSet,GeodesicSet[z],GeodesicTimesSet[z], threadNumber, closerIndexInSet, distanceToTheNearest, matchingsWithTheNearest, matchingsCostWithTheNearest, withMatchingsOrNot, matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest);
               
                kmeansClusterIds.push_back(initialClusters[closerIndexInSet]);
                nearestDist2[z]  = distanceToTheNearest*distanceToTheNearest;
                std::cout<<"le vecteur kmeansClusterIds contient a la place "<<z<<" l'element "<<kmeansClusterIds[z]<<std::endl;
               
            }
           
            double wcss = 0.0;
           
            for(int z = 0;z<nearestDist2.size();z++){
               
                wcss = wcss+nearestDist2[z];
               
            }

            bestWCSS    = wcss;
            bestCenters = clusterGeodesicSet;
            bestTimes   = clusterGeodesicTimesSet;
            bestIds     = kmeansClusterIds;
            std::cout<<"Initial clustering has a WCSS = "<<bestWCSS<<std::endl;
           
            for(int z = 0;z<numberOfClusters;z++){
               
                std::vector<PersistenceDiagramTimeSeries> geodesicSetToBarycenter(0);
                std::vector<std::vector<double>> geodesicTimesSetToBarycenter(0);
                 
                for (int j = 0; j < kmeansClusterIds.size(); j++) {
                    if (kmeansClusterIds[j] == initialClusters[z]) {
                        geodesicSetToBarycenter.push_back(GeodesicSet[j]);
                        geodesicTimesSetToBarycenter.push_back(GeodesicTimesSet[j]);
                    }
                }
               
                int geodesicSetToBarycenterSize = geodesicSetToBarycenter.size();
               
                std::vector<ttk::DiagramType> barycenterToBring;
                std::vector<double> barycenterTimeToBring;
               
                double myBestSumBarycenter(0);

               
                if(methodChoice==0){
                    totalStochasticBarycenterComputation(tau, geodesicSetToBarycenterSize, step, weight, geodesicSetToBarycenter, geodesicTimesSetToBarycenter, barycenterToBring, barycenterTimeToBring, pas, criteria, numberOfDeparturesStochasticBarycenterComputation, myBestSumBarycenter, threadNumber, geodesicForBarycenter);
                }
                else{
                    totalGreedyBarycenterComputation(tau, geodesicSetToBarycenterSize, greedySegmentation, step, weight, geodesicSetToBarycenter, geodesicTimesSetToBarycenter, barycenterToBring, barycenterTimeToBring, myBestSumBarycenter, threadNumber, geodesicForBarycenter);
                }
               
                geodesicSetClusters.push_back(barycenterToBring);
                geodesicTimesSetClusters.push_back(barycenterTimeToBring);
                 
                std::cout<<"Z = "<<z<<std::endl;
                std::cout<< " size : "<<geodesicSetClusters.size()<<std::endl;
            }
           
        }
        else{
           
           std::vector<PersistenceDiagramTimeSeries> clusterGeodesicSet;
           std::vector<std::vector<double>> clusterGeodesicTimesSet;
           
           clusterGeodesicSet = geodesicSetClusters;
           clusterGeodesicTimesSet = geodesicTimesSetClusters;
           
           
           std::vector<int> kmeansClusterIds(0);
           std::vector<double> nearestDist2(GeodesicSet.size(), 0.0);
           
            for(int z =0;z<GeodesicSet.size();z++){
               
                int closerIndexInSet = -1;
                double distanceToTheNearest = -1;
                std::vector<std::vector<int>> matchingsWithTheNearest(0);
                std::vector<double> matchingsCostWithTheNearest(0);
                bool withMatchingsOrNot = false;
                std::vector<double> matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest(0);

                distanceOfTargetFromSet(step, weight, clusterGeodesicSet, clusterGeodesicTimesSet,GeodesicSet[z],GeodesicTimesSet[z], threadNumber, closerIndexInSet, distanceToTheNearest, matchingsWithTheNearest, matchingsCostWithTheNearest, withMatchingsOrNot, matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest);
               
                kmeansClusterIds.push_back(closerIndexInSet);
                nearestDist2[z]  = distanceToTheNearest*distanceToTheNearest;
                std::cout<<"le vecteur kmeansClusterIds contient a la place "<<z<<" l'element "<<kmeansClusterIds[z]<<std::endl;
               
            }
           
            std::vector<int> clusterSizes(numberOfClusters, 0);
            for(int id : kmeansClusterIds)
                ++clusterSizes[id];
           
            double wcss = 0.0;
           
            for(int z = 0;z<nearestDist2.size();z++){
               
                wcss = wcss+nearestDist2[z];
               
            }
           
            bool empty = false;
           
            for(int c = 0; c < numberOfClusters; ++c){
                if(clusterSizes[c] == 0){
                    empty = true;
                }
            }
           
           
            if (wcss < bestWCSS && empty == false) {
                bestWCSS   = wcss;
                bestCenters = geodesicSetClusters;
                bestTimes   = geodesicTimesSetClusters;
                bestIds     = kmeansClusterIds;
                std::cout<<"New better clustering with a WCSS = "<<bestWCSS<<std::endl;
            }
           
            if(i==iterationsNumberKMeans){
                break;
            }

            bool hadEmpty = true;
            bool hadEmpty2 = false;
            while(hadEmpty){
                hadEmpty = false;

                for(int c = 0; c < numberOfClusters; ++c){
                    if(clusterSizes[c] == 0){                      
                        hadEmpty = true;
                        hadEmpty2 = true;
                        int newCenterIdxInData = farthestPoint(nearestDist2);

                        kmeansClusterIds[newCenterIdxInData] = c;
                        nearestDist2[newCenterIdxInData] = 0.0;
                    }
                }

                if(hadEmpty){
                    clusterSizes.assign(numberOfClusters, 0);
                    for (int id : kmeansClusterIds){
                        ++clusterSizes[id];}
                }
            }
           
            if(hadEmpty2){
                for(int z =0;z<GeodesicSet.size();z++){
                std::cout<<"Apres rearrangement initial le vecteur kmeansClusterIds contient a la place "<<z<<" l'element"<<kmeansClusterIds[z]<<std::endl;
                }
            }
           
            std::vector<PersistenceDiagramTimeSeries> littleGeodesicSetClusters(0);
            std::vector<std::vector<double>> littleGeodesicTimesSetClusters(0);
           
            for(int z = 0;z<numberOfClusters;z++){
 
                std::vector<PersistenceDiagramTimeSeries> geodesicSetToBarycenter(0);
                std::vector<std::vector<double>> geodesicTimesSetToBarycenter(0);
 
                for (int j = 0; j < kmeansClusterIds.size(); j++) {
                    if (kmeansClusterIds[j] == z) {
                        geodesicSetToBarycenter.push_back(GeodesicSet[j]);
                        geodesicTimesSetToBarycenter.push_back(GeodesicTimesSet[j]);
                    }
                }
                 
                int geodesicSetToBarycenterSize = geodesicSetToBarycenter.size();
               
                std::vector<ttk::DiagramType> barycenterToBring;
                std::vector<double> barycenterTimeToBring;
                double myBestSumBarycenter(0);
 
                if(methodChoice==0){
                    totalStochasticBarycenterComputation(tau, geodesicSetToBarycenterSize, step, weight, geodesicSetToBarycenter, geodesicTimesSetToBarycenter, barycenterToBring, barycenterTimeToBring, pas, criteria, numberOfDeparturesStochasticBarycenterComputation, myBestSumBarycenter, threadNumber, geodesicForBarycenter);
                }
                else{
                    totalGreedyBarycenterComputation(tau, geodesicSetToBarycenterSize, greedySegmentation, step, weight, geodesicSetToBarycenter, geodesicTimesSetToBarycenter, barycenterToBring, barycenterTimeToBring, myBestSumBarycenter, threadNumber, geodesicForBarycenter);
                }
                littleGeodesicSetClusters.push_back(barycenterToBring);
                littleGeodesicTimesSetClusters.push_back(barycenterTimeToBring);
               
                std::cout<<"Z = "<<z<<std::endl;
                std::cout<< " size : "<<geodesicSetClusters.size()<<std::endl;
                 
            }
           
           geodesicSetClusters = littleGeodesicSetClusters;
           geodesicTimesSetClusters = littleGeodesicTimesSetClusters;
           
        }
       
    }
   
    greedyClustersToBring = bestCenters;
    greedyClustersTimeToBring = bestTimes;
    wcssToBring = bestWCSS;
    idsToBring = bestIds;
}

void MDSVisualization(std::vector<std::vector<ttk::DiagramType>> &GeodesicSet, std::vector<std::vector<double>> &GeodesicTimesSet, std::vector<std::vector<double>> &MDSMatrix, std::vector<std::vector<double>> &MDSMatrixVerification, int &threadNumber, double &weight, std::vector<std::vector<double>> &outputData_)
{
   
    ttk::DimensionReduction eta2;
   
    eta2.setIsInputDistanceMatrix(true);
    eta2.setInputNumberOfComponents(3);
    eta2.setInputNumberOfNeighbors(5);
   
    std::vector<ttk::DiagramType> vecMDS;
    std::vector<double> vecTempMDS;
   
    int totalLengthOfMDSMatrix = 0;
   
            std::cout<<"JE SUIS SORTIS DE LA 1"<<std::endl;
 
   
    for(int i = 0; i<GeodesicSet.size(); i++){
    
        totalLengthOfMDSMatrix = totalLengthOfMDSMatrix + GeodesicSet[i].size();
    
    }
                std::cout<<"JE SUIS SORTIS DE LA 12"<<std::endl;
    
    std::vector<double> vecTempForMDS(totalLengthOfMDSMatrix,0);
    
    for(int k = 0; k < totalLengthOfMDSMatrix;k++){
    
        MDSMatrix.push_back(vecTempForMDS);
        MDSMatrixVerification.push_back(vecTempForMDS);
    
    }
                std::cout<<"JE SUIS SORTIS DE LA 13"<<std::endl;
    for(int i = 0; i<GeodesicSet.size(); i++){
       
        for(int k = 0; k<GeodesicSet[i].size(); k++) {

                    vecMDS.push_back(GeodesicSet[i][k]);
                    vecTempMDS.push_back(GeodesicTimesSet[i][k]);
                   
        }
       
    }
                std::cout<<"JE SUIS SORTIS DE LA 14"<<std::endl;
    std::vector<std::vector<int>> parameterList;
                   
    for(int k = 0; k<vecMDS.size(); k++) {

        for(int l = k+1; l<vecMDS.size(); l++) {

            std::vector<int> toFillParameterList(2);
                           
            toFillParameterList[0] = k;
            toFillParameterList[1] = l;
                           
            parameterList.push_back(toFillParameterList);

            }

    }
                       
    #ifdef TTK_ENABLE_OPENMP
    #pragma omp parallel for schedule(dynamic) num_threads(threadNumber)
    #endif
    for(size_t t = 0; t < parameterList.size(); t++){

        int k = -1;
        int l = -1;

        k = parameterList[t][0];
        l = parameterList[t][1];

        MDSMatrix[k][l] = costMatrixComputation(k, l, weight, vecMDS, vecMDS, vecTempMDS, vecTempMDS);

    }


    for(int l = 0; l<vecMDS.size(); l++){

        for(int k = l+1; k<vecMDS.size(); k++){

            MDSMatrix[k][l] = MDSMatrix[l][k];

        }

    }

    std::vector<double> toGiveToDimensionReductionExecute;

    for(int k =0; k<MDSMatrix.size(); k++){

        for(int l = 0; l<MDSMatrix.size(); l++){

            toGiveToDimensionReductionExecute.push_back(MDSMatrix[k][l]);

        }

    }

    std::cout<<"JE SUIS SORTIS DE LA 2"<<std::endl;

    outputData_.clear();
    eta2.execute(outputData_,toGiveToDimensionReductionExecute,totalLengthOfMDSMatrix,totalLengthOfMDSMatrix);
    std::cout<<"JE SUIS SORTIS DE LA 3"<<std::endl;

               
    for(int i =0;i<MDSMatrix.size();i++){
                   
        for(int j =i;j<MDSMatrix.size();j++){
                       
            MDSMatrixVerification[i][j]=pow(pow(outputData_[0][i]-outputData_[0][j],2)+pow(outputData_[1][i]-outputData_[1][j],2)+pow(outputData_[2][i]-outputData_[2][j],2),0.5);
            MDSMatrixVerification[j][i]=MDSMatrixVerification[i][j];
                       
        }
                   
    }
    std::cout<<"JE SUIS SORTIS DE LA 4"<<std::endl;

}

std::pair< std::vector<int>, std::vector<std::vector<int>> > ttk::TimeVaryingPersistenceDiagramClustering::execute(const std::vector< std::vector< std::pair< ttk::DiagramType, double > > >& TemporalPersistenceDiagramTimeSeriesSet, double step, double weight, std::vector<std::vector<double>>& MDSMatrix, bool doMds,std::vector<std::vector<double>>& outputData_, std::vector<std::vector<double>>& MDSMatrixVerification, std::vector<ttk::DiagramType>& DiagramPersistenceVectorOfGeodesic1, std::vector<std::vector<double>>& timeStampGeodesic, std::vector<std::vector<int>>& Matching, std::vector<ttk::DiagramType>& TargetGeodesic, std::vector<double>& TargetTimeGeodesic, double geodesicCoefficient,  std::vector<ttk::DiagramType>& DiagramPersistenceVectorOfGeodesic2, std::vector<ttk::DiagramType>& stochBarycenterToBring,std::vector<double>& stochBarycenterTimeToBring, std::vector<ttk::DiagramType>& greedyBarycenterToBring, std::vector<double>& greedyBarycenterTimeToBring, double pas, int tau, double greedySegmentation, std::vector<PersistenceDiagramTimeSeries> &greedyClustersToBring, std::vector<std::vector<double>> &greedyClustersTimeToBring,  int &numberOfClusters, bool &kMeanPlusPlus, int &iterationsNumberKMeans, int &methodChoice, int &numberOfDeparturesStochasticBarycenterComputation, bool &criteria, std::vector <std::vector<std::vector<double>>> &kmeansMDSMatrices, std::vector <std::vector<std::vector<double>>> &kmeansMDSVerificationMatrices,  std::vector <std::vector<std::vector<double>>> &kmeansOutputDataMatrices_, std::vector<int> &kmeansClusterIds, std::vector<double> &kmeansDistanceToTheNearestCentroid, std::vector<std::vector<std::vector<int>>> &kmeansMatchingsToTheNearestCentroid, std::vector<std::vector<double>> &kmeansMatchingsCostToTheNearestCentroid, bool &byCluster, std::vector<int> &kmeansCurveSetSize, std::vector<std::vector<double>> &kmeansTimesOfCurveSet,  std::vector<std::vector<double>> &kmeansMatchingsCostsWithDiagonalCostsNotIncludedWithTheNearest, double &wcssToBring, std::vector<int> &idsToBring, int &geodesicForBarycenter, std::vector<std::vector<ttk::DiagramType>> &TVPD_set, int &selectedCoreCount, double &beta) const
{
   
    //setGlobalSeedTVPD(1285746800);
    setGlobalSeedTVPD(3288439930);
    
    std::cout << "Seed : " <<globalSeedTVPD()<<std::endl;
    
    if(selectedCoreCount<=threadNumber_ && selectedCoreCount >0){
        threadNumber_=selectedCoreCount;
    }
    
    std::cout<<"threadNumber_ = "<<threadNumber_<<std::endl;
   
    if(beta<0){
        beta=std::abs(beta);
    }
    
    if(beta>1){
        beta = 1;
    }
    
    if(beta==0){
        beta = 0.00001;
    }
   
    ttk::PersistenceDiagramClustering persistenceDiagramClustering;
    ttk::DimensionReduction eta;
   
    eta.setIsInputDistanceMatrix(true);
    eta.setInputNumberOfComponents(3);
    eta.setInputNumberOfNeighbors(5);
   
    ///////////////// Making Curves from time series
   
   
    //We recover the number of temporal persistence diagram time series in the considered set
    int cardinalOfTemporalPersistenceDiagramTimeSeriesSet = TemporalPersistenceDiagramTimeSeriesSet.size();

    //We create an instance to contain the set of the approximated geodesics, we create separately another instance to contain all the temporal indices of the approximated geodesics
    std::vector<PersistenceDiagramTimeSeries> GeodesicSet(cardinalOfTemporalPersistenceDiagramTimeSeriesSet);
    std::vector<std::vector<double>> GeodesicTimesSet(cardinalOfTemporalPersistenceDiagramTimeSeriesSet);
   
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
       
        timeStampGeodesic[i] = temporalSampling;

        //We create an instance to contain the approximated geodesic of the i-th temporal persistence diagram time series, and separately its time indices
        PersistenceDiagramTimeSeries Geodesic; // verifier initialisation correcte
        std::vector<double> GeodesicTimes; // verifier initialisation correcte

        //In this loop we calculate the partial approximated geodesic between each consecutive j-th pair of temporal persistence diagram of the i-th time series
        for(int j = 0; j < cardinalOfTemporalPersistenceDiagramTimeSeries-1; j++) {  // verifier boucle

            //We create instances to contain the j-th and j+1-th temporal persistence diagrams of the i-th temporal persistence diagram time series
            TemporalPersistenceDiagram Pairj = TemporalPersistenceDiagramTimeSeriesNumberi[j];
            TemporalPersistenceDiagram Pairjplus1 = TemporalPersistenceDiagramTimeSeriesNumberi[j+1];

            //We create a variable to store only the sampling steps, of our i-th approximated geodesic, restricted to the portion between the j-th temporal persistence diagram and the j+1-th
            std::vector<double> concernedTimes; // verifier initialisation correcte

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

    ///////////////// Making Curves from time series

   
    ///////////////// Making MDS Representation or Not MDS Representation
   
    int GeodesicSetSize =GeodesicSet.size();
    numberOfClusters = std::min(numberOfClusters, GeodesicSetSize-1);
       
    std::pair< std::vector<int>, std::vector<std::vector<int>> > TVDistanceMatrix;
   
    if(doMds == true){
               
        DiagramPersistenceVectorOfGeodesic1.clear();
        DiagramPersistenceVectorOfGeodesic2.clear();
                   
        std::vector<ttk::DiagramType> vecMDS;
        std::vector<double> vecTempMDS;
               
        PersistenceDiagramTimeSeries CompletedGeodesic1 = GeodesicSet[0];
        PersistenceDiagramTimeSeries CompletedGeodesic2 = GeodesicSet[1];
               
        std::vector<double> CompletedGeodesic1t = GeodesicTimesSet[0];
        std::vector<double> CompletedGeodesic2t = GeodesicTimesSet[1];
               
        std::vector<double> vecTempForMDS(CompletedGeodesic1.size()+CompletedGeodesic2.size(),0);
               
        for(int k = 0; k < CompletedGeodesic1.size()+CompletedGeodesic2.size();k++){
                   
            MDSMatrix.push_back(vecTempForMDS);
            MDSMatrixVerification.push_back(vecTempForMDS);
            
        }
               
        for(int k = 0; k<CompletedGeodesic1.size(); k++) {
           
                   
            vecMDS.push_back(CompletedGeodesic1[k]);
            vecTempMDS.push_back(CompletedGeodesic1t[k]);
            DiagramPersistenceVectorOfGeodesic1.push_back(CompletedGeodesic1[k]);
               
        }
               
        for(int l = 0; l<CompletedGeodesic2.size(); l++) {
               
            vecMDS.push_back(CompletedGeodesic2[l]);
            vecTempMDS.push_back(CompletedGeodesic2t[l]);
            DiagramPersistenceVectorOfGeodesic2.push_back(CompletedGeodesic2[l]);
            
        }
               
        for(int l = 0; l<vecMDS.size(); l++){
                   
            for(int k = l; k<vecMDS.size(); k++){
                       
                std::vector<ttk::DiagramType> vecTemporaire(2);
                vecTemporaire[0]=vecMDS[k];
                vecTemporaire[1]=vecMDS[l];
                ttk::PersistenceDiagramDistanceMatrix MatrixCalculator2;
                       
                std::array<size_t, 2> nInputs{2, 0};
                MatrixCalculator2.setDos(true, true, true);
                MatrixCalculator2.setThreadNumber(threadNumber_);
                std::vector<std::vector<double>> distMatrix = MatrixCalculator2.execute(vecTemporaire, nInputs);
                       
                MDSMatrix[l][k] = (1-weight)*distMatrix[0][1]+weight*std::abs(vecTempMDS[k]-vecTempMDS[l]);
                MDSMatrix[k][l] = MDSMatrix[l][k];
                       
            }
            
        }
               
        std::vector<double> toGiveToDimensionReductionExecute;
               
        for(int k =0; k<MDSMatrix.size();k++){
                   
            for(int l = 0;l<MDSMatrix.size();l++){
                       
                toGiveToDimensionReductionExecute.push_back(MDSMatrix[k][l]);
                       
            }
                   
        }
               
        outputData_.clear();
        eta.execute(outputData_,toGiveToDimensionReductionExecute,CompletedGeodesic1.size()+CompletedGeodesic2.size(),CompletedGeodesic1.size()+CompletedGeodesic2.size());
               
        for(int i =0;i<MDSMatrix.size();i++){
                   
            for(int j =i;j<MDSMatrix.size();j++){
                       
                MDSMatrixVerification[i][j]=pow(pow(outputData_[0][i]-outputData_[0][j],2)+pow(outputData_[1][i]-outputData_[1][j],2)+pow(outputData_[2][i]-outputData_[2][j],2),0.5);
                MDSMatrixVerification[j][i]=MDSMatrixVerification[i][j];
                       
            }
            
        }
        
    }
   
    ///////////////// Making MDS Representation or Not MDS Representation
   
    for(int i =0; i < GeodesicSet.size(); i++) {
       
        GeodesicSet[i].pop_back();
        GeodesicTimesSet[i].pop_back();
        TVPD_set.push_back(GeodesicSet[i]);
       
    }
   
    //We compute here, if GeodesicSet.size() == 2, the CED-matchings between GeodesicSet[0] and GeodesicSet[1] for the MDS/temporal representation
    if(GeodesicSet.size() == 2){

        std::vector<std::vector<double>> costMatrix(GeodesicSet[0].size(), std::vector<double>(GeodesicSet[1].size()));
               
        std::vector<double> TimesOfithGeodesic = GeodesicTimesSet[0];
        std::vector<double> TimesOfjthGeodesic = GeodesicTimesSet[1];
               
        std::vector<std::vector<int>> parameterList;
                       
        for(int k = 0; k<GeodesicSet[0].size(); k++) {

            for(int l = 0; l<GeodesicSet[1].size(); l++) {

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
                           
            costMatrix[k][l] = costMatrixComputation(k, l, weight, GeodesicSet[0], GeodesicSet[1], TimesOfithGeodesic, TimesOfjthGeodesic);
                           
        }
               
               
        std::vector<std::vector<int>> usedMatchings(0);
        std::vector<double> usedMatchingsCost(0);
        double distanceBetweenCurves = -1;
        std::vector<double> usedMatchingsCostsWithDiagonalCostsNotIncluded(0);
                   
        CGEDDistanceWithBeta(weight, step, costMatrix, GeodesicSet[0], GeodesicSet[1], true, false,  usedMatchings, usedMatchingsCost, distanceBetweenCurves, usedMatchingsCostsWithDiagonalCostsNotIncluded, beta);
        
        TVDistanceMatrix.second = usedMatchings;
        TVDistanceMatrix.first.push_back(GeodesicSet[0].size());
        TVDistanceMatrix.first.push_back(GeodesicSet[1].size());
        
        kmeansMatchingsCostToTheNearestCentroid.push_back(usedMatchingsCost);
       
    }
   
    //We compute here, if GeodesicSet.size() == 2, a CED-geodesic between GeodesicSet[0] and GeodesicSet[1]
    if(GeodesicSet.size() == 2){
       
        geodesicComputation(step, weight, TargetGeodesic, TargetTimeGeodesic, geodesicCoefficient, GeodesicSet[0], GeodesicSet[1], GeodesicTimesSet[0], GeodesicTimesSet[1], threadNumber_);
       
        TVPD_set.push_back(TargetGeodesic);
       
    }
   
    //We compute here, if GeodesicSet.size() > 2, the centroids of the TVPD sample
    if(GeodesicSet.size() > 2){
        
        kMeanComputationWithBuffer(tau, greedySegmentation, step, weight, GeodesicSet, GeodesicTimesSet, greedyClustersToBring, greedyClustersTimeToBring, numberOfClusters, kMeanPlusPlus, iterationsNumberKMeans, methodChoice, numberOfDeparturesStochasticBarycenterComputation, pas, criteria, threadNumber_, wcssToBring, idsToBring, geodesicForBarycenter);
   
        if(byCluster == false){
            int CurveSetSize = GeodesicSet.size() + greedyClustersToBring.size();
           
            std::vector<std::vector<ttk::DiagramType>> CurveSet(0);
           
            std::vector<std::vector<double>> TimeCurveSet(0);

            for(int i =0; i < GeodesicSet.size(); i++){
               
                CurveSet.push_back(GeodesicSet[i]);
                TimeCurveSet.push_back(GeodesicTimesSet[i]);
                kmeansCurveSetSize.push_back(GeodesicSet[i].size());
                kmeansTimesOfCurveSet.push_back(GeodesicTimesSet[i]);
           
            }
           
            for(int i =0; i < greedyClustersToBring.size(); i++){
               
                CurveSet.push_back(greedyClustersToBring[i]);
                TimeCurveSet.push_back(greedyClustersTimeToBring[i]);
                kmeansCurveSetSize.push_back(greedyClustersToBring[i].size());
                kmeansTimesOfCurveSet.push_back(greedyClustersTimeToBring[i]);
            }
           
            std::vector<std::vector<double>> toPutInKmeansMDSMatrices(0);
            std::vector<std::vector<double>> toPutInKmeansMDSVerificationMatrices(0);
            std::vector<std::vector<double>> toPutInKmeansOutputDataMatrices_(0);
       
       
            kmeansMDSMatrices.push_back(toPutInKmeansMDSMatrices);
            kmeansMDSVerificationMatrices.push_back(toPutInKmeansMDSVerificationMatrices);
            kmeansOutputDataMatrices_.push_back(toPutInKmeansOutputDataMatrices_);

            MDSVisualization(CurveSet, TimeCurveSet, kmeansMDSMatrices[0], kmeansMDSVerificationMatrices[0], threadNumber_, weight, kmeansOutputDataMatrices_[0]);
           
            for(int z =0;z<GeodesicSet.size();z++){
                   
                int closerIndexInSet = -1;
                double distanceToTheNearest = -1;
                std::vector<std::vector<int>> matchingsWithTheNearest(0);
                std::vector<double> matchingsCostWithTheNearest(0);
                bool withMatchingsOrNot = true;
                std::vector<double> matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest(0);

               
                distanceOfTargetFromSet(step, weight, greedyClustersToBring, greedyClustersTimeToBring,GeodesicSet[z],GeodesicTimesSet[z], threadNumber_, closerIndexInSet, distanceToTheNearest, matchingsWithTheNearest, matchingsCostWithTheNearest, withMatchingsOrNot, matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest);
               
               
                kmeansClusterIds.push_back(closerIndexInSet);
                kmeansDistanceToTheNearestCentroid.push_back(distanceToTheNearest);
                
                swapPairsInPlace(matchingsWithTheNearest);
                kmeansMatchingsToTheNearestCentroid.push_back(matchingsWithTheNearest);
                
                kmeansMatchingsCostToTheNearestCentroid.push_back(matchingsCostWithTheNearest);
                kmeansMatchingsCostsWithDiagonalCostsNotIncludedWithTheNearest.push_back(matchingsCostsWithDiagonalCostsNotIncludedWithTheNearest);
           
           
                std::cout<<"final le vecteur kmeansClusterIds contient a la place "<<z<<" l'element "<<kmeansClusterIds[z]<<std::endl;
                   
            }
       
            for(int z =0;z<greedyClustersToBring.size();z++){
         
                kmeansClusterIds.push_back(z);
           
            }
       
        }
        
    }
   
    return TVDistanceMatrix;
}
