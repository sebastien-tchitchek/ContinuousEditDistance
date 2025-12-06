# state file generated using paraview version 5.11.1
import paraview
paraview.compatibility.major = 5
paraview.compatibility.minor = 11

#### import the simple module from the paraview
from paraview.simple import *
#### disable automatic camera reset on 'Show'
paraview.simple._DisableFirstRenderCameraReset()

# ----------------------------------------------------------------
# setup views used in the visualization
# ----------------------------------------------------------------

# Create a new 'Render View'
pD_002_1 = CreateView('RenderView')
pD_002_1.ViewSize = [737, 294]
pD_002_1.InteractionMode = '2D'
pD_002_1.AxesGrid = 'GridAxes3DActor'
pD_002_1.OrientationAxesVisibility = 0
pD_002_1.CenterOfRotation = [0.49068529810756445, 0.500000967644155, 0.0]
pD_002_1.StereoType = 'Crystal Eyes'
pD_002_1.CameraPosition = [0.49068529810756445, 0.500000967644155, 2.8170568346376177]
pD_002_1.CameraFocalPoint = [0.49068529810756445, 0.500000967644155, 0.0]
pD_002_1.CameraFocalDisk = 1.0
pD_002_1.CameraParallelScale = 0.5759016010111546
pD_002_1.UseColorPaletteForBackground = 0
pD_002_1.Background = [1.0, 1.0, 1.0]

# Create a new 'Render View'
pD_004_1 = CreateView('RenderView')
pD_004_1.ViewSize = [737, 293]
pD_004_1.InteractionMode = '2D'
pD_004_1.AxesGrid = 'GridAxes3DActor'
pD_004_1.OrientationAxesVisibility = 0
pD_004_1.CenterOfRotation = [0.4878192385658622, 0.500000967644155, 0.0]
pD_004_1.StereoType = 'Crystal Eyes'
pD_004_1.CameraPosition = [0.4878192385658622, 0.500000967644155, 2.8093117408165393]
pD_004_1.CameraFocalPoint = [0.4878192385658622, 0.500000967644155, 0.0]
pD_004_1.CameraFocalDisk = 1.0
pD_004_1.CameraParallelScale = 0.576278372658583
pD_004_1.UseColorPaletteForBackground = 0
pD_004_1.Background = [1.0, 1.0, 1.0]

# Create a new 'Render View'
pD_centroid1_1 = CreateView('RenderView')
pD_centroid1_1.ViewSize = [737, 223]
pD_centroid1_1.InteractionMode = '2D'
pD_centroid1_1.AxesGrid = 'GridAxes3DActor'
pD_centroid1_1.OrientationAxesVisibility = 0
pD_centroid1_1.CenterOfRotation = [0.4930142303928733, 0.500000967644155, 0.0]
pD_centroid1_1.StereoType = 'Crystal Eyes'
pD_centroid1_1.CameraPosition = [0.4930142303928733, 0.500000967644155, 2.8233667616206177]
pD_centroid1_1.CameraFocalPoint = [0.4930142303928733, 0.500000967644155, 0.0]
pD_centroid1_1.CameraFocalDisk = 1.0
pD_centroid1_1.CameraParallelScale = 0.5752720084517261
pD_centroid1_1.UseColorPaletteForBackground = 0
pD_centroid1_1.Background = [1.0, 1.0, 1.0]

# Create a new 'Render View'
tVPDs = CreateView('RenderView')
tVPDs.ViewSize = [737, 882]
tVPDs.AxesGrid = 'GridAxes3DActor'
tVPDs.OrientationAxesVisibility = 0
tVPDs.CenterOfRotation = [-0.024637922644615173, -0.0014095008373260498, -0.03664550185203552]
tVPDs.StereoType = 'Crystal Eyes'
tVPDs.CameraPosition = [1.5068761585466846, 0.8281549001414512, 0.8298979312807905]
tVPDs.CameraFocalPoint = [-0.024637922644615166, -0.0014095008373260453, -0.03664550185203553]
tVPDs.CameraViewUp = [0.4795935832085174, -0.8774577160140143, -0.007612591719280554]
tVPDs.CameraFocalDisk = 1.0
tVPDs.CameraParallelScale = 0.5035084886493149
tVPDs.UseColorPaletteForBackground = 0
tVPDs.Background = [1.0, 1.0, 1.0]

SetActiveView(None)

# ----------------------------------------------------------------
# setup view layouts
# ----------------------------------------------------------------

# create new layout object 'Layout #2'
layout2 = CreateLayout(name='Layout #2')
layout2.SplitHorizontal(0, 0.500000)
layout2.AssignView(1, tVPDs)
layout2.SplitVertical(2, 0.718818)
layout2.SplitVertical(5, 0.500000)
layout2.AssignView(11, pD_002_1)
layout2.AssignView(12, pD_004_1)
layout2.AssignView(6, pD_centroid1_1)
layout2.SetSize(1475, 882)

# ----------------------------------------------------------------
# restore active view
SetActiveView(tVPDs)
# ----------------------------------------------------------------

# ----------------------------------------------------------------
# setup the data processing pipelines
# ----------------------------------------------------------------

# create a new 'TTK CinemaReader'
tTKCinemaReader1 = TTKCinemaReader(registrationName='TTKCinemaReader1', DatabasePath='./data/002.cdb')

# create a new 'TTK CinemaProductReader'
tTKCinemaProductReader1 = TTKCinemaProductReader(registrationName='TTKCinemaProductReader1', Input=tTKCinemaReader1)

# create a new 'TTK CinemaReader'
tTKCinemaReader4 = TTKCinemaReader(registrationName='TTKCinemaReader4', DatabasePath='./data/1954.cdb')

# create a new 'TTK CinemaReader'
tTKCinemaReader3 = TTKCinemaReader(registrationName='TTKCinemaReader3', DatabasePath='./data/1952.cdb')

# create a new 'TTK CinemaProductReader'
tTKCinemaProductReader4 = TTKCinemaProductReader(registrationName='TTKCinemaProductReader4', Input=tTKCinemaReader4)

# create a new 'TTK CinemaProductReader'
tTKCinemaProductReader3 = TTKCinemaProductReader(registrationName='TTKCinemaProductReader3', Input=tTKCinemaReader3)

# create a new 'TTK CinemaReader'
tTKCinemaReader2 = TTKCinemaReader(registrationName='TTKCinemaReader2', DatabasePath='./data/004.cdb')

# create a new 'TTK CinemaProductReader'
tTKCinemaProductReader2 = TTKCinemaProductReader(registrationName='TTKCinemaProductReader2', Input=tTKCinemaReader2)

# create a new 'TTK BlockAggregator'
tTKBlockAggregator1 = TTKBlockAggregator(registrationName='TTKBlockAggregator1', Input=[tTKCinemaProductReader1, tTKCinemaProductReader2, tTKCinemaProductReader3, tTKCinemaProductReader4])
tTKBlockAggregator1.FlattenInput = 0

# create a new 'TTK TimeVaryingPersistenceDiagramClustering'
tTKTimeVaryingPersistenceDiagramClustering1 = TTKTimeVaryingPersistenceDiagramClustering(registrationName='TTKTimeVaryingPersistenceDiagramClustering1', Input=tTKBlockAggregator1)
tTKTimeVaryingPersistenceDiagramClustering1.Timestepscolumnname = 'TimeValue'
tTKTimeVaryingPersistenceDiagramClustering1.Delta = 0.2
tTKTimeVaryingPersistenceDiagramClustering1.Segmentationparameter = 6.0
tTKTimeVaryingPersistenceDiagramClustering1.Weight = 0.0029
tTKTimeVaryingPersistenceDiagramClustering1.numberOfDeparturesStochasticBarycenterComputation = 1
tTKTimeVaryingPersistenceDiagramClustering1.Kmeansiterations = 3
tTKTimeVaryingPersistenceDiagramClustering1.stochasticcriteriaforthekmeans = 'use of a stopping criteria for the stochastic kmeans'
tTKTimeVaryingPersistenceDiagramClustering1.GeodesicBarycenter = 'Enabled'
tTKTimeVaryingPersistenceDiagramClustering1.NumberofiterationsforthesmoothingoftheTVPDs = 15

# create a new 'Extract Block'
extractBlock2 = ExtractBlock(registrationName='ExtractBlock2', Input=tTKTimeVaryingPersistenceDiagramClustering1)
extractBlock2.Selectors = ['/Root/Block0/Block1/Block0']

# create a new 'Extract Surface'
extractSurface2 = ExtractSurface(registrationName='ExtractSurface2', Input=extractBlock2)

# create a new 'Tube'
tube2 = Tube(registrationName='Tube2', Input=extractSurface2)
tube2.Scalars = ['POINTS', 'CriticalType']
tube2.Vectors = ['POINTS', 'Coordinates']
tube2.Radius = 0.009999999536743189

# create a new 'Cell Data to Point Data'
cellDatatoPointData2 = CellDatatoPointData(registrationName='CellDatatoPointData2', Input=extractBlock2)
cellDatatoPointData2.CellDataArraytoprocess = ['Birth', 'IsFinite', 'PairIdentifier', 'PairType', 'Persistence']
cellDatatoPointData2.PassCellData = 1

# find source
tTKTimeVaryingPersistenceDiagramClustering1_1 = FindSource('TTKTimeVaryingPersistenceDiagramClustering1')

# create a new 'Extract Block'
extractBlock4 = ExtractBlock(registrationName='ExtractBlock4', Input=OutputPort(tTKTimeVaryingPersistenceDiagramClustering1_1,1))
extractBlock4.Selectors = ['/Root']

# create a new 'Extract Surface'
extractSurface4 = ExtractSurface(registrationName='ExtractSurface4', Input=extractBlock4)

# create a new 'Threshold'
threshold3 = Threshold(registrationName='Threshold3', Input=extractSurface4)
threshold3.Scalars = ['CELLS', 'Cell TVPD identifier']
threshold3.LowerThreshold = 4.0
threshold3.UpperThreshold = 5.0

# create a new 'Extract Surface'
extractSurface5 = ExtractSurface(registrationName='ExtractSurface5', Input=threshold3)

# create a new 'Tube'
tube4 = Tube(registrationName='Tube4', Input=extractSurface5)
tube4.Scalars = ['POINTS', 'Cluster']
tube4.Vectors = ['POINTS', '1']
tube4.Radius = 0.01

# create a new 'Threshold'
threshold4 = Threshold(registrationName='Threshold4', Input=extractSurface4)
threshold4.Scalars = ['CELLS', 'Cell TVPD identifier']
threshold4.UpperThreshold = 3.0

# create a new 'Extract Surface'
extractSurface6 = ExtractSurface(registrationName='ExtractSurface6', Input=threshold4)

# find source
tTKTimeVaryingPersistenceDiagramClustering1_2 = FindSource('TTKTimeVaryingPersistenceDiagramClustering1')

# create a new 'Threshold'
threshold2 = Threshold(registrationName='Threshold2', Input=OutputPort(tTKTimeVaryingPersistenceDiagramClustering1_2,2))
threshold2.Scalars = ['CELLS', 'Cluster']
threshold2.LowerThreshold = 1.0
threshold2.UpperThreshold = 1.0

