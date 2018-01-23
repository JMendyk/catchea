/**
 * @file real_tile.h
 * @author JMendyk
 * @date 21.01.18
 */

#ifndef CATCHEA_REAL_TILE_H
#define CATCHEA_REAL_TILE_H

struct Texture;

struct RealTile {
    int lat, lon;
    int lat_size, lon_size;

    int height, width;

    struct Data {
        unsigned char red;
        unsigned char green;
        unsigned char blue;
        unsigned char alpha;
    }* coloring;

    signed short* heights;

    Texture* tex;
};

bool operator==(const RealTile::Data& lhs, const RealTile::Data& rhs);
bool operator!=(const RealTile::Data& lhs, const RealTile::Data& rhs);

RealTile::Data RealTile__random_color();

RealTile* RealTile__create(int lat, int lon, int lat_size, int lon_size);

bool RealTile__is_equal(const RealTile* lhs, const RealTile* rhs);
bool RealTile__is_next_lat(const RealTile* first, const RealTile* next);
bool RealTile__is_next_lon(const RealTile* first, const RealTile* next);

bool RealTile__data_alloc(RealTile* tile, int height, int width);
void RealTile__data_place(RealTile* tile, RealTile* data, int place_y, int place_x);
void RealTile__data_dealloc(RealTile* tile);

bool RealTile__texture_alloc(RealTile* tile);
bool RealTile__texture_dealloc(RealTile* tile);

bool RealTile__texture_generate(RealTile *tile);

void RealTile__destroy(RealTile* tile);

#endif //CATCHEA_REAL_TILE_H
