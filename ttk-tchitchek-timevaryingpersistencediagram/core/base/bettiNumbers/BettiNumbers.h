/// TODO 1: Provide your information
///
/// \ingroup base
/// \class ttk::BettiNumbers
/// \author TORI tchitchek <sebastien.tchitchek@lip6.fr>
/// \date 01/01/2023.
///
/// This module defines the %BettiNumbers class that computes for each vertex of a
/// triangulation the average scalar value of itself and its direct neighbors.
///
/// \b Related \b publication: \n
/// 'BettiNumbers'
/// Jonas Lukasczyk and Julien Tierny.
/// TTK Publications.
/// 2021.
///


// ttk common includes
#include <Debug.h>
#include <Triangulation.h>
#include <../unionFind/UnionFind.h>

namespace ttk {

  /**
   * The BettiNumbers class provides methods to compute for each vertex of a
   * triangulation the average scalar value of itself and its direct neighbors.
   */
  class BettiNumbers : virtual public Debug {

  public:
    BettiNumbers();

    /**
     * TODO 2: This method preconditions the triangulation for all operations
     *         the algorithm of this module requires. For instance,
     *         preconditionVertexNeighbors, preconditionBoundaryEdges, ...
     *
     *         Note: If the algorithm does not require a triangulation then
     *               this method can be deleted.
     */
    int preconditionTriangulation(
      ttk::AbstractTriangulation *triangulation) const {
      
      triangulation->preconditionBoundaryTriangles();      
      triangulation->preconditionEdgeTriangles(); 
      triangulation-> preconditionTriangles(); 
      triangulation-> preconditionTriangleEdges(); 

      
      return triangulation->preconditionVertexNeighbors();
    }