# create a new 'Threshold'
threshold1 = Threshold(registrationName='Threshold1', Input=OutputPort(tTKTimeVaryingPersistenceDiagramClustering1_2,2))
threshold1.Scalars = ['CELLS', 'Cluster']

# find source
tTKTimeVaryingPersistenceDiagramClustering1_3 = FindSource('TTKTimeVaryingPersistenceDiagramClustering1')

# create a new 'Extract Block'
extractBlock3 = ExtractBlock(registrationName='ExtractBlock3', Input=OutputPort(tTKTimeVaryingPersistenceDiagramClustering1_3,7))
extractBlock3.Selectors = ['/Root/Block0/Block0']

# create a new 'Extract Surface'
extractSurface3 = ExtractSurface(registrationName='ExtractSurface3', Input=extractBlock3)

# create a new 'Tube'
tube3 = Tube(registrationName='Tube3', Input=extractSurface3)
tube3.Scalars = ['POINTS', 'CriticalType']
tube3.Vectors = ['POINTS', 'Coordinates']
tube3.Radius = 0.009999999536743189

# create a new 'Tube'
tube5 = Tube(registrationName='Tube5', Input=extractSurface6)
tube5.Scalars = ['POINTS', 'Cluster']
tube5.Vectors = ['POINTS', '1']
tube5.Radius = 0.006374821066856384

# create a new 'Python Calculator'
pythonCalculator2 = PythonCalculator(registrationName='PythonCalculator2', Input=cellDatatoPointData2)
pythonCalculator2.Expression = '(inputs[0].PointData["IsFinite"] - 1)*10 - inputs[0].PointData["CriticalType"]'

# create a new 'TTK IcospheresFromPoints'
tTKIcospheresFromPoints2 = TTKIcospheresFromPoints(registrationName='TTKIcospheresFromPoints2', Input=pythonCalculator2)
tTKIcospheresFromPoints2.Subdivisions = 4
tTKIcospheresFromPoints2.Radius = 0.02

# create a new 'Extract Block'
extractBlock1 = ExtractBlock(registrationName='ExtractBlock1', Input=tTKTimeVaryingPersistenceDiagramClustering1)
extractBlock1.Selectors = ['/Root/Block0/Block0/Block0']

# create a new 'Cell Data to Point Data'
cellDatatoPointData1 = CellDatatoPointData(registrationName='CellDatatoPointData1', Input=extractBlock1)
cellDatatoPointData1.CellDataArraytoprocess = ['Birth', 'IsFinite', 'PairIdentifier', 'PairType', 'Persistence']
cellDatatoPointData1.PassCellData = 1

# create a new 'Extract Surface'
extractSurface1 = ExtractSurface(registrationName='ExtractSurface1', Input=extractBlock1)

# create a new 'Tube'
tube1 = Tube(registrationName='Tube1', Input=extractSurface1)
tube1.Scalars = ['POINTS', 'CriticalType']
tube1.Vectors = ['POINTS', 'Coordinates']
tube1.Radius = 0.009999999536743189

# create a new 'Cell Data to Point Data'
cellDatatoPointData3 = CellDatatoPointData(registrationName='CellDatatoPointData3', Input=extractBlock3)
cellDatatoPointData3.CellDataArraytoprocess = ['Birth', 'IsFinite', 'PairIdentifier', 'PairType', 'Persistence']
cellDatatoPointData3.PassCellData = 1

# create a new 'Python Calculator'
pythonCalculator3 = PythonCalculator(registrationName='PythonCalculator3', Input=cellDatatoPointData3)
pythonCalculator3.Expression = '(inputs[0].PointData["IsFinite"] - 1)*10 - inputs[0].PointData["CriticalType"]'

# create a new 'TTK IcospheresFromPoints'
tTKIcospheresFromPoints3 = TTKIcospheresFromPoints(registrationName='TTKIcospheresFromPoints3', Input=pythonCalculator3)
tTKIcospheresFromPoints3.Subdivisions = 4
tTKIcospheresFromPoints3.Radius = 0.02

# create a new 'Python Calculator'
pythonCalculator1 = PythonCalculator(registrationName='PythonCalculator1', Input=cellDatatoPointData1)
pythonCalculator1.Expression = '(inputs[0].PointData["IsFinite"] - 1)*10 - inputs[0].PointData["CriticalType"]'

# create a new 'TTK IcospheresFromPoints'
tTKIcospheresFromPoints1 = TTKIcospheresFromPoints(registrationName='TTKIcospheresFromPoints1', Input=pythonCalculator1)
tTKIcospheresFromPoints1.Subdivisions = 4
tTKIcospheresFromPoints1.Radius = 0.02

# ----------------------------------------------------------------
# setup the visualization in view 'pD_002_1'
# ----------------------------------------------------------------

# show data from extractBlock1
extractBlock1Display = Show(extractBlock1, pD_002_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
extractBlock1Display.Representation = 'Surface'
extractBlock1Display.ColorArrayName = [None, '']
extractBlock1Display.SelectTCoordArray = 'None'
extractBlock1Display.SelectNormalArray = 'None'
extractBlock1Display.SelectTangentArray = 'None'
extractBlock1Display.OSPRayScaleArray = 'Coordinates'
extractBlock1Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractBlock1Display.SelectOrientationVectors = 'Coordinates'
extractBlock1Display.ScaleFactor = 0.0999999953674319
extractBlock1Display.SelectScaleArray = 'Coordinates'
extractBlock1Display.GlyphType = 'Arrow'
extractBlock1Display.GlyphTableIndexArray = 'Coordinates'
extractBlock1Display.GaussianRadius = 0.004999999768371594
extractBlock1Display.SetScaleArray = ['POINTS', 'Coordinates']
extractBlock1Display.ScaleTransferFunction = 'PiecewiseFunction'
extractBlock1Display.OpacityArray = ['POINTS', 'Coordinates']
extractBlock1Display.OpacityTransferFunction = 'PiecewiseFunction'
extractBlock1Display.DataAxesGrid = 'GridAxesRepresentation'
extractBlock1Display.PolarAxes = 'PolarAxesRepresentation'
extractBlock1Display.ScalarOpacityUnitDistance = 0.2239137566271583
extractBlock1Display.OpacityArrayName = ['POINTS', 'Coordinates']
extractBlock1Display.SelectInputVectors = ['POINTS', 'Coordinates']
extractBlock1Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
extractBlock1Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
extractBlock1Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from extractSurface1
extractSurface1Display = Show(extractSurface1, pD_002_1, 'GeometryRepresentation')

# trace defaults for the display properties.
extractSurface1Display.Representation = 'Surface'
extractSurface1Display.ColorArrayName = [None, '']
extractSurface1Display.SelectTCoordArray = 'None'
extractSurface1Display.SelectNormalArray = 'None'
extractSurface1Display.SelectTangentArray = 'None'
extractSurface1Display.OSPRayScaleArray = 'Coordinates'
extractSurface1Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractSurface1Display.SelectOrientationVectors = 'Coordinates'
extractSurface1Display.ScaleFactor = 0.0999999953674319
extractSurface1Display.SelectScaleArray = 'Coordinates'
extractSurface1Display.GlyphType = 'Arrow'
extractSurface1Display.GlyphTableIndexArray = 'Coordinates'
extractSurface1Display.GaussianRadius = 0.004999999768371594
extractSurface1Display.SetScaleArray = ['POINTS', 'Coordinates']
extractSurface1Display.ScaleTransferFunction = 'PiecewiseFunction'
extractSurface1Display.OpacityArray = ['POINTS', 'Coordinates']
extractSurface1Display.OpacityTransferFunction = 'PiecewiseFunction'
extractSurface1Display.DataAxesGrid = 'GridAxesRepresentation'
extractSurface1Display.PolarAxes = 'PolarAxesRepresentation'
extractSurface1Display.SelectInputVectors = ['POINTS', 'Coordinates']
extractSurface1Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
extractSurface1Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
extractSurface1Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from tube1
tube1Display = Show(tube1, pD_002_1, 'GeometryRepresentation')

# get 2D transfer function for 'PairType'
pairTypeTF2D = GetTransferFunction2D('PairType')

# get color transfer function/color map for 'PairType'
pairTypeLUT = GetColorTransferFunction('PairType')
pairTypeLUT.TransferFunction2D = pairTypeTF2D
pairTypeLUT.RGBPoints = [-1.0, 0.196078, 0.368627, 0.6, 0.020000000000000018, 1.0, 0.964706, 0.964706, 1.0099999999999998, 0.572549, 0.431373, 0.772549, 2.0, 0.247059, 0.0, 0.490196]
pairTypeLUT.ScalarRangeInitialized = 1.0

# trace defaults for the display properties.
tube1Display.Representation = 'Surface'
tube1Display.ColorArrayName = ['CELLS', 'PairType']
tube1Display.LookupTable = pairTypeLUT
tube1Display.SelectTCoordArray = 'None'
tube1Display.SelectNormalArray = 'TubeNormals'
tube1Display.SelectTangentArray = 'None'
tube1Display.OSPRayScaleArray = 'Coordinates'
tube1Display.OSPRayScaleFunction = 'PiecewiseFunction'
tube1Display.SelectOrientationVectors = 'Coordinates'
tube1Display.ScaleFactor = 0.1006123677827418
tube1Display.SelectScaleArray = 'Coordinates'
tube1Display.GlyphType = 'Arrow'
tube1Display.GlyphTableIndexArray = 'Coordinates'
tube1Display.GaussianRadius = 0.0050306183891370895
tube1Display.SetScaleArray = ['POINTS', 'Coordinates']
tube1Display.ScaleTransferFunction = 'PiecewiseFunction'
tube1Display.OpacityArray = ['POINTS', 'Coordinates']
tube1Display.OpacityTransferFunction = 'PiecewiseFunction'
tube1Display.DataAxesGrid = 'GridAxesRepresentation'
tube1Display.PolarAxes = 'PolarAxesRepresentation'
tube1Display.SelectInputVectors = ['POINTS', 'Coordinates']
tube1Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tube1Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tube1Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from cellDatatoPointData1
cellDatatoPointData1Display = Show(cellDatatoPointData1, pD_002_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
cellDatatoPointData1Display.Representation = 'Surface'
cellDatatoPointData1Display.ColorArrayName = [None, '']
cellDatatoPointData1Display.SelectTCoordArray = 'None'
cellDatatoPointData1Display.SelectNormalArray = 'None'
cellDatatoPointData1Display.SelectTangentArray = 'None'
cellDatatoPointData1Display.OSPRayScaleArray = 'Birth'
cellDatatoPointData1Display.OSPRayScaleFunction = 'PiecewiseFunction'
cellDatatoPointData1Display.SelectOrientationVectors = 'Coordinates'
cellDatatoPointData1Display.ScaleFactor = 0.0999999953674319
cellDatatoPointData1Display.SelectScaleArray = 'Birth'
cellDatatoPointData1Display.GlyphType = 'Arrow'
cellDatatoPointData1Display.GlyphTableIndexArray = 'Birth'
cellDatatoPointData1Display.GaussianRadius = 0.004999999768371594
cellDatatoPointData1Display.SetScaleArray = ['POINTS', 'Birth']
cellDatatoPointData1Display.ScaleTransferFunction = 'PiecewiseFunction'
cellDatatoPointData1Display.OpacityArray = ['POINTS', 'Birth']
cellDatatoPointData1Display.OpacityTransferFunction = 'PiecewiseFunction'
cellDatatoPointData1Display.DataAxesGrid = 'GridAxesRepresentation'
cellDatatoPointData1Display.PolarAxes = 'PolarAxesRepresentation'
cellDatatoPointData1Display.ScalarOpacityUnitDistance = 0.2239137566271583
cellDatatoPointData1Display.OpacityArrayName = ['POINTS', 'Birth']
cellDatatoPointData1Display.SelectInputVectors = ['POINTS', 'Coordinates']
cellDatatoPointData1Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
cellDatatoPointData1Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9813696265770165, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
cellDatatoPointData1Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9813696265770165, 1.0, 0.5, 0.0]

# show data from pythonCalculator1
pythonCalculator1Display = Show(pythonCalculator1, pD_002_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
pythonCalculator1Display.Representation = 'Surface'
pythonCalculator1Display.ColorArrayName = [None, '']
pythonCalculator1Display.SelectTCoordArray = 'None'
pythonCalculator1Display.SelectNormalArray = 'None'
pythonCalculator1Display.SelectTangentArray = 'None'
pythonCalculator1Display.OSPRayScaleArray = 'Birth'
pythonCalculator1Display.OSPRayScaleFunction = 'PiecewiseFunction'
pythonCalculator1Display.SelectOrientationVectors = 'Coordinates'
pythonCalculator1Display.ScaleFactor = 0.0999999953674319
pythonCalculator1Display.SelectScaleArray = 'Birth'
pythonCalculator1Display.GlyphType = 'Arrow'
pythonCalculator1Display.GlyphTableIndexArray = 'Birth'
pythonCalculator1Display.GaussianRadius = 0.004999999768371594
pythonCalculator1Display.SetScaleArray = ['POINTS', 'Birth']
pythonCalculator1Display.ScaleTransferFunction = 'PiecewiseFunction'
pythonCalculator1Display.OpacityArray = ['POINTS', 'Birth']
pythonCalculator1Display.OpacityTransferFunction = 'PiecewiseFunction'
pythonCalculator1Display.DataAxesGrid = 'GridAxesRepresentation'
pythonCalculator1Display.PolarAxes = 'PolarAxesRepresentation'
pythonCalculator1Display.ScalarOpacityUnitDistance = 0.2239137566271583
pythonCalculator1Display.OpacityArrayName = ['POINTS', 'Birth']
pythonCalculator1Display.SelectInputVectors = ['POINTS', 'Coordinates']
pythonCalculator1Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
pythonCalculator1Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9813696265770165, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
pythonCalculator1Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9813696265770165, 1.0, 0.5, 0.0]

# show data from tTKIcospheresFromPoints1
tTKIcospheresFromPoints1Display = Show(tTKIcospheresFromPoints1, pD_002_1, 'GeometryRepresentation')

# get 2D transfer function for 'CriticalType'
criticalTypeTF2D = GetTransferFunction2D('CriticalType')

# get color transfer function/color map for 'CriticalType'
criticalTypeLUT = GetColorTransferFunction('CriticalType')
criticalTypeLUT.TransferFunction2D = criticalTypeTF2D
criticalTypeLUT.RGBPoints = [0.0, 0.196078, 0.368627, 0.6, 1.02, 1.0, 0.964706, 0.964706, 2.01, 0.572549, 0.431373, 0.772549, 3.0, 0.247059, 0.0, 0.490196]
criticalTypeLUT.ScalarRangeInitialized = 1.0

# trace defaults for the display properties.
tTKIcospheresFromPoints1Display.Representation = 'Surface'
tTKIcospheresFromPoints1Display.ColorArrayName = ['POINTS', 'CriticalType']
tTKIcospheresFromPoints1Display.LookupTable = criticalTypeLUT
tTKIcospheresFromPoints1Display.SelectTCoordArray = 'None'
tTKIcospheresFromPoints1Display.SelectNormalArray = 'Normals'
tTKIcospheresFromPoints1Display.SelectTangentArray = 'None'
tTKIcospheresFromPoints1Display.OSPRayScaleArray = 'Birth'
tTKIcospheresFromPoints1Display.OSPRayScaleFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints1Display.SelectOrientationVectors = 'Coordinates'
tTKIcospheresFromPoints1Display.ScaleFactor = 0.10399999339133502
tTKIcospheresFromPoints1Display.SelectScaleArray = 'Birth'
tTKIcospheresFromPoints1Display.GlyphType = 'Arrow'
tTKIcospheresFromPoints1Display.GlyphTableIndexArray = 'Birth'
tTKIcospheresFromPoints1Display.GaussianRadius = 0.00519999966956675
tTKIcospheresFromPoints1Display.SetScaleArray = ['POINTS', 'Birth']
tTKIcospheresFromPoints1Display.ScaleTransferFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints1Display.OpacityArray = ['POINTS', 'Birth']
tTKIcospheresFromPoints1Display.OpacityTransferFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints1Display.DataAxesGrid = 'GridAxesRepresentation'
tTKIcospheresFromPoints1Display.PolarAxes = 'PolarAxesRepresentation'
tTKIcospheresFromPoints1Display.SelectInputVectors = ['POINTS', 'Coordinates']
tTKIcospheresFromPoints1Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tTKIcospheresFromPoints1Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9813696265770165, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tTKIcospheresFromPoints1Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9813696265770165, 1.0, 0.5, 0.0]

