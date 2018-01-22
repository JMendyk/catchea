/**
 * @file geotile.c
 * @author JMendyk
 * @date 05.01.18
 */

#include <cstdio>
#include "geo_tile.h"



GeoTile* GeoTile__create(geo_cord_t lat, geo_cord_t lon, geo_cord_t lat_size, geo_cord_t lon_size) {
    GeoTile* tile = (GeoTile*) malloc(sizeof(GeoTile));
    if(!tile) return tile;

    tile->lat = lat;
    tile->lon = lon;
    tile->lat_size = lat_size;
    tile->lon_size = lon_size;
    tile->data = NULL;

    return tile;
}

bool GeoTile__data_set(GeoTile *tile, geo_sample_dim_t height, geo_sample_dim_t width, geo_sample_t *data) {
    if(tile->width != width || tile->height != height) {
        GeoTile__data_clear(tile);
        if(!GeoTile__data_alloc(tile, height, width)) {
            return false;
        }
    }

    memcpy(tile->data, data, height * width * sizeof(geo_sample_t));

    return true;
}

bool GeoTile__data_alloc(GeoTile *tile, geo_sample_dim_t height, geo_sample_dim_t width) {
    tile->data = (geo_sample_t*) calloc(height * width, sizeof(geo_sample_t));
    tile->height = height;
    tile->width = width;

    return tile->data != NULL;
}

void GeoTile__data_place(
    GeoTile *tile,
    geo_sample_dim_t height, geo_sample_dim_t width, geo_sample_t *data,
    geo_sample_dim_t place_y, geo_sample_dim_t place_x
) {
    geo_sample_dim_t tile_offset = place_y * tile->width + place_x;
    geo_sample_dim_t data_offset = 0;

    for(geo_sample_dim_t v_pos = 0; v_pos < height; v_pos++) {
        memcpy(tile->data + tile_offset, data + data_offset, width * sizeof(geo_sample_t));
        tile_offset += tile->width;
        data_offset += width;
    }
}

void GeoTile__data_clear(GeoTile *tile) {
    if(tile->data)
        free(tile->data);
}

void GeoTile__destroy(GeoTile* tile) {
    GeoTile__data_clear(tile);
    free(tile);
}

bool GeoTile__is_equal(const GeoTile* lhs, const GeoTile* rhs) {
    return lhs->lat == rhs->lat
        && lhs->lon == rhs->lon
        && lhs->lat_size == rhs->lat_size
        && lhs->lon_size == rhs->lon_size
        && lhs->width == rhs->width
        && lhs->height == rhs->height;
}

bool GeoTile__is_next_lat(const GeoTile *first, const GeoTile *next) {
    return first->lat + first->lat_size == next->lat
        && first->lon == next->lon
        && first->lon_size == next->lon_size;
}

bool GeoTile__is_next_lon(const GeoTile *first, const GeoTile *next) {
    return first->lon + first->lon_size == next->lon
        && first->lat == next->lat
        && first->lat_size == next->lat_size;
}

GeoTile* GeoTile__merge(const GeoTile* first, const GeoTile* second) {
    GeoTile *merged = NULL;

    if(GeoTile__is_next_lat(first, second)) {
        merged = GeoTile__create(first->lat, first->lon, first->lat_size + second->lat_size, first->lon_size);
        if(!merged) return merged;

        geo_sample_dim_t height = first->height + second->height - 1;
        geo_sample_dim_t width = first->width;
        geo_sample_t* data = (geo_sample_t*) calloc(height * width, sizeof(geo_sample_t));
        if(!data) return NULL;

        geo_sample_dim_t offset = 0;
        // WARNING: may crash if second->heights == 0
        memcpy(data + offset, second->data, ((second->height - 1) * second->width)*sizeof(geo_sample_t));
        offset += (second->height - 1) * second->width;

        memcpy(data + offset, first->data, (first->height * first->width)*sizeof(geo_sample_t));
        offset += first->height * first->width;

        assert(offset == height * width);
        GeoTile__data_set(merged, height, width, data);
        free(data);
    } else if(GeoTile__is_next_lon(first, second)) {
        merged = GeoTile__create(first->lat, first->lon, first->lat_size, first->lon_size + second->lon_size);
        if(!merged) return merged;

        geo_sample_dim_t height = first->height;
        geo_sample_dim_t width = first->width + second->width - 1;
        geo_sample_t* data = (geo_sample_t*) calloc(height * width, sizeof(geo_sample_t));
        if(!data) return NULL;

        geo_sample_dim_t offset = 0;
        for(geo_sample_dim_t h = 0; h < height; h++) {
            // WARNING: may crash if first->width == 0
            memcpy(data + offset, first->data + first->width * h, (first->width - 1)*sizeof(geo_sample_t));
            offset += first->width - 1;
            memcpy(data + offset, second->data + second->width * h, (second->width)*sizeof(geo_sample_t));
            offset += second->width;
        }

        assert(offset == height * width);
        GeoTile__data_set(merged, height, width, data);
        free(data);
    }

    return merged;
}
