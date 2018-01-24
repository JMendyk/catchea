/**
 * @file catchmenter.h
 * @author JMendyk
 * @date 19.01.18
 */

#ifndef CATCHEA_CATCHMENTER_H
#define CATCHEA_CATCHMENTER_H

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

bool is_local_minimum(const RealTile* tile, const int& pos_x, const int& pos_y, const Kernel& kernel);

void Catchmenter__color_pixel(RealTile* tile, const int& x, const int& y, const Kernel& kernel);

void Catchmenter__color_all(RealTile* tile, const Kernel& kernel);

void Catchmenter__color_all_immediate(RealTile* tile, const Kernel& kernel);

void Catchmenter__color_all_immediate_heightwise(RealTile* tile, const Kernel& kernel);

void Catchmenter__from(RealTile* tile, bool* visi_matrix, const int& from_x, const int& from_y, const Kernel& kernel);

void Catchmenter__all(RealTile* tile, const Kernel& kernel);


#endif //CATCHEA_CATCHMENTER_H
