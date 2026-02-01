#pragma once

#include <nh3api/core/combat.hpp>


constexpr size_t MAX_UG_OBSTALCES = 10;

extern combatManager::TObstacleInfo UgObstacleInfos[MAX_UG_OBSTALCES];

const extern uint8_t UndergroundObstaclesMap[250];


void InitializeUndergroundObstacles();
