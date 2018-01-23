/**
 * @file catchmenter.h
 * @author JMendyk
 * @date 19.01.18
 */

#ifndef CATCHEA_CATCHMENTER_H
#define CATCHEA_CATCHMENTER_H

#include <dis_tile/dis_tile.h>

#include <vector>
#include "real_tile/real_tile.h"

struct Kernel {
    int count;
    bool hard_min;
    std::vector<int> dx;
    std::vector<int> dy;
};

extern Kernel K4, K4_HARD_MIN;
extern Kernel K8, K8_HARD_MIN;

bool is_local_minimum(const GeoTile* geo_tile, const int& pos_x, const int& pos_y, const Kernel& kernel);

bool is_local_minimum(const RealTile* tile, const int& pos_x, const int& pos_y, const Kernel& kernel);

DisTile* Catchmenter__color_pixel(const GeoTile* geo_tile, DisTile* dis_tile, const int& x, const int& y, const Kernel& kernel);

DisTile* Catchmenter__color_all(const GeoTile* geo_tile, DisTile* dis_tile, const Kernel& kernel);

DisTile* Catchmenter__color_all_immediate(const GeoTile* geo_tile, DisTile* dis_tile, const Kernel& kernel);

void Catchmenter__color_pixel(RealTile* tile, const int& x, const int& y, const Kernel& kernel);

void Catchmenter__color_all(RealTile* tile, const Kernel& kernel);

void Catchmenter__color_all_immediate(RealTile* tile, const Kernel& kernel);

void Catchmenter__color_all_immediate_heightwise(RealTile* tile, const Kernel& kernel);


#endif //CATCHEA_CATCHMENTER_H
