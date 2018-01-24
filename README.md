# Catchea
*Topographic Data Browser*

![logicon_promo](demo/catchea_demo.gif)

## Compile from sources

Requirements:
- Modern C++ compiler supporting C++11,
- Make and CMake,
- `xorg-dev`,
- Development version of OpenGL - version 1.2+.

Get sources: 
```bash
git clone "https://github.com/JMendyk/catchea.git" catchea && cd catchea
# git checkout develop # to build development version (may be unstable)
```

```bash
mkdir build && cd build
cmake -H.. -B. -G "CodeBlocks - Unix Makefiles"
cmake --build . --target catchea --
make install
```

To utilize multiple cores of processor, parameter `-j X` (where X = cores to use) can be added
to improve build speed.

In project's root folder, directory `dist` will be created 
with redistributable package.

## Usage
When app starts it load example tile. To load own HGT files select "folder" icon it top-left
corner. 

You can also load multiple files using Top menu's second option. This modules
provides functionality of loading rectangular area from HGT files in provided file.

On the right side of the screen customization of Elevation coloring and Catchment area generation
are available.

Elevation coloring lets use color loaded map based on heights of the samples on the map.
Two presets - Black & White and Color area available. You can also create custom coloring rules
that match your needs.

Catchment area generation finds regions which shared common local minimum.
Five algorithm for catchment area generation are available - each of them works in different
ways and returns different results to match user requirements.

Additionally, you can click any point on map using Left or Right mouse button - then single
catchment area will be generated using provided coordinate as starting point.

Zoom the map view using Ctrl + Mouse Wheel or Page Up/Down buttons.
Move around using Arrow Buttons; Mouse Wheel (vertically), Shift + Mouse Wheel (horizontally) or by
holding Middle Mouse Button and moving mouse around.

**Important Note**: 
Single HGT tile (that is 1201^2 samples representing 1x1 degree excerpt of earths surface)
can consume from 20 to 30 MB of memory. Thus loading and operations on large area (such as Poland - 60 tiles)
can use up to 1.8 GB. Keep it in mind when loading such big area and while performing operations on them
(which will obviously take longer to finish).

## Used libraries

This project uses following libraries:

- [OpenGL](https://www.opengl.org/)

- [GL3W](https://github.com/skaslev/gl3w)

- [GLFW](https://github.com/glfw/glfw)

- [ImGUI](https://github.com/ocornut/imgui) with addons by [Flix01](https://github.com/ocornut/imgui)

- [stb_image](https://github.com/nothings/stb)

## Notes

This project includes script to download HGT files from [https://dds.cr.usgs.gov](https://dds.cr.usgs.gov)
and provides limited amount of these files in project's file structure. Read usage and licensing
information of these data's authors before using them.

This project uses icons from [Material Design Icons's collection](https://material.io/icons/). 
These icons are available under the Apache License Version 2.0.