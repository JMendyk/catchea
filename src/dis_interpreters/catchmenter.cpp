/**
 * @file catchmenter.cpp
 * @author JMendyk
 * @date 19.01.18
 */

#include <utility>
#include <queue>
#include <cstdio>
#include <utils.h>
#include <cstdlib>

#include "catchmenter.h"

#define CORD(x, y, w) (y * w + x)
#define TCORD(x, y) CORD(x, y, tile->width)
#define FROM_CORD_X(pos, w) (pos % w)
#define FROM_CORD_Y(pos, w) (pos / w)

bool is_local_minimum(const RealTile* tile, const int& pos_x, const int& pos_y, const Kernel& kernel) {

    for(int i = 0; i < kernel.count; i++) {
        int x = pos_x + kernel.dx[i];
        int y = pos_y + kernel.dy[i];

        if(x < 0 || x >= tile->width || y < 0 || y >= tile->height)
            continue;

        if(tile->heights[TCORD(x, y)] < tile->heights[TCORD(pos_x, pos_y)]
           || (kernel.hard_min && tile->heights[TCORD(x, y)] == tile->heights[TCORD(pos_x, pos_y)]))
            return false;
    }

    return true;
}

Kernel K4 = {
    4,
    false,
    { -1, 0, 1, 0 },
    {  0,-1, 0, 1 }
};

Kernel K4_HARD_MIN = {
    4,
    true,
    { -1, 0, 1, 0 },
    {  0,-1, 0, 1 }
};

Kernel K8 = {
    8,
    false,
    { -1,  0,  1, 1, 1, 0, -1, -1 },
    { -1, -1, -1, 0, 1, 1,  1,  0 }
};

Kernel K8_HARD_MIN = {
    8,
    true,
    { -1,  0,  1, 1, 1, 0, -1, -1 },
    { -1, -1, -1, 0, 1, 1,  1,  0 }
};

void Catchmenter__color_pixel(RealTile* tile, const int& x, const int& y, const Kernel& kernel) {
    START_BENCH(catchmenter)

    std::queue<std::pair<std::pair<int, int>, RealTile::Coloring>> q;

    RealTile::Coloring white = {255, 255, 255, 255};

    for(size_t it = 0; it <= (tile->height * tile->width); it ++) {
        int px = (int)FROM_CORD_X(it, tile->width);
        int py = (int)FROM_CORD_Y(it, tile->width);
        tile->coloring[TCORD(px, py)] = white;
    }

    RealTile::Coloring my_color = RealTile__random_color();
    q.push(std::make_pair(std::make_pair(x, y), my_color));

    RealTile::Coloring same_color = {
            (unsigned char) (my_color.red * 0.5f),
            (unsigned char) (my_color.green * 0.5f),
            (unsigned char) (my_color.blue * 0.5f),
            255
    };
    tile->coloring[TCORD(x, y)] = same_color;

    //fprintf(stderr, "local minima: %lu\n", q.size());

    while(!q.empty()) {
        auto xpos = q.front();
        auto pos = xpos.first;
        auto color = xpos.second;
        q.pop();

        for(int i = 0; i < kernel.count; i++) {
            int nx = pos.first + kernel.dx[i];
            int ny = pos.second + kernel.dy[i];

            if(nx < 0 || nx >= tile->width || ny < 0 || ny >= tile->height)
                continue;

            if(tile->coloring[CORD(nx, ny, tile->width)] == white) {
                if(tile->heights[CORD(nx, ny, tile->width)] >= tile->heights[CORD(pos.first, pos.second, tile->width)]) {
                    tile->coloring[CORD(nx, ny, tile->width)] = color;
                    q.push(std::make_pair(std::make_pair(nx, ny), color));
                }
            }
        }

    }

    STOP_BENCH(catchmenter)

    fprintf(stderr, "Catchmented in %lf\n", GET_BENCH(catchmenter));
}

void Catchmenter__from(RealTile* tile, bool* visi_matrix, const int& from_x, const int& from_y, const Kernel& kernel) {
    //START_BENCH(catchmenter)
    //
    //fprintf(stderr, "Kernel %d with%s HM\n", kernel.count, kernel.hard_min ? "": "out");

    std::queue<std::pair<std::pair<int, int>, RealTile::Coloring>> q;

    RealTile::Coloring my_color = RealTile__random_color();
    q.push(std::make_pair(std::make_pair(from_x, from_y), my_color));
    visi_matrix[CORD(from_x, from_y, tile->width)] = true;

    RealTile::Coloring same_color = {
            (unsigned char) (my_color.red * 0.5f),
            (unsigned char) (my_color.green * 0.5f),
            (unsigned char) (my_color.blue * 0.5f),
            255
    };

    tile->coloring[CORD(from_x, from_y, tile->width)] = same_color;

    while(!q.empty()) {
        auto xpos = q.front();
        q.pop();
        auto pos = xpos.first;
        auto color = xpos.second;

        for(int i = 0; i < kernel.count; i++) {
            int nx = pos.first + kernel.dx[i];
            int ny = pos.second + kernel.dy[i];

            if(nx < 0 || nx >= tile->width || ny < 0 || ny >= tile->height)
                continue;

            if(!visi_matrix[CORD(nx, ny, tile->width)]) {
                if(tile->heights[CORD(nx, ny, tile->width)] >= tile->heights[CORD(pos.first, pos.second, tile->width)]) {
                    visi_matrix[CORD(nx, ny, tile->width)] = true;
                    tile->coloring[CORD(nx, ny, tile->width)] = color;
                    q.push(std::make_pair(std::make_pair(nx, ny), color));
                }
            }
        }
    }

    //STOP_BENCH(catchmenter)
    //
    //fprintf(stderr, "Catchmented from (%d, %d) in %lf\n", from_x, from_y, GET_BENCH(catchmenter));
}

