/**
 * @file topographer.cpp
 * @author JMendyk
 * @date 07.01.18
 */

#include <dis_tile/dis_tile.h>
#include <math.h>
#include <cstdio>
#include <climits>

#include "topographer.h"

#include "utils.h"
#include "real_tile/real_tile.h"

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

inline DisTileSample sample_gradient(const DisTileSample& start, const DisTileSample& finish, const float& grad_norm) {
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

DisTile* Topographer__interpret_param(GeoTile* geo_tile, const DisTileSample& lower, const DisTileSample& upper,
                                      const std::vector< std::pair<DisTileSample, geo_sample_t> >& steps) {
    //START_BENCH(interpret_color)

    DisTile* dis_tile = DisTile__create(geo_tile);
    if(!dis_tile) return dis_tile;

    DisTileSample* dis_data = (DisTileSample*) calloc(dis_tile->tile->height * dis_tile->tile->width, sizeof(DisTileSample));

    for(size_t it = 0; it <= (dis_tile->tile->height * dis_tile->tile->width); it++) {
        if(dis_tile->tile->data[it] < steps.begin()->second) {
            dis_data[it] = lower;
        } else if((steps.end()-1)->second < dis_tile->tile->data[it]) {
            dis_data[it] = upper;
        } else for(auto step = steps.begin(); step != steps.end(); step++) {
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

    //fprintf(stderr, "Interpreted in %lf\n", GET_BENCH(interpret_color));

    return dis_tile;
}

inline RealTile::Coloring sample_gradient(const RealTile::Coloring& start, const RealTile::Coloring& finish, const float& grad_norm) {
    //assert(grad_norm >= 0);
    return {
        (unsigned char) (start.red + grad_norm*(finish.red - start.red)),
        (unsigned char) (start.green + grad_norm*(finish.green - start.green)),
        (unsigned char) (start.blue + grad_norm*(finish.blue - start.blue)),
        (unsigned char) (start.alpha + grad_norm*(finish.alpha - start.alpha))
    };
}

void Topographer__interpret(RealTile* real_tile,
                            const RealTile::Coloring &lower,
                            const RealTile::Coloring &upper,
                            const std::vector<std::pair<int, RealTile::Coloring>> &steps) {
    //START_BENCH(interpret_color)

    for(size_t it = 0; it <= (real_tile->height * real_tile->width); it++) {
        if(real_tile->heights[it] < steps.begin()->first) {
            real_tile->coloring[it] = lower;
        } else if((steps.end()-1)->first < real_tile->heights[it]) {
            real_tile->coloring[it] = upper;
        } else for(auto step = steps.begin(); step != steps.end(); step++) {
                if(real_tile->heights[it] <= step->first) {
                    float grad_norm = ((float)(real_tile->heights[it] - (step-1)->first)/(step->first - (step-1)->first));
                    real_tile->coloring[it] = sample_gradient((step-1)->second, step->second, grad_norm);
                    break;
                }
            }
    }

    //STOP_BENCH(interpret_color)
    //
    //fprintf(stderr, "Topographer__interpret done in %.2lf\n", GET_BENCH(interpret_color));
}

DisTile* Topographer__interpret(GeoTile* geo_tile, int is_color) {

    std::vector< std::pair<DisTileSample, geo_sample_t> > steps;



    if(is_color == 0) {
        steps = {
            std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32768),
            std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32767),
            std::make_pair((DisTileSample){ 000, 000, 000, 255 },      0),
            std::make_pair((DisTileSample){ 255, 255, 255, 255 },   2000)
        };
        //return Topographer__interpret_grayscale(geo_tile);
    } else if(is_color == 1) {
        steps = {
            std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32768),
            std::make_pair((DisTileSample){ 000, 000, 000, 255 }, -32767),
            std::make_pair((DisTileSample){ 000, 000, 255, 255 },      0),
            std::make_pair((DisTileSample){ 000, 255, 000, 255 },      0),
            std::make_pair((DisTileSample){ 255, 255, 000, 255 },    300),
            std::make_pair((DisTileSample){ 255, 000, 000, 255 },   2000),
            std::make_pair((DisTileSample){ 000, 000, 000, 255 },   8000)
        };
        //return Topographer__interpret_color(geo_tile);
    }

    return Topographer__interpret_param(geo_tile, { 000, 000, 000, 255 }, { 000, 000, 000, 255 }, steps);

    return NULL;
}
