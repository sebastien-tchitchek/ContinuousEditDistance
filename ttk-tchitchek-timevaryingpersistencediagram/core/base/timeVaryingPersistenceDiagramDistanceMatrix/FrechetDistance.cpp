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

#include <FrechetDistance.h>
#include <vector>
#include <cmath>
#include <algorithm> 

void FrechetDistance(int const& GeodesicSetISize, int const& GeodesicSetJSize, std::vector<std::vector<double>> const& costMatrix, double &distanceBetweenCurves){
            
    const size_t rows = static_cast<size_t>(GeodesicSetISize);
    const size_t cols = static_cast<size_t>(GeodesicSetJSize);
    
    std::vector<double> DynP5(rows * cols, 0.0);

    auto idx = [&](size_t r, size_t c) -> double& {
        return DynP5[r * cols + c];
    };
    
    idx(0,0)=costMatrix[0][0];

    for(size_t q = 1; q < rows; ++q) {

        idx(q,0)= std::max(costMatrix[q][0],idx(q-1,0));

    }

    for(size_t s = 1; s < cols; ++s) {

        idx(0,s)= std::max(costMatrix[0][s],idx(0,s-1));

    }

    for(size_t r = 1; r < rows; ++r) {

        for(size_t t = 1; t < cols; ++t) {

            double cost  = costMatrix[r][t];
            double del = idx(r-1,t);
            double add = idx(r,t-1);
            double match = idx(r-1,t-1);

            double temp = std::min(del,add);
            double temp2 = std::min(match,temp);

            idx(r,t) = std::max(temp2,cost);
        }

    }

    distanceBetweenCurves = idx(rows-1, cols-1);
}
