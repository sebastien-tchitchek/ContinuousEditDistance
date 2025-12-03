/// \ingroup vtk
/// \class ttkTimeVaryingPersistenceDiagramClustering
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

// VTK includes
#include <vtkInformation.h>
#include <vtkInformationVector.h>

// VTK Module
#include <ttkTimeVaryingPersistenceDiagramClusteringModule.h>

// ttk code includes
#include <TimeVaryingPersistenceDiagramClustering.h>
#include <ttkAlgorithm.h>

class TTKTIMEVARYINGPERSISTENCEDIAGRAMCLUSTERING_EXPORT
  ttkTimeVaryingPersistenceDiagramClustering
  : public ttkAlgorithm,
    protected ttk::TimeVaryingPersistenceDiagramClustering {

public:
  static ttkTimeVaryingPersistenceDiagramClustering *New();

  vtkTypeMacro(ttkTimeVaryingPersistenceDiagramClustering, ttkAlgorithm);


  vtkSetMacro(TimestepColumnName, const std::string &);
  vtkGetMacro(TimestepColumnName, std::string);
  
  void SetWassersteinMetric(const std::string &data) {
    Wasserstein = (data == "inf") ? -1 : stoi(data);
    Modified();
  }
  std::string GetWassersteinMetric() {
    return Wasserstein == -1 ? "inf" : std::to_string(Wasserstein);
  }

  void SetAntiAlpha(double data) {
    data = 1 - data;
    if(data > 0 && data <= 1) {
      Alpha = data;
    } else if(data > 1) {
      Alpha = 1;
    } else {
      Alpha = 0.001;
    }
    Modified();
  }
  vtkGetMacro(Alpha, double);

  vtkSetMacro(DeltaLim, double);
  vtkGetMacro(DeltaLim, double);

  void SetPairType(const int data) {
    switch(data) {
      case(0):
        this->setDos(true, false, false);
        break;
      case(1):
        this->setDos(false, true, false);
        break;
      case(2):
        this->setDos(false, false, true);
        break;
      default:
        this->setDos(true, true, true);
        break;
    }
    Modified();
  }
  int GetPairType() {
    if(do_min_ && do_sad_ && do_max_) {
      return -1;
    } else if(do_min_) {
      return 0;
    } else if(do_sad_) {
      return 1;
    } else if(do_max_) {
      return 2;
    }
    return -1;
  }

  void SetConstraint(const int arg_) {
    this->setConstraint(arg_);
    this->Modified();
  }
  int GetConstraint() {
    switch(this->Constraint) {
      case ConstraintType::FULL_DIAGRAMS:
        return 0;
      case ConstraintType::NUMBER_PAIRS:
        return 1;
      case ConstraintType::ABSOLUTE_PERSISTENCE:
        return 2;
      case ConstraintType::RELATIVE_PERSISTENCE_PER_DIAG:
        return 3;
      case ConstraintType::RELATIVE_PERSISTENCE_GLOBAL:
        return 4;
    }
    return -1;
  }

  vtkSetMacro(MaxNumberOfPairs, unsigned int);
  vtkGetMacro(MaxNumberOfPairs, unsigned int);

  vtkSetMacro(MinPersistence, double);
  vtkGetMacro(MinPersistence, double);
  
  
    void Settau(int newtau) {
    tau = newtau;
    Modified();
  }
  vtkGetMacro(tau, int);
  
    void SetgeodesicForBarycenter(int newgeodesicForBarycenter) {
    geodesicForBarycenter = newgeodesicForBarycenter;
    Modified();
  }
  vtkGetMacro(geodesicForBarycenter, int);
  
    void SetselectedCoreCount(int newselectedCoreCount) {
    selectedCoreCount = newselectedCoreCount;
    Modified();
  }
  vtkGetMacro(selectedCoreCount, int);
  
     void SetWeight(double newWeight) {
    Weight = newWeight;
    Modified();
  }
  vtkGetMacro(Weight, double);
  
     void SetgeodesicCoefficient(double newgeodesicCoefficient) {
    geodesicCoefficient = newgeodesicCoefficient;
    Modified();
  }
  vtkGetMacro(geodesicCoefficient, double);
  
       void Setpas(double newpas) {
    pas = newpas;
    Modified();
  }
  vtkGetMacro(pas, double);
  
     void SetMDS(int newMDS) {
    MDS = newMDS;
    Modified();
  }
  vtkGetMacro(MDS, int);
  
     void SetgreedySegmentation(double newgreedySegmentation) {
    greedySegmentation = newgreedySegmentation;
    Modified();
  }
  vtkGetMacro(greedySegmentation, double);
  
       void Setdelta(double newdelta) {
    delta = newdelta;
    Modified();
  }
  vtkGetMacro(delta, double);
  
       void SetverticalSpace(double newverticalSpace) {
    verticalSpace = newverticalSpace;
    Modified();
  }
  vtkGetMacro(verticalSpace, double);
  
       void SetnumberOfClusters(int newnumberOfClusters) {
    numberOfClusters = newnumberOfClusters;
    Modified();
  }
  vtkGetMacro(numberOfClusters, int);
  
       void SetkMeanPlusPlus(int newkMeanPlusPlus) {
    kMeanPlusPlus = newkMeanPlusPlus;
    Modified();
  }
  vtkGetMacro(kMeanPlusPlus, int);
  
       void SetiterationsNumberKMeans(int newiterationsNumberKMeans) {
    iterationsNumberKMeans = newiterationsNumberKMeans;
    Modified();
  }
  vtkGetMacro(iterationsNumberKMeans, int);
  
     void SetnumberOfDeparturesStochasticBarycenterComputation(int newnumberOfDeparturesStochasticBarycenterComputation) {
    numberOfDeparturesStochasticBarycenterComputation = newnumberOfDeparturesStochasticBarycenterComputation;
    Modified();
  }
  vtkGetMacro(numberOfDeparturesStochasticBarycenterComputation, int);
  
    void SetmethodChoice(int newmethodChoice) {
    methodChoice = newmethodChoice;
    Modified();
  }
  vtkGetMacro(methodChoice, int);
  
    void Setcriteria(int newcriteria) {
    criteria = newcriteria;
    Modified();
  }
  vtkGetMacro(criteria, int);
  
    void SetwithChronologicalDeparture(int newwithChronologicalDeparture) {
    withChronologicalDeparture = newwithChronologicalDeparture;
    Modified();
  }
  vtkGetMacro(withChronologicalDeparture, int);
  
    void Setbeta(double newbeta) {
    beta = newbeta;
    Modified();
  }
  vtkGetMacro(beta, double);
  
    void SetiterationsNumberSmoothing(int newiterationsNumberSmoothing) {
    iterationsNumberSmoothing = newiterationsNumberSmoothing;
    Modified();
  }
  vtkGetMacro(iterationsNumberSmoothing, int);
  
  
    void SetantiShrink(int newantiShrink) {
    antiShrink = newantiShrink;
    Modified();
  }
  vtkGetMacro(antiShrink, int);
  
    void SetsmoothChoice(int newsmoothChoice) {
    smoothChoice = newsmoothChoice;
    Modified();
  }
  vtkGetMacro(smoothChoice, int);
  
  
    void SetlambdaTaubinParameter(double newlambdaTaubinParameter) {
    lambdaTaubinParameter = newlambdaTaubinParameter;
    Modified();
  }
  vtkGetMacro(lambdaTaubinParameter, double);
  
protected:
  ttkTimeVaryingPersistenceDiagramClustering();
  ~ttkTimeVaryingPersistenceDiagramClustering() override = default;

  int FillInputPortInformation(int port, vtkInformation *info) override;
  int FillOutputPortInformation(int port, vtkInformation *info) override;

  int RequestData(vtkInformation *request,
                  vtkInformationVector **inputVector,
                  vtkInformationVector *outputVector) override;
                  
  private:
  std::string TimestepColumnName{"TimeStep"};
  int MDS = 0;
  int numberOfDeparturesStochasticBarycenterComputation = 3;
  int numberOfClusters = 2;
  int kMeanPlusPlus = 1;
  int iterationsNumberKMeans = 3;
  int methodChoice = 0;
  int criteria = 0;
  int tau = 1;
  int geodesicForBarycenter = 0;
  int selectedCoreCount = 999;
  int withChronologicalDeparture = 0;
  int iterationsNumberSmoothing = 0;
  int antiShrink = 1;
  int smoothChoice = 0;
  double Weight = 0.0;
  double geodesicCoefficient = 0.5;
  double pas = 0.5;
  double greedySegmentation = 2;
  double delta = 0.25;
  double verticalSpace = 5;
  double beta = 1;
  double lambdaTaubinParameter = 0.5;

};
