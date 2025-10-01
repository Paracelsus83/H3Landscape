# Changelog

All notable changes to the **H3Landscape** project will be documented in this file.

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
    - Castle/Inferno/Necropolis/Conflux on rockland: `SgRkBack.pcx` [surface], `SgRkUgBk.pcx` [underground],
    - Tower on magic clouds - original H3 "magic clounds" background [surface], `CmBkMCUg.pcx` [underground],
    - all underground towns not listed above - `Sg##UgBg.pcx` (## is town code).
- Changed algorithm for selecting obstacles on the battlefield:
  - During naval battles in magical terrains, obstacles matching the boat's deck are displayed (instead of obstacles matching the magical terrain).

### Build system
- Added project files for Microsoft Visual Studio 2022:
  - `H3Landscape.sln` - solution file
  - `Landscape/Landscape.vcxproj` - project for building Landscape.dll and Landscape.lod
  - `Installer.vdproj` - project for building H3Landscape.msi
