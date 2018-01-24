/**
 * @file topographer.cpp
 * @author JMendyk
 * @date 07.01.18
 */

#include <cstdio>

#include "topographer.h"

#include "utils.h"
#include "real_tile/real_tile.h"

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
                            const std::vector<RealTileSample> &steps) {
    //START_BENCH(interpret_color)

    for(size_t it = 0; it <= (real_tile->height * real_tile->width); it++) {
        if(real_tile->heights[it] < steps.begin()->height) {
            real_tile->coloring[it] = lower;
        } else if((steps.end()-1)->height < real_tile->heights[it]) {
            real_tile->coloring[it] = upper;
        } else for(auto step = steps.begin(); step != steps.end(); step++) {
                if(real_tile->heights[it] <= step->height) {
                    float grad_norm = ((float)(real_tile->heights[it] - (step-1)->height)/(step->height - (step-1)->height));
                    real_tile->coloring[it] = sample_gradient((step-1)->coloring, step->coloring, grad_norm);
                    break;
                }
            }
    }

    //STOP_BENCH(interpret_color)
    //
    //fprintf(stderr, "Topographer__interpret done in %.2lf\n", GET_BENCH(interpret_color));
}
