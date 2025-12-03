#pragma once

#include <vector>

void FrechetDistance(int const& GeodesicSetISize, int const& GeodesicSetJSize, std::vector<std::vector<double>> const& costMatrix, double &distanceBetweenCurves);
