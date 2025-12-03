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

#pragma once

#include <array>

#include <Debug.h>
#include <PersistenceDiagramAuction.h>
#include <PersistenceDiagramUtils.h>
#include <PersistenceDiagramClustering.h>
#include <PersistenceDiagramDistanceMatrix.h>
#include <DimensionReduction.h>

#include <cmath>



typedef std::pair<ttk::DiagramType,double> TemporalPersistenceDiagram;
typedef std::vector<TemporalPersistenceDiagram> TemporalPersistenceDiagramTimeSeries;

typedef std::vector<ttk::DiagramType> PersistenceDiagramTimeSeries;

namespace ttk {

  class TimeVaryingPersistenceDiagramClustering : virtual public Debug {

  public:
    TimeVaryingPersistenceDiagramClustering() {
      this->setDebugMsgPrefix("TimeVaryingPersistenceDiagramClustering");
    }

    std::pair< std::vector<int>, std::vector<std::vector<int>> > execute(std::vector<std::vector<std::pair<ttk::DiagramType, double>>> const& TemporalPersistenceDiagramTimeSeriesSet, double step, double weight, std::vector<std::vector<double>>& MDSMatrix, bool doMds, std::vector<std::vector<double>>& outputData_, std::vector<std::vector<double>>& MDSMatrixVerification, std::vector<ttk::DiagramType>& DiagramPersistenceVectorOfGeodesic1, std::vector<std::vector<double>>& timeStampGeodesic, std::vector<std::vector<int>>& Matching, std::vector<ttk::DiagramType>& Geodesic2, std::vector<double>& GeodesicTimes2,  double geodesicCoefficient,  std::vector<ttk::DiagramType>& DiagramPersistenceVectorOfGeodesic2, std::vector<ttk::DiagramType>& stochBarycenterToBring,std::vector<double>& stochBarycenterTimeToBring, std::vector<ttk::DiagramType>& greedyBarycenterToBring, std::vector<double>& greedyBarycenterTimeToBring, double pas, int tau, double greedySegmentation, std::vector<PersistenceDiagramTimeSeries> &greedyClustersToBring, std::vector<std::vector<double>> &greedyClustersTimeToBring,  int &numberOfClusters, bool &kMeanPlusPlus, int &iterationsNumberKMeans,  int &methodChoice, int &numberOfDeparturesStochasticBarycenterComputation, bool &criteria, std::vector <std::vector<std::vector<double>>> &kmeansMDSMatrices, std::vector <std::vector<std::vector<double>>> &kmeansMDSVerificationMatrices,  std::vector <std::vector<std::vector<double>>> &kmeansOutputDataMatrices_, std::vector<int> &kmeansClusterIds, std::vector<double> &kmeansDistanceToTheNearestCentroid, std::vector<std::vector<std::vector<int>>> &kmeansMatchingsToTheNearestCentroid, std::vector<std::vector<double>> &kmeansMatchingsCostToTheNearestCentroid, bool &byCluster, std::vector<int> &kmeansCurveSetSize, std::vector<std::vector<double>> &kmeansTimesOfCurveSet,   std::vector<std::vector<double>> &kmeansMatchingsCostsWithDiagonalCostsNotIncludedWithTheNearest, double &wcssToBring, std::vector<int> &idsToBring, int &geodesicForBarycenter, std::vector<std::vector<ttk::DiagramType>> &TVPD_set, int &selectedCoreCount, double &beta) const;

    inline void setWasserstein(const int data) {
      Wasserstein = data;
    }
    inline void setDos(const bool min, const bool sad, const bool max) {
      do_min_ = min;
      do_sad_ = sad;
      do_max_ = max;
    }
    inline void setAlpha(const double alpha) {
      Alpha = alpha;
    }
    inline void setDeltaLim(const double deltaLim) {
      DeltaLim = deltaLim;
    }
    inline void setMaxNumberOfPairs(const size_t data) {
      MaxNumberOfPairs = data;
    }
    inline void setMinPersistence(const double data) {
      MinPersistence = data;
    }
    inline void setConstraint(const int data) {
      if(data == 0) {
        this->Constraint = ConstraintType::FULL_DIAGRAMS;
      } else if(data == 1) {
        this->Constraint = ConstraintType::NUMBER_PAIRS;
      } else if(data == 2) {
        this->Constraint = ConstraintType::ABSOLUTE_PERSISTENCE;
      } else if(data == 3) {
        this->Constraint = ConstraintType::RELATIVE_PERSISTENCE_PER_DIAG;
      } else if(data == 4) {
        this->Constraint = ConstraintType::RELATIVE_PERSISTENCE_GLOBAL;
      }
    }

  protected:
    double
      getMostPersistent(const std::vector<BidderDiagram> &bidder_diags) const;
    double computePowerDistance(const BidderDiagram &D1,
                                const BidderDiagram &D2) const;
    void getDiagramsDistMat(const std::array<size_t, 2> &nInputs,
                            std::vector<std::vector<double>> &distanceMatrix,
                            const std::vector<BidderDiagram> &diags_min,
                            const std::vector<BidderDiagram> &diags_sad,
                            const std::vector<BidderDiagram> &diags_max) const;
    void setBidderDiagrams(const size_t nInputs,
                           std::vector<DiagramType> &inputDiagrams,
                           std::vector<BidderDiagram> &bidder_diags) const;

    void enrichCurrentBidderDiagrams(
      const std::vector<BidderDiagram> &bidder_diags,
      std::vector<BidderDiagram> &current_bidder_diags,
      const std::vector<double> &maxDiagPersistence) const;

    int Wasserstein{2};
    double Alpha{1.0};
    double DeltaLim{0.01};
    size_t MaxNumberOfPairs{20};
    double MinPersistence{0.1};
    bool do_min_{true}, do_sad_{true}, do_max_{true};

    enum class ConstraintType {
      FULL_DIAGRAMS,
      NUMBER_PAIRS,
      ABSOLUTE_PERSISTENCE,
      RELATIVE_PERSISTENCE_PER_DIAG,
      RELATIVE_PERSISTENCE_GLOBAL,
    };
    ConstraintType Constraint{ConstraintType::RELATIVE_PERSISTENCE_GLOBAL};
  };
} // namespace ttk
