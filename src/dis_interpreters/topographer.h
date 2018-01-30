/**
 * @file topographer.h
 * @author JMendyk
 * @date 07.01.18
 */

#ifndef CATCHEA_TOPOGRAPHER_H
#define CATCHEA_TOPOGRAPHER_H


#include <vector>

#include "real_tile/real_tile.h"

/**
 * Interpert height data of RealTile and assign colors
 * @param real_tile RealTile to color
 * @param lower color value for heights under minimum step
 * @param upper color value for heights above maximum step
 * @param steps colors for height ranges
 */
void Topographer__interpret(RealTile* real_tile,
                            const RealTile::Coloring &lower,
                            const RealTile::Coloring &upper,
                            const std::vector<RealTileSample> &steps);

#endif //CATCHEA_TOPOGRAPHER_H
