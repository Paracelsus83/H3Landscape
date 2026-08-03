# Changelog

All notable changes to the **H3Landscape** project will be documented in this file.


## [0.2.1]

### Game modification
- Changed algorithm for determining the battlefield background:
  - The following WoG objects have been classified as caves, which affects the choice of battlefield terrain and background image in these objects:
    - Behemoth Tomb - battlefield terrain and background selection is the same as for Behemoth Crag,
    - Vast Cavern - if the terrain type is grass, snow or highlands, the terrain type for the battle is changed to “dirt” and the “underground dirt” image (`CmBkDrUg.pcx`) is used as the battlefield background, for other types of terrain, an image matching the given terrain in its underground version is used.
    - Grotto, Palace of Martial Spirit, Citadel of Pacification, Monastery of Magicans, Library of Legends - battlefield background selection is the same as for Cyclops Stockpile,
    - Snow-covered Grotto - if the terrain type is grass or highlands, the “underground dirt” image (`CmBkDrUg.pcx`) is used as the battlefield background, for other types of terrain, an image matching the given terrain in its underground version is used.


## [0.2.0] - 2026-08-01

### Game modification
- {for SoD/HotA in 32bit graphics mode} Improved procedure for displaying creatures on the battlefield, with an additional visual effects:
  - Effect of having your legs submerged in water when the creature is in a water-filled moat or in lucid pools,
  - Partial fog effect on the legs (gradient) when the creature is on magic clouds or evil fog,
  - No shadows and a flame gradient on the legs when the creature is on fiery fields or in the inferno moat,
  - Air and storm elementals are semi-transparent.

### Modified components
- Rewritten `H3L_HotA_Patcher.exe` (generated file `H3HotA_HD_L.exe` has the same size as the source file `H3HotA_HD.exe`).

### Build system
- Updated project files for Visual Studio 2026, version 18.8.0,
- Clang-CL has been set as the project's default compiler in MSBuild,
- Added support for Ninja and GNU G++ in `CMakeLists.txt`,
- Added the `Utils/ClangTidy.conf` file - Clang-Tidy configuration.


## [0.1.3] - 2026-06-15

### Game modification
- Corrected algorithm for selecting the town's appearance on the adventure map:
  - Fixed a bug in determining the town type when the hero is standing at the town gate;
- Changed algorithm for determining the battlefield background during town sieges:
  - The original H3 background for the Tower siege [surface battle] or the image `SgTwUgBk.pcx` [underground battle] is used as the background for:
     - Necropolis with fort/citadel/castle on snow or cracked ice,
     - Stronghold with fort on snow.


## [0.1.2] - 2026-06-01

### Added game files
- Improved/fixed image:
  - `SgFFBack.pcx` - background used during the siege of Inferno on the fiery fields;
- New town sprites on the adventure map - for various types of terrain:
  - Castle on a snow:
    - `AVCCasVS.def` - village,
    - `AVCCasFS.def` - fort,
    - `AVCCasCS.def` - citadel,
    - `AVCCasXS.def` - castle,
    - `AVCCasZS.def` - capitol;
  - Inferno on a snow:
    - `AVCInfVS.def` - village,
    - `AVCInfFS.def` - fort,
    - `AVCInfCS.def` - citadel,
    - `AVCInfXS.def` - castle,
    - `AVCInfZS.def` - capitol;
  - Necropolis on a snow:
    - `AVCNecVS.def` - village,
    - `AVCNecFS.def` - fort,
    - `AVCNecCS.def` - citadel,
    - `AVCNecXS.def` - castle,
    - `AVCNecZS.def` - capitol;
  - Stronghold on a snow:
    - `AVCStrCS.def` - village,
    - `AVCStrFS.def` - fort,
    - `AVCStrVS.def` - citadel,
    - `AVCStrXS.def` - castle,
    - `AVCStrZS.def` - capitol;
  - Tower on a grass/swamp/highlands:
    - `AVCTowCG.def` - village,
    - `AVCTowFG.def` - fort,
    - `AVCTowVG.def` - citadel,
    - `AVCTowXG.def` - castle,
    - `AVCTowZG.def` - capitol.

### Game modification
- Changed algorithm for selecting the town's appearance on the adventure map:
  - Snowy versions of the sprites are used for the Castle, Inferno, Necropolis amd Stronghold located on snow and cracked ice,
  - Grassy versions of the sprites are used for the Tower located on grass, swamp, highlands, magic plains, clover field, and holy ground.


