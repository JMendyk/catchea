/**
 * @file real_tile.h
 * @author JMendyk
 * @date 21.01.18
 */

#ifndef CATCHEA_REAL_TILE_H
#define CATCHEA_REAL_TILE_H

#include <vector>

struct Texture;

struct RealTile {
    int lat, lon;
    int lat_size, lon_size;

    int height, width;

    struct Coloring {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        unsigned char alpha;
    }* coloring;

    signed short* heights;

    Texture* tex;
};

struct RealTileSample {
    signed int height;
    RealTile::Coloring coloring;
};

extern const RealTile::Coloring REALTILE_DEFAULT_COLORING_LOWER;
extern const RealTile::Coloring REALTILE_DEFAULT_COLORING_UPPER;

extern const std::vector<RealTileSample> REALTILE_PRESET_COLOR;
extern const std::vector<RealTileSample> REALTILE_PRESET_BLACK_AND_WHITE;

bool operator==(const RealTile::Coloring& lhs, const RealTile::Coloring& rhs);
bool operator!=(const RealTile::Coloring& lhs, const RealTile::Coloring& rhs);

RealTile::Coloring RealTile__random_color();

/**
 * Create new RealTile instance
 * @return NULL if memory allocation was impossible, otherwise a valid pointer to RealTile
 * @memberof RealTile
 */
RealTile* RealTile__create(int lat, int lon, int lat_size, int lon_size);

/**
 * Allocate memory for RealTile's coloring
 * @param tile RealTile to allocate memory for
 * @param height height of coloring's resolution
 * @param width width of coloring's resolution
 * @return was allocation successful
 * @memberof RealTile
 */
bool RealTile__coloring_alloc(RealTile* tile, int height, int width);

/**
 * Set values in provided rectangle of RealTile's coloring
 * @param tile target RealTile
 * @param data source RealTile
 * @param place_y y coordinate of target coloring where setting should start
 * @param place_x x coordinate of target coloring where setting should start
 * @memberof RealTile
 */
void RealTile__coloring_place(RealTile* tile, RealTile* data, int place_y, int place_x);

/**
 * Deallocate memory of RealTile's coloring
 * @param tile
 * @memberof RealTile
 */
void RealTile__coloring_dealloc(RealTile* tile);

/**
 * Allocate memory for RealTile's texture
 * @param tile
 * @return
 * @memberof RealTile
 */
bool RealTile__texture_alloc(RealTile* tile);

/**
 *
 * @param tile
 * @return
 * @memberof RealTile
 */
bool RealTile__texture_dealloc(RealTile* tile);

/**
 * Generate RealTile's texture based on coloring
 * @param tile RealTile to generate texture for
 * @memberof RealTile
 */
void RealTile__texture_generate(RealTile* tile);

/**
 * Apply default coloring algorithm for RealTile
 * @param tile RealTile to color
 * @memberof RealTile
 */
void RealTile__apply_default_coloring(RealTile* tile);

/**
 * Destroy RealTile instance
 * @param tile RealTile to destroy
 * @memberof RealTile
 */
void RealTile__destroy(RealTile* tile);

#endif //CATCHEA_REAL_TILE_H
