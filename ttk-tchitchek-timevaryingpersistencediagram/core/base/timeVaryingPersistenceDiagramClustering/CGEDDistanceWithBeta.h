#pragma once
#include "PersistenceDiagramUtils.h"
#include <vector>


void CGEDDistanceWithBeta( double &weight, double const &step, std::vector<std::vector<double>> const &costMatrix, std::vector<ttk::DiagramType> const &GeodesicSeti, std::vector<ttk::DiagramType> const &GeodesicSetj, bool assignements, bool withDiagonalMatching, std::vector<std::vector<int>> &matching, std::vector<double> &matchingsCost, double &distanceBetweenCurves, std::vector<double> &matchingsCostsWithDiagonalCostsIncluded, double &beta);
