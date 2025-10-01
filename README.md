# H3Landscape
Plugin for the [HoMM3 HD+](https://sites.google.com/site/heroes3hd/) (Heroes of Might & Magic III patch)

![](Installer/H3L_Logo.png)

## About

This add-on makes the battlefield backgrounds better match the terrain on which the battle takes place.\
First and foremost, it adds a lot of new images of underground battlefields.

![](Landscape/img/CmBkDrUg.bmp "example: a muddy underground battlefield")


- [Full list of changes to the game introduced by this addon](Changelog.md)
- [Full list of new battlefield background images](Landscape/img/Readme.md)

## Download and installation

You can download the H3Landscape plugin installer from the [Last release](https://github.com/Paracelsus83/H3Landscape/releases/latest). \
The installer `H3Landscape.msi` should automatically find the location of the installed “Heroes of Might & Magic III” game. If it does not, you have to specify the folder where the game is located.\
After installing the plugin, run `HD_Launcher.exe` and add “Landscape” to the list of plugins.

## Build
To build the H3Landscape plugin, you need Microsoft Visual Studio 2022 with the following components installed:
- MSBuild
- C++ Windows XP Support for VS 2017 (v141) tools
- Windows Universal CRT SDK

To build the `Landscape.lod` resource file, you need the [mmarch.exe](https://github.com/might-and-magic/mmarch/releases/tag/v3.2) tool.
If Visual Studio cannot find this tool during the build process, it will attempt to download it automatically.

H3Landscape uses the [NH3API library](https://github.com/void2012/NH3API), which is included in this project as a git submodule. \
To download the NH3API submodule, after cloning the H3Landscape repository, you have to also execute the command:
```
git submodule update --init
```