    /**
     * TODO 3: Implmentation of the algorithm.
     *
     *         Note: If the algorithm requires a triangulation then this
     *               method must be called after the triangulation has been
     *               preconditioned for the upcoming operations.
     */
    template <class dataType,
              class triangulationType = ttk::AbstractTriangulation>
    int computeAverages(dataType *outputData,
                        const dataType *inputData,
                        const triangulationType *triangulation) const {
      // start global timer
      ttk::Timer globalTimer;

      // print horizontal separator
      this->printMsg(ttk::debug::Separator::L1); // L1 is the '=' separator

      // print input parameters in table format
      this->printMsg({
        {"#Threads", std::to_string(this->threadNumber_)},
        {"#Vertices", std::to_string(triangulation->getNumberOfVertices())},
      });
      this->printMsg(ttk::debug::Separator::L1);

      // -----------------------------------------------------------------------
      // Compute Vertex Averages
      // -----------------------------------------------------------------------
      {
        // start a local timer for this subprocedure
        ttk::Timer localTimer;

        // print the progress of the current subprocedure (currently 0%)
        this->printMsg("Computing Averages",
                       0, // progress form 0-1
                       0, // elapsed time so far
                       this->threadNumber_, ttk::debug::LineMode::REPLACE);

        // compute the average of each vertex in parallel
        size_t nVertices = triangulation->getNumberOfVertices();
#ifdef TTK_ENABLE_OPENMP
#pragma omp parallel for num_threads(this->threadNumber_)
#endif
        for(size_t i = 0; i < nVertices; i++) {
          // initialize average
          outputData[i] = inputData[i];

          // add neighbor values to average
          size_t nNeighbors = triangulation->getVertexNeighborNumber(i);
          ttk::SimplexId neighborId{-1};
          for(size_t j = 0; j < nNeighbors; j++) {
            triangulation->getVertexNeighbor(i, j, neighborId);
            outputData[i] += inputData[neighborId];
          }

          // devide by neighbor number
          outputData[i] /= (nNeighbors + 1);
        }

        // print the progress of the current subprocedure with elapsed time
        this->printMsg("Computing Averages",
                       1, // progress
                       localTimer.getElapsedTime(), this->threadNumber_);
      }

      // ---------------------------------------------------------------------
      // print global performance
      // ---------------------------------------------------------------------
      {
        this->printMsg(ttk::debug::Separator::L2); // horizontal '-' separator
        this->printMsg(
          "Complete", 1, globalTimer.getElapsedTime() // global progress, time
        );
        this->printMsg(ttk::debug::Separator::L1); // horizontal '=' separator
      }







//#############################################################################
//#############################################################################
//#############################################################################
//#############################################################################












      
  int vertexNumber = triangulation->getNumberOfVertices();












  //











  std::vector<ttk::UnionFind> UF_vector(vertexNumber);
  std::vector<ttk::UnionFind *> UF_vector_pt(vertexNumber);
 
  for(SimplexId i = 0; i < UF_vector.size(); i++){
     UF_vector_pt[i] = &(UF_vector[i]);
  }

  for(int edgeID = 0; edgeID < triangulation->getNumberOfEdges(); edgeID++){
	 
     std::array<SimplexId, 2> vertexIds{};
     triangulation->getEdgeVertex(edgeID, 0, vertexIds[0]);
     triangulation->getEdgeVertex(edgeID, 1, vertexIds[1]);
    
     UF_vector_pt[vertexIds[0]] = UnionFind::makeUnion(UF_vector_pt.at(vertexIds[0]), UF_vector_pt.at(vertexIds[1]));
     UF_vector_pt[vertexIds[1]] =  UF_vector_pt[vertexIds[0]];
    
  }

  for(SimplexId i = 0; i < UF_vector_pt.size(); i++){
  
     UF_vector_pt[i] = UF_vector_pt[i]->find();
     
  }
 
  std::sort(UF_vector_pt.begin(), UF_vector_pt.end());
  int B0 = std::unique(UF_vector_pt.begin(), UF_vector_pt.end()) - UF_vector_pt.begin();
 











//










  const SimplexId triangles_number = triangulation->getNumberOfTriangles();
  std::vector<SimplexId> List1;

  std::vector<SimplexId> List2(triangles_number, -1);


  for(auto i = 0; i < triangles_number; i++){
     if(triangulation->isTriangleOnBoundary(i)){
        List2[i] = List1.size();
        List1.push_back(i);
     }
  }


  std::vector<ttk::UnionFind> UF_vector_2(List1.size());
  std::vector<ttk::UnionFind *> UF_vector_pt_2(List1.size());

  for(SimplexId i = 0; i < UF_vector_2.size(); i++){
     UF_vector_pt_2[i] = &(UF_vector_2[i]);
  }




for(long unsigned int i = 0; i < List1.size(); i++){
   
  int triangleId = i;
  ttk::SimplexId triangle = List1[triangleId];
  

  for(auto j = 0; j < 3; j++){
     SimplexId edgeId = -1;

     triangulation->getTriangleEdge(triangle, j, edgeId);
  // retrieve the number of triangles that have this edge
     int triangles_on_edge_number = triangulation->getEdgeTriangleNumber(edgeId);

  // one speedout the set of its neighboring triangles
     for(int k = 0; k < triangles_on_edge_number; k++){
         SimplexId triangleNeighbourID = -1;
         triangulation->getEdgeTriangle(edgeId, k, triangleNeighbourID);

         if(triangulation->isTriangleOnBoundary(triangleNeighbourID)){

            int l = List2[triangleNeighbourID];
            UF_vector_pt_2[i] = UnionFind::makeUnion(UF_vector_pt_2.at(i), UF_vector_pt_2.at(l));
            UF_vector_pt_2[l] = UF_vector_pt_2[i] ;

         }
      }
   }
 }


  for(SimplexId i = 0; i < UF_vector_pt_2.size(); i++)
  UF_vector_pt_2[i] = UF_vector_pt_2[i]->find();



  std::sort(UF_vector_pt_2.begin(), UF_vector_pt_2.end());
  int B2_tmp = std::unique(UF_vector_pt_2.begin(), UF_vector_pt_2.end()) - UF_vector_pt_2.begin();

  int B2 = B2_tmp  - B0;
















  //
  
  
  
  
  
  
  
  
  
  
  
  

  int edges_number = triangulation->getNumberOfEdges(), tetras_number = triangulation->getNumberOfCells(), triangles_number_2 = triangulation->getNumberOfTriangles(), vertices_numbers = triangulation->getNumberOfVertices();
  int B1 = B2 + B0 + tetras_number + edges_number - vertices_numbers - triangles_number_2;  
  
  std::cout << std::endl << "The 0-dimensional Betti number of this simplicial complex is " << B0 << ", its 1-dimensional Betti number is " << B1 << ", and its 2-dimensional Betti number is " << B2<<"."<<std::endl<<std::endl<<std::endl;
      
      
      
      return 1; // return success
      
      
      
    }

  }; // BettiNumbers class
  




} // namespace ttk