# setup the color legend parameters for each legend in this view

# get color legend/bar for pairTypeLUT in view pD_002_1
pairTypeLUTColorBar = GetScalarBar(pairTypeLUT, pD_002_1)
pairTypeLUTColorBar.Title = 'PairType'
pairTypeLUTColorBar.ComponentTitle = ''

# set color bar visibility
pairTypeLUTColorBar.Visibility = 0

# get color legend/bar for criticalTypeLUT in view pD_002_1
criticalTypeLUTColorBar = GetScalarBar(criticalTypeLUT, pD_002_1)
criticalTypeLUTColorBar.WindowLocation = 'Upper Right Corner'
criticalTypeLUTColorBar.Title = 'CriticalType'
criticalTypeLUTColorBar.ComponentTitle = ''

# set color bar visibility
criticalTypeLUTColorBar.Visibility = 0

# hide data in view
Hide(extractBlock1, pD_002_1)

# hide data in view
Hide(extractSurface1, pD_002_1)

# hide data in view
Hide(cellDatatoPointData1, pD_002_1)

# ----------------------------------------------------------------
# setup the visualization in view 'pD_004_1'
# ----------------------------------------------------------------

# show data from extractBlock3
extractBlock3Display = Show(extractBlock3, pD_004_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
extractBlock3Display.Representation = 'Surface'
extractBlock3Display.ColorArrayName = [None, '']
extractBlock3Display.SelectTCoordArray = 'None'
extractBlock3Display.SelectNormalArray = 'None'
extractBlock3Display.SelectTangentArray = 'None'
extractBlock3Display.OSPRayScaleArray = 'Coordinates'
extractBlock3Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractBlock3Display.SelectOrientationVectors = 'Coordinates'
extractBlock3Display.ScaleFactor = 0.0999999953674319
extractBlock3Display.SelectScaleArray = 'Coordinates'
extractBlock3Display.GlyphType = 'Arrow'
extractBlock3Display.GlyphTableIndexArray = 'Coordinates'
extractBlock3Display.GaussianRadius = 0.004999999768371594
extractBlock3Display.SetScaleArray = ['POINTS', 'Coordinates']
extractBlock3Display.ScaleTransferFunction = 'PiecewiseFunction'
extractBlock3Display.OpacityArray = ['POINTS', 'Coordinates']
extractBlock3Display.OpacityTransferFunction = 'PiecewiseFunction'
extractBlock3Display.DataAxesGrid = 'GridAxesRepresentation'
extractBlock3Display.PolarAxes = 'PolarAxesRepresentation'
extractBlock3Display.ScalarOpacityUnitDistance = 0.15809134825114163
extractBlock3Display.OpacityArrayName = ['POINTS', 'Coordinates']
extractBlock3Display.SelectInputVectors = ['POINTS', 'Coordinates']
extractBlock3Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
extractBlock3Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
extractBlock3Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from extractSurface2
extractSurface2Display = Show(extractSurface2, pD_004_1, 'GeometryRepresentation')

# trace defaults for the display properties.
extractSurface2Display.Representation = 'Surface'
extractSurface2Display.ColorArrayName = [None, '']
extractSurface2Display.SelectTCoordArray = 'None'
extractSurface2Display.SelectNormalArray = 'None'
extractSurface2Display.SelectTangentArray = 'None'
extractSurface2Display.OSPRayScaleArray = 'Coordinates'
extractSurface2Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractSurface2Display.SelectOrientationVectors = 'Coordinates'
extractSurface2Display.ScaleFactor = 0.0999999953674319
extractSurface2Display.SelectScaleArray = 'Coordinates'
extractSurface2Display.GlyphType = 'Arrow'
extractSurface2Display.GlyphTableIndexArray = 'Coordinates'
extractSurface2Display.GaussianRadius = 0.004999999768371594
extractSurface2Display.SetScaleArray = ['POINTS', 'Coordinates']
extractSurface2Display.ScaleTransferFunction = 'PiecewiseFunction'
extractSurface2Display.OpacityArray = ['POINTS', 'Coordinates']
extractSurface2Display.OpacityTransferFunction = 'PiecewiseFunction'
extractSurface2Display.DataAxesGrid = 'GridAxesRepresentation'
extractSurface2Display.PolarAxes = 'PolarAxesRepresentation'
extractSurface2Display.SelectInputVectors = ['POINTS', 'Coordinates']
extractSurface2Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
extractSurface2Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
extractSurface2Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from tube2
tube2Display = Show(tube2, pD_004_1, 'GeometryRepresentation')

# trace defaults for the display properties.
tube2Display.Representation = 'Surface'
tube2Display.ColorArrayName = ['CELLS', 'PairType']
tube2Display.LookupTable = pairTypeLUT
tube2Display.SelectTCoordArray = 'None'
tube2Display.SelectNormalArray = 'TubeNormals'
tube2Display.SelectTangentArray = 'None'
tube2Display.OSPRayScaleArray = 'Coordinates'
tube2Display.OSPRayScaleFunction = 'PiecewiseFunction'
tube2Display.SelectOrientationVectors = 'Coordinates'
tube2Display.ScaleFactor = 0.1006123677827418
tube2Display.SelectScaleArray = 'Coordinates'
tube2Display.GlyphType = 'Arrow'
tube2Display.GlyphTableIndexArray = 'Coordinates'
tube2Display.GaussianRadius = 0.0050306183891370895
tube2Display.SetScaleArray = ['POINTS', 'Coordinates']
tube2Display.ScaleTransferFunction = 'PiecewiseFunction'
tube2Display.OpacityArray = ['POINTS', 'Coordinates']
tube2Display.OpacityTransferFunction = 'PiecewiseFunction'
tube2Display.DataAxesGrid = 'GridAxesRepresentation'
tube2Display.PolarAxes = 'PolarAxesRepresentation'
tube2Display.SelectInputVectors = ['POINTS', 'Coordinates']
tube2Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tube2Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tube2Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from cellDatatoPointData2
cellDatatoPointData2Display = Show(cellDatatoPointData2, pD_004_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
cellDatatoPointData2Display.Representation = 'Surface'
cellDatatoPointData2Display.ColorArrayName = [None, '']
cellDatatoPointData2Display.SelectTCoordArray = 'None'
cellDatatoPointData2Display.SelectNormalArray = 'None'
cellDatatoPointData2Display.SelectTangentArray = 'None'
cellDatatoPointData2Display.OSPRayScaleArray = 'Birth'
cellDatatoPointData2Display.OSPRayScaleFunction = 'PiecewiseFunction'
cellDatatoPointData2Display.SelectOrientationVectors = 'Coordinates'
cellDatatoPointData2Display.ScaleFactor = 0.0999999953674319
cellDatatoPointData2Display.SelectScaleArray = 'Birth'
cellDatatoPointData2Display.GlyphType = 'Arrow'
cellDatatoPointData2Display.GlyphTableIndexArray = 'Birth'
cellDatatoPointData2Display.GaussianRadius = 0.004999999768371594
cellDatatoPointData2Display.SetScaleArray = ['POINTS', 'Birth']
cellDatatoPointData2Display.ScaleTransferFunction = 'PiecewiseFunction'
cellDatatoPointData2Display.OpacityArray = ['POINTS', 'Birth']
cellDatatoPointData2Display.OpacityTransferFunction = 'PiecewiseFunction'
cellDatatoPointData2Display.DataAxesGrid = 'GridAxesRepresentation'
cellDatatoPointData2Display.PolarAxes = 'PolarAxesRepresentation'
cellDatatoPointData2Display.ScalarOpacityUnitDistance = 0.15125728948797518
cellDatatoPointData2Display.OpacityArrayName = ['POINTS', 'Birth']
cellDatatoPointData2Display.SelectInputVectors = ['POINTS', 'Coordinates']
cellDatatoPointData2Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
cellDatatoPointData2Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9756375067127729, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
cellDatatoPointData2Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9756375067127729, 1.0, 0.5, 0.0]

# show data from pythonCalculator2
pythonCalculator2Display = Show(pythonCalculator2, pD_004_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
pythonCalculator2Display.Representation = 'Surface'
pythonCalculator2Display.ColorArrayName = [None, '']
pythonCalculator2Display.SelectTCoordArray = 'None'
pythonCalculator2Display.SelectNormalArray = 'None'
pythonCalculator2Display.SelectTangentArray = 'None'
pythonCalculator2Display.OSPRayScaleArray = 'Birth'
pythonCalculator2Display.OSPRayScaleFunction = 'PiecewiseFunction'
pythonCalculator2Display.SelectOrientationVectors = 'Coordinates'
pythonCalculator2Display.ScaleFactor = 0.0999999953674319
pythonCalculator2Display.SelectScaleArray = 'Birth'
pythonCalculator2Display.GlyphType = 'Arrow'
pythonCalculator2Display.GlyphTableIndexArray = 'Birth'
pythonCalculator2Display.GaussianRadius = 0.004999999768371594
pythonCalculator2Display.SetScaleArray = ['POINTS', 'Birth']
pythonCalculator2Display.ScaleTransferFunction = 'PiecewiseFunction'
pythonCalculator2Display.OpacityArray = ['POINTS', 'Birth']
pythonCalculator2Display.OpacityTransferFunction = 'PiecewiseFunction'
pythonCalculator2Display.DataAxesGrid = 'GridAxesRepresentation'
pythonCalculator2Display.PolarAxes = 'PolarAxesRepresentation'
pythonCalculator2Display.ScalarOpacityUnitDistance = 0.15125728948797518
pythonCalculator2Display.OpacityArrayName = ['POINTS', 'Birth']
pythonCalculator2Display.SelectInputVectors = ['POINTS', 'Coordinates']
pythonCalculator2Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
pythonCalculator2Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9756375067127729, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
pythonCalculator2Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9756375067127729, 1.0, 0.5, 0.0]

# show data from tTKIcospheresFromPoints2
tTKIcospheresFromPoints2Display = Show(tTKIcospheresFromPoints2, pD_004_1, 'GeometryRepresentation')

# trace defaults for the display properties.
tTKIcospheresFromPoints2Display.Representation = 'Surface'
tTKIcospheresFromPoints2Display.ColorArrayName = ['POINTS', 'CriticalType']
tTKIcospheresFromPoints2Display.LookupTable = criticalTypeLUT
tTKIcospheresFromPoints2Display.SelectTCoordArray = 'None'
tTKIcospheresFromPoints2Display.SelectNormalArray = 'Normals'
tTKIcospheresFromPoints2Display.SelectTangentArray = 'None'
tTKIcospheresFromPoints2Display.OSPRayScaleArray = 'Birth'
tTKIcospheresFromPoints2Display.OSPRayScaleFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints2Display.SelectOrientationVectors = 'Coordinates'
tTKIcospheresFromPoints2Display.ScaleFactor = 0.10399999339133502
tTKIcospheresFromPoints2Display.SelectScaleArray = 'Birth'
tTKIcospheresFromPoints2Display.GlyphType = 'Arrow'
tTKIcospheresFromPoints2Display.GlyphTableIndexArray = 'Birth'
tTKIcospheresFromPoints2Display.GaussianRadius = 0.00519999966956675
tTKIcospheresFromPoints2Display.SetScaleArray = ['POINTS', 'Birth']
tTKIcospheresFromPoints2Display.ScaleTransferFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints2Display.OpacityArray = ['POINTS', 'Birth']
tTKIcospheresFromPoints2Display.OpacityTransferFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints2Display.DataAxesGrid = 'GridAxesRepresentation'
tTKIcospheresFromPoints2Display.PolarAxes = 'PolarAxesRepresentation'
tTKIcospheresFromPoints2Display.SelectInputVectors = ['POINTS', 'Coordinates']
tTKIcospheresFromPoints2Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tTKIcospheresFromPoints2Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9756375067127729, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tTKIcospheresFromPoints2Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9756375067127729, 1.0, 0.5, 0.0]

