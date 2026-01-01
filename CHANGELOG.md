# Changelog

All notable changes to the **H3Landscape** project will be documented in this file.


## [0.0.3] - 2026-01-01

### Added game files
- new image (pcx file in Landscape.lod archive):
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
- new images (pcx files in Landscape.lod archive):
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
- battlefield background images modified compared to version 0.0.1:
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
- Added `CMakeLists.txt` file to enable building `Landscape.dll` and `Landscape.lod` using cmake.


## [0.0.1] - 2025-11-01

### Added components
- MSI installer that copies `Landscape.dll` and `Landscape.lod` files to the `_HD3_Data\Packs\Landscape` subfolder inside the game folder.

### Added game files
- new battlefield background images (pcx files in Landscape.lod archive):
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
    - if the terrain type is grass, snow or swamp, the terrain type for the battle is changed to “dirt” and the CmBkDrUg.pcx is used as the battlefield background,
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
  - `H3Landscape.sln` - solution file
  - `Landscape/Landscape.vcxproj` - project for building Landscape.dll and Landscape.lod
  - `Installer.vdproj` - project for building H3Landscape.msi
