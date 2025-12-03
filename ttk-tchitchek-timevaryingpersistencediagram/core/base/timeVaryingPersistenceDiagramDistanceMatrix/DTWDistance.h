#pragma once

#include <vector>

void DTWDistance(int const& GeodesicSetISize, int const& GeodesicSetJSize, std::vector<std::vector<double>> const& costMatrix, double &distanceBetweenCurves);
