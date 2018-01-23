/**
 * @file topographer.h
 * @author JMendyk
 * @date 07.01.18
 */

#ifndef CATCHEA_TOPOGRAPHER_H
#define CATCHEA_TOPOGRAPHER_H


#include <vector>

#include <dis_tile/dis_tile.h>

#include "real_tile/real_tile.h"

DisTile* Topographer__interpret_grayscale(GeoTile* geo_tile);

DisTile* Topographer__interpret_color(GeoTile* geo_tile);

DisTile* Topographer__interpret_param(GeoTile* geo_tile, const DisTileSample& lower, const DisTileSample& upper,
                                      const std::vector< std::pair<DisTileSample, geo_sample_t> >& steps);

void Topographer__interpret(RealTile* real_tile,
                            const RealTile::Coloring &lower,
                            const RealTile::Coloring &upper,
                            const std::vector<RealTileSample> &steps);


DisTile* Topographer__interpret(GeoTile* geo_tile, int is_color);

#endif //CATCHEA_TOPOGRAPHER_H