void Catchmenter__all(RealTile* tile, const Kernel& kernel) {
    //START_BENCH(catchmenter)


    static bool* visi_matrix = NULL;
    static size_t it = 0;

    if(visi_matrix == NULL) {
        visi_matrix = (bool*) calloc(tile->height * tile->width, sizeof(bool));
        it = 0;
    }

    int minimums_left = tile->height * tile->width;//50000;

    for(; it < (tile->height * tile->width); it++) {
        int px = (int)FROM_CORD_X(it, tile->width);
        int py = (int)FROM_CORD_Y(it, tile->width);

        if(is_local_minimum(tile, px, py, kernel)) {
            //if(visi_matrix[TCORD(px, py)]) {
            //    fprintf(stderr, "oppps... %d %d\n", px, py);
            //}
            Catchmenter__from(tile, visi_matrix, px, py, kernel);
            minimums_left--;

            if(minimums_left == 0) {
                it++;
                break;
            }
        }
    }

    if(it == tile->height * tile->width) {
        free(visi_matrix);
        visi_matrix = NULL;
    }

    //STOP_BENCH(catchmenter)
    //
    //fprintf(stderr, "Catchmented in %lf\n", GET_BENCH(catchmenter));
}

void Catchmenter__color_all(RealTile* tile, const Kernel& kernel) {
    START_BENCH(catchmenter)

    std::queue<std::pair<std::pair<int, int>, RealTile::Coloring>> q;

    RealTile::Coloring white = {255, 255, 255, 255};

    for(size_t it = 0; it <= (tile->height * tile->width); it ++) {
        int px = (int)FROM_CORD_X(it, tile->width);
        int py = (int)FROM_CORD_Y(it, tile->width);

        tile->coloring[TCORD(px, py)] = white;

        if(is_local_minimum(tile, px, py, kernel)) {
            RealTile::Coloring my_color = RealTile__random_color();
            q.push(std::make_pair(std::make_pair(px, py), my_color));

            RealTile::Coloring same_color = {
                    (unsigned char) (my_color.red * 0.5f),
                    (unsigned char) (my_color.green * 0.5f),
                    (unsigned char) (my_color.blue * 0.5f),
                    255
            };
            tile->coloring[TCORD(px, py)] = same_color;
        }
    }

    //fprintf(stderr, "local minima: %lu\n", q.size());

    while(!q.empty()) {
        auto xpos = q.front();
        auto pos = xpos.first;
        auto color = xpos.second;
        q.pop();

        for(int i = 0; i < kernel.count; i++) {
            int nx = pos.first + kernel.dx[i];
            int ny = pos.second + kernel.dy[i];

            if(nx < 0 || nx >= tile->width || ny < 0 || ny >= tile->height)
                continue;

            if(tile->coloring[CORD(nx, ny, tile->width)] == white) {
                if(tile->heights[CORD(nx, ny, tile->width)] >= tile->heights[CORD(pos.first, pos.second, tile->width)]) {
                    tile->coloring[CORD(nx, ny, tile->width)] = color;
                    q.push(std::make_pair(std::make_pair(nx, ny), color));
                }
            }
        }

    }

    STOP_BENCH(catchmenter)

    fprintf(stderr, "Catchmented in %lf\n", GET_BENCH(catchmenter));
}

