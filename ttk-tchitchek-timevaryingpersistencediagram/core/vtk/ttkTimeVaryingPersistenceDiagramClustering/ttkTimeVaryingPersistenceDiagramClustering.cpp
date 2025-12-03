#include <ttkTimeVaryingPersistenceDiagramClustering.h>
#include <ttkPersistenceDiagramUtils.h>
#include <ttkDimensionReduction.h>
#include <ttkDataSetToTable.h>
#include <ttkGeometrySmoother.h>

#include <vtkCellData.h>
#include <vtkCharArray.h>
#include <vtkDataArray.h>
#include <vtkDataSet.h>
#include <vtkDoubleArray.h>
#include <vtkFiltersCoreModule.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkMultiBlockDataSet.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>
#include <vtkPointData.h>
#include <vtkStringArray.h>
#include <vtkTable.h>
#include <vtkUnstructuredGrid.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>




using namespace ttk;

vtkStandardNewMacro(ttkTimeVaryingPersistenceDiagramClustering);

ttkTimeVaryingPersistenceDiagramClustering::ttkTimeVaryingPersistenceDiagramClustering() {
    SetNumberOfInputPorts(1);
    SetNumberOfOutputPorts(8);
}

int ttkTimeVaryingPersistenceDiagramClustering::FillInputPortInformation(
    int port, vtkInformation *info) {
    if(port == 0) {
        info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkMultiBlockDataSet");
        info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
        return 1;
    }
    return 0;
}

int ttkTimeVaryingPersistenceDiagramClustering::FillOutputPortInformation(int port,
                                                     vtkInformation *info) {
  if(port == 0 || port == 1 || port == 2 || port == 3|| port == 6 || port == 7) {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkMultiBlockDataSet");
  } else if(port == 4 or port == 5) {
    info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkTable");
  } else {
    return 0;
  }
  return 1;  
}

static vtkSmartPointer<vtkPolyData> makePolyDataFromPolyline(const std::vector<std::vector<double>> &pts, bool closed) {
    
  const vtkIdType n = static_cast<vtkIdType>(pts.size());

  auto pd     = vtkSmartPointer<vtkPolyData>::New();
  auto points = vtkSmartPointer<vtkPoints>::New();
  
  points->SetDataTypeToDouble();
  points->SetNumberOfPoints(n);

  for(vtkIdType i = 0; i < n; ++i) {
      
    const double x = (pts[i].size() > 0 ? pts[i][0] : 0.0);
    const double y = (pts[i].size() > 1 ? pts[i][1] : 0.0);
    const double z = (pts[i].size() > 2 ? pts[i][2] : 0.0);
    
    points->SetPoint(i, x, y, z);
    
  }

  auto lines = vtkSmartPointer<vtkCellArray>::New();
  
  if(n >= 2) {

    for(vtkIdType i = 0; i + 1 < n; ++i) {
        
      vtkIdType seg[2] = {i, i + 1};
      lines->InsertNextCell(2, seg);
      
    }

    if(closed && n > 2) {
        
      vtkIdType seg[2] = {n - 1, 0};
      lines->InsertNextCell(2, seg);
      
    }
    
  }

  pd->SetPoints(points);
  pd->SetLines(lines);
  
  return pd;
}

static void smoothPolylineWithTTK_VTK( const std::vector<std::vector<double>> &pts, int iterations, std::vector<std::vector<double>> &outPts, bool closed = false, bool pinEnds = true) 
{
  const vtkIdType n = static_cast<vtkIdType>(pts.size());
  outPts.clear();
  
  if(n == 0) return;

  
  auto poly = makePolyDataFromPolyline(pts, closed);

  
  if(pinEnds && !closed && n >= 2) {
      
    auto mask = vtkSmartPointer<vtkCharArray>::New();
    
    mask->SetName("Mask");
    mask->SetNumberOfComponents(1);
    mask->SetNumberOfTuples(n);
    
    for(vtkIdType i = 0; i < n; ++i) mask->SetValue(i, 1);
    
    mask->SetValue(0, 0);
    mask->SetValue(n - 1, 0);
    poly->GetPointData()->AddArray(mask);
    
  }

  
  auto smoother = vtkSmartPointer<ttkGeometrySmoother>::New();
  smoother->SetInputData(poly);
  smoother->SetNumberOfIterations(iterations);

  
  if(pinEnds && !closed && n >= 2) {
      
    smoother->SetUseMaskScalarField(true);
    
    smoother->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "Mask");
    
  }

  //smoother->SetUseAllCores(true);
  //smoother->SetDebugLevel(2);

  smoother->Update();

  vtkPolyData *out = vtkPolyData::SafeDownCast(smoother->GetOutput());
  vtkPoints   *P   = out->GetPoints();
  outPts.resize(n, std::vector<double>(3, 0.0));
  double p[3];
  
  for(vtkIdType i = 0; i < n; ++i) {
      
    P->GetPoint(i, p);
    outPts[i][0] = p[0];
    outPts[i][1] = p[1];
    outPts[i][2] = p[2];
    
  }
  
}

struct Vec3 {
  double x{0}, y{0}, z{0};
  Vec3() = default;
  Vec3(double X,double Y,double Z):x(X),y(Y),z(Z){}
  Vec3 operator+(const Vec3 &o) const { return {x+o.x, y+o.y, z+o.z}; }
  Vec3 operator-(const Vec3 &o) const { return {x-o.x, y-o.y, z-o.z}; }
  Vec3 operator*(double s)     const { return {x*s, y*s, z*s}; }
  Vec3 &operator+=(const Vec3 &o){ x+=o.x; y+=o.y; z+=o.z; return *this; }
  Vec3 &operator*=(double s){ x*=s; y*=s; z*=s; return *this; }
};

static inline double dist2(const Vec3 &a, const Vec3 &b){
  const double dx=a.x-b.x, dy=a.y-b.y, dz=a.z-b.z; return dx*dx+dy*dy+dz*dz;
}
static inline double dist (const Vec3 &a, const Vec3 &b){ return std::sqrt(dist2(a,b)); }
static inline Vec3 lerp(const Vec3 &a, const Vec3 &b, double t){ return a*(1.0-t)+b*t; }

