/**
 * @file hgt_plugin.cpp
 * @author JMendyk
 * @date 21.01.18
 */

#include "hgt_plugin.h"

#include "utils.h"

#include <cstdlib>
#include <cstring>
#include <cstdio>

RealTile* RealTile__from_hgt_file(const char* filename) {
    char lat_side, lon_side;
    int lat, lon;

    char* filename_not_const = (char*) calloc(strlen(filename), sizeof(char));
    strcpy(filename_not_const, filename);

    char* true_name = util_basename(filename_not_const);

    if(sscanf(true_name, "%c%02d%c%03d.hgt", &lat_side, &lat, &lon_side, &lon) != 4) {
        return NULL;
    }

    if(lat_side == 'S')
        lat = -lat;

    if(lon_side == 'W')
        lon = -lon;

    return RealTile__from_hgt_file(filename, lat, lon);
}

RealTile* RealTile__from_hgt_file(const char* filename, const int& lat, const int& lon) {
    int height = SAMPLES_PER_AXIS, width = SAMPLES_PER_AXIS;

    RealTile *real_tile = RealTile__create(lat, lon, 1, 1);
    if(!real_tile) return NULL;

    if(!RealTile__data_alloc(real_tile, height, width)) return NULL;

    size_t read_samples = 0;

    {
        FILE* f = fopen(filename, "rb");
        if(!f) {
            fprintf(stderr, "Failed to open HGT file!\n");
            RealTile__destroy(real_tile);
            return NULL;
        }

        unsigned char buf[2];
        while(fread(buf, 1, 2, f) > 0 && read_samples < height * width) {
            int receiver = (buf[0] << 8) | (buf[1]);
            real_tile->heights[read_samples] = receiver;
            read_samples++;
        }

        if(!feof(f) || read_samples != height * width) {
            // There is still data to read or less the expected was read.
            // This should not happen in case of proper HGT file.
            fprintf(stderr, "Not enough or too much data to read from HGT file!\n");
            RealTile__destroy(real_tile);
            return NULL;
        }
        fclose(f);
    }

    return real_tile;
}

RealTile* RealTile__from_hgt_file_batch(
    const char* folder_path,
    const int& lat_min, const int& lon_min,
    const int& lat_max, const int& lon_max
) {
    int lat_size = lat_max-lat_min+1;
    int lon_size = lon_max-lon_min+1;
    RealTile* ret = RealTile__create(lat_min, lon_min, lat_size, lon_size);

    int height = SAMPLES_PER_AXIS * lat_size - (lat_size-1);
    int width = SAMPLES_PER_AXIS * lon_size - (lon_size-1);

    if(!RealTile__data_alloc(ret, height, width)) {
        RealTile__destroy(ret);
        return NULL;
    }

    for(int curr_lat = lat_max; curr_lat >= lat_min; curr_lat--) {
        for(int curr_lon = lon_min; curr_lon <= lon_max; curr_lon++) {
            char filename[strlen(folder_path)+20];
            sprintf(filename, "%s/%c%02d%c%03d.hgt",
                    folder_path,
                    curr_lat >= 0 ? 'N' : 'S', abs(curr_lat),
                    curr_lon >= 0 ? 'E' : 'W', abs(curr_lon)
            );
            RealTile* tile = RealTile__from_hgt_file(filename);

            int place_y = (SAMPLES_PER_AXIS-1) * (lat_max - curr_lat);
            int place_x = (SAMPLES_PER_AXIS-1) * (curr_lon - lon_min);

            RealTile__data_place(ret, SAMPLES_PER_AXIS, SAMPLES_PER_AXIS, tile->heights, tile->coloring, place_y, place_x);

            RealTile__destroy(tile);
        }
    }

    return ret;
}
