/**
 * @file catchmenter.cpp
 * @author JMendyk
 * @date 19.01.18
 */

#include <utility>
#include <queue>
#include <dis_tile/dis_tile.h>
#include <cstdio>
#include <utils.h>

#include "catchmenter.h"

#define CORD(x, y) (y * dis_tile->tile->width + x)
#define FROM_CORD_X(pos) (pos % dis_tile->tile->width)
#define FROM_CORD_Y(pos) (pos / dis_tile->tile->width)

DisTile* Catchmenter__color_pixel(GeoTile* geo_tile, DisTile* dis_tile, int x, int y) {
    START_BENCH(catchmenter)

    std::queue<std::pair<int, int>> q;

    q.push(std::make_pair(x, y));

    for(size_t it = 0; it <= (dis_tile->tile->height * dis_tile->tile->width); it ++) {
        int px = (int)FROM_CORD_X(it);
        int py = (int)FROM_CORD_Y(it);

        if((x-px)*(x-px) + (y-py)*(y-py) <= 128) {
            dis_tile->data[it] = {255, 0, 0, 255};
            q.push(std::make_pair(px, py));
        }
    }

    while(!q.empty()) {
        auto pos = q.front();
        q.pop();

        int dx[] = { -1, 0, 1, 0 };
        int dy[] = {  0,-1, 0, 1 };

        for(int i = 0; i < 4; i++) {
            int x = pos.first + dx[i];
            int y = pos.second + dy[i];

            if(x < 0 || x >= dis_tile->tile->width || y < 0 || y >= dis_tile->tile->height)
                continue;

            if(dis_tile->data[CORD(x, y)].red != 255 || dis_tile->data[CORD(x, y)].green != 255 || dis_tile->data[CORD(x, y)].blue != 255 || dis_tile->data[CORD(x, y)].alpha != 255) {
                if(geo_tile->data[CORD(x, y)] >= geo_tile->data[CORD(pos.first, pos.second)]) {
                    dis_tile->data[CORD(x, y)] = {255, 255, 255, 255};
                    q.push(std::make_pair(x, y));
                }
            }
        }

    }

    STOP_BENCH(catchmenter)

    fprintf(stderr, "Catchmented in %lf\n", GET_BENCH(catchmenter));

    //if(!DisTile__set_data(dis_tile, dis_data)) {
    //    free(dis_data);
    //    free(dis_tile);
    //    return NULL;
    //}

    //return dis_tile;
}
