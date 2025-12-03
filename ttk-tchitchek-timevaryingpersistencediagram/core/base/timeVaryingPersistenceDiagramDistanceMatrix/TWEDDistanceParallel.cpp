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

#include <TWEDDistanceParallel.h>
#include <vector>
#include <cmath>
#include <limits>

void TWEDDistance2(double weight, double step, std::vector<std::vector<double>> const& costMatrix, std::vector<ttk::DiagramType> const& GeodesicSeti, std::vector<ttk::DiagramType> const& GeodesicSetj, std::vector<double>& GeodesicSetiTime, std::vector<double>& GeodesicSetjTime, double &distanceBetweenCurves, int &threadNumber){

            std::vector<double> PersistenceOfCurvei;
            std::vector<double> PersistenceOfCurvej;


            for(int iterateur1 = 0; iterateur1<GeodesicSeti.size(); iterateur1++) {

                
                double distanceToEmptyDiagram(0);

                for(int iterateur2 = 0; iterateur2<GeodesicSeti[iterateur1].size(); iterateur2++) {

                    if(GeodesicSeti[iterateur1][iterateur2].isFinite == true){
                        
                        double di = GeodesicSeti[iterateur1][iterateur2].death.sfValue;
                        double bi = GeodesicSeti[iterateur1][iterateur2].birth.sfValue;
                        
                        double distanceToDiagonalStep1 = ((di-bi)/sqrt(2));
                        double distanceToDiagonalStep2 = pow(distanceToDiagonalStep1,2);

                        distanceToEmptyDiagram = distanceToEmptyDiagram + distanceToDiagonalStep2;
                        
                    }
                    
                }
                
                PersistenceOfCurvei.push_back(sqrt(distanceToEmptyDiagram));

            }


            for(int iterateur1 = 0; iterateur1<GeodesicSetj.size(); iterateur1++) {
               
                double distanceToEmptyDiagram(0);

                for(int iterateur2 = 0; iterateur2<GeodesicSetj[iterateur1].size(); iterateur2++) {

                    if(GeodesicSetj[iterateur1][iterateur2].isFinite == true){
                        
                        double di = GeodesicSetj[iterateur1][iterateur2].death.sfValue;
                        double bi = GeodesicSetj[iterateur1][iterateur2].birth.sfValue;
                        
                        double distanceToDiagonalStep1 = ((di-bi)/sqrt(2));
                        double distanceToDiagonalStep2 = pow(distanceToDiagonalStep1,2);

                        distanceToEmptyDiagram = distanceToEmptyDiagram + distanceToDiagonalStep2;
                        
                    }
                }
                
                PersistenceOfCurvej.push_back(sqrt(distanceToEmptyDiagram));

            }
            
            
            
            std::vector<double> intraDistanceI;
            std::vector<double> intraDistanceJ;
            
            
            for(int q = 0; q<GeodesicSeti.size()-1; q++) {

                std::vector<ttk::DiagramType> vec2(2);
                vec2[0]=GeodesicSeti[q];
                vec2[1]=GeodesicSeti[q+1];
                ttk::PersistenceDiagramDistanceMatrix MatrixCalculator2;
                std::array<size_t, 2> nInputs{2, 0};
                MatrixCalculator2.setDos(true, true, true); 
                MatrixCalculator2.setThreadNumber(threadNumber); 
                std::vector<std::vector<double>> distMatrix = MatrixCalculator2.execute(vec2, nInputs);
                
                double TemporalDistanceBetweenKAndL;

                TemporalDistanceBetweenKAndL = (1-weight)*distMatrix[0][1]+weight*std::abs(GeodesicSetiTime[q]-GeodesicSetiTime[q+1]);
                
                intraDistanceI.push_back(TemporalDistanceBetweenKAndL);
            }
            
            for(int s = 0; s<GeodesicSetj.size()-1; s++) {

                std::vector<ttk::DiagramType> vec2(2);
                vec2[0]=GeodesicSetj[s];
                vec2[1]=GeodesicSetj[s+1];
                ttk::PersistenceDiagramDistanceMatrix MatrixCalculator2;
                std::array<size_t, 2> nInputs{2, 0};
                MatrixCalculator2.setDos(true, true, true); 
                MatrixCalculator2.setThreadNumber(threadNumber); 
                std::vector<std::vector<double>> distMatrix = MatrixCalculator2.execute(vec2, nInputs);
                
                double TemporalDistanceBetweenKAndL;

                TemporalDistanceBetweenKAndL = (1-weight)*distMatrix[0][1]+weight*std::abs(GeodesicSetjTime[s]-GeodesicSetjTime[s+1]);
                
                intraDistanceJ.push_back(TemporalDistanceBetweenKAndL);
            }
            
            
            const size_t rows = GeodesicSeti.size() + 1;
            const size_t cols = GeodesicSetj.size() + 1;
            
            std::vector<double> DynP4(rows * cols, 0.0);
            
            auto idx = [&](size_t r, size_t c) -> double& {
                return DynP4[r * cols + c];
            };

            idx(0, 0) = 0.0;

            for(size_t q = 1; q < rows; ++q) {

                idx(q, 0)= INFINITY;

            }


            for(size_t s = 1; s < cols; ++s) {

                idx(0, s)= INFINITY;

            }

            for(size_t r = 1; r < rows; ++r) {

                for(size_t t = 1; t < cols; ++t) {


                    double cost  = costMatrix[r-1][t-1];
                    double cost2;

                    double costr;
                    double costt;
                    

                    if(r==1 && t== 1) {
                        
                        cost2  = 0;
                        costr = 0;
                        costt = 0;

                    }

                    else if(r==1 && t!= 1) {
                        
                        cost2 = 0;
                        costr = 0;
                        
                        costt = intraDistanceJ[t-2];
                    }

                    else if(r!=1 && t==1) {
                        
                        cost2 = 0;
                        
                        costr = intraDistanceI[r-2];

                        costt = 0;

                    }

                    else if(r !=1 && t !=1) {
                        
                        cost2 = costMatrix[r-2][t-2];

                        costr = intraDistanceI[r-2];
                        
                        costt = intraDistanceJ[t-2];
                        
                    }


                    double del = idx(r-1, t)+costr;
                    double add = idx(r,   t-1)+costt;
                    double match = idx(r-1, t-1)+cost+cost2;

                    double temp = std::min(del,add);
                    idx(r, t) = std::min(match,temp);


                }

            }

            distanceBetweenCurves=idx(rows - 1, cols - 1);

}
