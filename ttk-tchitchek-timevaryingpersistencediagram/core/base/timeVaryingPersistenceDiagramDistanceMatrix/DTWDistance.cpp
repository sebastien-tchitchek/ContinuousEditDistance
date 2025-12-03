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

#include <DTWDistance.h>
#include <vector>
#include <cmath>
#include <algorithm>

void DTWDistance(int const& GeodesicSetISize, int const& GeodesicSetJSize, std::vector<std::vector<double>> const& costMatrix, double &distanceBetweenCurves){
            
    const size_t rows = static_cast<size_t>(GeodesicSetISize) + 1;
    const size_t cols = static_cast<size_t>(GeodesicSetJSize) + 1;
    
    std::vector<double> DynP3(rows * cols, 0.0);

            
    auto idx = [&](size_t r, size_t c) -> double& {
        return DynP3[r * cols + c];
    };

    idx(0,0) = 0.0;

    for(size_t q = 1; q < rows; ++q) {
        idx(q,0) = INFINITY;
    }

    for(size_t s = 1; s < cols; ++s) {
        idx(0,s) = INFINITY;
    }

    for(size_t r = 1; r < rows; ++r) {
        for(size_t t = 1; t < cols; ++t) {
            double cost  = costMatrix[r-1][t-1];
            double del   = idx(r-1,t) + cost;
            double add   = idx(r,t-1) + cost;
            double match = idx(r-1,t-1) + cost;
            
            double temp = std::min(del,add);
            idx(r,t) = std::min(match,temp);
        }
    }

    distanceBetweenCurves=idx(rows-1, cols-1);
}
