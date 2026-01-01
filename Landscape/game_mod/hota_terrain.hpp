#pragma once

#include <nh3api/core/terrain.hpp>


constexpr TTerrainType eTerrainHighlands = static_cast<TTerrainType>(10);
constexpr TTerrainType eTerrainWasteland = static_cast<TTerrainType>(11);

constexpr uint32_t MAX_HOTA_TERRAIN_TYPES = 12;

constexpr EMagicTerrain MAGIC_TERRAIN_CRACKED_ICE = static_cast<EMagicTerrain>(11);
constexpr EMagicTerrain MAGIC_TERRAIN_DUNES = static_cast<EMagicTerrain>(12);
constexpr EMagicTerrain MAGIC_TERRAIN_FIELDS_GLORY = static_cast<EMagicTerrain>(13);

constexpr uint32_t MAX_HOTA_MAGIC_TERRAINS = 14;

constexpr TTownType eTownCove = static_cast<TTownType>(9);
constexpr TTownType eTownFactory = static_cast<TTownType>(10);
constexpr TTownType eTownBulwark = static_cast<TTownType>(11);

constexpr uint32_t MAX_HOTA_TOWN_TYPES = 12;
