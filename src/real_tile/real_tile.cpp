/**
 * @file real_tile.cpp
 * @author JMendyk
 * @date 21.01.18
 */

#include "real_tile.h"

#include <cstdlib>
#include <cstring>
#include <cassert>
#include <GL/gl.h>
#include <resource_manager.h>
#include <cstdio>

RealTile* RealTile__create(int lat, int lon, int lat_size, int lon_size) {
    RealTile* tile = (RealTile*) malloc(sizeof(RealTile));
    if (!tile) return tile;

    tile->lat = lat;
    tile->lon = lon;
    tile->lat_size = lat_size;
    tile->lon_size = lon_size;

    tile->height = tile->width = -1;
    tile->data = NULL;

    //tile->heights = NULL;

    tile->tex = NULL;

    return tile;
}

bool RealTile__is_equal(const RealTile* lhs, const RealTile* rhs) {
    return lhs->lat == rhs->lat
        && lhs->lon == rhs->lon
        && lhs->lat_size == rhs->lat_size
        && lhs->lon_size == rhs->lon_size
        && lhs->width == rhs->width
        && lhs->height == rhs->height;
}

bool RealTile__is_next_lat(const RealTile* first, const RealTile* next) {
    return first->lat + first->lat_size == next->lat
        && first->lon == next->lon
        && first->lon_size == next->lon_size;
}

bool RealTile__is_next_lon(const RealTile* first, const RealTile* next) {
    return first->lon + first->lon_size == next->lon
        && first->lat == next->lat
        && first->lat_size == next->lat_size;
}

bool RealTile__data_alloc(RealTile* tile, int height, int width) {
    assert(tile->data == NULL);

    tile->height = height;
    tile->width = width;
    size_t size = static_cast<size_t>(height * width);
    tile->data = (RealTile::Data*) calloc(size, sizeof(RealTile::Data));
    //tile->heights = (int*) calloc(size, sizeof(int));

    return tile->data != NULL;// && tile->heights != NULL;
}

void RealTile__data_place(RealTile* tile,
                          int height,
                          int width,
                          int* heights,
                          RealTile::Data* data,
                          int place_y,
                          int place_x) {
    int tile_offset = place_y * tile->width + place_x;
    int data_offset = 0;

    for (int v_pos = 0; v_pos < height; v_pos++) {
        memcpy(tile->data + tile_offset, data + data_offset, width * sizeof(RealTile::Data));
        //memcpy(tile->heights + tile_offset, heights + data_offset, width * sizeof(int));
        tile_offset += tile->width;
        data_offset += width;
    }
}

void RealTile__data_dealloc(RealTile* tile) {
    assert(tile->data != NULL);

    free(tile->data);
    tile->height = tile->width = -1;
    tile->data = NULL;

    //free(tile->heights);
    //tile->heights = NULL;
}

bool RealTile__texture_alloc(RealTile* tile) {
    assert(tile->tex == NULL);

    tile->tex = (Texture*) malloc(sizeof(Texture));
    if (!tile->tex) return false;

    return true;
}

bool RealTile__texture_set(RealTile* tile) {

}

bool RealTile__texture_dealloc(RealTile* tile) {
    assert(tile->tex != NULL);

    glDeleteTextures(1, &tile->tex->texture_id);
    free(tile->tex);
    tile->tex = NULL;
}

//RealTile* RealTile__merge(const RealTile* first, const RealTile* second) {
//    RealTile* merged = NULL;
//
//    if (RealTile__is_next_lat(first, second)) {
//        merged = RealTile__create(first->lat, first->lon, first->lat_size + second->lat_size, first->lon_size);
//        if (!merged) return merged;
//
//        int heights = first->heights + second->heights - 1;
//        int width = first->width;
//        if (RealTile__data_alloc(merged, heights, width)) return NULL;
//
//        int offset = 0;
//        // WARNING: may crash if second->heights == 0
//        memcpy(merged->data + offset, second->data, ((second->heights - 1) * second->width) * sizeof(RealTile::Data));
//        offset += (second->heights - 1) * second->width;
//
//        memcpy(merged->data + offset, first->data, (first->heights * first->width) * sizeof(RealTile::Data));
//        offset += first->heights * first->width;
//
//        assert(offset == heights * width);
//    } else if (RealTile__is_next_lon(first, second)) {
//        merged = RealTile__create(first->lat, first->lon, first->lat_size, first->lon_size + second->lon_size);
//        if (!merged) return merged;
//
//        int heights = first->heights;
//        int width = first->width + second->width - 1;
//        if (RealTile__data_alloc(merged, heights, width)) return NULL;
//
//        int offset = 0;
//        for (int h = 0; h < heights; h++) {
//            // WARNING: may crash if first->width == 0
//            memcpy(merged->data + offset, first->data + first->width * h, (first->width - 1) * sizeof(RealTile::Data));
//            offset += first->width - 1;
//            memcpy(merged->data + offset, second->data + second->width * h, (second->width) * sizeof(RealTile::Data));
//            offset += second->width;
//        }
//
//        assert(offset == heights * width);
//    }
//
//    return merged;
//}

void RealTile__destroy(RealTile* tile) {
    RealTile__data_dealloc(tile);
    free(tile);
}
bool RealTile__texture_create(RealTile* tile) {
    RealTile__texture_alloc(tile);

    //fprintf(stderr, "RealTile::Data: %zu\n", sizeof(RealTile::Data));

    tile->tex->width = tile->width;
    tile->tex->height = tile->height;

    glGenTextures(1, &tile->tex->texture_id);

    GLuint target = GL_TEXTURE_2D;

    // Bind the texture to a name
    glBindTexture(target, tile->tex->texture_id);

    // Set texture clamping method
    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_CLAMP);

    // OpenGL wants pixels to be (by default) every 4 bytes,
    // thus with RGB image every pixel lacks 1 byte. Some libs may add dummy byte to align properly.
    // One way to fix it is to modify OpenGL settings, however these are global and I'd like to avoid that.
    // Instead I will add dummy alpha channels to data provided to this function
    //
    // glPixelStorei(GL_UNPACK_ROW_LENGTH, 3);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 4); // correction, should be 1

    GLint unpack_row_length, unpack_skip_pixels, unpack_alignment;

    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &unpack_row_length);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &unpack_skip_pixels);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);

    fprintf(stderr, "row len %d, skip pixels %d, align %d\n", unpack_row_length, unpack_skip_pixels, unpack_alignment);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, tile->tex->width/8);
    //glPixelStorei(GL_UNPACK_SKIP_PIXELS, 2);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, tile->tex->width);

    // Set texture interpolation method to use linear interpolation (no MIPMAPS)
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(target, 0, GL_RGBA,
                 tile->tex->width,
                 tile->tex->height,
                 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, tile->data);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, unpack_row_length);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, unpack_skip_pixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);

    return true;
}
