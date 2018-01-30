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
#include <dis_interpreters/topographer.h>

const RealTile::Coloring REALTILE_DEFAULT_COLORING_LOWER = {000, 000, 000, 255};
const RealTile::Coloring REALTILE_DEFAULT_COLORING_UPPER = {000, 000, 000, 255};

const std::vector<RealTileSample> REALTILE_PRESET_COLOR = {
        { 0000, {000, 000, 255, 255} },
        { 0000, {000, 255, 000, 255} },
        { 0300, {255, 255, 000, 255} },
        { 2000, {255, 000, 000, 255} },
};

const std::vector<RealTileSample> REALTILE_PRESET_BLACK_AND_WHITE = {
        { 0000, { 000, 000, 000, 255 } },
        { 2000, { 255, 255, 255, 255 } },
};

bool operator==(const RealTile::Coloring& lhs, const RealTile::Coloring& rhs) {
    return lhs.red == rhs.red
           && lhs.green == rhs.green
           && lhs.blue == rhs.blue
           && lhs.alpha == rhs.alpha;
}

bool operator!=(const RealTile::Coloring& lhs, const RealTile::Coloring& rhs) {
    return !(lhs == rhs);
}

RealTile::Coloring RealTile__random_color() {
    return {
            (unsigned char) (rand() % 256),
            (unsigned char) (rand() % 256),
            (unsigned char) (rand() % 256),
            255
    };
}

RealTile* RealTile__create(int lat, int lon, int lat_size, int lon_size) {
    RealTile* tile = (RealTile*) malloc(sizeof(RealTile));
    if (!tile) return tile;

    tile->lat = lat;
    tile->lon = lon;
    tile->lat_size = lat_size;
    tile->lon_size = lon_size;

    tile->height = tile->width = -1;
    tile->coloring = NULL;

    tile->heights = NULL;

    tile->tex = NULL;
    RealTile__texture_alloc(tile);

    return tile;
}

bool RealTile__coloring_alloc(RealTile* tile, int height, int width) {
    assert(tile->coloring == NULL);

    tile->height = height;
    tile->width = width;
    size_t size = static_cast<size_t>(height * width);
    tile->coloring = (RealTile::Coloring*) calloc(size, sizeof(RealTile::Coloring));
    tile->heights = (signed short*) calloc(size, sizeof(signed short));

    return tile->coloring != NULL && tile->heights != NULL;
}

void RealTile__coloring_place(RealTile* tile, RealTile* place_tile, int place_y, int place_x) {
    int tile_offset = place_y * tile->width + place_x;
    int data_offset = 0;

    for (int v_pos = 0; v_pos < place_tile->height; v_pos++) {
        memcpy(tile->coloring + tile_offset, place_tile->coloring + data_offset, place_tile->width * sizeof(RealTile::Coloring));
        memcpy(tile->heights + tile_offset, place_tile->heights + data_offset, place_tile->width * sizeof(signed short));
        tile_offset += tile->width;
        data_offset += place_tile->width;
    }
}

void RealTile__coloring_dealloc(RealTile* tile) {
    assert(tile->coloring != NULL);

    free(tile->coloring);
    tile->height = tile->width = -1;
    tile->coloring = NULL;

    free(tile->heights);
    tile->heights = NULL;
}

bool RealTile__texture_alloc(RealTile* tile) {
    assert(tile->tex == NULL);

    tile->tex = (Texture*) malloc(sizeof(Texture));
    if (!tile->tex) return false;

    return true;
}

bool RealTile__texture_dealloc(RealTile* tile) {
    assert(tile->tex != NULL);

    glDeleteTextures(1, &tile->tex->texture_id);
    free(tile->tex);
    tile->tex = NULL;
}

void RealTile__destroy(RealTile* tile) {
    RealTile__coloring_dealloc(tile);
    RealTile__texture_dealloc(tile);

    free(tile);
}

void RealTile__texture_generate(RealTile* tile) {
    glDeleteTextures(1, &tile->tex->texture_id);

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
    // Instead I will add dummy alpha channels to coloring provided to this function
    //
    // glPixelStorei(GL_UNPACK_ROW_LENGTH, 3);
    // glPixelStorei(GL_UNPACK_ALIGNMENT, 8); // correction, should be 1

    GLint unpack_row_length, unpack_skip_pixels, unpack_alignment;

    glGetIntegerv(GL_UNPACK_ROW_LENGTH, &unpack_row_length);
    glGetIntegerv(GL_UNPACK_SKIP_PIXELS, &unpack_skip_pixels);
    glGetIntegerv(GL_UNPACK_ALIGNMENT, &unpack_alignment);

    //glPixelStorei(GL_UNPACK_ROW_LENGTH, 1 * tile->tex->width);
    //glPixelStorei(GL_UNPACK_SKIP_PIXELS, 2);
    //glPixelStorei(GL_UNPACK_ALIGNMENT, tile->tex->width);

    // Set texture interpolation method to use linear interpolation (no MIPMAPS)
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    glTexImage2D(target, 0, GL_RGBA,
                 tile->tex->width,
                 tile->tex->height,
                 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, tile->coloring);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, unpack_row_length);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, unpack_skip_pixels);
    glPixelStorei(GL_UNPACK_ALIGNMENT, unpack_alignment);
}

void RealTile__apply_default_coloring(RealTile* tile) {
    Topographer__interpret(tile, REALTILE_DEFAULT_COLORING_LOWER, REALTILE_DEFAULT_COLORING_UPPER, REALTILE_PRESET_COLOR);
    RealTile__texture_generate(tile);
}
