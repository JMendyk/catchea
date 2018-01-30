/**
 * @file hgt_plugin.h
 * @author JMendyk
 * @date 21.01.18
 */

#ifndef CATCHEA_HGT_PLUGIN_H
#define CATCHEA_HGT_PLUGIN_H

#include "real_tile.h"

#define SAMPLES_PER_AXIS 1201
#define SAMPLES_PER_TILE (SAMPLES_PER_AXIS*SAMPLES_PER_AXIS)

char* hgt_path_for(char* folder_path, int lat, int lon);

/**
 * Create RealTile from HGT file, deducing tile's coordinates based on filename
 * @param [in] filename path to HGT file
 * @return NULL if HGT file does not exists or other errors happened, otherwise a valid pointer to RealTile
 */
RealTile* RealTile__from_hgt_file(const char* filename);

/**
 * Create RealTile from HGT file
 * @param [in] filename path to HGT file
 * @param [in] lat bottom-left corner's latitude of loaded tile
 * @param [in] lon bottom-left corner's longitude of loaded tile
 * @return NULL if HGT file does not exists or other errors happened, otherwise a valid pointer to RealTile
 */
RealTile* RealTile__from_hgt_file(const char* filename, const int& lat, const int& lon);

/**
 * Load set of HGT file and merge them into one RealTile
 * For files to be loaded in batch, they must match standard pattern,
 * ex. N49E014.hgt.
 * @param [in] folder_path path prefix for HGT files
 * @param [in] lat_min latitude of first tile to load
 * @param [in] lon_min longitude of first tile to load
 * @param [in] lat_max latitude of last tile to load
 * @param [in] lon_max longitude of last tile to load
 * @return NULL if any HGT file does not exist or other errors happened, otherwise a valid pointer to RealTile
 */
RealTile* RealTile__from_hgt_file_batch(
    const char* folder_path,
    const int& lat_min, const int& lon_min,
    const int& lat_max, const int& lon_max
);

#endif //CATCHEA_HGT_PLUGIN_H
