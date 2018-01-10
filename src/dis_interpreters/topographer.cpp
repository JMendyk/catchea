/**
 * @file topographer.cpp
 * @author JMendyk
 * @date 07.01.18
 */

#include <vector>
#include <dis_tile/dis_tile.h>
#include <math.h>
#include <cstdio>

#include "topographer.h"

#include "utils.h"



inline unsigned char clamp(signed short minn, signed short s, signed short maxx) {
    if(s > maxx) s = maxx;
    if(s < minn) s = minn;
    return (unsigned char) s;
}

DisTile* Topographer__interpret_grayscale(GeoTile* geo_tile) {
    //START_BENCH(interpret_grayscale)

    DisTile* dis_tile = DisTile__create(geo_tile);
    if(!dis_tile) return dis_tile;

    DisTileSample* dis_data = (DisTileSample*) calloc(dis_tile->tile->height * dis_tile->tile->width, sizeof(DisTileSample));

    for(size_t it = 0; it <= (dis_tile->tile->height * dis_tile->tile->width); it ++) {
        dis_data[it].red = clamp(0, dis_tile->tile->data[it], 255);
        dis_data[it].green = clamp(0, dis_tile->tile->data[it], 255);
        dis_data[it].blue = clamp(0, dis_tile->tile->data[it], 255);
        dis_data[it].alpha = 255;
    }

    if(!DisTile__set_data(dis_tile, dis_data)) {
        free(dis_data);
        free(dis_tile);
        return NULL;
    }

    //STOP_BENCH(interpret_grayscale)

    //fprintf(stderr, "Interpreted in grayscale in %.2lf\n", GET_BENCH(interpret_grayscale));

    return dis_tile;
}

inline DisTileSample sample_gradient(const DisTileSample& start, const DisTileSample& finish, float grad_norm) {
    //assert(grad_norm >= 0);
    return {
        (unsigned char) (start.red + grad_norm*(finish.red - start.red)),
        (unsigned char) (start.green + grad_norm*(finish.green - start.green)),
        (unsigned char) (start.blue + grad_norm*(finish.blue - start.blue)),
        (unsigned char) (start.alpha + grad_norm*(finish.alpha - start.alpha))
    };
}

DisTile* Topographer__interpret_color(GeoTile* geo_tile) {
    //START_BENCH(interpret_color)

    DisTile* dis_tile = DisTile__create(geo_tile);
    if(!dis_tile) return dis_tile;

    std::vector< std::pair<DisTileSample, geo_sample_t> > steps = {
        std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32768),
        std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32767),
        std::make_pair((DisTileSample){ 000, 000, 255, 255 },      0),
        std::make_pair((DisTileSample){ 000, 255, 000, 255 },      0),
        std::make_pair((DisTileSample){ 255, 255, 000, 255 },    300),
        std::make_pair((DisTileSample){ 255, 000, 000, 255 },   2000),
        std::make_pair((DisTileSample){ 000, 000, 000, 255 },   8000)
    };

    DisTileSample* dis_data = (DisTileSample*) calloc(dis_tile->tile->height * dis_tile->tile->width, sizeof(DisTileSample));

    for(size_t it = 0; it <= (dis_tile->tile->height * dis_tile->tile->width); it++) {
        //dis_data[it].red = clamp(0, dis_tile->tile->data[it], 255);
        //dis_data[it].green = clamp(0, dis_tile->tile->data[it], 255);
        //dis_data[it].blue = clamp(0, dis_tile->tile->data[it], 255);
        //dis_data[it].alpha = 255;
        for(auto step = steps.begin(); step != steps.end(); step++) {
            if(dis_tile->tile->data[it] <= step->second) {
                float grad_norm = ((float)(dis_tile->tile->data[it] - (step-1)->second))/(step->second - (step-1)->second);
                dis_data[it] = sample_gradient((step-1)->first, step->first, grad_norm);
                break;
            }
        }
    }

    if(!DisTile__set_data(dis_tile, dis_data)) {
        free(dis_data);
        free(dis_tile);
        return NULL;
    }

    //STOP_BENCH(interpret_color)

    //fprintf(stderr, "Interpreted in color in %.2lf\n", GET_BENCH(interpret_color));

    return dis_tile;
}

DisTile* Topographer__interpret(GeoTile* geo_tile, int is_color) {
    if(is_color == 0)
        return Topographer__interpret_grayscale(geo_tile);
    else if(is_color == 1)
        return Topographer__interpret_color(geo_tile);
    return NULL;
}