## [0.1.1] - 2026-05-03

### Added game files
- New image (.pcx file in `Landscape.lod` archive):
  - `CmBkDeU.pcx` - new background of a sandy battlefield in the underground (replacement for removed `CmBkDesUg.pcx`);
- New sprites of adventure map objects (.def file in `Landscape.lod` archive):
  - `Sanct_Sd.def` - Beholders' Sanctuary on land,
  - `UtopiaSn.def` - Dragon Utopia on snow,
  - `GoRsnow.def`  - Garden of Revelation on snow,
  - `GoRlava.def`  - Garden of Revelation on lava and in underground,
  - `AVXhilSd.def` - Hill Fort on sand,
  - `AVXhilU0.def` - Hill Fort in underground,
  - `HAPCavSn.def` - Pirate Cavern on snow,
  - `AVXruiSn.def` - Ruins on snow,
  - `AVXstbl1.def` - Stables on snow,
  - `AVStmpl1.def` - Temple on snow,
  - `AVXtrek3.def` - Tree of Knowledge on rough and wasteland,
  - `AVXtrkLv.def` - Tree of Knowledge on lava and in underground,
  - `AVSuniv1.def` - University on snow,
  - `AVMwwhRg.def` - Water Wheel on rough and wasteland and in underground.

### Game modification
- When starting a new game, the appearance of certain objects on the adventure map is adjusted to better match the terrain type:
  - Beholders' Sanctuary on land,
  - Dragon Utopia on snow,
  - Garden of Revelation on snow / lava / in underground,
  - Hill Fort on sand / in underground,
  - Pirate Cavern on snow,
  - Ruins on snow,
  - Stables on snow,
  - Temple on snow,
  - Tree of Knowledge on rough / wasteland / lava / in underground,
  - University on snow,
  - Water Wheel on rough / wasteland / in underground;
- The appearance of garrisons is no longer adjusted to snow terrain (removal of a feature introduced in version 0.1.0).


## [0.1.0] - 2026-04-05

### Game modification
- When starting a new game (while the map is loading), the appearance of certain objects on the adventure map is adjusted to better match the terrain type:
  - For maps in the “Restoration of Erathia” version:
    - Any Abandoned Mine located on snow, lava, or underground, but not on magical terrain, is replaced with an equivalent from “Armageddon's Blade”,
    - Any Trading Post located on snow, but not on magical terrain, is replaced with a snow equivalent from “Armageddon's Blade”;
  - For maps in the "Shadow of Death" or earlier version, loaded in HotA and ERA games, the following objects located on snow, but not on magical terrain, are replaced with their snow equivalents from HotA/ERA:
    - Garrison {only in ERA},
    - Hill Fort,
    - Idol of Fortune,
    - Library of Enlightenment,
    - Tavern,
    - Den of Thieves,
    - Learning Stone,
    - Tree of Knowledge,
    - Subterranean Gate,
    - Witch Hut.

### Modified components
- The new MSI installer — created using the WiX Toolset.

### Build system
- The procedure for determining the project version based on GIT tag has been moved to the `Directory.Build.props` file.


## [0.0.5] - 2026-03-01

### Added game files
- New image (.pcx file in `Landscape.lod` archive):
  - `CmBkMCBt.pcx` - battlefield background with two boats on magical clouds.

### Game modification
- Changed algorithm for determining the battlefield background:
  - The image `CmBkMCBt.pcx` is used as the background for the battlefield on two boats on magical clouds,
  - The algorithm for selecting the background in objects classified as caves has been unified - it works the same way in SoD, HotA, and WoG,
  - For battles in a gold or crystal mine:
    - if the terrain type is grass, snow, swamp or highlands, the terrain type for the battle is changed to “rough” and the `CmBkRghUg.pcx` is used as the battlefield background,
    - for other types of terrain, an image matching the given terrain in its underground version (`CmBk##Ug.pcx`) is used as the battlefield background;
  - For battles in water objects located on land:
    - Shipwreck uses a background that matches the type of terrain it is located on,
    - Temple of the Sea (considered a cave) uses a background that matches the type of terrain it is located on, except for lava, for which the “dirt” background (`CmBkDrUg.pcx`) is used,
    - Beholders' Sanctuary (considered a cave) uses a background that matches the type of terrain it is located on, with the following exceptions:
      - on “grass” and “highland” terrains, the “swamp” background (`CmBkSwUg.pcx`) is used,
      - on “snow” and “lava” terrains, the “dirt” background (`CmBkDrUg.pcx`) is used;
  - For battles on the coast - unified method of selecting backgrounds in SoD and HotA:
    - the original “coast” background is used on the surface, unless there is an object considered to be a cave,
    - the “underground sandy coast” background (`CmBkCstUg.pcx`) is used in underground areas and caves, unless the terrain type is “subterranean”,
    - the “subterranean shore” background (`CmBkLkUg.pcx`) is used in underground areas and caves, if the terrain type is “subterranean”;