# show data from extractSurface3
extractSurface3Display = Show(extractSurface3, pD_004_1, 'GeometryRepresentation')

# trace defaults for the display properties.
extractSurface3Display.Representation = 'Surface'
extractSurface3Display.ColorArrayName = [None, '']
extractSurface3Display.SelectTCoordArray = 'None'
extractSurface3Display.SelectNormalArray = 'None'
extractSurface3Display.SelectTangentArray = 'None'
extractSurface3Display.OSPRayScaleArray = 'Coordinates'
extractSurface3Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractSurface3Display.SelectOrientationVectors = 'Coordinates'
extractSurface3Display.ScaleFactor = 0.0999999953674319
extractSurface3Display.SelectScaleArray = 'Coordinates'
extractSurface3Display.GlyphType = 'Arrow'
extractSurface3Display.GlyphTableIndexArray = 'Coordinates'
extractSurface3Display.GaussianRadius = 0.004999999768371594
extractSurface3Display.SetScaleArray = ['POINTS', 'Coordinates']
extractSurface3Display.ScaleTransferFunction = 'PiecewiseFunction'
extractSurface3Display.OpacityArray = ['POINTS', 'Coordinates']
extractSurface3Display.OpacityTransferFunction = 'PiecewiseFunction'
extractSurface3Display.DataAxesGrid = 'GridAxesRepresentation'
extractSurface3Display.PolarAxes = 'PolarAxesRepresentation'
extractSurface3Display.SelectInputVectors = ['POINTS', 'Coordinates']
extractSurface3Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
extractSurface3Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
extractSurface3Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from tube3
tube3Display = Show(tube3, pD_004_1, 'GeometryRepresentation')

# trace defaults for the display properties.
tube3Display.Representation = 'Surface'
tube3Display.ColorArrayName = ['CELLS', 'PairType']
tube3Display.LookupTable = pairTypeLUT
tube3Display.SelectTCoordArray = 'None'
tube3Display.SelectNormalArray = 'TubeNormals'
tube3Display.SelectTangentArray = 'None'
tube3Display.OSPRayScaleArray = 'Coordinates'
tube3Display.OSPRayScaleFunction = 'PiecewiseFunction'
tube3Display.SelectOrientationVectors = 'Coordinates'
tube3Display.ScaleFactor = 0.1006123677827418
tube3Display.SelectScaleArray = 'Coordinates'
tube3Display.GlyphType = 'Arrow'
tube3Display.GlyphTableIndexArray = 'Coordinates'
tube3Display.GaussianRadius = 0.0050306183891370895
tube3Display.SetScaleArray = ['POINTS', 'Coordinates']
tube3Display.ScaleTransferFunction = 'PiecewiseFunction'
tube3Display.OpacityArray = ['POINTS', 'Coordinates']
tube3Display.OpacityTransferFunction = 'PiecewiseFunction'
tube3Display.DataAxesGrid = 'GridAxesRepresentation'
tube3Display.PolarAxes = 'PolarAxesRepresentation'
tube3Display.SelectInputVectors = ['POINTS', 'Coordinates']
tube3Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tube3Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tube3Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from cellDatatoPointData3
cellDatatoPointData3Display = Show(cellDatatoPointData3, pD_004_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
cellDatatoPointData3Display.Representation = 'Surface'
cellDatatoPointData3Display.ColorArrayName = [None, '']
cellDatatoPointData3Display.SelectTCoordArray = 'None'
cellDatatoPointData3Display.SelectNormalArray = 'None'
cellDatatoPointData3Display.SelectTangentArray = 'None'
cellDatatoPointData3Display.OSPRayScaleArray = 'Birth'
cellDatatoPointData3Display.OSPRayScaleFunction = 'PiecewiseFunction'
cellDatatoPointData3Display.SelectOrientationVectors = 'Coordinates'
cellDatatoPointData3Display.ScaleFactor = 0.0999999953674319
cellDatatoPointData3Display.SelectScaleArray = 'Birth'
cellDatatoPointData3Display.GlyphType = 'Arrow'
cellDatatoPointData3Display.GlyphTableIndexArray = 'Birth'
cellDatatoPointData3Display.GaussianRadius = 0.004999999768371594
cellDatatoPointData3Display.SetScaleArray = ['POINTS', 'Birth']
cellDatatoPointData3Display.ScaleTransferFunction = 'PiecewiseFunction'
cellDatatoPointData3Display.OpacityArray = ['POINTS', 'Birth']
cellDatatoPointData3Display.OpacityTransferFunction = 'PiecewiseFunction'
cellDatatoPointData3Display.DataAxesGrid = 'GridAxesRepresentation'
cellDatatoPointData3Display.PolarAxes = 'PolarAxesRepresentation'
cellDatatoPointData3Display.ScalarOpacityUnitDistance = 0.15809134825114163
cellDatatoPointData3Display.OpacityArrayName = ['POINTS', 'Birth']
cellDatatoPointData3Display.SelectInputVectors = ['POINTS', 'Coordinates']
cellDatatoPointData3Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
cellDatatoPointData3Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9860274699327625, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
cellDatatoPointData3Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9860274699327625, 1.0, 0.5, 0.0]

