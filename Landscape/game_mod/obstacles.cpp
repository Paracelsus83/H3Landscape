#include "obstacles.hpp"


combatManager::TObstacleInfo UgObstacleInfos[MAX_UG_OBSTALCES] = {
    { 0, 0, 0, 0,   5, {}, "ObBDt1Ug.def" },
    { 0, 0, 0, 0,  12, {}, "ObDtS3Ug.def" },
    { 0, 0, 0, 0,  13, {}, "ObDtS4Ug.def" },
    { 0, 0, 0, 0,  19, {}, "ObGMs2Ug.def" },
    { 0, 0, 0, 0,  21, {}, "ObGMs1Ug.def" },
    { 0, 0, 0, 0,  26, {}, "ObSnRk3U.def" },
    { 0, 0, 0, 0,  28, {}, "ObSnRk5U.def" },
    { 0, 0, 0, 0,  29, {}, "ObSnRk6U.def" },
    { 0, 0, 0, 0,  33, {}, "OBSnCrUg.def" },
    { 0, 0, 0, 0,  34, {}, "ObSwMsUg.def" },
};


const uint8_t UndergroundObstaclesMap[250] = {
    0,  0,  0,  0,  0,  1,  0,  0,  0,  0,
    0,  0,  2,  3,  0,  0,  0,  0,  0,  4,
    0,  5,  0,  0,  0,  0,  6,  0,  7,  8,
    0,  0,  0,  9, 10,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


void InitializeUndergroundObstacles() {
    constexpr size_t OBST_PARAMS_SIZE = offsetof(combatManager::TObstacleInfo, FileName);

    for (size_t i = 0; i < MAX_UG_OBSTALCES; ++i) {
        size_t origObsIdx = UgObstacleInfos[i].underlay;
        memcpy(&UgObstacleInfos[i], &combatManager::ObstacleInfo[origObsIdx], OBST_PARAMS_SIZE);
    }
}