- Changed algorithm for selecting obstacles on the battlefield:
  - If the battle takes place on two boats, obstacle graphics matching the boats are always used (even in the case of magical terrain).

### Modified components
- `H3L_HotA_Patcher.exe` is compatible with Windows XP.

### Build system
- Improved CMake project files:
  - Added static analysis of C++ code using CppCheck and Clang-Tidy,
  - Improved project version setting based on data from the GIT repository.


## [0.0.4] - 2026-02-01

### Added game files
- New obstacle sprites for the underground battlefield:
  - `ObBDt1Ug.def`, `ObDtS3Ug.def`, `ObDtS4Ug.def` - rocks on dirt,
  - `ObGMs1Ug.def`, `ObGMs2Ug.def`, `ObSwMsUg.def` - mushrooms on grass/swamp,
  - `OBSnCrUg.def` - ice crevasse,
  - `ObSnRk3U.def`, `ObSnRk5U.def`, `ObSnRk6U.def` - rocks on snow;
- Background images of underground battlefields modified compared to version 0.0.3:
  - `CmBkDkUg.pcx` - improved boat,
  - `CmBkCFUg.pcx` - improved clover field,
  - `CmBkHGUg.pcx` - improved holy ground,
  - `CmBkMCUg.pcx` - improved magic clouds,
  - `SgInUgBk.pcx` - improved Inferno siege,
  - `SgTwUgBk.pcx` - improved Tower siege.

### Game modification
- Changed algorithm for determining the battlefield background:
  - Pirate Cavern and Spit are considered caves, which affects the choice of battlefield background image in these objects,
  - Battles in creature banks considered caves use an underground background, selected according to the following rules:
    - on grass, snow or highlands, if there is no magical terrain, the “dirt” battlefield background (`CmBkDrUg.pcx`) is used,
    - for other terrain types, a battlefield background matching the terrain is used,
    - {for SoD/WoG} in underground, the original “subterranean” battlefield background is used (default H3 behavior);
  - Battle in Beholders' Sanctuary located on land use an underground background, selected according to the following rules:
    - on grass, swamp or highlands, if there is no magical terrain, the “swamp” battlefield background (`CmBkSwUg.pcx`) is used,
    - on “subterranean” or magical terrain, a battlefield background matching the terrain is used,
    - for other terrain types, the “dirt” battlefield background (`CmBkDrUg.pcx`) is used;
- Changed algorithm for selecting obstacles on the battlefield:
  - For underground battles, some obstacles containing tree trunks are replaced with their underground equivalents.


## [0.0.3] - 2026-01-01

### Added game files
- New image (.pcx file in `Landscape.lod` archive):
  - `SgBwUgBk.pcx` - background of the underground battlefield in Bulwark.

### Game modification
- Added support for Horn of the Abyss 1.8.0,
- Changed algorithm for determining the battlefield background:
  - The image `SgBwUgBk.pcx` is used as background during the siege of the Bulwark in the underground,
  - For battles in water objects located on land:
    - Beholders' Sanctuary - the image `CmBkSwUg.pcx` is used as the background,
    - Temple of the Sea - depending on the type of terrain on which it stands - the underground version of the background is used;
  - If the battle takes place in Cyclops Stockpile on the surface, on grass or snow terrain, the image `CmBkDrUg.pcx` is used as the background,
  - Changing the battlefield background in creature banks (e.g. Cyclops Stockpile) does not change the terrain type that is taken into account when determining creature movement bonuses.


## [0.0.2] - 2025-12-25

### Added components
- `H3L_HotA_Patcher.exe` - tool for integrating the H3Landscape add-on with Horn of the Abyss.

