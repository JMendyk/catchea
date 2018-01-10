/**
 * @file topographer.h
 * @author JMendyk
 * @date 07.01.18
 */

#ifndef CATCHEA_TOPOGRAPHER_H
#define CATCHEA_TOPOGRAPHER_H

#include <dis_tile/dis_tile.h>



DisTile* Topographer__interpret_grayscale(GeoTile* geo_tile);

DisTile* Topographer__interpret_color(GeoTile* geo_tile);

DisTile* Topographer__interpret(GeoTile* geo_tile, int is_color);

#endif //CATCHEA_TOPOGRAPHER_H
