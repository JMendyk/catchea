/**
 * @file distile.h
 * @author JMendyk
 * @date 06.01.18
 */

#ifndef CATCHEA_DISTILE_H
#define CATCHEA_DISTILE_H

#include <resource_manager.h>
#include <geo_tile/geo_tile.h>
#include "geo_tile/geo_tile.h"

typedef struct {
    unsigned char red, green, blue, alpha;
} DisTileSample;

/**
 * @brief Represents graphical interpretation of data from GeoTile
 * @details
 * Effectively DisTile stores bitmap that is a representation of data from GeoTile
 * and thus is easy to convert to texture which, for example, can be loaded to OpenGL
 * and displayed on screen.
 */
typedef struct {
    GeoTile* tile;
    DisTileSample* data;
} DisTile;



/**
 * Create DisTile with provided parameters
 * @warning DisTile does not copy GeoTile's data, thus it must not be freed until DisTile is used.
 * @param [in] tile GeoTile which DisTile should represent
 * @return NULL is arguments were invalid or memory allocation was impossible, otherwise a valid pointer to DisTile
 */
DisTile* DisTile__create(GeoTile* gtile);

/**
 * Sets new data for DisTile by copying provided data to internal data field.
 * @remark If DisTile already has data set, it will be freed and new data will be allocated
 * with proper size to fit provided data.
 * @warning data argument must match size of object's tile
 * @param [out] tile DisTile on which set operation should be performed
 * @param [in] data bitmap data
 * @return false if allocation of new memory failed, true if operation was successful
 */
bool DisTile__set_data(DisTile *tile, DisTileSample* data);

/**
 * Frees data of DisTile
 * @param [out] tile
 */
void DisTile__clear_data(DisTile *tile);

/**
 * Destroy DisTile
 * @param [in] tile to be destroyed
 */
void DisTile__destroy(DisTile* tile);

/**
 * Check if DisTiles represent the same area
 * @remark Check if forwarded to GeoTile
 * @param [in] lhs
 * @param [in] rhs
 * @return true if DisTiles represent the same area
 */
bool DisTile__is_equal(const DisTile* lhs, const DisTile* rhs);

/**
 * Check if next DisTile follows first DisTile along latitude axis
 * @remark Check if forwarded to GeoTile
 * @param [in] first
 * @param [in] next
 * @return true if next DisTile follows first DisTile along latitude axis
 */
bool DisTile__is_next_lat(const DisTile *first, const DisTile *next);

/**
 * Check if next DisTile follows first DisTile along longitude axis
 * @remark Check if forwarded to GeoTile
 * @param [in] first
 * @param [in] next
 * @return true if next DisTile follows first DisTile along longitude axis
 */
bool DisTile__is_next_lon(const DisTile *first, const DisTile *next);

/**
 * Merges two DisTiles
 * @remark If merge is impossible (DisTile__is_next_lat and DisTile__is_next_lon return false)
 * returned value is a valid pointer to DisTile. Otherwise NULL is returned.
 * @remark Some functionality is forwarded to GeoTile
 * @remark If one of the DisTiles don't have data set, behaviour is undefined
 * @remark If one of the DisTiles are NULL, behaviour is undefined
 * @param [in] first DisTile to merge
 * @param [in] second DisTile to merge
 * @return NULL if merge was impossible, otherwise a valid pointer to DisTile
 */
DisTile* DisTile__merge(const DisTile* first, const DisTile* second);

Texture DisTile__to_texture(const DisTile* dis_tile);

void DisTile__save_to_file(const DisTile* dis_tile, char const* filename);

#endif //CATCHEA_DISTILE_H