### Added game files
- New images (.pcx files in `Landscape.lod` archive):
  - background images of underground battlefields:
    - `CmBkCstUg.pcx` - sandy coast,
    - `CmBkFGrUg.pcx` - fairy grass - underground equivalent of highlands,
    - `CmBkWlUg.pcx` - wasteland;
  - background images of underground battlefields on magical terrains:
    - `CmBkDunUg.bmp` - dunes,
    - `CmBkFGUg.bmp` - fields of glory,
    - `CmBkIceUg.bmp` - cracked ice;
  - background images of the siege battlefield:
    - `SgCurBack.pcx` - Necropolis/Dungeon/Stronghold/Conflux on cursed ground (surface),
    - `SgCvUgBk.pcx` - Cove in the underground,
    - `SgFaUgBk.pcx` - Factory in the underground;
  - images of the moat on the battlefield:
    - `SgCFMoat.pcx` - Stronghold moat in the clover field,
    - `SgDnCGMlip.pcx` - Dungeon moat lip on cursed ground,
    - `SgDnRkMlip.pcx` - Dungeon moat lip on rockland,
    - `SgFaMoatUg.pcx` - Factory moat in the underground;
- Battlefield background images modified compared to version 0.0.1:
  - `SgElUgBk.pcx` - siege of the Conflux in the underground (adjustment to HotA),
  - `SgCFBack.pcx` - siege on the clover field on the surface (corrected),
  - `SgCFUgBk.pcx` - siege on the clover field in the underground (corrected).

### Game modification
- Added support for towns and terrain types from Horn of the Abyss,
- Changed algorithm for determining the battlefield background:
  - If the battle takes place underground, but not in a mine or cave, and directly borders on water, then:
    - {for HotA} if the terrain type is not “subterranean”, then the “underground sandy coast” battlefield background (`CmBkCstUg.pcx`) is used,
	- in other cases - as in version 0.0.1 - the “subterranean shore” battlefield background (`CmBkLkUg.pcx`) is used;
  - The following battlefield backgrounds are used during town sieges:
    - Cove - original HotA Cove background [surface], `SgCvUgBk.pcx` [underground],
    - Factory - original HotA Factory background [surface], `SgFaUgBk.pcx` [underground],
	  - in the underground, `SgFaMoatUg.pcx` is used as a moat image;
  - If the siege takes place on magical terrain, then the following battlefield backgrounds are used:
    - on the cursed ground, for Necropolis/Dungeon/Stronghold/Conflux or any town with a fort, except for the Tower - `SgCurBack.pcx` [surface], `SgStUgBk.pcx` [underground]
	  - for Dungeon, `SgDnCGMlip.pcx` is used as a moat lip image,
	- in the evil fog, for any town with a fort, except for the Tower - original H3 “evil fog” background [surface], `CmBkEFUg.pcx` [underground]
	- on the clover field, for Castle/Rampart/Stronghold/Fortress/Conflux/Cove - `SgCFBack.pcx` [surface], `SgCFUgBk.pcx` [underground],
	  - for Stronghold, `SgCFMoat.pcx` is used as a moat image,
	- on the rockland, for Castle/Inferno/Necropolis/Dungeon/Conflux/Cove - `SgRkBack.pcx` [surface], `SgRkUgBk.pcx` [underground],
	  - for Dungeon, `SgDnRkMlip.pcx` is used as a moat lip image.

### Build system
- Added `CMakeLists.txt` file to enable building `Landscape.dll` and `Landscape.lod` using CMake.


## [0.0.1] - 2025-11-01

### Added components
- MSI installer that copies `Landscape.dll` and `Landscape.lod` files to the `_HD3_Data\Packs\Landscape` subfolder inside the game folder.

