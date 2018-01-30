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

/**
 * Check if given position is at local minimum
 * @param tile RealTile to get height data from
 * @param pos_x position's x coordinate
 * @param pos_y position's y coordinate
 * @param kernel Kernel to use for check
 * @return Is position at local minimum
 */
bool is_local_minimum(const RealTile* tile, const int& pos_x, const int& pos_y, const Kernel& kernel);

/**
 * @deprecated
 */
void Catchmenter__color_pixel(RealTile* tile, const int& x, const int& y, const Kernel& kernel);

/**
 * @deprecated
 */
void Catchmenter__color_all(RealTile* tile, const Kernel& kernel);

/**
 * Immediately color whole RealTile
 * @param tile RealTile to catchment
 * @param kernel Kernel to use for coloring
 */
void Catchmenter__color_all_immediate(RealTile* tile, const Kernel& kernel);

/**
 * Immediately color whole RealTile performing coloring based on height order
 * @param tile RealTile to catchment
 * @param kernel Kernel to use for coloring
 */
void Catchmenter__color_all_immediate_heightwise(RealTile* tile, const Kernel& kernel);

/**
 * Color RealTile from given position
 * @param tile RealTile to catchment
 * @param visi_matrix Matrix representing which positions have already been visited by Catchmeneter
 * @param from_x starting position's x coordinate
 * @param from_y starting position's y coordinate
 * @param kernel Kernel to use for coloring
 * @param jump_max Radius of area to check for expansion possibility
 */
void Catchmenter__from(RealTile* tile, bool* visi_matrix, const int& from_x, const int& from_y, const Kernel& kernel, int jump_max);

/**
 * Color whole RealTile using "Disjunctive" algorithm
 * @param tile RealTile to catchment
 * @param kernel Kernel to use for coloring
 * @param jump_max Radius of area to check for expansion possibility
 */
void Catchmenter__all(RealTile* tile, const Kernel& kernel, int jump_max);


#endif //CATCHEA_CATCHMENTER_H
