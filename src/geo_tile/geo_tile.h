/**
 * @file geotile.h
 * @author JMendyk
 * @date 05.01.18
 */

#ifndef CATCHEA_GEOTILE_H
#define CATCHEA_GEOTILE_H

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

typedef unsigned short geo_cord_t;
typedef   signed short geo_sample_t;
typedef unsigned  long geo_sample_dim_t;

/**
 * @brief Represents excerpt of geographic data
 * of rectangular area on earth.
 * @details
 * GeoTile stores information about which region it represents
 * using bottom-left corner's latitude and longitude and dimension along
 * latitude and longitude axes.
 *
 * Region's data is represented by height * width samples.
 * If two GeoTiles are adjacent they share one row/column, as the result
 * if you merge horizontally (along longitude axis) two tiles (f and s), the resulting
 * tile will have a with of f.width + s.width - 1.
 */
typedef struct {
    geo_cord_t lat, lon;
    geo_cord_t lat_size, lon_size;

    geo_sample_dim_t height, width;
    geo_sample_t* data;
} GeoTile;



/**
 * Create GeoTile with provided parameters
 * @param [in] lat bottom-left corner's latitude of represented area
 * @param [in] lon bottom-left corner's longitude of represented area
 * @param [in] lat_size size of area's along latitude axis
 * @param [in] lon_size size of area's along longitude axis
 * @return NULL is arguments were invalid or memory allocation was impossible, otherwise a valid pointer to GeoTile
 */
GeoTile* GeoTile__create(geo_cord_t lat, geo_cord_t lon, geo_cord_t lat_size, geo_cord_t lon_size);

/**
 * Sets new data for GeoTile by copying provided data to internal data field.
 * @remark If GeoTile already has data set, it will be freed and new data will be allocated
 * with proper size to fit provided data.
 * @param [out] tile GeoTile on which set operation should be performed
 * @param [in] height data's height
 * @param [in] width data's width
 * @param [in] data geographic data
 * @return false if allocation of new memory failed, true if operation was successful
 */
bool GeoTile__set_data(GeoTile *tile, geo_sample_dim_t height, geo_sample_dim_t width, geo_sample_t* data);

/**
 * Frees data of GeoTile
 * @param [out] tile
 */
void GeoTile__clear_data(GeoTile *tile);

/**
 * Destroy GeoTile
 * @param [in] tile to be destroyed
 */
void GeoTile__destroy(GeoTile* tile);

/**
 * Check if GeoTiles represent the same area
 * @param [in] lhs
 * @param [in] rhs
 * @return true if GeoTiles represent the same area
 */
bool GeoTile__is_equal(const GeoTile* lhs, const GeoTile* rhs);

/**
 * Check if next GeoTile follows first GeoTile along latitude axis
 * @param [in] first
 * @param [in] next
 * @return true if next GeoTile follows first GeoTile along latitude axis
 */
bool GeoTile__is_next_lat(const GeoTile *first, const GeoTile *next);

/**
 * Check if next GeoTile follows first GeoTile along longitude axis
 * @param [in] first
 * @param [in] next
 * @return true if next GeoTile follows first GeoTile along longitude axis
 */
bool GeoTile__is_next_lon(const GeoTile *first, const GeoTile *next);

/**
 * Merges two GeoTiles
 * @remark If merge is impossible (GeoTile__is_next_lat and GeoTile__is_next_lon return false)
 * returned value is a valid pointer to GeoTile. Otherwise NULL is returned.
 * @remark If one of the GeoTiles don't have data set, behaviour is undefined
 * @remark If one of the GeoTiles are NULL, behaviour is undefined
 * @param [in] first GeoTile to merge
 * @param [in] second GeoTile to merge
 * @return NULL if merge was impossible, otherwise a valid pointer to GeoTile
 */
GeoTile* GeoTile__merge(const GeoTile* first, const GeoTile* second);


#endif //CATCHEA_GEOTILE_H