static void resampleUniform(const std::vector<Vec3> &in, std::vector<Vec3> &out, bool closed, int targetCount){
    
  out.clear();
  
  const int nOrig = (int)in.size();
  
  if(nOrig==0){ return; }
  if(nOrig==1 || targetCount<=1){ out = in; return; }

  std::vector<Vec3> pts = in;
  if(closed && nOrig>=2){
      
    const double eps2 = 1e-24;
    if(dist2(pts.front(), pts.back()) < eps2) pts.pop_back();
    
  }
  const int n = (int)pts.size();
  if(n==0){ return; }

  std::vector<double> s(n,0.0);
  
  for(int i=1;i<n;i++) s[i] = s[i-1] + dist(pts[i-1], pts[i]);
  
  double L = s.back();
  
  if(closed){
    
    L += dist(pts.back(), pts.front());
    
  }
  if(L==0.0){ out.assign(targetCount, pts.front()); return; }

  
  const int M = targetCount;
  out.reserve(M);

  auto sampleAt = [&](double sk)->Vec3{
    
    if(!closed){
      
      if(sk<=0) return pts.front();
      if(sk>=s.back()) return pts.back();
      
      auto it = std::upper_bound(s.begin(), s.end(), sk);
      int i = std::max(0, (int)(it - s.begin()) - 1);
      double t = (sk - s[i]) / (s[i+1]-s[i]);
      
      return lerp(pts[i], pts[i+1], t);
      
    } else {
      
      double ss = std::fmod(sk, L);
      
      if(ss<0) ss += L;

      for(int i=0;i<n-1;i++){
          
        if(ss <= s[i+1]) {
            
          double t = (ss - s[i])/(s[i+1]-s[i]);
          return lerp(pts[i], pts[i+1], t);
          
        }
        
      }
      
      double sCloseStart = s.back();
      double t = (ss - sCloseStart) / (L - sCloseStart);
      
      return lerp(pts.back(), pts.front(), t);
    }
  };

  if(!closed){
      
    const double step = (M==1? 0.0 : (s.back() / (M-1)));
    
    for(int k=0;k<M;k++){
        
      out.push_back(sampleAt(k*step));
      
    }
    
  } else {
      
    const double step = (L / M);
    
    for(int k=0;k<M;k++){
        
      out.push_back(sampleAt(k*step));
      
    }
    
  }
  
}

static void taubinSmooth(std::vector<Vec3> &p, int iterations, bool closed, bool pinEnds, double lambda, double mu)
{
  const int n = (int)p.size();
  
  if(n<=2 || iterations<=0) return;
  
  std::vector<Vec3> L(n), X1(n);

  auto computeLaplacian = [&](const std::vector<Vec3> &X, std::vector<Vec3> &out){
      
    if(closed){
        
      for(int i=0;i<n;i++){
          
        int im = (i-1+n)%n, ip = (i+1)%n;
        Vec3 avg = (X[im] + X[ip]) * 0.5;
        out[i] = avg - X[i];
        
      }
      
    } else {
      
      Vec3 avg0 = (n>1? X[1] : X[0]);
      out[0] = avg0 - X[0];
      
      for(int i=1;i<n-1;i++){
          
        Vec3 avg = (X[i-1] + X[i+1]) * 0.5;
        out[i] = avg - X[i];
        
      }
      
      Vec3 avgn = (n>1? X[n-2] : X[n-1]);
      out[n-1] = avgn - X[n-1];
      
    }
    
  };

  for(int it=0; it<iterations; ++it){
    
    computeLaplacian(p, L);
    for(int i=0;i<n;i++){
        
      if(pinEnds && !closed && (i==0 || i==n-1)) { X1[i]=p[i]; continue; }
      X1[i] = p[i] + L[i]*lambda;
      
    }
    
    computeLaplacian(X1, L);
    for(int i=0;i<n;i++){
        
      if(pinEnds && !closed && (i==0 || i==n-1)) continue;
      p[i] = X1[i] + L[i]*mu;
      
    }
    
  }
  
}

void smoothPolylineRobust(const std::vector<std::vector<double>> &ptsIn, std::vector<std::vector<double>> &outPts, int iterations = 5, bool closed = false, bool pinEnds = true, bool resampleFirst = true, double lambda = 0.5, double mu = -0.53) 
{
  
  const int N = (int)ptsIn.size();
  
  std::vector<Vec3> P; P.reserve(N);
  
  for(const auto &q : ptsIn){
      
    if(q.size()<3) { P.emplace_back(0,0,0); } else { P.emplace_back(q[0], q[1], q[2]); }
    
  }
  
  if(N<=2){ outPts = ptsIn; return; }

  std::vector<Vec3> Q;
  
  if(resampleFirst){
      
    resampleUniform(P, Q, closed, N);
    
  } else {
    Q = P;
  }

  taubinSmooth(Q, iterations, closed, pinEnds, lambda, mu);

  outPts.assign(N, std::vector<double>(3));
  
  for(int i=0;i<N;i++){
      
    outPts[i][0] = Q[i].x;
    outPts[i][1] = Q[i].y;
    outPts[i][2] = Q[i].z;
    
  }
  
}

