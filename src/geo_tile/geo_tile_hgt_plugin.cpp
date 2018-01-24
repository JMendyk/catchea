/**
 * @file geotile_hgt_plugin.cpp
 * @author JMendyk
 * @date 06.01.18
 */

#include <cstdio>
#include "geo_tile_hgt_plugin.h"

char* hgt_path_for(char* folder_path, int lat, int lon) {
    char* filename = (char*) calloc(strlen(folder_path)+20, sizeof(char));
    sprintf(filename, "%s%s%c%02d%c%03d.hgt",
            folder_path,
            strlen(folder_path) > 0 ? "/" : "",
            lat >= 0 ? 'N' : 'S', abs(lat),
            lon >= 0 ? 'E' : 'W', abs(lon)
    );
    return filename;
}

GeoTile* GeoTile__from_hgt_file(const char* filename) {
    char lat_side, lon_side;
    geo_cord_t lat, lon;

    char* filename_not_const = (char*) calloc(strlen(filename), sizeof(char));
    strcpy(filename_not_const, filename);

    char* true_name = util_basename(filename_not_const);

    if(sscanf(true_name, "%c%02hu%c%03hu.hgt", &lat_side, &lat, &lon_side, &lon) != 4) {
        return NULL;
    }

    if(lat_side == 'S')
        lat = -lat;

    if(lon_side == 'W')
        lon = -lon;

    return GeoTile__from_hgt_file(filename, lat, lon);
}

GeoTile* GeoTile__from_hgt_file(const char* filename, const geo_cord_t lat, const geo_cord_t lon) {
    geo_sample_dim_t height = SAMPLES_PER_AXIS, width = SAMPLES_PER_AXIS;

    geo_sample_t* tile_data = (geo_sample_t*) calloc(height * width, sizeof(geo_sample_t));
    if(!tile_data) return NULL;

    GeoTile *geo_tile = GeoTile__create(lat, lon, 1, 1);

    size_t read_samples = 0;

    {
        FILE* f = fopen(filename, "rb");
        if(!f) {
            fprintf(stderr, "Failed to open HGT file!\n");
            free(tile_data);
            GeoTile__destroy(geo_tile);
            return NULL;
        }

        unsigned char buf[2];
        while(fread(buf, 1, 2, f) > 0 && read_samples < height * width) {
            tile_data[read_samples] = (buf[0] << 8) | (buf[1]);
            read_samples++;
        }

        if(!feof(f) || read_samples != height * width) {
            // There is still data to read or less the expected was read.
            // This should not happen in case of proper HGT file.
            fprintf(stderr, "Not enough or too much data to read from HGT file!\n");
            free(tile_data);
            GeoTile__destroy(geo_tile);
            return NULL;
        }
        fclose(f);
    }

    if(!GeoTile__data_set(geo_tile, height, width, tile_data)) {
        free(tile_data);
        GeoTile__destroy(geo_tile);
        return NULL;
    }

    return geo_tile;
}

GeoTile* GeoTile__from_hgt_file_batch(
    const char* folder_path,
    const geo_cord_t lat_min, const geo_cord_t lon_min,
    const geo_cord_t lat_max, const geo_cord_t lon_max
) {
    geo_cord_t lat_size = lat_max-lat_min+1;
    geo_cord_t lon_size = lon_max-lon_min+1;
    GeoTile* ret = GeoTile__create(lat_min, lon_min, lat_size, lon_size);

    geo_sample_dim_t height = SAMPLES_PER_AXIS * lat_size - (lat_size-1);
    geo_sample_dim_t width = SAMPLES_PER_AXIS * lon_size - (lon_size-1);

    if(!GeoTile__data_alloc(ret, height, width)) {
        GeoTile__destroy(ret);
        return NULL;
    }

    for(geo_cord_t curr_lat = lat_max; curr_lat >= lat_min; curr_lat--) {
        for(geo_cord_t curr_lon = lon_min; curr_lon <= lon_max; curr_lon++) {
            char filename[strlen(folder_path)+20];
            sprintf(filename, "%s/%c%02d%c%03d.hgt",
                    folder_path,
                    curr_lat >= 0 ? 'N' : 'S', abs(curr_lat),
                    curr_lon >= 0 ? 'E' : 'W', abs(curr_lon)
            );
            GeoTile* tile = GeoTile__from_hgt_file(filename);

            geo_sample_dim_t place_y = (SAMPLES_PER_AXIS-1) * (geo_sample_dim_t)(lat_max - curr_lat);
            geo_sample_dim_t place_x = (SAMPLES_PER_AXIS-1) * (geo_sample_dim_t)(curr_lon - lon_min) ;

            GeoTile__data_place(ret, SAMPLES_PER_AXIS, SAMPLES_PER_AXIS, tile->data, place_y, place_x);

            GeoTile__destroy(tile);
        }
    }

    return ret;
}
