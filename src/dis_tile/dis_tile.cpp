/**
 * @file distile.cpp
 * @author JMendyk
 * @date 06.01.18
 */


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "dis_tile.h"



DisTile* DisTile__create(GeoTile* gtile) {
    DisTile* dtile = (DisTile*) malloc(sizeof(DisTile));
    if(!dtile) return dtile;

    dtile->tile = gtile;
    dtile->data = NULL;

    return dtile;
}

bool DisTile__set_data(DisTile *tile, DisTileSample* data) {
    DisTile__clear_data(tile);

    tile->data = (DisTileSample*) calloc(tile->tile->height * tile->tile->width, sizeof(DisTileSample));
    if(!tile->data)
        return false;

    memcpy(tile->data, data, tile->tile->height * tile->tile->width * sizeof(DisTileSample));

    return true;
}

void DisTile__clear_data(DisTile *tile) {
    if(tile->data) {
        free(tile->data);
        tile->data = NULL;
    }
}

void DisTile__destroy(DisTile* tile) {
    DisTile__clear_data(tile);
    free(tile);
}

bool DisTile__is_equal(const DisTile* lhs, const DisTile* rhs) {
    // TODO: Implement
    return false;
}

bool DisTile__is_next_lat(const DisTile *first, const DisTile *next) {
    return GeoTile__is_next_lat(first->tile, next->tile);
}

bool DisTile__is_next_lon(const DisTile *first, const DisTile *next) {
    return GeoTile__is_next_lon(first->tile, next->tile);
}

DisTile* DisTile__merge(const DisTile* first, const DisTile* second) {
    // TODO: Implement
    return NULL;
}

Texture DisTile__to_texture(const DisTile* dis_tile) {
    return rm_texture_from_memory(dis_tile->data, dis_tile->tile->width, dis_tile->tile->height, 4);
}

void DisTile__save_to_file(const DisTile* dis_tile, char const* filename) {
    stbi_write_png(filename, dis_tile->tile->width, dis_tile->tile->height, 4, dis_tile->data, dis_tile->tile->width * sizeof(DisTileSample));
}
