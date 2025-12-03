#include <cmath>
#include <CGEDDistanceMatrix.h>
#include <vector>
#include <deque>
#include <algorithm>

void CGEDDistanceMatrix( double &weight, double const &step, std::vector<std::vector<double>> const &costMatrix, std::vector<ttk::DiagramType> const &GeodesicSeti, std::vector<ttk::DiagramType> const &GeodesicSetj, bool assignements, bool withDiagonalMatching, std::vector<std::vector<int>> &matching, std::vector<double> &matchingsCost, double &distanceBetweenCurves, std::vector<double> &matchingsCostsWithDiagonalCostsIncluded, double &beta){

            std::vector<double> PersistenceOfCurvei;
            std::vector<double> PersistenceOfCurvej;

            double valeur(-1);

            for(int iterateur1 = 0; iterateur1<GeodesicSeti.size(); iterateur1++) {

                double distanceToEmptyDiagram(0);

                for(int iterateur2 = 0; iterateur2<GeodesicSeti[iterateur1].size(); iterateur2++) {
                        
                        double di = GeodesicSeti[iterateur1][iterateur2].death.sfValue;
                        double bi = GeodesicSeti[iterateur1][iterateur2].birth.sfValue;
                        
                        double distanceToDiagonalStep1 = ((di-bi)/sqrt(2));
                        double distanceToDiagonalStep2 = pow(distanceToDiagonalStep1,2);

                        distanceToEmptyDiagram = distanceToEmptyDiagram + distanceToDiagonalStep2;
                    
                }
                
                PersistenceOfCurvei.push_back(sqrt(distanceToEmptyDiagram));

            }

            for(int iterateur1 = 0; iterateur1<GeodesicSetj.size(); iterateur1++) {

                double distanceToEmptyDiagram(0);

                for(int iterateur2 = 0; iterateur2<GeodesicSetj[iterateur1].size(); iterateur2++) {
                        
                        double di = GeodesicSetj[iterateur1][iterateur2].death.sfValue;
                        double bi = GeodesicSetj[iterateur1][iterateur2].birth.sfValue;
                        
                        double distanceToDiagonalStep1 = ((di-bi)/sqrt(2));
                        double distanceToDiagonalStep2 = pow(distanceToDiagonalStep1,2);

                        distanceToEmptyDiagram = distanceToEmptyDiagram + distanceToDiagonalStep2;
                        
                }
                
                PersistenceOfCurvej.push_back(sqrt(distanceToEmptyDiagram));

            }
            
            const size_t rows = GeodesicSeti.size() + 1;
            const size_t cols = GeodesicSetj.size() + 1;
            
            std::vector<double> DynP2(rows * cols, 0.0);
            
            auto idx = [&](size_t r, size_t c) -> double& {
                return DynP2[r * cols + c];
            };

            idx(0,0) = 0;

            for(size_t q = 1; q < rows; q++) {

                idx(q,0)= idx(q-1,0)+(1-weight)*PersistenceOfCurvei[q-1]*(step)*beta;

            }

            for(size_t s = 1; s < cols; s++) {

                idx(0,s)= idx(0,s-1)+(1-weight)*PersistenceOfCurvej[s-1]*(step)*beta;

            }

            for(size_t r = 1; r < rows; r++) {

                for(size_t t = 1; t < cols; t++) {

                    double cost  = costMatrix[r-1][t-1];
                    double del = idx(r-1,t)+(1-weight)*PersistenceOfCurvei[r-1]*(step)*beta;
                    double add = idx(r,t-1)+(1-weight)*PersistenceOfCurvej[t-1]*(step)*beta;
                    double match = idx(r-1,t-1)+cost*(step);

                    double temp = std::min(del,add);
                    idx(r,t) = std::min(match,temp);

                }

            }

            valeur = idx(rows-1, cols-1);

            distanceBetweenCurves = valeur;
            
            if(assignements == true){

                std::deque<std::vector<int>> Assignement;
                std::deque<std::vector<int>> TempMatching;
                
                int Mx(GeodesicSeti.size());
                int My(GeodesicSetj.size());
                
                while(Mx != 0 || My != 0){
                    
                    if(Mx==0 && My != 0){
                            
                            std::vector<int> AssignementToAdd2(2);
                            AssignementToAdd2[0] = -1;
                            AssignementToAdd2[1] = My;
                            TempMatching.push_front(AssignementToAdd2);
                                    
                            if(withDiagonalMatching == true){
                                std::vector<int> AssignementToAdd(2);
                                AssignementToAdd[0] = -1;
                                AssignementToAdd[1] = My;
                                Assignement.push_front(AssignementToAdd);
                            }
                            My = My -1;
                    }
                    else if(Mx !=0 && My == 0){
                        
                            std::vector<int> AssignementToAdd2(2);
                            AssignementToAdd2[0] = Mx;
                            AssignementToAdd2[1] = -1;
                            TempMatching.push_front(AssignementToAdd2);
                                
                            if(withDiagonalMatching == true){   
                                std::vector<int> AssignementToAdd(2);
                                AssignementToAdd[0] = Mx;
                                AssignementToAdd[1] = -1;
                                Assignement.push_front(AssignementToAdd);
                            }
                            Mx = Mx -1;
                    }
                    
                    else if(Mx !=0 && My != 0){
                        
                        double min1 = std::abs(idx(Mx,My)-idx(Mx-1,My-1)-costMatrix[Mx-1][My-1]*(step));
                        double min2 = std::abs(idx(Mx,My)-idx(Mx-1,My)-(1-weight)*PersistenceOfCurvei[Mx-1]*(step)*beta);
                        double min3 = std::abs(idx(Mx,My)-idx(Mx,My-1)-(1-weight)*PersistenceOfCurvej[My-1]*(step)*beta);
                        
                        if( min1<= min2 && min1<= min3 ){
                                        
                            std::vector<int> AssignementToAdd(2);
                                                        
                            AssignementToAdd[0] = Mx;
                                                        
                            AssignementToAdd[1] = My;
                                                        
                            Assignement.push_front(AssignementToAdd);
                            TempMatching.push_front(AssignementToAdd);

                            Mx = Mx -1;
                            My = My -1;
                            
                        }

                        else if( min2 <= min1 && min2 <= min3 ){
                            
                            std::vector<int> AssignementToAdd2(2);
                            AssignementToAdd2[0] = Mx;
                            AssignementToAdd2[1] = -1;
                            TempMatching.push_front(AssignementToAdd2);
                            
                            if(withDiagonalMatching == true){

                                std::vector<int> AssignementToAdd(2);
                                AssignementToAdd[0] = Mx;
                                AssignementToAdd[1] = -1;
                                Assignement.push_front(AssignementToAdd);
                            }
                            
                            Mx = Mx -1;
                        }
                        
                        else if( min3<=min1 && min3<=min2 ){
                            
                            std::vector<int> AssignementToAdd2(2);
                            AssignementToAdd2[0] = -1;
                            AssignementToAdd2[1] = My;
                            TempMatching.push_front(AssignementToAdd2);
                            
                            if(withDiagonalMatching == true){    
                                std::vector<int> AssignementToAdd(2);
                                AssignementToAdd[0] = -1;
                                AssignementToAdd[1] = My;
                                Assignement.push_front(AssignementToAdd);
                            }
                            My = My -1;
                            
                        }
                                    
                    }
                    
                }
                
                for(int i = 0; i < Assignement.size(); i++){
                                
                    matching.push_back(Assignement[i]);
                    
                }
                
                for(int i = 0; i < matching.size(); i++){

                    int a = matching[i][0]-1;
                    int b = matching[i][1]-1;
                    
                    if(a == -2 || b == -2){
                        
                        if(a == -2){
                            
                            matchingsCostsWithDiagonalCostsIncluded.push_back(PersistenceOfCurvej[b]*(step)*(1-weight)*beta);
                            
                        }
                        else if(b == -2){
                            
                            matchingsCostsWithDiagonalCostsIncluded.push_back(PersistenceOfCurvei[a]*(step)*(1-weight)*beta);
                            
                        }
                        
                    }
                    else{
                        
                        matchingsCost.push_back(costMatrix[a][b]*(step));         
                        matchingsCostsWithDiagonalCostsIncluded.push_back(costMatrix[a][b]*(step));
                        
                    }
                    
                }
                
            }

}
