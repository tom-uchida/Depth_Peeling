# Depth_Peeling

## Original Repository
- https://github.com/naohisas/DepthPeeling

## Usage
```
Usage: ./DepthPeeling [options]

Options:
-h          : Output help message.
-width      : Screen width. (defulat: 512)
-height     : Screen height. (defulat: 512)
-nrepeats   : Number of repetitions for IPBR. (defulat: 1)
-npeels     : Number of peels for DP. (defulat: 1)
-npolygons  : Number of polygons. (defulat: 10)
-opacity    : Opacity value for polygon object. (default: 0.5)
-color      : Color value for polygon object. (default: 0, 0, 0)
-background : Background color. (default: 255, 255, 255)
-offscreen  : Offscreen rendering [0:disable, 1:enable]. (default: 0)
```

### Example
```
$ make rebuild
$ ./DepthPeeling -npeels 1
```

## Visualization Results

### Point Cloud Data
#### Bunny
|Layer1|Layer2|
|:-:|:-:|
|![](figures/Bunny/PointCloud/Layer1.png)|![](figures/Bunny/PointCloud/Layer2.png)|

|Layer3|Layer4|
|:-:|:-:|
|![](figures/Bunny/PointCloud/Layer3.png)|![](figures/Bunny/PointCloud/Layer4.png)|

|Layer5|Layer6|
|:-:|:-:|
|![](figures/Bunny/PointCloud/Layer5.png)|![](figures/Bunny/PointCloud/Layer6.png)|

|Layer7|Layer8|
|:-:|:-:|
|![](figures/Bunny/PointCloud/Layer7.png)|![](figures/Bunny/PointCloud/Layer8.png)|

|Layer9|Layer10|
|:-:|:-:|
|![](figures/Bunny/PointCloud/Layer9.png)|![](figures/Bunny/PointCloud/Layer10.png)|

#### Dragon
|Layer1|Layer2|
|:-:|:-:|
|![](figures/Dragon/PointCloud/Layer1.png)|![](figures/Dragon/PointCloud/Layer2.png)|

|Layer3|Layer4|
|:-:|:-:|
|![](figures/Dragon/PointCloud/Layer3.png)|![](figures/Dragon/PointCloud/Layer4.png)|

|Layer5|Layer6|
|:-:|:-:|
|![](figures/Dragon/PointCloud/Layer5.png)|![](figures/Dragon/PointCloud/Layer6.png)|

|Layer7|Layer8|
|:-:|:-:|
|![](figures/Dragon/PointCloud/Layer7.png)|![](figures/Dragon/PointCloud/Layer8.png)|

|Layer9|Layer10|
|:-:|:-:|
|![](figures/Dragon/PointCloud/Layer9.png)|![](figures/Dragon/PointCloud/Layer10.png)|



### Polygon Data
#### Bunny
|Layer1|Layer2|Layer3|
|:-:|:-:|:-:|
|![](figures/Bunny/Polygon/Layer1.png)|![](figures/Bunny/Polygon/Layer2.png)|![](figures/Bunny/Polygon/Layer3.png)|

|Layer4|Layer5|Layer6|
|:-:|:-:|:-:|
|![](figures/Bunny/Polygon/Layer4.png)|![](figures/Bunny/Polygon/Layer5.png)|![](figures/Bunny/Polygon/Layer6.png)|

#### Dragon
|Layer1|Layer2|Layer3|
|:-:|:-:|:-:|
|![](figures/Dragon/Polygon/Layer1.png)|![](figures/Dragon/Polygon/Layer2.png)|![](figures/Dragon/Polygon/Layer3.png)|

|Layer4|Layer5|Layer6|
|:-:|:-:|:-:|
|![](figures/Dragon/Polygon/Layer4.png)|![](figures/Dragon/Polygon/Layer5.png)|![](figures/Dragon/Polygon/Layer6.png)|

#### Polygon
|Layer1|Layer2|Layer3|
|:-:|:-:|:-:|
|![](figures/Polygon/Layer1.png)|![](figures/Polygon/Layer2.png)|![](figures/Polygon/Layer3.png)|

|Layer4|Layer5|Layer6|
|:-:|:-:|:-:|
|![](figures/Polygon/Layer4.png)|![](figures/Polygon/Layer5.png)|![](figures/Polygon/Layer6.png)|