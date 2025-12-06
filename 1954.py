from paraview.simple import *
paraview.simple._DisableFirstRenderCameraReset()

import os
import glob
import vtk

try:
    SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
except NameError:
    SCRIPT_DIR = os.getcwd()

REPO_ROOT = SCRIPT_DIR

base_path = os.path.join(
    REPO_ROOT,
    "vestec-wp3",
    "demonstrator",
    "space_weather_in-situ",
    "data",
    "pdiags.cdb",
    "data",
)

output_dir = os.path.join(REPO_ROOT, "data")
os.makedirs(output_dir, exist_ok=True)
cinema_db_path = os.path.join(output_dir, "1954.cdb")

readers = []

for idx in range(2150, 2271):  

    time_value = 537.5 + 0.25 * (idx - 2150)

    time_str = f"{time_value:g}"

    filename = os.path.join(
        base_path,
        f"GEM_{idx}_0.0195_0_0.0195_4_particles_{time_str}_mag(B).vtu",
    )
    reg_name = f"GEM_{idx}_0.0195_0_0.0195_4_particles_{time_str}_mag(B).vtu"

    reader = XMLUnstructuredGridReader(
        registrationName=reg_name,
        FileName=[filename]
    )

    RenameSource(f"particles_{idx}", reader)

    readers.append(reader)

tTKBlockAggregator1 = TTKBlockAggregator(
    registrationName='TTKBlockAggregator1',
    Input=readers
)

UpdatePipeline(time=537.5, proxy=tTKBlockAggregator1)


tTKCinemaWriter1 = TTKCinemaWriter(
    registrationName='TTKCinemaWriter1',
    Input=tTKBlockAggregator1,
    DatabasePath=cinema_db_path,
)

tTKCinemaWriter1.IterateMultiBlock = 1

UpdatePipeline(time=537.5, proxy=tTKCinemaWriter1)

# ----------------------------------------------------------------

data_dir = os.path.join(cinema_db_path, "data")

for idx in range(2150, 2271):
    time_value = 537.5 + 0.25 * (idx - 2150)
    time_str = f"{time_value:g}"

    basename = f"GEM_{idx}_0.0195_0_0.0195_4_particles_{time_str}_mag(B).vtu"
    vtu_path = os.path.join(data_dir, basename)

    if not os.path.isfile(vtu_path):
        print(f"  WARNING: {vtu_path} not found, skipping.")
        continue

    print(f"  Updating TimeValue in {basename} -> {time_value}")

    r = vtk.vtkXMLUnstructuredGridReader()
    r.SetFileName(vtu_path)
    r.Update()

    ug = r.GetOutput()
    field_data = ug.GetFieldData()

    time_arr = field_data.GetArray("TimeValue")
    if time_arr is None:
        time_arr = vtk.vtkDoubleArray()
        time_arr.SetName("TimeValue")
        time_arr.SetNumberOfComponents(1)
        time_arr.SetNumberOfTuples(1)
        field_data.AddArray(time_arr)

    time_arr.SetTuple1(0, time_value)

    w = vtk.vtkXMLUnstructuredGridWriter()
    w.SetFileName(vtu_path)
    w.SetInputData(ug)
    w.Write()

# ----------------------------------------------------------------

print(f"Post-processing VTK files in: {data_dir}")

vtu_files = sorted(glob.glob(os.path.join(data_dir, "*.vtu")))

for vtu_path in vtu_files:
    print(f"  Updating {os.path.basename(vtu_path)}")

    reader = vtk.vtkXMLUnstructuredGridReader()
    reader.SetFileName(vtu_path)
    reader.Update()

    ug = reader.GetOutput()
    point_data = ug.GetPointData()
    cell_data = ug.GetCellData()

    if cell_data.GetArray("Birth") is not None and \
       cell_data.GetArray("IsFinite") is not None:
        print("Birth / IsFinite already present in CellData, skip.")
        continue

    points = ug.GetPoints()
    coords = points.GetData()

    n_cells = ug.GetNumberOfCells()

    birth_arr = vtk.vtkDoubleArray()
    birth_arr.SetName("Birth")
    birth_arr.SetNumberOfComponents(1)
    birth_arr.SetNumberOfTuples(n_cells)

    isfinite_arr = vtk.vtkIntArray()
    isfinite_arr.SetName("IsFinite")
    isfinite_arr.SetNumberOfComponents(1)
    isfinite_arr.SetNumberOfTuples(n_cells)

    id_list = vtk.vtkIdList()

    for cid in range(n_cells):
        ug.GetCellPoints(cid, id_list)
        n_ids = id_list.GetNumberOfIds()

        sx = 0.0
        for k in range(n_ids):
            pid = id_list.GetId(k)
            x, y, z = coords.GetTuple3(pid)
            sx += x
        birth_value = sx / n_ids if n_ids > 0 else 0.0

        birth_arr.SetValue(cid, birth_value)
        isfinite_arr.SetValue(cid, 1)

    
    cell_data.AddArray(birth_arr)
    cell_data.AddArray(isfinite_arr)

    writer = vtk.vtkXMLUnstructuredGridWriter()
    writer.SetFileName(vtu_path)
    writer.SetInputData(ug)
    writer.Write()
    
    
