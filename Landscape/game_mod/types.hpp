#pragma once


typedef const char* CStrPtr;

typedef signed char TerrainType8;


template<typename LExpr, typename ...RExpr>
inline constexpr bool IsOneOf(LExpr lexpr, RExpr... rexpr) {
    return ((lexpr == rexpr) || ...);
}
