from paraview.simple import *
paraview.simple._DisableFirstRenderCameraReset()

import os

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
cinema_db_path = os.path.join(output_dir, "1952.cdb")

readers = []

for idx in range(1850, 1971):  

    time_value = 462.5 + 0.25 * (idx - 1850)

    time_str = f"{time_value:g}"

    filename = os.path.join(
        base_path,
        f"GEM_{idx}_0.0195_0_0.0195_2_particles_{time_str}_mag(B).vtu",
    )
    reg_name = f"GEM_{idx}_0.0195_0_0.0195_2_particles_{time_str}_mag(B).vtu"

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

UpdatePipeline(time=462.5, proxy=tTKBlockAggregator1)


tTKCinemaWriter1 = TTKCinemaWriter(
    registrationName='TTKCinemaWriter1',
    Input=tTKBlockAggregator1,
    DatabasePath=cinema_db_path,
)

tTKCinemaWriter1.IterateMultiBlock = 1

UpdatePipeline(time=462.5, proxy=tTKCinemaWriter1)