# show data from pythonCalculator3
pythonCalculator3Display = Show(pythonCalculator3, pD_004_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
pythonCalculator3Display.Representation = 'Surface'
pythonCalculator3Display.ColorArrayName = [None, '']
pythonCalculator3Display.SelectTCoordArray = 'None'
pythonCalculator3Display.SelectNormalArray = 'None'
pythonCalculator3Display.SelectTangentArray = 'None'
pythonCalculator3Display.OSPRayScaleArray = 'Birth'
pythonCalculator3Display.OSPRayScaleFunction = 'PiecewiseFunction'
pythonCalculator3Display.SelectOrientationVectors = 'Coordinates'
pythonCalculator3Display.ScaleFactor = 0.0999999953674319
pythonCalculator3Display.SelectScaleArray = 'Birth'
pythonCalculator3Display.GlyphType = 'Arrow'
pythonCalculator3Display.GlyphTableIndexArray = 'Birth'
pythonCalculator3Display.GaussianRadius = 0.004999999768371594
pythonCalculator3Display.SetScaleArray = ['POINTS', 'Birth']
pythonCalculator3Display.ScaleTransferFunction = 'PiecewiseFunction'
pythonCalculator3Display.OpacityArray = ['POINTS', 'Birth']
pythonCalculator3Display.OpacityTransferFunction = 'PiecewiseFunction'
pythonCalculator3Display.DataAxesGrid = 'GridAxesRepresentation'
pythonCalculator3Display.PolarAxes = 'PolarAxesRepresentation'
pythonCalculator3Display.ScalarOpacityUnitDistance = 0.15809134825114163
pythonCalculator3Display.OpacityArrayName = ['POINTS', 'Birth']
pythonCalculator3Display.SelectInputVectors = ['POINTS', 'Coordinates']
pythonCalculator3Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
pythonCalculator3Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9860274699327625, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
pythonCalculator3Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9860274699327625, 1.0, 0.5, 0.0]

# setup the color legend parameters for each legend in this view

# get color legend/bar for pairTypeLUT in view pD_004_1
pairTypeLUTColorBar_1 = GetScalarBar(pairTypeLUT, pD_004_1)
pairTypeLUTColorBar_1.Title = 'PairType'
pairTypeLUTColorBar_1.ComponentTitle = ''

# set color bar visibility
pairTypeLUTColorBar_1.Visibility = 0

# get color legend/bar for criticalTypeLUT in view pD_004_1
criticalTypeLUTColorBar_1 = GetScalarBar(criticalTypeLUT, pD_004_1)
criticalTypeLUTColorBar_1.Title = 'CriticalType'
criticalTypeLUTColorBar_1.ComponentTitle = ''

# set color bar visibility
criticalTypeLUTColorBar_1.Visibility = 0

# hide data in view
Hide(extractBlock3, pD_004_1)

# hide data in view
Hide(extractSurface2, pD_004_1)

# hide data in view
Hide(cellDatatoPointData2, pD_004_1)

# hide data in view
Hide(extractSurface3, pD_004_1)

# hide data in view
Hide(cellDatatoPointData3, pD_004_1)

# ----------------------------------------------------------------
# setup the visualization in view 'pD_centroid1_1'
# ----------------------------------------------------------------