### Added game files
- New battlefield background images (.pcx files in `Landscape.lod` archive):
  - terrain on the surface:
    - `CmBkRedMt.pcx` - “read rocks” - replacement for “subterranean”
  - underground terrains:
    - `CmBkDrUg.pcx` - dirt,
    - `CmBkDesUg.pcx` - sand,
    - `CmBkGrUg.pcx` - grass,
    - `CmBkSnUg.pcx` - snow,
    - `CmBkSwUg.pcx` - swamp,
    - `CmBkRghUg.pcx` - rough / cursed ground,
    - `CmBkLvUg.pcx` - lava,
    - `CmBkLkUg.pcx` - subterranean shore;
  - underground magical terrains: 
    - `CmBkMgUg.pcx` - magic plains,
    - `CmBkCFUg.pcx` - clover field,
    - `CmBkHGUg.pcx` - holy ground,
    - `CmBkEFUg.pcx` - evil fog,
    - `CmBkLPUg.pcx` - lucid pools,
    - `CmBkFFUg.pcx` - fiery fields,
    - `CmBkRkUg.pcx` - rockland,
    - `CmBkMCUg.pcx` - magic clouds;
  - boats:
    - `CmBkDkUg.pcx` - on underground waters,
    - `CmBkMCDk.pcx` - on magic clouds;
  - towns on the surface:
    - `SgDnSfBk.pcx` - Dungeon (siege on “red rocks”),
    - `SgCFBack.pcx` - Castle/Rampart/Fortress/Conflux on clover field,
    - `SgFFBack.pcx` - Inferno on fiery fields,
    - `SgRkBack.pcx` - Castle/Inferno/Necropolis/Conflux on rockland;
  - underground towns:
    - `SgCsUgBk.pcx` - Castle,
    - `SgRmUgBk.pcx` - Rampart,
    - `SgTwUgBk.pcx` - Tower,
    - `SgInUgBk.pcx` - Inferno,
    - `SgNcUgBk.pcx` - Necropolis,
    - `SgStUgBk.pcx` - Stronghold,
    - `SgFrUgBk.pcx` - Fortress,
    - `SgElUgBk.pcx` - Conflux,
    - `SgCFUgBk.pcx` - Castle/Rampart/Fortress/Conflux on clover field,
    - `SgRkUgBk.pcx` - Castle/Inferno/Necropolis/Conflux on rockland.

### Game modification
- Changed algorithm for determining the battlefield background:
  - Behemoth Crag, Cyclops Cave and Cyclops Stockpile are considered caves, which affects the choice of battlefield background image in these objects,
  - If the battle takes place in a “subterranean” type field, but not underground, not in a mine, and not in a cave, then the “red rocks” battlefield background is used,
  - If the battle takes place underground, the underground equivalent of the battlefield background (`CmBk##Ug.pcx`) is used,
  - If the battle takes place in a gold or crystal mine:
    - if the terrain type is grass, snow or swamp, the terrain type for the battle is changed to “dirt” and the `CmBkDrUg.pcx` is used as the battlefield background,
    - for other types of terrain, an image matching the given terrain in its underground version (`CmBk##Ug.pcx`) is used as the battlefield background;
  - If the battle takes place in Behemoth Crag, Cyclops Cave, or Cyclops Stockpile:
    - if the terrain type is grass or snow, the terrain type for the battle is changed to “rough” and the “underground rough” image (`CmBkRghUg.pcx`) is used as the battlefield background,
    - for other types of terrain, an image matching the given terrain in its underground version (`CmBk##Ug.pcx`) is used as the battlefield background,
    - the original H3 “subterranean” battlefield background is always used for the underground Cyclops Stockpile;
  - If the battle takes place underground, but not in a mine or cave, and borders directly on water, then the “subterranean shore” battlefield background is used,
  - If the battle takes place on a boat on a magical terrain, the boat battlefield background is used instead of the magical terrain background,
  - If the battle takes place on a boat and the enemy is not a hero on another boat:
    - if boat is on magic clouds, the `CmBkMCDk.pcx` battlefield background is used,
    - if boat is underground (but not on magic clouds), the `CmBkDkUg.pcx` battlefield background is used;
  - The following battlefield backgrounds are used during town sieges:
    - Dungeon - `SgDnSfBk.pcx` [surface], original H3 Dungeon background [underground],
    - Necropolis on snow - original H3 Tower background [surface], `SgTwUgBk.pcx` [underground],
    - Necropolis/Stronghold/Conflux on cursed ground - original H3 Stronghold background [surface], `SgStUgBk.pcx` [underground]
    - Castle/Rampart/Fortress/Conflux on clover field - `SgCFBack.pcx` [surface], `SgCFUgBk.pcx` [underground],
    - Inferno on fiery fields - `SgFFBack.pcx` [surface and underground],
    - Castle/Inferno/Necropolis/Conflux on rockland - `SgRkBack.pcx` [surface], `SgRkUgBk.pcx` [underground],
    - Tower on magic clouds - original H3 “magic clounds” background [surface], `CmBkMCUg.pcx` [underground],
    - all underground towns not listed above - `Sg##UgBg.pcx` (## is town code).
- Changed algorithm for selecting obstacles on the battlefield:
  - During naval battles in magical terrains, obstacles matching the boat's deck are displayed (instead of obstacles matching the magical terrain).

### Build system
- Added project files for Microsoft Visual Studio 2022:
  - `H3Landscape.sln` - solution file,
  - `Landscape/Landscape.vcxproj` - project for building `Landscape.dll` and `Landscape.lod`,
  - `Installer.vdproj` - project for building `H3Landscape.msi`.
