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

#include <L2.h>
#include <vector>
#include <cmath>

void L2(int const& GeodesicSetISize, int const& GeodesicSetJSize, std::vector<std::vector<double>> const& costMatrix, double &distanceBetweenCurves){
    
    double distanceStep1L2=0;
    
    for(int i =0;i<GeodesicSetISize;i++){
        
        distanceStep1L2 = distanceStep1L2 + costMatrix[i][i];
        
    }
    
    
    distanceBetweenCurves=sqrt(distanceStep1L2);
    
}