# show data from extractBlock2
extractBlock2Display = Show(extractBlock2, pD_centroid1_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
extractBlock2Display.Representation = 'Surface'
extractBlock2Display.ColorArrayName = [None, '']
extractBlock2Display.SelectTCoordArray = 'None'
extractBlock2Display.SelectNormalArray = 'None'
extractBlock2Display.SelectTangentArray = 'None'
extractBlock2Display.OSPRayScaleArray = 'Coordinates'
extractBlock2Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractBlock2Display.SelectOrientationVectors = 'Coordinates'
extractBlock2Display.ScaleFactor = 0.0999999953674319
extractBlock2Display.SelectScaleArray = 'Coordinates'
extractBlock2Display.GlyphType = 'Arrow'
extractBlock2Display.GlyphTableIndexArray = 'Coordinates'
extractBlock2Display.GaussianRadius = 0.004999999768371594
extractBlock2Display.SetScaleArray = ['POINTS', 'Coordinates']
extractBlock2Display.ScaleTransferFunction = 'PiecewiseFunction'
extractBlock2Display.OpacityArray = ['POINTS', 'Coordinates']
extractBlock2Display.OpacityTransferFunction = 'PiecewiseFunction'
extractBlock2Display.DataAxesGrid = 'GridAxesRepresentation'
extractBlock2Display.PolarAxes = 'PolarAxesRepresentation'
extractBlock2Display.ScalarOpacityUnitDistance = 0.15125728948797518
extractBlock2Display.OpacityArrayName = ['POINTS', 'Coordinates']
extractBlock2Display.SelectInputVectors = ['POINTS', 'Coordinates']
extractBlock2Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
extractBlock2Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
extractBlock2Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from tTKIcospheresFromPoints3
tTKIcospheresFromPoints3Display = Show(tTKIcospheresFromPoints3, pD_centroid1_1, 'GeometryRepresentation')

# trace defaults for the display properties.
tTKIcospheresFromPoints3Display.Representation = 'Surface'
tTKIcospheresFromPoints3Display.ColorArrayName = ['POINTS', 'CriticalType']
tTKIcospheresFromPoints3Display.LookupTable = criticalTypeLUT
tTKIcospheresFromPoints3Display.SelectTCoordArray = 'None'
tTKIcospheresFromPoints3Display.SelectNormalArray = 'Normals'
tTKIcospheresFromPoints3Display.SelectTangentArray = 'None'
tTKIcospheresFromPoints3Display.OSPRayScaleArray = 'Birth'
tTKIcospheresFromPoints3Display.OSPRayScaleFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints3Display.SelectOrientationVectors = 'Coordinates'
tTKIcospheresFromPoints3Display.ScaleFactor = 0.10399999339133502
tTKIcospheresFromPoints3Display.SelectScaleArray = 'Birth'
tTKIcospheresFromPoints3Display.GlyphType = 'Arrow'
tTKIcospheresFromPoints3Display.GlyphTableIndexArray = 'Birth'
tTKIcospheresFromPoints3Display.GaussianRadius = 0.00519999966956675
tTKIcospheresFromPoints3Display.SetScaleArray = ['POINTS', 'Birth']
tTKIcospheresFromPoints3Display.ScaleTransferFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints3Display.OpacityArray = ['POINTS', 'Birth']
tTKIcospheresFromPoints3Display.OpacityTransferFunction = 'PiecewiseFunction'
tTKIcospheresFromPoints3Display.DataAxesGrid = 'GridAxesRepresentation'
tTKIcospheresFromPoints3Display.PolarAxes = 'PolarAxesRepresentation'
tTKIcospheresFromPoints3Display.SelectInputVectors = ['POINTS', 'Coordinates']
tTKIcospheresFromPoints3Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tTKIcospheresFromPoints3Display.ScaleTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9860274699327625, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tTKIcospheresFromPoints3Display.OpacityTransferFunction.Points = [5e-07, 0.0, 0.5, 0.0, 0.9860274699327625, 1.0, 0.5, 0.0]

# show data from extractBlock3
extractBlock3Display_1 = Show(extractBlock3, pD_centroid1_1, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
extractBlock3Display_1.Representation = 'Surface'
extractBlock3Display_1.AmbientColor = [0.0, 0.0, 0.0]
extractBlock3Display_1.ColorArrayName = [None, '']
extractBlock3Display_1.DiffuseColor = [0.0, 0.0, 0.0]
extractBlock3Display_1.SelectTCoordArray = 'None'
extractBlock3Display_1.SelectNormalArray = 'None'
extractBlock3Display_1.SelectTangentArray = 'None'
extractBlock3Display_1.OSPRayScaleArray = 'Coordinates'
extractBlock3Display_1.OSPRayScaleFunction = 'PiecewiseFunction'
extractBlock3Display_1.SelectOrientationVectors = 'Coordinates'
extractBlock3Display_1.ScaleFactor = 0.0999999953674319
extractBlock3Display_1.SelectScaleArray = 'Coordinates'
extractBlock3Display_1.GlyphType = 'Arrow'
extractBlock3Display_1.GlyphTableIndexArray = 'Coordinates'
extractBlock3Display_1.GaussianRadius = 0.004999999768371594
extractBlock3Display_1.SetScaleArray = ['POINTS', 'Coordinates']
extractBlock3Display_1.ScaleTransferFunction = 'PiecewiseFunction'
extractBlock3Display_1.OpacityArray = ['POINTS', 'Coordinates']
extractBlock3Display_1.OpacityTransferFunction = 'PiecewiseFunction'
extractBlock3Display_1.DataAxesGrid = 'GridAxesRepresentation'
extractBlock3Display_1.PolarAxes = 'PolarAxesRepresentation'
extractBlock3Display_1.ScalarOpacityUnitDistance = 0.15809134825114163
extractBlock3Display_1.OpacityArrayName = ['POINTS', 'Coordinates']
extractBlock3Display_1.SelectInputVectors = ['POINTS', 'Coordinates']
extractBlock3Display_1.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
extractBlock3Display_1.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
extractBlock3Display_1.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from extractSurface3
extractSurface3Display_1 = Show(extractSurface3, pD_centroid1_1, 'GeometryRepresentation')

# trace defaults for the display properties.
extractSurface3Display_1.Representation = 'Surface'
extractSurface3Display_1.AmbientColor = [0.0, 0.0, 0.0]
extractSurface3Display_1.ColorArrayName = [None, '']
extractSurface3Display_1.DiffuseColor = [0.0, 0.0, 0.0]
extractSurface3Display_1.SelectTCoordArray = 'None'
extractSurface3Display_1.SelectNormalArray = 'None'
extractSurface3Display_1.SelectTangentArray = 'None'
extractSurface3Display_1.OSPRayScaleArray = 'Coordinates'
extractSurface3Display_1.OSPRayScaleFunction = 'PiecewiseFunction'
extractSurface3Display_1.SelectOrientationVectors = 'Coordinates'
extractSurface3Display_1.ScaleFactor = 0.0999999953674319
extractSurface3Display_1.SelectScaleArray = 'Coordinates'
extractSurface3Display_1.GlyphType = 'Arrow'
extractSurface3Display_1.GlyphTableIndexArray = 'Coordinates'
extractSurface3Display_1.GaussianRadius = 0.004999999768371594
extractSurface3Display_1.SetScaleArray = ['POINTS', 'Coordinates']
extractSurface3Display_1.ScaleTransferFunction = 'PiecewiseFunction'
extractSurface3Display_1.OpacityArray = ['POINTS', 'Coordinates']
extractSurface3Display_1.OpacityTransferFunction = 'PiecewiseFunction'
extractSurface3Display_1.DataAxesGrid = 'GridAxesRepresentation'
extractSurface3Display_1.PolarAxes = 'PolarAxesRepresentation'
extractSurface3Display_1.SelectInputVectors = ['POINTS', 'Coordinates']
extractSurface3Display_1.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
extractSurface3Display_1.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
extractSurface3Display_1.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# show data from tube3
tube3Display_1 = Show(tube3, pD_centroid1_1, 'GeometryRepresentation')

# trace defaults for the display properties.
tube3Display_1.Representation = 'Surface'
tube3Display_1.ColorArrayName = ['CELLS', 'PairType']
tube3Display_1.LookupTable = pairTypeLUT
tube3Display_1.SelectTCoordArray = 'None'
tube3Display_1.SelectNormalArray = 'TubeNormals'
tube3Display_1.SelectTangentArray = 'None'
tube3Display_1.OSPRayScaleArray = 'Coordinates'
tube3Display_1.OSPRayScaleFunction = 'PiecewiseFunction'
tube3Display_1.SelectOrientationVectors = 'Coordinates'
tube3Display_1.ScaleFactor = 0.1006123677827418
tube3Display_1.SelectScaleArray = 'Coordinates'
tube3Display_1.GlyphType = 'Arrow'
tube3Display_1.GlyphTableIndexArray = 'Coordinates'
tube3Display_1.GaussianRadius = 0.0050306183891370895
tube3Display_1.SetScaleArray = ['POINTS', 'Coordinates']
tube3Display_1.ScaleTransferFunction = 'PiecewiseFunction'
tube3Display_1.OpacityArray = ['POINTS', 'Coordinates']
tube3Display_1.OpacityTransferFunction = 'PiecewiseFunction'
tube3Display_1.DataAxesGrid = 'GridAxesRepresentation'
tube3Display_1.PolarAxes = 'PolarAxesRepresentation'
tube3Display_1.SelectInputVectors = ['POINTS', 'Coordinates']
tube3Display_1.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tube3Display_1.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tube3Display_1.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 20.3125, 1.0, 0.5, 0.0]

# setup the color legend parameters for each legend in this view

# get color legend/bar for criticalTypeLUT in view pD_centroid1_1
criticalTypeLUTColorBar_2 = GetScalarBar(criticalTypeLUT, pD_centroid1_1)
criticalTypeLUTColorBar_2.Title = 'CriticalType'
criticalTypeLUTColorBar_2.ComponentTitle = ''

# set color bar visibility
criticalTypeLUTColorBar_2.Visibility = 0

# get color legend/bar for pairTypeLUT in view pD_centroid1_1
pairTypeLUTColorBar_2 = GetScalarBar(pairTypeLUT, pD_centroid1_1)
pairTypeLUTColorBar_2.Title = 'PairType'
pairTypeLUTColorBar_2.ComponentTitle = ''

# set color bar visibility
pairTypeLUTColorBar_2.Visibility = 0

# hide data in view
Hide(extractBlock3, pD_centroid1_1)

# hide data in view
Hide(extractSurface3, pD_centroid1_1)

# ----------------------------------------------------------------
# setup the visualization in view 'tVPDs'
# ----------------------------------------------------------------

# show data from tTKTimeVaryingPersistenceDiagramClustering1_1
tTKTimeVaryingPersistenceDiagramClustering1_1Display = Show(OutputPort(tTKTimeVaryingPersistenceDiagramClustering1_1, 1), tVPDs, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
tTKTimeVaryingPersistenceDiagramClustering1_1Display.Representation = 'Surface'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.ColorArrayName = [None, '']
tTKTimeVaryingPersistenceDiagramClustering1_1Display.SelectTCoordArray = 'None'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.SelectNormalArray = 'None'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.SelectTangentArray = 'None'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.OSPRayScaleArray = 'Cluster'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.OSPRayScaleFunction = 'PiecewiseFunction'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.SelectOrientationVectors = 'None'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.ScaleFactor = 0.06376309096813203
tTKTimeVaryingPersistenceDiagramClustering1_1Display.SelectScaleArray = 'Cluster'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.GlyphType = 'Arrow'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.GlyphTableIndexArray = 'Cluster'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.GaussianRadius = 0.003188154548406601
tTKTimeVaryingPersistenceDiagramClustering1_1Display.SetScaleArray = ['POINTS', 'Cluster']
tTKTimeVaryingPersistenceDiagramClustering1_1Display.ScaleTransferFunction = 'PiecewiseFunction'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.OpacityArray = ['POINTS', 'Cluster']
tTKTimeVaryingPersistenceDiagramClustering1_1Display.OpacityTransferFunction = 'PiecewiseFunction'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.DataAxesGrid = 'GridAxesRepresentation'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.PolarAxes = 'PolarAxesRepresentation'
tTKTimeVaryingPersistenceDiagramClustering1_1Display.ScalarOpacityUnitDistance = 0.10453401213954372
tTKTimeVaryingPersistenceDiagramClustering1_1Display.OpacityArrayName = ['POINTS', 'Cluster']
tTKTimeVaryingPersistenceDiagramClustering1_1Display.SelectInputVectors = [None, '']
tTKTimeVaryingPersistenceDiagramClustering1_1Display.WriteLog = ''

# show data from tTKTimeVaryingPersistenceDiagramClustering1_2
tTKTimeVaryingPersistenceDiagramClustering1_2Display = Show(OutputPort(tTKTimeVaryingPersistenceDiagramClustering1_2, 2), tVPDs, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
tTKTimeVaryingPersistenceDiagramClustering1_2Display.Representation = 'Surface'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.ColorArrayName = [None, '']
tTKTimeVaryingPersistenceDiagramClustering1_2Display.SelectTCoordArray = 'None'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.SelectNormalArray = 'None'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.SelectTangentArray = 'None'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.OSPRayScaleArray = 'Is Real Vertice'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.OSPRayScaleFunction = 'PiecewiseFunction'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.SelectOrientationVectors = 'None'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.ScaleFactor = 0.06376309096813203
tTKTimeVaryingPersistenceDiagramClustering1_2Display.SelectScaleArray = 'Is Real Vertice'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.GlyphType = 'Arrow'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.GlyphTableIndexArray = 'Is Real Vertice'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.GaussianRadius = 0.003188154548406601
tTKTimeVaryingPersistenceDiagramClustering1_2Display.SetScaleArray = ['POINTS', 'Is Real Vertice']
tTKTimeVaryingPersistenceDiagramClustering1_2Display.ScaleTransferFunction = 'PiecewiseFunction'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.OpacityArray = ['POINTS', 'Is Real Vertice']
tTKTimeVaryingPersistenceDiagramClustering1_2Display.OpacityTransferFunction = 'PiecewiseFunction'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.DataAxesGrid = 'GridAxesRepresentation'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.PolarAxes = 'PolarAxesRepresentation'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.ScalarOpacityUnitDistance = 0.11939506480204391
tTKTimeVaryingPersistenceDiagramClustering1_2Display.OpacityArrayName = ['POINTS', 'Is Real Vertice']
tTKTimeVaryingPersistenceDiagramClustering1_2Display.SelectInputVectors = [None, '']
tTKTimeVaryingPersistenceDiagramClustering1_2Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
tTKTimeVaryingPersistenceDiagramClustering1_2Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]

# show data from threshold1
threshold1Display = Show(threshold1, tVPDs, 'UnstructuredGridRepresentation')

# get separate 2D transfer function for 'Assignmentcost'
separate_threshold1Display_AssignmentcostTF2D = GetTransferFunction2D('Assignmentcost', threshold1Display, separate=True)

# get separate color transfer function/color map for 'Assignmentcost'
separate_threshold1Display_AssignmentcostLUT = GetColorTransferFunction('Assignmentcost', threshold1Display, separate=True)
separate_threshold1Display_AssignmentcostLUT.TransferFunction2D = separate_threshold1Display_AssignmentcostTF2D
separate_threshold1Display_AssignmentcostLUT.RGBPoints = [0.04793558269739151, 0.196078, 0.368627, 0.6, 0.054879922866821286, 1.0, 0.964706, 0.964706, 0.06162001773715019, 0.572549, 0.431373, 0.772549, 0.0683601126074791, 0.247059, 0.0, 0.490196]
separate_threshold1Display_AssignmentcostLUT.ScalarRangeInitialized = 1.0

# get separate opacity transfer function/opacity map for 'Assignmentcost'
separate_threshold1Display_AssignmentcostPWF = GetOpacityTransferFunction('Assignmentcost', threshold1Display, separate=True)
separate_threshold1Display_AssignmentcostPWF.Points = [0.04793558269739151, 0.0, 0.15, 0.2, 0.04793558269739151, 0.0, 0.5, 0.0, 0.0683601126074791, 1.0, 0.8, 1.0]
separate_threshold1Display_AssignmentcostPWF.ScalarRangeInitialized = 1

# trace defaults for the display properties.
threshold1Display.Representation = 'Surface'
threshold1Display.ColorArrayName = ['CELLS', 'Assignment cost']
threshold1Display.LookupTable = separate_threshold1Display_AssignmentcostLUT
threshold1Display.LineWidth = 2.0
threshold1Display.SelectTCoordArray = 'None'
threshold1Display.SelectNormalArray = 'None'
threshold1Display.SelectTangentArray = 'None'
threshold1Display.OSPRayScaleArray = 'Is Real Vertice'
threshold1Display.OSPRayScaleFunction = 'PiecewiseFunction'
threshold1Display.SelectOrientationVectors = 'None'
threshold1Display.ScaleFactor = 0.059129524230957034
threshold1Display.SelectScaleArray = 'Is Real Vertice'
threshold1Display.GlyphType = 'Arrow'
threshold1Display.GlyphTableIndexArray = 'Is Real Vertice'
threshold1Display.GaussianRadius = 0.0029564762115478517
threshold1Display.SetScaleArray = ['POINTS', 'Is Real Vertice']
threshold1Display.ScaleTransferFunction = 'PiecewiseFunction'
threshold1Display.OpacityArray = ['POINTS', 'Is Real Vertice']
threshold1Display.OpacityTransferFunction = 'PiecewiseFunction'
threshold1Display.DataAxesGrid = 'GridAxesRepresentation'
threshold1Display.PolarAxes = 'PolarAxesRepresentation'
threshold1Display.ScalarOpacityFunction = separate_threshold1Display_AssignmentcostPWF
threshold1Display.ScalarOpacityUnitDistance = 0.10416631152222773
threshold1Display.OpacityArrayName = ['POINTS', 'Is Real Vertice']
threshold1Display.SelectInputVectors = [None, '']
threshold1Display.WriteLog = ''

# init the 'PiecewiseFunction' selected for 'ScaleTransferFunction'
threshold1Display.ScaleTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]

# init the 'PiecewiseFunction' selected for 'OpacityTransferFunction'
threshold1Display.OpacityTransferFunction.Points = [0.0, 0.0, 0.5, 0.0, 1.1757813367477812e-38, 1.0, 0.5, 0.0]

# set separate color map
threshold1Display.UseSeparateColorMap = True

# show data from threshold2
threshold2Display = Show(threshold2, tVPDs, 'UnstructuredGridRepresentation')

# get separate 2D transfer function for 'Assignmentcost'
separate_threshold2Display_AssignmentcostTF2D = GetTransferFunction2D('Assignmentcost', threshold2Display, separate=True)

# get separate color transfer function/color map for 'Assignmentcost'
separate_threshold2Display_AssignmentcostLUT = GetColorTransferFunction('Assignmentcost', threshold2Display, separate=True)
separate_threshold2Display_AssignmentcostLUT.TransferFunction2D = separate_threshold2Display_AssignmentcostTF2D
separate_threshold2Display_AssignmentcostLUT.RGBPoints = [0.038063131272792816, 0.196078, 0.368627, 0.6, 0.044413716569542884, 1.0, 0.964706, 0.964706, 0.0505775199458003, 0.572549, 0.431373, 0.772549, 0.056741323322057724, 0.247059, 0.0, 0.490196]
separate_threshold2Display_AssignmentcostLUT.ScalarRangeInitialized = 1.0

# get separate opacity transfer function/opacity map for 'Assignmentcost'
separate_threshold2Display_AssignmentcostPWF = GetOpacityTransferFunction('Assignmentcost', threshold2Display, separate=True)
separate_threshold2Display_AssignmentcostPWF.Points = [0.038063131272792816, 0.0, 0.15, 0.2, 0.038063131272792816, 0.0, 0.5, 0.0, 0.056741323322057724, 1.0, 0.8, 1.0]
separate_threshold2Display_AssignmentcostPWF.ScalarRangeInitialized = 1

# trace defaults for the display properties.
threshold2Display.Representation = 'Surface'
threshold2Display.ColorArrayName = ['CELLS', 'Assignment cost']
threshold2Display.LookupTable = separate_threshold2Display_AssignmentcostLUT
threshold2Display.LineWidth = 2.0
threshold2Display.SelectTCoordArray = 'None'
threshold2Display.SelectNormalArray = 'None'
threshold2Display.SelectTangentArray = 'None'
threshold2Display.OSPRayScaleFunction = 'PiecewiseFunction'
threshold2Display.SelectOrientationVectors = 'None'
threshold2Display.ScaleFactor = -2.0000000000000002e+298
threshold2Display.SelectScaleArray = 'None'
threshold2Display.GlyphType = 'Arrow'
threshold2Display.GlyphTableIndexArray = 'None'
threshold2Display.GaussianRadius = -1e+297
threshold2Display.SetScaleArray = [None, '']
threshold2Display.ScaleTransferFunction = 'PiecewiseFunction'
threshold2Display.OpacityArray = [None, '']
threshold2Display.OpacityTransferFunction = 'PiecewiseFunction'
threshold2Display.DataAxesGrid = 'GridAxesRepresentation'
threshold2Display.PolarAxes = 'PolarAxesRepresentation'
threshold2Display.ScalarOpacityFunction = separate_threshold2Display_AssignmentcostPWF
threshold2Display.OpacityArrayName = [None, '']
threshold2Display.SelectInputVectors = [None, '']
threshold2Display.WriteLog = ''

# set separate color map
threshold2Display.UseSeparateColorMap = True

# show data from extractBlock4
extractBlock4Display = Show(extractBlock4, tVPDs, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
extractBlock4Display.Representation = 'Surface'
extractBlock4Display.ColorArrayName = [None, '']
extractBlock4Display.SelectTCoordArray = 'None'
extractBlock4Display.SelectNormalArray = 'None'
extractBlock4Display.SelectTangentArray = 'None'
extractBlock4Display.OSPRayScaleArray = 'Cluster'
extractBlock4Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractBlock4Display.SelectOrientationVectors = 'None'
extractBlock4Display.ScaleFactor = 0.06376309096813203
extractBlock4Display.SelectScaleArray = 'Cluster'
extractBlock4Display.GlyphType = 'Arrow'
extractBlock4Display.GlyphTableIndexArray = 'Cluster'
extractBlock4Display.GaussianRadius = 0.003188154548406601
extractBlock4Display.SetScaleArray = ['POINTS', 'Cluster']
extractBlock4Display.ScaleTransferFunction = 'PiecewiseFunction'
extractBlock4Display.OpacityArray = ['POINTS', 'Cluster']
extractBlock4Display.OpacityTransferFunction = 'PiecewiseFunction'
extractBlock4Display.DataAxesGrid = 'GridAxesRepresentation'
extractBlock4Display.PolarAxes = 'PolarAxesRepresentation'
extractBlock4Display.ScalarOpacityUnitDistance = 0.10453401213954372
extractBlock4Display.OpacityArrayName = ['POINTS', 'Cluster']
extractBlock4Display.SelectInputVectors = [None, '']
extractBlock4Display.WriteLog = ''

# show data from extractSurface4
extractSurface4Display = Show(extractSurface4, tVPDs, 'GeometryRepresentation')

# trace defaults for the display properties.
extractSurface4Display.Representation = 'Surface'
extractSurface4Display.ColorArrayName = [None, '']
extractSurface4Display.SelectTCoordArray = 'None'
extractSurface4Display.SelectNormalArray = 'None'
extractSurface4Display.SelectTangentArray = 'None'
extractSurface4Display.OSPRayScaleArray = 'Cluster'
extractSurface4Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractSurface4Display.SelectOrientationVectors = 'None'
extractSurface4Display.ScaleFactor = 0.06376309096813203
extractSurface4Display.SelectScaleArray = 'Cluster'
extractSurface4Display.GlyphType = 'Arrow'
extractSurface4Display.GlyphTableIndexArray = 'Cluster'
extractSurface4Display.GaussianRadius = 0.003188154548406601
extractSurface4Display.SetScaleArray = ['POINTS', 'Cluster']
extractSurface4Display.ScaleTransferFunction = 'PiecewiseFunction'
extractSurface4Display.OpacityArray = ['POINTS', 'Cluster']
extractSurface4Display.OpacityTransferFunction = 'PiecewiseFunction'
extractSurface4Display.DataAxesGrid = 'GridAxesRepresentation'
extractSurface4Display.PolarAxes = 'PolarAxesRepresentation'
extractSurface4Display.SelectInputVectors = [None, '']
extractSurface4Display.WriteLog = ''

# show data from threshold3
threshold3Display = Show(threshold3, tVPDs, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
threshold3Display.Representation = 'Surface'
threshold3Display.ColorArrayName = ['POINTS', '']
threshold3Display.SelectTCoordArray = 'None'
threshold3Display.SelectNormalArray = 'None'
threshold3Display.SelectTangentArray = 'None'
threshold3Display.OSPRayScaleArray = 'Cluster'
threshold3Display.OSPRayScaleFunction = 'PiecewiseFunction'
threshold3Display.SelectOrientationVectors = 'None'
threshold3Display.ScaleFactor = 0.053903725743293766
threshold3Display.SelectScaleArray = 'Cluster'
threshold3Display.GlyphType = 'Arrow'
threshold3Display.GlyphTableIndexArray = 'Cluster'
threshold3Display.GaussianRadius = 0.0026951862871646883
threshold3Display.SetScaleArray = ['POINTS', 'Cluster']
threshold3Display.ScaleTransferFunction = 'PiecewiseFunction'
threshold3Display.OpacityArray = ['POINTS', 'Cluster']
threshold3Display.OpacityTransferFunction = 'PiecewiseFunction'
threshold3Display.DataAxesGrid = 'GridAxesRepresentation'
threshold3Display.PolarAxes = 'PolarAxesRepresentation'
threshold3Display.ScalarOpacityUnitDistance = 0.10130951198355219
threshold3Display.OpacityArrayName = ['POINTS', 'Cluster']
threshold3Display.SelectInputVectors = [None, '']
threshold3Display.WriteLog = ''

# show data from extractSurface5
extractSurface5Display = Show(extractSurface5, tVPDs, 'GeometryRepresentation')

# trace defaults for the display properties.
extractSurface5Display.Representation = 'Surface'
extractSurface5Display.ColorArrayName = [None, '']
extractSurface5Display.SelectTCoordArray = 'None'
extractSurface5Display.SelectNormalArray = 'None'
extractSurface5Display.SelectTangentArray = 'None'
extractSurface5Display.OSPRayScaleArray = 'Cluster'
extractSurface5Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractSurface5Display.SelectOrientationVectors = 'None'
extractSurface5Display.ScaleFactor = 0.053903725743293766
extractSurface5Display.SelectScaleArray = 'Cluster'
extractSurface5Display.GlyphType = 'Arrow'
extractSurface5Display.GlyphTableIndexArray = 'Cluster'
extractSurface5Display.GaussianRadius = 0.0026951862871646883
extractSurface5Display.SetScaleArray = ['POINTS', 'Cluster']
extractSurface5Display.ScaleTransferFunction = 'PiecewiseFunction'
extractSurface5Display.OpacityArray = ['POINTS', 'Cluster']
extractSurface5Display.OpacityTransferFunction = 'PiecewiseFunction'
extractSurface5Display.DataAxesGrid = 'GridAxesRepresentation'
extractSurface5Display.PolarAxes = 'PolarAxesRepresentation'
extractSurface5Display.SelectInputVectors = [None, '']
extractSurface5Display.WriteLog = ''

# show data from tube4
tube4Display = Show(tube4, tVPDs, 'GeometryRepresentation')

# get 2D transfer function for 'TimeStamp'
timeStampTF2D = GetTransferFunction2D('TimeStamp')

# get color transfer function/color map for 'TimeStamp'
timeStampLUT = GetColorTransferFunction('TimeStamp')
timeStampLUT.TransferFunction2D = timeStampTF2D
timeStampLUT.RGBPoints = [425.0, 1.0, 1.0, 1.0, 567.3, 0.0, 0.0, 0.0]
timeStampLUT.ColorSpace = 'RGB'
timeStampLUT.NanColor = [1.0, 0.0, 0.0]
timeStampLUT.ScalarRangeInitialized = 1.0

# trace defaults for the display properties.
tube4Display.Representation = 'Surface'
tube4Display.ColorArrayName = ['POINTS', 'Time Stamp']
tube4Display.LookupTable = timeStampLUT
tube4Display.SelectTCoordArray = 'None'
tube4Display.SelectNormalArray = 'TubeNormals'
tube4Display.SelectTangentArray = 'None'
tube4Display.OSPRayScaleArray = 'Cluster'
tube4Display.OSPRayScaleFunction = 'PiecewiseFunction'
tube4Display.SelectOrientationVectors = 'None'
tube4Display.ScaleFactor = 0.0552222803235054
tube4Display.SelectScaleArray = 'Cluster'
tube4Display.GlyphType = 'Arrow'
tube4Display.GlyphTableIndexArray = 'Cluster'
tube4Display.GaussianRadius = 0.00276111401617527
tube4Display.SetScaleArray = ['POINTS', 'Cluster']
tube4Display.ScaleTransferFunction = 'PiecewiseFunction'
tube4Display.OpacityArray = ['POINTS', 'Cluster']
tube4Display.OpacityTransferFunction = 'PiecewiseFunction'
tube4Display.DataAxesGrid = 'GridAxesRepresentation'
tube4Display.PolarAxes = 'PolarAxesRepresentation'
tube4Display.SelectInputVectors = ['POINTS', 'TubeNormals']
tube4Display.WriteLog = ''

# show data from threshold4
threshold4Display = Show(threshold4, tVPDs, 'UnstructuredGridRepresentation')

# trace defaults for the display properties.
threshold4Display.Representation = 'Surface'
threshold4Display.ColorArrayName = [None, '']
threshold4Display.SelectTCoordArray = 'None'
threshold4Display.SelectNormalArray = 'None'
threshold4Display.SelectTangentArray = 'None'
threshold4Display.OSPRayScaleArray = 'Cluster'
threshold4Display.OSPRayScaleFunction = 'PiecewiseFunction'
threshold4Display.SelectOrientationVectors = 'None'
threshold4Display.ScaleFactor = 0.06374821066856384
threshold4Display.SelectScaleArray = 'Cluster'
threshold4Display.GlyphType = 'Arrow'
threshold4Display.GlyphTableIndexArray = 'Cluster'
threshold4Display.GaussianRadius = 0.003187410533428192
threshold4Display.SetScaleArray = ['POINTS', 'Cluster']
threshold4Display.ScaleTransferFunction = 'PiecewiseFunction'
threshold4Display.OpacityArray = ['POINTS', 'Cluster']
threshold4Display.OpacityTransferFunction = 'PiecewiseFunction'
threshold4Display.DataAxesGrid = 'GridAxesRepresentation'
threshold4Display.PolarAxes = 'PolarAxesRepresentation'
threshold4Display.ScalarOpacityUnitDistance = 0.1196503773289648
threshold4Display.OpacityArrayName = ['POINTS', 'Cluster']
threshold4Display.SelectInputVectors = [None, '']
threshold4Display.WriteLog = ''

# show data from extractSurface6
extractSurface6Display = Show(extractSurface6, tVPDs, 'GeometryRepresentation')

# trace defaults for the display properties.
extractSurface6Display.Representation = 'Surface'
extractSurface6Display.ColorArrayName = [None, '']
extractSurface6Display.SelectTCoordArray = 'None'
extractSurface6Display.SelectNormalArray = 'None'
extractSurface6Display.SelectTangentArray = 'None'
extractSurface6Display.OSPRayScaleArray = 'Cluster'
extractSurface6Display.OSPRayScaleFunction = 'PiecewiseFunction'
extractSurface6Display.SelectOrientationVectors = 'None'
extractSurface6Display.ScaleFactor = 0.06374821066856384
extractSurface6Display.SelectScaleArray = 'Cluster'
extractSurface6Display.GlyphType = 'Arrow'
extractSurface6Display.GlyphTableIndexArray = 'Cluster'
extractSurface6Display.GaussianRadius = 0.003187410533428192
extractSurface6Display.SetScaleArray = ['POINTS', 'Cluster']
extractSurface6Display.ScaleTransferFunction = 'PiecewiseFunction'
extractSurface6Display.OpacityArray = ['POINTS', 'Cluster']
extractSurface6Display.OpacityTransferFunction = 'PiecewiseFunction'
extractSurface6Display.DataAxesGrid = 'GridAxesRepresentation'
extractSurface6Display.PolarAxes = 'PolarAxesRepresentation'
extractSurface6Display.SelectInputVectors = [None, '']
extractSurface6Display.WriteLog = ''

# show data from tube5
tube5Display = Show(tube5, tVPDs, 'GeometryRepresentation')

# trace defaults for the display properties.
tube5Display.Representation = 'Surface'
tube5Display.ColorArrayName = ['POINTS', 'Time Stamp']
tube5Display.LookupTable = timeStampLUT
tube5Display.SelectTCoordArray = 'None'
tube5Display.SelectNormalArray = 'TubeNormals'
tube5Display.SelectTangentArray = 'None'
tube5Display.OSPRayScaleArray = 'Cluster'
tube5Display.OSPRayScaleFunction = 'PiecewiseFunction'
tube5Display.SelectOrientationVectors = 'None'
tube5Display.ScaleFactor = 0.06477347016334534
tube5Display.SelectScaleArray = 'Cluster'
tube5Display.GlyphType = 'Arrow'
tube5Display.GlyphTableIndexArray = 'Cluster'
tube5Display.GaussianRadius = 0.003238673508167267
tube5Display.SetScaleArray = ['POINTS', 'Cluster']
tube5Display.ScaleTransferFunction = 'PiecewiseFunction'
tube5Display.OpacityArray = ['POINTS', 'Cluster']
tube5Display.OpacityTransferFunction = 'PiecewiseFunction'
tube5Display.DataAxesGrid = 'GridAxesRepresentation'
tube5Display.PolarAxes = 'PolarAxesRepresentation'
tube5Display.SelectInputVectors = ['POINTS', 'TubeNormals']
tube5Display.WriteLog = ''

# setup the color legend parameters for each legend in this view

# get 2D transfer function for 'Assignmentcost'
assignmentcostTF2D = GetTransferFunction2D('Assignmentcost')

# get color transfer function/color map for 'Assignmentcost'
assignmentcostLUT = GetColorTransferFunction('Assignmentcost')
assignmentcostLUT.TransferFunction2D = assignmentcostTF2D
assignmentcostLUT.RGBPoints = [0.038063131272792816, 0.196078, 0.368627, 0.6, 0.048364104926586146, 1.0, 0.964706, 0.964706, 0.058362108767032614, 0.572549, 0.431373, 0.772549, 0.0683601126074791, 0.247059, 0.0, 0.490196]
assignmentcostLUT.ScalarRangeInitialized = 1.0

# get color legend/bar for assignmentcostLUT in view tVPDs
assignmentcostLUTColorBar = GetScalarBar(assignmentcostLUT, tVPDs)
assignmentcostLUTColorBar.Title = 'Assignment cost'
assignmentcostLUTColorBar.ComponentTitle = ''

# set color bar visibility
assignmentcostLUTColorBar.Visibility = 0

# get 2D transfer function for 'Cluster'
clusterTF2D = GetTransferFunction2D('Cluster')

# get color transfer function/color map for 'Cluster'
clusterLUT = GetColorTransferFunction('Cluster')
clusterLUT.TransferFunction2D = clusterTF2D
clusterLUT.ScalarRangeInitialized = 1.0

# get color legend/bar for clusterLUT in view tVPDs
clusterLUTColorBar = GetScalarBar(clusterLUT, tVPDs)
clusterLUTColorBar.Title = 'Cluster'
clusterLUTColorBar.ComponentTitle = ''

# set color bar visibility
clusterLUTColorBar.Visibility = 0

# get color legend/bar for timeStampLUT in view tVPDs
timeStampLUTColorBar = GetScalarBar(timeStampLUT, tVPDs)
timeStampLUTColorBar.Title = 'Time Stamp'
timeStampLUTColorBar.ComponentTitle = ''

# set color bar visibility
timeStampLUTColorBar.Visibility = 0

# get color legend/bar for separate_threshold1Display_AssignmentcostLUT in view tVPDs
separate_threshold1Display_AssignmentcostLUTColorBar = GetScalarBar(separate_threshold1Display_AssignmentcostLUT, tVPDs)
separate_threshold1Display_AssignmentcostLUTColorBar.Title = 'Assignment cost'
separate_threshold1Display_AssignmentcostLUTColorBar.ComponentTitle = ''

# set color bar visibility
separate_threshold1Display_AssignmentcostLUTColorBar.Visibility = 0

# get color legend/bar for separate_threshold2Display_AssignmentcostLUT in view tVPDs
separate_threshold2Display_AssignmentcostLUTColorBar = GetScalarBar(separate_threshold2Display_AssignmentcostLUT, tVPDs)
separate_threshold2Display_AssignmentcostLUTColorBar.Title = 'Assignment cost'
separate_threshold2Display_AssignmentcostLUTColorBar.ComponentTitle = ''

# set color bar visibility
separate_threshold2Display_AssignmentcostLUTColorBar.Visibility = 0

# hide data in view
Hide(OutputPort(tTKTimeVaryingPersistenceDiagramClustering1_1, 1), tVPDs)

# hide data in view
Hide(OutputPort(tTKTimeVaryingPersistenceDiagramClustering1_2, 2), tVPDs)

# hide data in view
Hide(extractBlock4, tVPDs)

# hide data in view
Hide(extractSurface4, tVPDs)

# hide data in view
Hide(threshold3, tVPDs)

# hide data in view
Hide(extractSurface5, tVPDs)

# hide data in view
Hide(threshold4, tVPDs)

# hide data in view
Hide(extractSurface6, tVPDs)

# ----------------------------------------------------------------
# setup color maps and opacity mapes used in the visualization
# note: the Get..() functions create a new object, if needed
# ----------------------------------------------------------------

# get opacity transfer function/opacity map for 'Cluster'
clusterPWF = GetOpacityTransferFunction('Cluster')
clusterPWF.ScalarRangeInitialized = 1

# get opacity transfer function/opacity map for 'CriticalType'
criticalTypePWF = GetOpacityTransferFunction('CriticalType')
criticalTypePWF.Points = [0.0, 0.0, 0.5, 0.0, 3.0, 1.0, 0.5, 0.0]
criticalTypePWF.ScalarRangeInitialized = 1

# get opacity transfer function/opacity map for 'TimeStamp'
timeStampPWF = GetOpacityTransferFunction('TimeStamp')
timeStampPWF.Points = [425.0, 0.0, 0.5, 0.0, 567.3, 1.0, 0.5, 0.0]
timeStampPWF.ScalarRangeInitialized = 1

# get opacity transfer function/opacity map for 'PairType'
pairTypePWF = GetOpacityTransferFunction('PairType')
pairTypePWF.Points = [-1.0, 0.0, 0.15, 0.2, -1.0, 0.0, 0.5, 0.0, 2.0, 1.0, 0.8, 1.0]
pairTypePWF.ScalarRangeInitialized = 1

# get opacity transfer function/opacity map for 'Assignmentcost'
assignmentcostPWF = GetOpacityTransferFunction('Assignmentcost')
assignmentcostPWF.Points = [0.038063131272792816, 0.0, 0.15, 0.2, 0.038063131272792816, 0.0, 0.5, 0.0, 0.0683601126074791, 1.0, 0.8, 1.0]
assignmentcostPWF.ScalarRangeInitialized = 1

# ----------------------------------------------------------------
# restore active source
SetActiveSource(threshold2)
# ----------------------------------------------------------------


if __name__ == '__main__':
    # generate extracts
    SaveExtracts(ExtractsOutputDirectory='extracts')