int ttkTimeVaryingPersistenceDiagramClustering::RequestData(
    vtkInformation * /*request*/,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
    ttk::Memory m;
    
    // From here_1,...
    
    auto blocks = vtkMultiBlockDataSet::GetData(inputVector[0], 0);

    int nBlocks = blocks->GetNumberOfBlocks();
    cout << "nBlocks in the first vtkMultiBlockDataSet (inputVector[0]): " << nBlocks << endl ;

    std::vector<std::vector<std::pair<ttk::DiagramType, double>>> TemporalPersistenceDiagramTimeSeriesSet;

    for(int i = 0; i < nBlocks; i++) {

        auto block = blocks->GetBlock(i);
        vtkMultiBlockDataSet * multiBlockDataSet = vtkMultiBlockDataSet::SafeDownCast(block);
        cout << "block " << i <<" has "<< multiBlockDataSet->GetNumberOfBlocks() << " elements" << endl;
        
        //From here_4,...
        
        for(int j = 0;j<multiBlockDataSet->GetNumberOfBlocks();j++){
            
            if(i==0){
            
                vtkNew<vtkIntArray> cluster{};
                cluster->SetName("Cluster");
                cluster->SetNumberOfTuples(1);
                cluster->SetTuple1(0,1);   
            
                multiBlockDataSet->GetBlock(j)->GetFieldData()->AddArray(cluster);            
            
            }
        
            else{
            
                vtkNew<vtkIntArray> cluster{};
                cluster->SetName("Cluster");
                cluster->SetNumberOfTuples(1);
                cluster->SetTuple1(0,2);   
            
                multiBlockDataSet->GetBlock(j)->GetFieldData()->AddArray(cluster);              
            
            
            }
                        
        }
        // ...to here_4, output_clusters filling

        std::vector<std::pair<ttk::DiagramType, double>> TemporalPersistenceDiagramTimeSeries;

        for(int j = 0; j < multiBlockDataSet->GetNumberOfBlocks(); j++) {

            ttk::DiagramType diagram{};
            auto *input = vtkUnstructuredGrid::SafeDownCast(multiBlockDataSet->GetBlock(j));

            VTUToDiagram(diagram, input, *this);

            auto array = multiBlockDataSet->GetBlock(j)->GetFieldData()->GetArray(this->TimestepColumnName.c_str());
            double value = array->GetTuple1(0);

            std::pair<ttk::DiagramType,double> pair = std::make_pair(diagram,value);
            TemporalPersistenceDiagramTimeSeries.push_back(pair);
            cout << "element added " << " at timestep " << value << endl;

        }

        TemporalPersistenceDiagramTimeSeriesSet.push_back(TemporalPersistenceDiagramTimeSeries);

    }
    
    // ...to here_1, Preprocessing
    

    // From here_2,... 
    
    auto output_clusters = vtkMultiBlockDataSet::GetData(outputVector, 0);

    auto SetOfAllCurves = vtkMultiBlockDataSet::GetData(outputVector, 1);

    auto SetOfAllAssignments = vtkMultiBlockDataSet::GetData(outputVector, 2);

    //auto PersistenceDiagramsDistanceMatrix = vtkTable::GetData(outputVector, 3);
    
    auto SetOfAllCurves2 = vtkMultiBlockDataSet::GetData(outputVector, 3);

    auto MDSPointsDistanceMatrix = vtkTable::GetData(outputVector, 4);

    auto DistanceMatrixDistorsion = vtkTable::GetData(outputVector, 5);
   
    auto geodesic = vtkMultiBlockDataSet::GetData(outputVector, 7);
    
    //auto output_diagrams_second_geodesic = vtkMultiBlockDataSet::GetData(outputVector, 8);
     
    vtkNew<vtkUnstructuredGrid> vtu2{};
    vtkNew<vtkUnstructuredGrid> vtu_output_matchings{};
    
    vtkNew<vtkUnstructuredGrid> kmeansVtu{};
    vtkNew<vtkUnstructuredGrid> kmeansVtu_output_matchings;
     
    // From here_3,...
    
    std::vector<std::vector<double>> MDSMatrix;
    std::vector<std::vector<double>> MDSMatrix2;
    std::vector<std::vector<double>> MDSMatrixVerification;
    std::vector<std::vector<double>> outputData_{};
 
    std::vector<double> greedBarycenterTimeToBring;
    std::vector<double> stochBarycenterTimeToBring;
        
    std::vector<ttk::DiagramType> DiagramPersistenceVectorOfGeodesic1;
    std::vector<ttk::DiagramType> DiagramPersistenceVectorOfGeodesic2;
    std::vector<ttk::DiagramType> stochBarycenter;
    std::vector<ttk::DiagramType> greedBarycenter;
    
    std::vector<std::vector<ttk::DiagramType>> TVPD_set;

    int TemporalPersistenceDiagramTimeSeriesSetSize = TemporalPersistenceDiagramTimeSeriesSet.size();

    std::vector<std::vector<double>> timeStampGeodesic(TemporalPersistenceDiagramTimeSeriesSetSize);

    std::vector<std::vector<int>> Matching(0);

    std::vector<ttk::DiagramType> Geodesic2;
    std::vector<double> GeodesicTimes2;
        
    std::vector<std::vector<ttk::DiagramType>> greedyClustersToBring;
    std::vector<std::vector<double>> greedyClustersTimeToBring;
        
    std::vector <std::vector<std::vector<double>>> kmeansMDSMatrices(0);
    std::vector <std::vector<std::vector<double>>> kmeansMDSVerificationMatrices(0);
    std::vector <std::vector<std::vector<double>>> kmeansOutputDataMatrices_(0);
    std::vector<int> kmeansClusterIds(0);
    std::vector<double> kmeansDistanceToTheNearestCentroid(0);
    std::vector<std::vector<std::vector<int>>> kmeansMatchingsToTheNearestCentroid(0);
    std::vector<std::vector<double>> kmeansMatchingsCostToTheNearestCentroid(0);
    std::vector<int> kmeansCurveSetSize(0);
    std::vector<std::vector<double>> kmeansTimesOfCurveSet(0);
    std::vector<std::vector<double>> kmeansMatchingsCostsWithDiagonalCostsNotIncludedWithTheNearest(0);
    double wcssToBring;
    std::vector<int> idsToBring;
  
    bool boolCriteria = false;
    bool boolKMeanPlusPlus = false;
    bool byCluster = false;
    bool doMDS = false;
  
    if(criteria == 1){
        boolCriteria = true;
    }
    else{
        boolCriteria = false;
    }
        
    if(kMeanPlusPlus == 1){
        boolKMeanPlusPlus = true;
    }
    else{
        boolKMeanPlusPlus = false;
    }
    
    if(MDS == 1){
        
        doMDS = true;
        
    }

    const auto XAEAEZR = this->execute(TemporalPersistenceDiagramTimeSeriesSet, delta, Weight, MDSMatrix, doMDS, outputData_, MDSMatrixVerification, DiagramPersistenceVectorOfGeodesic1,timeStampGeodesic, Matching, Geodesic2, GeodesicTimes2, geodesicCoefficient,DiagramPersistenceVectorOfGeodesic2, stochBarycenter, stochBarycenterTimeToBring, greedBarycenter, greedBarycenterTimeToBring, pas, tau, greedySegmentation, greedyClustersToBring, greedyClustersTimeToBring, numberOfClusters, boolKMeanPlusPlus, iterationsNumberKMeans, methodChoice, numberOfDeparturesStochasticBarycenterComputation, boolCriteria, kmeansMDSMatrices, kmeansMDSVerificationMatrices, kmeansOutputDataMatrices_, kmeansClusterIds, kmeansDistanceToTheNearestCentroid, kmeansMatchingsToTheNearestCentroid, kmeansMatchingsCostToTheNearestCentroid, byCluster, kmeansCurveSetSize, kmeansTimesOfCurveSet, kmeansMatchingsCostsWithDiagonalCostsNotIncludedWithTheNearest, wcssToBring, idsToBring, geodesicForBarycenter,TVPD_set, selectedCoreCount, beta);
        
        
    if(TemporalPersistenceDiagramTimeSeriesSetSize>2){
        
        const auto kmeansPointData = kmeansVtu->GetPointData();
        const auto kmeansCellData = kmeansVtu->GetCellData();
        
        const auto kmeansPointData_output_matchings = kmeansVtu_output_matchings->GetPointData();
        const auto kmeansCellData_output_matchings = kmeansVtu_output_matchings->GetCellData();
        
        int totalSumKmeansCurveSetSize = 0;
        int numberOfCurve = kmeansCurveSetSize.size();
        
        for(int i = 0; i < kmeansCurveSetSize.size(); i++){
        
          totalSumKmeansCurveSetSize = totalSumKmeansCurveSetSize + kmeansCurveSetSize[i];
          
        }
        
        vtkNew<vtkUnsignedCharArray> kmeansIsRealVertice{};
        kmeansIsRealVertice->SetName("Is Real Vertice");
        kmeansIsRealVertice->SetNumberOfTuples(totalSumKmeansCurveSetSize);
        kmeansPointData->AddArray(kmeansIsRealVertice);
        
        vtkNew<vtkDoubleArray> kmeansTimeStamp{};
        kmeansTimeStamp->SetName("Time Stamp");
        kmeansTimeStamp->SetNumberOfTuples(totalSumKmeansCurveSetSize);
        kmeansPointData->AddArray(kmeansTimeStamp);
        
        vtkNew<vtkDoubleArray> kmeansCluster{};
        kmeansCluster->SetName("Cluster");
        kmeansCluster->SetNumberOfTuples(totalSumKmeansCurveSetSize);
        kmeansPointData->AddArray(kmeansCluster);
        
        vtkNew<vtkIntArray> TVPDIdentifier{};
        TVPDIdentifier->SetName("TVPD identifier");
        TVPDIdentifier->SetNumberOfTuples(totalSumKmeansCurveSetSize);
        kmeansPointData->AddArray(TVPDIdentifier);
        
        vtkNew<vtkIntArray> TVPDAssignmentIdentifier{};
        TVPDAssignmentIdentifier->SetNumberOfComponents(1);
        TVPDAssignmentIdentifier->SetName("Cell TVPD identifier");
        TVPDAssignmentIdentifier->SetNumberOfTuples(totalSumKmeansCurveSetSize-numberOfCurve);
        kmeansCellData->AddArray(TVPDAssignmentIdentifier);
        
        vtkNew<vtkFloatArray> kmeansAssignmentCost{};
        kmeansAssignmentCost->SetNumberOfComponents(1);
        kmeansAssignmentCost->SetName("Assignment cost");
        kmeansAssignmentCost->SetNumberOfTuples(totalSumKmeansCurveSetSize-numberOfCurve);
        kmeansCellData->AddArray(kmeansAssignmentCost);
        
        vtkNew<vtkUnsignedCharArray> kmeansIsAssignment{};
        kmeansIsAssignment->SetName("Is assignment");
        kmeansIsAssignment->SetNumberOfTuples(totalSumKmeansCurveSetSize-numberOfCurve);
        kmeansCellData->AddArray(kmeansIsAssignment);
        
        std::vector<std::vector<std::vector<int>>> kmeansMatchingsToTheNearestCentroidWithoutDiagonalMatchings(kmeansMatchingsToTheNearestCentroid.size());
        
        for(int i = 0; i < kmeansMatchingsToTheNearestCentroid.size(); i++){
            
            for(int j = 0; j < kmeansMatchingsToTheNearestCentroid[i].size(); j++){
                
                if(kmeansMatchingsToTheNearestCentroid[i][j][0] != -1 && kmeansMatchingsToTheNearestCentroid[i][j][1] != -1){
                    
                    kmeansMatchingsToTheNearestCentroidWithoutDiagonalMatchings[i].push_back(kmeansMatchingsToTheNearestCentroid[i][j]);
                    
                }
                
            }
            
        }
        
        int totalSumKmeansNumberOfMatchingPoints = 0;
        int totalSumKmeansNumberOfMatchingCells = 0;
        
        for(int i = 0; i < kmeansMatchingsToTheNearestCentroidWithoutDiagonalMatchings.size(); i++){
        
          totalSumKmeansNumberOfMatchingPoints = totalSumKmeansNumberOfMatchingPoints + 2*kmeansMatchingsToTheNearestCentroidWithoutDiagonalMatchings[i].size();
          
          totalSumKmeansNumberOfMatchingCells = totalSumKmeansNumberOfMatchingCells + kmeansMatchingsToTheNearestCentroidWithoutDiagonalMatchings[i].size();
          
        }
        
        vtkNew<vtkUnsignedCharArray> kmeansIsRealVertice_output_matchings{};
        kmeansIsRealVertice_output_matchings->SetName("Is Real Vertice");
        kmeansIsRealVertice_output_matchings->SetNumberOfTuples(totalSumKmeansCurveSetSize);
        kmeansPointData_output_matchings->AddArray(kmeansIsRealVertice_output_matchings);
        
        vtkNew<vtkUnsignedCharArray> kmeansIsAssignment_output_matchings{};
        kmeansIsAssignment_output_matchings->SetName("Is assignment");
        kmeansIsAssignment_output_matchings->SetNumberOfTuples(totalSumKmeansNumberOfMatchingCells);
        kmeansCellData_output_matchings->AddArray(kmeansIsAssignment_output_matchings);
        
        vtkNew<vtkFloatArray> kmeansAssignmentCost_output_matchings{};
        kmeansAssignmentCost_output_matchings->SetNumberOfComponents(1);
        kmeansAssignmentCost_output_matchings->SetName("Assignment cost");
        kmeansAssignmentCost_output_matchings->SetNumberOfTuples(totalSumKmeansNumberOfMatchingCells);
        kmeansCellData_output_matchings->AddArray(kmeansAssignmentCost_output_matchings);
        
        vtkNew<vtkIntArray> kmeansClusterAssignment{};
        kmeansClusterAssignment->SetName("Cluster");
        kmeansClusterAssignment->SetNumberOfTuples(totalSumKmeansNumberOfMatchingCells);
        kmeansCellData_output_matchings->AddArray(kmeansClusterAssignment);
        
        vtkNew<vtkPoints> kmeansPoints{};
        kmeansPoints->SetNumberOfPoints(totalSumKmeansCurveSetSize);
        
        vtkNew<vtkPoints> kmeansPoints_output_matchings{};
        kmeansPoints_output_matchings->SetNumberOfPoints(totalSumKmeansCurveSetSize);
        
        std::vector <std::vector<std::vector<double>>> SeparatedkmeansOutputDataMatrices_(kmeansCurveSetSize.size());

        int z = 0;
        int k = 0; 
        
        for(int i = 0 ; i < kmeansOutputDataMatrices_[0][0].size(); i ++){
            
            if(z == kmeansCurveSetSize[k]){
                
                k++;
                z = 0;
                
            }
            
            double Mx = -1;
            double My = -1;
            double Mz = -1;
            
            Mx = kmeansOutputDataMatrices_[0][0][i];
            My = kmeansOutputDataMatrices_[0][1][i];
            Mz = kmeansOutputDataMatrices_[0][2][i];
            
            std::vector<double> toAddInThisLoop(3);
            
            toAddInThisLoop[0] = Mx;
            toAddInThisLoop[1] = My;
            toAddInThisLoop[2] = Mz;
            
            SeparatedkmeansOutputDataMatrices_[k].push_back(toAddInThisLoop);
            
            z++;    
        
        }
        
        if(iterationsNumberSmoothing>0){
            
            for(int i = 0 ; i < SeparatedkmeansOutputDataMatrices_.size(); i ++){
                
                std::vector<std::vector<double>> pts = SeparatedkmeansOutputDataMatrices_[i];
                std::vector<std::vector<double>> smoothed;
                
                if(smoothChoice == 1){
                    
                    if(antiShrink==0){
                        
                        smoothPolylineRobust(pts, smoothed,iterationsNumberSmoothing,false,false,true,lambdaTaubinParameter, lambdaTaubinParameter*(-1.06) );
                        
                    }
                    else{
                        
                        smoothPolylineRobust(pts, smoothed,iterationsNumberSmoothing,false,true,true,lambdaTaubinParameter, lambdaTaubinParameter*(-1.06) );
                        
                    }
                    
                    SeparatedkmeansOutputDataMatrices_[i] = smoothed;
                    
                }else if(smoothChoice == 0){
                    
                    if(antiShrink==0){
                        
                        smoothPolylineWithTTK_VTK(pts,iterationsNumberSmoothing,smoothed,false,false);
                        
                    }else{
                        
                        smoothPolylineWithTTK_VTK(pts,iterationsNumberSmoothing,smoothed,false,true);
                        
                    }
                    
                    SeparatedkmeansOutputDataMatrices_[i] = smoothed;
                    
                }
                
            }
            
        }
        
        
        z = 0;
        k = 0;
        
        for(int i = 0; i < totalSumKmeansCurveSetSize; i++){
            
            if( z == kmeansCurveSetSize[k]){
                
                k++;
                z = 0;
            }
            
            kmeansPoints->SetPoint(i, SeparatedkmeansOutputDataMatrices_[k][z][0], SeparatedkmeansOutputDataMatrices_[k][z][1], SeparatedkmeansOutputDataMatrices_[k][z][2]);
            
            kmeansPoints_output_matchings->SetPoint(i, SeparatedkmeansOutputDataMatrices_[k][z][0], SeparatedkmeansOutputDataMatrices_[k][z][1], SeparatedkmeansOutputDataMatrices_[k][z][2]);
            
            if(i > 0){
                
                if(z > 0){
                    std::array<vtkIdType, 2> diag{i-1, i};
                    kmeansVtu->InsertNextCell(VTK_LINE, 2, diag.data());

                }
            }
            
            kmeansIsRealVertice->SetTuple1(i, true);
            kmeansIsRealVertice_output_matchings->SetTuple1(i, false);
            TVPDIdentifier->SetTuple1(i, k);
            kmeansCluster->SetTuple1(i,kmeansClusterIds[k]);
            kmeansTimeStamp->SetTuple1(i,kmeansTimesOfCurveSet[k][z]);
        
            z++;
        }
        
        int kmeansNumberOfCells = totalSumKmeansCurveSetSize-numberOfCurve;
        
        for(int i = 0; i < kmeansNumberOfCells; i++){
            
                kmeansAssignmentCost->SetTuple1(i, -1);
                kmeansIsAssignment->SetTuple1(i, false);
            
        }
        
        vtkIdType idx = 0;
        
        const int K = (int)kmeansCurveSetSize.size();
        
        for(int i = 0; i < K; i++){
            
            int count = kmeansCurveSetSize[i]-1;
            
            if (count<= 0) continue;
            
            for(int t =0; t < count && idx < TVPDAssignmentIdentifier->GetNumberOfTuples(); t++){
            
                TVPDAssignmentIdentifier->SetValue(idx++, i);
                
            }
            
        }
        
        std::vector<vtkIdType> base(kmeansCurveSetSize.size() + 1, 0);
        
        for(int s = 0; s < (int)kmeansCurveSetSize.size(); ++s)
        base[s + 1] = base[s] + static_cast<vtkIdType>(kmeansCurveSetSize[s]);
                                                    
        auto centroidSetOf = [&](int i) -> int {
        
            const int cid = kmeansClusterIds[i];
            const int guess = TemporalPersistenceDiagramTimeSeriesSetSize + cid; 
            
            if(0 <= guess && guess < (int)kmeansCurveSetSize.size())
            return guess;
            
            if(0 <= cid && cid < (int)kmeansCurveSetSize.size())
            return cid;
            
            return -1;
            
        };

        int v = 0;
        
        z = 0;
        k = 0;
        
        while(v<totalSumKmeansNumberOfMatchingCells){
            
            if(z == kmeansMatchingsToTheNearestCentroidWithoutDiagonalMatchings[k].size()){
                
                k++;
                z = 0;
                
            }
            
            kmeansAssignmentCost_output_matchings->SetTuple1(v, kmeansMatchingsCostToTheNearestCentroid[k][z]);
            kmeansClusterAssignment->SetTuple1(v, kmeansClusterIds[k]);
            
            kmeansIsAssignment_output_matchings->SetTuple1(v, true);
            
            v++;
            z++;
        }

        for(int i = 0; i < TemporalPersistenceDiagramTimeSeriesSetSize; ++i) {
            
            const int cset = centroidSetOf(i);
            
            if(cset < 0) continue;

            const auto &pairs = kmeansMatchingsToTheNearestCentroidWithoutDiagonalMatchings[i];
                
            for(const auto &pair : pairs) {
                            
                if(pair.size() < 2) continue;
                const int localA = pair[0]-1;
                const int localB = pair[1]-1; 
                                            
                if(localA < 0 || localB < 0) continue;

                const vtkIdType u = base[i]    + static_cast<vtkIdType>(localA);
                const vtkIdType v = base[cset] + static_cast<vtkIdType>(localB);

                std::array<vtkIdType, 2> diag{u, v};
                kmeansVtu_output_matchings->InsertNextCell(VTK_LINE, 2, diag.data());
            
            }
                
        }

        kmeansVtu->SetPoints(kmeansPoints);
        kmeansVtu_output_matchings->SetPoints(kmeansPoints_output_matchings);
        
        SetOfAllCurves->SetBlock(0, kmeansVtu);
        SetOfAllAssignments->SetBlock(0, kmeansVtu_output_matchings);
        
    }
    
    if(TemporalPersistenceDiagramTimeSeriesSetSize == 2){
        
        const auto pd = vtu2->GetPointData();
        const auto cd = vtu2->GetCellData();
    
        const auto pd_output_matchings = vtu_output_matchings->GetPointData();
        const auto cd_output_matchings = vtu_output_matchings->GetCellData();

        vtkNew<vtkUnsignedCharArray> isRealVertice{};
        isRealVertice->SetName("Is Real Vertice");
        isRealVertice->SetNumberOfTuples(XAEAEZR.first[0]+1+XAEAEZR.first[1]+1);
        pd->AddArray(isRealVertice);
        
        vtkNew<vtkDoubleArray> TimeStamp{};
        TimeStamp->SetName("Time Stamp");
        TimeStamp->SetNumberOfTuples(XAEAEZR.first[0]+1+XAEAEZR.first[1]+1);
        pd->AddArray(TimeStamp);
        
        vtkNew<vtkIntArray> TVPDIdentifier2{};
        TVPDIdentifier2->SetName("TVPD identifier");
        TVPDIdentifier2->SetNumberOfTuples(XAEAEZR.first[0]+1+XAEAEZR.first[1]+1);
        pd->AddArray(TVPDIdentifier2);
        
        vtkNew<vtkFloatArray> assignmentCost{};
        assignmentCost->SetNumberOfComponents(1);
        assignmentCost->SetName("Assignment cost");
        assignmentCost->SetNumberOfTuples(XAEAEZR.first[0]+XAEAEZR.first[1]);
        cd->AddArray(assignmentCost);
        
        vtkNew<vtkUnsignedCharArray> isAssignment{};
        isAssignment->SetName("Is assignment");
        isAssignment->SetNumberOfTuples(XAEAEZR.first[0]+XAEAEZR.first[1]);
        cd->AddArray(isAssignment);
        
        vtkNew<vtkUnsignedCharArray> isRealVertice_output_matchings{};
        isRealVertice_output_matchings->SetName("Is Real Vertice");
        isRealVertice_output_matchings->SetNumberOfTuples(2*XAEAEZR.second.size());
        pd_output_matchings->AddArray(isRealVertice_output_matchings);

        vtkNew<vtkUnsignedCharArray> isAssignment_output_matchings{};
        isAssignment_output_matchings->SetName("Is assignment");
        isAssignment_output_matchings->SetNumberOfTuples(XAEAEZR.second.size());
        cd_output_matchings->AddArray(isAssignment_output_matchings);
        
        vtkNew<vtkFloatArray> assignmentCost_output_matchings{};
        assignmentCost_output_matchings->SetNumberOfComponents(1);
        assignmentCost_output_matchings->SetName("Assignment cost");
        assignmentCost_output_matchings->SetNumberOfTuples(XAEAEZR.second.size());
        cd_output_matchings->AddArray(assignmentCost_output_matchings);
        
        vtkNew<vtkPoints> points{};
        points->SetNumberOfPoints(XAEAEZR.first[0]+1+XAEAEZR.first[1]+1);
        
        vtkNew<vtkPoints> points_output_matchings{};
        points_output_matchings->SetNumberOfPoints(2*XAEAEZR.second.size());
        
        if(MDS == 0){
            
            if(withChronologicalDeparture == 0){

                double departureTVPD1 = 0;
                double departureTVPD2 = 0;
                
                // First curve

                points->SetPoint(0, departureTVPD1, 0, 0);
                isRealVertice->SetTuple1(0, true);
                TimeStamp->SetTuple1(0,timeStampGeodesic[0][0]);
                TVPDIdentifier2->SetTuple1(0,0);
                        
                for(int i =1; i<XAEAEZR.first[0]+1;i++){
                    
                    points->SetPoint(i, departureTVPD1+i, 0, 0);
                    std::array<vtkIdType, 2> diag{i-1, i};
                    assignmentCost->SetTuple1(i-1, -1);
                    isAssignment->SetTuple1(i-1, false);
                    vtu2->InsertNextCell(VTK_LINE, 2, diag.data()); // horizontal cell assignments
                    isRealVertice->SetTuple1(i, true);
                    
                    TimeStamp->SetTuple1(i,timeStampGeodesic[0][i]);
                    TVPDIdentifier2->SetTuple1(i,0);

                }
                
                // Second curve
                
                points->SetPoint(XAEAEZR.first[0]+1, departureTVPD2, -verticalSpace, 0);
                isRealVertice->SetTuple1(XAEAEZR.first[0]+1, true);
                TimeStamp->SetTuple1(XAEAEZR.first[0]+1,timeStampGeodesic[1][0]);
                TVPDIdentifier2->SetTuple1(XAEAEZR.first[0]+1,1);
                
                for(int i =1; i<XAEAEZR.first[1]+1;i++){
                    
                    points->SetPoint(XAEAEZR.first[0]+1+i, departureTVPD2+i, -verticalSpace, 0);
                    std::array<vtkIdType, 2> diag{XAEAEZR.first[0]+i, XAEAEZR.first[0]+1+i};
                    assignmentCost->SetTuple1(XAEAEZR.first[0]+i-1, -1);
                    isAssignment->SetTuple1(XAEAEZR.first[0]+i-1, false);
                    vtu2->InsertNextCell(VTK_LINE, 2, diag.data()); // horizontal cell assignments
                    isRealVertice->SetTuple1(XAEAEZR.first[0]+1+i, true);
                    
                    TimeStamp->SetTuple1(XAEAEZR.first[0]+1+i,timeStampGeodesic[1][i]);
                    TVPDIdentifier2->SetTuple1(XAEAEZR.first[0]+1+i,1);

                }

                // vertical cell assignments
                
                int v = 0;
                
                while(v<XAEAEZR.second.size()){
                        
                        points_output_matchings->SetPoint(2*v, 0.5+departureTVPD1+(XAEAEZR.second[v][0]-1), 0, 0);
                        isRealVertice_output_matchings->SetTuple1(2*v, false);

                        points_output_matchings->SetPoint(2*v+1, 0.5+departureTVPD2+(XAEAEZR.second[v][1]-1), -verticalSpace, 0);
                        isRealVertice_output_matchings->SetTuple1(2*v+1, false);            
                    
                    std::array<vtkIdType, 2> diag{2*v, 2*v+1};
                    assignmentCost_output_matchings->SetTuple1(v, kmeansMatchingsCostToTheNearestCentroid[0][v]);
                    isAssignment_output_matchings->SetTuple1(v, true);
                    vtu_output_matchings->InsertNextCell(VTK_LINE, 2, diag.data()); 
                    
                    v++;
                }
            
            }
            else if(withChronologicalDeparture == 1){
                
                double departureTVPD1 = timeStampGeodesic[0][0];
                double departureTVPD2 = timeStampGeodesic[1][0];
                
                // First curve

                points->SetPoint(0, departureTVPD1, 0, 0);
                isRealVertice->SetTuple1(0, true);
                TimeStamp->SetTuple1(0,timeStampGeodesic[0][0]);
                TVPDIdentifier2->SetTuple1(0,0);
                        
                for(int i =1; i<XAEAEZR.first[0]+1;i++){
                    
                    points->SetPoint(i, timeStampGeodesic[0][i], 0, 0);
                    std::array<vtkIdType, 2> diag{i-1, i};
                    assignmentCost->SetTuple1(i-1, -1);
                    isAssignment->SetTuple1(i-1, false);
                    vtu2->InsertNextCell(VTK_LINE, 2, diag.data()); // horizontal cell assignments
                    isRealVertice->SetTuple1(i, true);
                    
                    TimeStamp->SetTuple1(i,timeStampGeodesic[0][i]);
                    TVPDIdentifier2->SetTuple1(i,0);

                }
                
                // Second curve
                
                points->SetPoint(XAEAEZR.first[0]+1, departureTVPD2, -verticalSpace, 0);
                isRealVertice->SetTuple1(XAEAEZR.first[0]+1, true);
                TimeStamp->SetTuple1(XAEAEZR.first[0]+1,timeStampGeodesic[1][0]);
                TVPDIdentifier2->SetTuple1(XAEAEZR.first[0]+1,1);
                
                for(int i =1; i<XAEAEZR.first[1]+1;i++){
                    
                    points->SetPoint(XAEAEZR.first[0]+1+i, timeStampGeodesic[1][i], -verticalSpace, 0);
                    std::array<vtkIdType, 2> diag{XAEAEZR.first[0]+i, XAEAEZR.first[0]+1+i};
                    assignmentCost->SetTuple1(XAEAEZR.first[0]+i-1, -1);
                    isAssignment->SetTuple1(XAEAEZR.first[0]+i-1, false);
                    vtu2->InsertNextCell(VTK_LINE, 2, diag.data()); // horizontal cell assignments
                    isRealVertice->SetTuple1(XAEAEZR.first[0]+1+i, true);
                    
                    TimeStamp->SetTuple1(XAEAEZR.first[0]+1+i,timeStampGeodesic[1][i]);
                    TVPDIdentifier2->SetTuple1(XAEAEZR.first[0]+1+i,1);

                }

                // vertical cell assignments
                
                int v = 0;
                
                while(v<XAEAEZR.second.size()){
                        
                        points_output_matchings->SetPoint(2*v, (delta/2)+departureTVPD1+delta*(XAEAEZR.second[v][0]-1), 0, 0);
                        isRealVertice_output_matchings->SetTuple1(2*v, false);

                        points_output_matchings->SetPoint(2*v+1, (delta/2)+departureTVPD2+delta*(XAEAEZR.second[v][1]-1), -verticalSpace, 0);
                        isRealVertice_output_matchings->SetTuple1(2*v+1, false);            
                    
                    std::array<vtkIdType, 2> diag{2*v, 2*v+1};
                    assignmentCost_output_matchings->SetTuple1(v, kmeansMatchingsCostToTheNearestCentroid[0][v]);
                    isAssignment_output_matchings->SetTuple1(v, true);
                    vtu_output_matchings->InsertNextCell(VTK_LINE, 2, diag.data()); 
                    
                    v++;
                }
            
            }
        
        }
        
        else{
            
            std::vector <std::vector<std::vector<double>>> SeparatedkmeansOutputDataMatrices2_(2);
            
            int z = 0;
            int k = 0; 
            
            for(int i = 0 ; i < XAEAEZR.first[0]+1+XAEAEZR.first[1]+1; i++){
                
                if(z == XAEAEZR.first[0]+1){
                    
                    k++;
                    z = 0;
                    
                }
                
                double Mx = -1;
                double My = -1;
                double Mz = -1;
                
                Mx = outputData_[0][i];
                My = outputData_[1][i];
                Mz = outputData_[2][i];
                
                std::vector<double> toAddInThisLoop(3);
                
                toAddInThisLoop[0] = Mx;
                toAddInThisLoop[1] = My;
                toAddInThisLoop[2] = Mz;
                
                SeparatedkmeansOutputDataMatrices2_[k].push_back(toAddInThisLoop);
                
                z++;    
            
            }
            
            if(iterationsNumberSmoothing>0){
                
                for(int i = 0 ; i < SeparatedkmeansOutputDataMatrices2_.size(); i ++){
                    
                    std::vector<std::vector<double>> pts = SeparatedkmeansOutputDataMatrices2_[i];
                    std::vector<std::vector<double>> smoothed;
                    
                    if(smoothChoice == 1){
                        
                        if(antiShrink==0){
                            
                            smoothPolylineRobust(pts, smoothed,iterationsNumberSmoothing,false,false,true,lambdaTaubinParameter, lambdaTaubinParameter*(-1.06) );
                            
                        }
                        else{
                            
                            smoothPolylineRobust(pts, smoothed,iterationsNumberSmoothing,false,true,true,lambdaTaubinParameter, lambdaTaubinParameter*(-1.06) );
                            
                        }
                        
                        SeparatedkmeansOutputDataMatrices2_[i] = smoothed;
                        
                    }else if(smoothChoice == 0){
                        
                        if(antiShrink==0){
                            
                            smoothPolylineWithTTK_VTK(pts,iterationsNumberSmoothing,smoothed,false,false);
                            
                        }else{
                            
                            smoothPolylineWithTTK_VTK(pts,iterationsNumberSmoothing,smoothed,false,true);
                            
                        }
                        
                        SeparatedkmeansOutputDataMatrices2_[i] = smoothed;
                        
                    }
                    
                }
                
            }
            
            if(smoothChoice != 2){
                
                std::vector<std::vector<double>> Z = SeparatedkmeansOutputDataMatrices2_[0];
                
                Z.insert( Z.end(),  SeparatedkmeansOutputDataMatrices2_[1].begin(), SeparatedkmeansOutputDataMatrices2_[1].end()  );
                
                for(int i =0; i < outputData_[0].size(); i++){
                    
                    outputData_[0][i]= Z[i][0];
                    outputData_[1][i]= Z[i][1];
                    outputData_[2][i]= Z[i][2];
                    
                }
                
            }
            
            // First curve
            for(int i =0; i<XAEAEZR.first[0]+1;i++){
            
                points->SetPoint(i, outputData_[0][i], outputData_[1][i], outputData_[2][i]);
                
                if(i >0){
                    std::array<vtkIdType, 2> diag{i-1, i};
                    assignmentCost->SetTuple1(i-1, -1);
                    isAssignment->SetTuple1(i-1, false);
                    vtu2->InsertNextCell(VTK_LINE, 2, diag.data()); // horizontal cell assignments
                }
                
                isRealVertice->SetTuple1(i, true);
                TimeStamp->SetTuple1(i,timeStampGeodesic[0][i]);
                TVPDIdentifier2->SetTuple1(i,0);

            }
            
            // Second curve
            for(int i =0; i<XAEAEZR.first[1]+1;i++){
            
                points->SetPoint(XAEAEZR.first[0]+1+i, outputData_[0][XAEAEZR.first[0]+1+i], outputData_[1][XAEAEZR.first[0]+1+i], outputData_[2][XAEAEZR.first[0]+1+i]);
                
                if(i >0){
                    std::array<vtkIdType, 2> diag{XAEAEZR.first[0]+i, XAEAEZR.first[0]+1+i};
                    assignmentCost->SetTuple1(XAEAEZR.first[0]+i-1, -1);
                    isAssignment->SetTuple1(XAEAEZR.first[0]+i-1, false);
                    vtu2->InsertNextCell(VTK_LINE, 2, diag.data()); // horizontal cell assignments
                }
                
                isRealVertice->SetTuple1(XAEAEZR.first[0]+1+i, true);
                TimeStamp->SetTuple1(XAEAEZR.first[0]+1+i,timeStampGeodesic[1][i]);
                TVPDIdentifier2->SetTuple1(XAEAEZR.first[0]+1+i,1);

            }
            
            // vertical cell assignments
        
            int v = 0;
        
            while(v<XAEAEZR.second.size()){
            
                points_output_matchings->SetPoint(2*v, (outputData_[0][(XAEAEZR.second[v][0]-1)]+outputData_[0][(XAEAEZR.second[v][0])])/2, (outputData_[1][(XAEAEZR.second[v][0]-1)]+outputData_[1][(XAEAEZR.second[v][0])])/2, (outputData_[2][(XAEAEZR.second[v][0]-1)]+outputData_[2][(XAEAEZR.second[v][0])])/2);
                isRealVertice_output_matchings->SetTuple1(2*v, false);
                
                
                points_output_matchings->SetPoint(2*v+1, (outputData_[0][(XAEAEZR.first[0]+1+XAEAEZR.second[v][1]-1)]+outputData_[0][XAEAEZR.first[0]+1+(XAEAEZR.second[v][1])])/2, (outputData_[1][(XAEAEZR.first[0]+1+XAEAEZR.second[v][1]-1)]+outputData_[1][(XAEAEZR.first[0]+1+XAEAEZR.second[v][1])])/2, (outputData_[2][(XAEAEZR.first[0]+1+XAEAEZR.second[v][1]-1)]+outputData_[2][(XAEAEZR.first[0]+1+XAEAEZR.second[v][1])])/2);
                isRealVertice_output_matchings->SetTuple1(2*v+1, false);

                std::array<vtkIdType, 2> diag{2*v, 2*v+1};
                assignmentCost_output_matchings->SetTuple1(v, kmeansMatchingsCostToTheNearestCentroid[0][v]);
                isAssignment_output_matchings->SetTuple1(v, true);
                vtu_output_matchings->InsertNextCell(VTK_LINE, 2, diag.data());        // vertical cell assignments
            
                v++;
                
            }
            
        }

        vtu2->SetPoints(points);
        vtu_output_matchings->SetPoints(points_output_matchings);
        
        SetOfAllCurves->SetBlock(0, vtu2);
        SetOfAllAssignments->SetBlock(0, vtu_output_matchings);

    }        
    
    output_clusters->SetBlock(0,blocks);
    
    
    
    
/*                        
        for(int z =0;z<DiagramPersistenceVectorOfGeodesic1.size();z++){
            
            vtkNew<vtkDoubleArray> dummy{};
            vtkNew<vtkUnstructuredGrid> vtu{};
            DiagramToVTU(  vtu, DiagramPersistenceVectorOfGeodesic1[z], dummy, *this, 3, false);

            output_diagrams_first_geodesic->SetBlock(z, vtu);
            
        }
        */
    


        for(int i =0;i<greedyClustersToBring.size();i++){
            
            
            vtkSmartPointer<vtkMultiBlockDataSet> vtkBlockNodes = vtkSmartPointer<vtkMultiBlockDataSet>::New();            
            for(int j =0;j<greedyClustersToBring[i].size();j++){
            
                vtkNew<vtkDoubleArray> dummy{};
                vtkNew<vtkUnstructuredGrid> vtu{};
                DiagramToVTU(  vtu, greedyClustersToBring[i][j], dummy, *this, 3, false);

                vtkBlockNodes->SetBlock(j, vtu);
            
            }
            
            geodesic->SetBlock(i, vtkBlockNodes);
            
        }
        




        
   /*     
        for(int z =0;z<DiagramPersistenceVectorOfGeodesic2.size();z++){
            
            vtkNew<vtkDoubleArray> dummy{};
            vtkNew<vtkUnstructuredGrid> vtu{};
            DiagramToVTU(  vtu, DiagramPersistenceVectorOfGeodesic2[z], dummy, *this, 3, false);

            output_diagrams_second_geodesic->SetBlock(z, vtu);
            
        }
        */
    /*    
        for(int z =0;z<stochBarycenter.size();z++){
            
            vtkNew<vtkDoubleArray> dummy{};
            vtkNew<vtkUnstructuredGrid> vtu{};
            DiagramToVTU(  vtu, stochBarycenter[z], dummy, *this, 3, false);

            SetOfAllCurves->SetBlock(z, vtu);
            
        }
        */
        
        
        for(int i =0;i<TVPD_set.size();i++){
            
            
            vtkSmartPointer<vtkMultiBlockDataSet> vtkBlockNodes = vtkSmartPointer<vtkMultiBlockDataSet>::New();            
            for(int j =0;j<TVPD_set[i].size();j++){
            
                vtkNew<vtkDoubleArray> dummy{};
                vtkNew<vtkUnstructuredGrid> vtu{};
                DiagramToVTU(  vtu, TVPD_set[i][j], dummy, *this, 3, false);

                vtkBlockNodes->SetBlock(j, vtu);
            
            }
            
            SetOfAllCurves2->SetBlock(i, vtkBlockNodes);
            
        }
    
    
    /*
        for(int z =0;z<greedBarycenter.size();z++){
            
            vtkNew<vtkDoubleArray> dummy{};
            vtkNew<vtkUnstructuredGrid> vtu{};
            DiagramToVTU(  vtu, greedBarycenter[z], dummy, *this, 3, false);

            SetOfAllAssignments->SetBlock(z, vtu);
            
        }
        */
        
        
/*        
        for(int z =0;z<Geodesic2.size();z++){
            
            vtkNew<vtkDoubleArray> dummy{};
            vtkNew<vtkUnstructuredGrid> vtu{};
            DiagramToVTU(  vtu, Geodesic2[z], dummy, *this, 3, false);

            geodesic->SetBlock(z, vtu);
            
        }
        */
        


        
        // ...to here_3, vtu2 process
        
        
    
    // ...to here_2, Set output
    
    
    
        const auto zeroPad
    = [](std::string &colName, const size_t numberCols, const size_t colIdx) {
        std::string max{std::to_string(numberCols - 1)};
        std::string cur{std::to_string(colIdx)};
        std::string zer(max.size() - cur.size(), '0');
        colName.append(zer).append(cur);
    };

/*
    for(size_t i = 0; i < MDSMatrix.size(); ++i) {
        std::string name{"Diagram "};
        zeroPad(name, MDSMatrix.size(), i);

        vtkNew<vtkDoubleArray> col{};
        col->SetNumberOfTuples(MDSMatrix.size());
        col->SetName(name.c_str());
        for(size_t j = 0; j < MDSMatrix[i].size(); ++j) {
            col->SetTuple1(j, MDSMatrix[i][j]);
        }
        PersistenceDiagramsDistanceMatrix->AddColumn(col);
    }

    */
    
    
    
    
            const auto zeroPad1
    = [](std::string &colName, const size_t numberCols, const size_t colIdx) {
        std::string max{std::to_string(numberCols - 1)};
        std::string cur{std::to_string(colIdx)};
        std::string zer(max.size() - cur.size(), '0');
        colName.append(zer).append(cur);
    };


    for(size_t i = 0; i < MDSMatrixVerification.size(); ++i) {
        std::string name{"Point "};
        zeroPad1(name, MDSMatrixVerification.size(), i);

        vtkNew<vtkDoubleArray> col{};
        col->SetNumberOfTuples(MDSMatrixVerification.size());
        col->SetName(name.c_str());
        for(size_t j = 0; j < MDSMatrixVerification[i].size(); ++j) {
            col->SetTuple1(j, MDSMatrixVerification[i][j]);
        }
        MDSPointsDistanceMatrix->AddColumn(col);
    }

    
    
    const auto zeroPad2
    = [](std::string &colName, const size_t numberCols, const size_t colIdx) {
        std::string max{std::to_string(numberCols - 1)};
        std::string cur{std::to_string(colIdx)};
        std::string zer(max.size() - cur.size(), '0');
        colName.append(zer).append(cur);
    };

/*
    for(size_t i = 0; i < MDSMatrix.size(); ++i) {
        std::string name{"Comparison "};
        zeroPad2(name, MDSMatrix.size(), i);

        vtkNew<vtkDoubleArray> col{};
        col->SetNumberOfTuples(MDSMatrix.size());
        col->SetName(name.c_str());
        for(size_t j = 0; j < MDSMatrix[i].size(); ++j) {
            col->SetTuple1(j, std::abs(MDSMatrix[i][j]-MDSMatrixVerification[i][j]));
        }
        DistanceMatrixDistorsion->AddColumn(col);
    }
    */

    for(size_t i = 0; i < MDSMatrix2.size(); ++i) {
        std::string name{"Diagram "};
        zeroPad2(name, MDSMatrix2.size(), i);

        vtkNew<vtkDoubleArray> col{};
        col->SetNumberOfTuples(MDSMatrix2.size());
        col->SetName(name.c_str());
        for(size_t j = 0; j < MDSMatrix2[i].size(); ++j) {
            col->SetTuple1(j, MDSMatrix2[i][j]);
        }
        DistanceMatrixDistorsion->AddColumn(col);
    }
    
    return 1;

}
