/**
 * @file topographer.h
 * @author JMendyk
 * @date 07.01.18
 */

#ifndef CATCHEA_TOPOGRAPHER_H
#define CATCHEA_TOPOGRAPHER_H


#include <vector>

#include "real_tile/real_tile.h"

void Topographer__interpret(RealTile* real_tile,
                            const RealTile::Coloring &lower,
                            const RealTile::Coloring &upper,
                            const std::vector<RealTileSample> &steps);

#endif //CATCHEA_TOPOGRAPHER_H