void Catchmenter__color_all_immediate(RealTile* tile, const Kernel& kernel) {
    START_BENCH(catchmenter)

    std::queue<std::pair<std::pair<int, int>, RealTile::Coloring>> q;

    RealTile::Coloring white = {255, 255, 255, 255};

    for(size_t it = 0; it <= (tile->height * tile->width); it ++) {
        int px = (int)FROM_CORD_X(it, tile->width);
        int py = (int)FROM_CORD_Y(it, tile->width);

        tile->coloring[TCORD(px, py)] = white;
    }

    for(size_t it = 0; it <= (tile->height * tile->width); it ++) {
        int px = (int)FROM_CORD_X(it, tile->width);
        int py = (int)FROM_CORD_Y(it, tile->width);

        if(is_local_minimum(tile, px, py, kernel)) {
            RealTile::Coloring my_color = RealTile__random_color();
            q.push(std::make_pair(std::make_pair(px, py), my_color));

            RealTile::Coloring same_color = {
                    (unsigned char) (my_color.red * 0.5f),
                    (unsigned char) (my_color.green * 0.5f),
                    (unsigned char) (my_color.blue * 0.5f),
                    255
            };
            tile->coloring[TCORD(px, py)] = same_color;

            //fprintf(stderr, "local minimum (%d, %d)\n", px, py);

            while(!q.empty()) {
                auto xpos = q.front();
                auto pos = xpos.first;
                auto color = xpos.second;
                q.pop();

                for(int i = 0; i < kernel.count; i++) {
                    int nx = pos.first + kernel.dx[i];
                    int ny = pos.second + kernel.dy[i];

                    if(nx < 0 || nx >= tile->width || ny < 0 || ny >= tile->height)
                        continue;

                    if(tile->coloring[CORD(nx, ny, tile->width)] == white) {
                        if(tile->heights[CORD(nx, ny, tile->width)] >= tile->heights[CORD(pos.first, pos.second, tile->width)]) {
                            tile->coloring[CORD(nx, ny, tile->width)] = color;
                            q.push(std::make_pair(std::make_pair(nx, ny), color));
                        }
                    }
                }

            }
        }
    }

    STOP_BENCH(catchmenter)

    fprintf(stderr, "Catchmented in %lf\n", GET_BENCH(catchmenter));
}

void Catchmenter__color_all_immediate_heightwise(RealTile* tile, const Kernel& kernel) {
    START_BENCH(catchmenter)

    struct Elem {
        signed short height;
        std::pair<int, int> pos;
        RealTile::Coloring coloring;

        bool operator<(const Elem& other) const {
            return height > other.height;
        }
    };

    std::priority_queue<Elem> local_minimum_q;

    RealTile::Coloring white = {255, 255, 255, 255};

    for(size_t it = 0; it <= (tile->height * tile->width); it ++) {
        int px = (int)FROM_CORD_X(it, tile->width);
        int py = (int)FROM_CORD_Y(it, tile->width);

        tile->coloring[TCORD(px, py)] = white;
    }

    for(size_t it = 0; it <= (tile->height * tile->width); it ++) {
        int px = (int)FROM_CORD_X(it, tile->width);
        int py = (int)FROM_CORD_Y(it, tile->width);

        if(is_local_minimum(tile, px, py, kernel)) {
            RealTile::Coloring my_color = RealTile__random_color();
            local_minimum_q.push((Elem){ tile->heights[TCORD(px, py)], std::make_pair(px, py), my_color });

            RealTile::Coloring same_color = {
                    (unsigned char) (my_color.red * 0.5f),
                    (unsigned char) (my_color.green * 0.5f),
                    (unsigned char) (my_color.blue * 0.5f),
                    255
            };
            tile->coloring[TCORD(px, py)] = same_color;

            //fprintf(stderr, "local minimum (%d, %d)\n", px, py);
        }
    }

    while(!local_minimum_q.empty()) {
        auto starter = local_minimum_q.top();
        local_minimum_q.pop();

        RealTile::Coloring same_color = {
                (unsigned char) (starter.coloring.red * 0.5f),
                (unsigned char) (starter.coloring.green * 0.5f),
                (unsigned char) (starter.coloring.blue * 0.5f),
                255
        };
        if(tile->coloring[CORD(starter.pos.first, starter.pos.second, tile->width)] != same_color) {
            // already visited... but why?
            fprintf(stderr, "why???\n");
            continue;
        }

        std::queue<std::pair<std::pair<int, int>, RealTile::Coloring>> q;
        q.push(std::make_pair(starter.pos, starter.coloring));

        while(!q.empty()) {
            auto xpos = q.front();
            auto pos = xpos.first;
            auto color = xpos.second;
            q.pop();

            for(int i = 0; i < kernel.count; i++) {
                int nx = pos.first + kernel.dx[i];
                int ny = pos.second + kernel.dy[i];

                if(nx < 0 || nx >= tile->width || ny < 0 || ny >= tile->height)
                    continue;

                if(tile->coloring[CORD(nx, ny, tile->width)] == white) {
                    if(tile->heights[CORD(nx, ny, tile->width)] >= tile->heights[CORD(pos.first, pos.second, tile->width)]) {
                        tile->coloring[CORD(nx, ny, tile->width)] = color;
                        q.push(std::make_pair(std::make_pair(nx, ny), color));
                    }
                }
            }
        }
    }

    STOP_BENCH(catchmenter)

    fprintf(stderr, "Catchmented in %lf\n", GET_BENCH(catchmenter));
}
