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

### Polygon
|Layer1|Layer2|Layer3|
|:-:|:-:|:-:|
|![](figures/Polygon/Layer1.png)|![](figures/Polygon/Layer2.png)|![](figures/Polygon/Layer3.png)|

|Layer4|Layer5|Layer6|
|:-:|:-:|:-:|
|![](figures/Polygon/Layer4.png)|![](figures/Polygon/Layer5.png)|![](figures/Polygon/Layer6.png)|

### Bunny
|Layer1|Layer2|Layer3|
|:-:|:-:|:-:|
|![](figures/Bunny/Layer1.png)|![](figures/Bunny/Layer2.png)|![](figures/Bunny/Layer3.png)|

|Layer4|Layer5|Layer6|
|:-:|:-:|:-:|
|![](figures/Bunny/Layer4.png)|![](figures/Bunny/Layer5.png)|![](figures/Bunny/Layer6.png)|

### Dragon
|Layer1|Layer2|Layer3|
|:-:|:-:|:-:|
|![](figures/Dragon/Layer1.png)|![](figures/Dragon/Layer2.png)|![](figures/Dragon/Layer3.png)|

|Layer4|Layer5|Layer6|
|:-:|:-:|:-:|
|![](figures/Dragon/Layer4.png)|![](figures/Dragon/Layer5.png)|![](figures/Dragon/Layer6.png)|