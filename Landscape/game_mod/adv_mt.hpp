#pragma once

#include <nh3api/core/map.hpp>
#include "types.hpp"


bool __fastcall IsTerrainType(const NewfullMap& map, const CObject& obj, int width, TerrainType8 terType);

TerrainType8 __fastcall GetTerrainType(const NewfullMap& map, const CObject& obj, int width);

TerrainType8 __fastcall GetTerrainType(const NewfullMap& map, const class town& tw);

void __fastcall MarkMagicTerrain(NewfullMap& map);
