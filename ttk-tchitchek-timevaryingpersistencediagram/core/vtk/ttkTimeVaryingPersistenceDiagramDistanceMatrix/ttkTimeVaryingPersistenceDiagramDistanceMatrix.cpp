#include <ttkTimeVaryingPersistenceDiagramDistanceMatrix.h>
#include <ttkPersistenceDiagramUtils.h>
//#include <PersistenceDiagramClustering.h>


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


using namespace ttk;

vtkStandardNewMacro(ttkTimeVaryingPersistenceDiagramDistanceMatrix);

ttkTimeVaryingPersistenceDiagramDistanceMatrix::ttkTimeVaryingPersistenceDiagramDistanceMatrix() {
    SetNumberOfInputPorts(1);
    SetNumberOfOutputPorts(1);
}

int ttkTimeVaryingPersistenceDiagramDistanceMatrix::FillInputPortInformation(
    int port, vtkInformation *info) {
    if(port == 0) {
        info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkMultiBlockDataSet");
        info->Set(vtkAlgorithm::INPUT_IS_REPEATABLE(), 1);
        return 1;
    }
    return 0;
}

int ttkTimeVaryingPersistenceDiagramDistanceMatrix::FillOutputPortInformation(
    int port, vtkInformation *info) {
    if(port == 0) {
        info->Set(vtkDataObject::DATA_TYPE_NAME(), "vtkTable");
        return 1;
    }
    return 0;
}

// to adapt if your wrapper does not inherit from vtkDataSetAlgorithm
int ttkTimeVaryingPersistenceDiagramDistanceMatrix::RequestData(
    vtkInformation * /*request*/,
    vtkInformationVector **inputVector,
    vtkInformationVector *outputVector) {
    ttk::Memory m;

    auto blocks = vtkMultiBlockDataSet::GetData(inputVector[0], 0);

    int nBlocks = blocks->GetNumberOfBlocks();
    cout << "nBlocks in the first vtkMultiBlockDataSet (inputVector[0]): " << nBlocks << endl ;

    std::vector<std::vector<std::pair<ttk::DiagramType, double>>> TemporalPersistenceDiagramTimeSeriesSet;

    for(int i = 0; i < nBlocks; i++) {

        auto block = blocks->GetBlock(i);
        vtkMultiBlockDataSet * multiBlockDataSet = vtkMultiBlockDataSet::SafeDownCast(block);
        cout << "block " << i <<" has "<< multiBlockDataSet->GetNumberOfBlocks() << " elements" << endl;

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

    // Set output
    auto diagramsDistTable = vtkTable::GetData(outputVector);
    
    const auto diagramsDistMat = this->execute(TemporalPersistenceDiagramTimeSeriesSet, Delta, Weight, Distance, beta);

    const auto zeroPad
    = [](std::string &colName, const size_t numberCols, const size_t colIdx) {
        std::string max{std::to_string(numberCols - 1)};
        std::string cur{std::to_string(colIdx)};
        std::string zer(max.size() - cur.size(), '0');
        colName.append(zer).append(cur);
    };


    for(size_t i = 0; i < diagramsDistMat.size(); ++i) {
        std::string name{"Diagram"};
        zeroPad(name, diagramsDistMat.size(), i);

        vtkNew<vtkDoubleArray> col{};
        col->SetNumberOfTuples(diagramsDistMat.size());
        col->SetName(name.c_str());
        for(size_t j = 0; j < diagramsDistMat[i].size(); ++j) {
            col->SetTuple1(j, diagramsDistMat[i][j]);
        }
        diagramsDistTable->AddColumn(col);
    }


    return 1;

}


