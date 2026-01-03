#include <bits/stdc++.h>
#include <SDL2/SDL.h>
#include <unistd.h>

#define CELL_COUNT 200
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define GRID_W 200
#define GRID_H 150
#define FPS 60
#define RECORD_SECONDS 10

struct Cell {
    int state; // 0: off, 1: on, 2: dying
};

std::vector<std::vector<Cell>> grid(GRID_H, std::vector<Cell>(GRID_W, {0}));
std::vector<std::vector<Cell>> nextGrid = grid;
SDL_Rect cellRect = {0, 0, WINDOW_WIDTH / GRID_W + 1, WINDOW_HEIGHT / GRID_H + 1};

void SpawnCells(std::vector<std::vector<Cell>> &grid) {
    for (int i = 0; i < CELL_COUNT; ++i) {
        int y = rand() % grid.size();
        int x = rand() % grid[0].size();
        grid[y][x].state = 1;
    }
}

void resetGrid(std::vector<std::vector<Cell>> &grid) {
    for (int y = 0; y < GRID_H; ++y) {
        for (int x = 0; x < GRID_W; ++x) {
            grid[y][x].state = 0;
        }
    }
    SpawnCells(grid);
}

int countNeighbors(const std::vector<std::vector<Cell>> &grid, int y, int x) {
    int count = 0;
    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dy == 0 && dx == 0) continue;
            int ny = (y + dy + GRID_H) % GRID_H;
            int nx = (x + dx + GRID_W) % GRID_W;
            if (grid[ny][nx].state == 1) count++;
        }
    }
    return count;
}

void BriansBrain() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Random Walk Visualization",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SpawnCells(grid);

    bool running = true;

    while(running) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;

            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_r) {
                resetGrid(grid);
            }
        }

        for(int y=0;y<GRID_H;++y){
            for(int x=0;x<GRID_W;++x){
                nextGrid[y][x].state = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        for(int y=0;y<GRID_H;++y) {
            for(int x=0;x<GRID_W;++x) {
                int onNeighbors = countNeighbors(grid, y, x);
                if(grid[y][x].state==0&&onNeighbors==2)nextGrid[y][x].state=1;
                else if(grid[y][x].state==1)nextGrid[y][x].state=2;
                else if(grid[y][x].state==2)nextGrid[y][x].state=0;

                SDL_Rect cellRect = {x * (WINDOW_WIDTH / GRID_W), y * (WINDOW_HEIGHT / GRID_H), (WINDOW_WIDTH / GRID_W), (WINDOW_HEIGHT / GRID_H)};
                if(grid[y][x].state == 0) {
                    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
                } else if(grid[y][x].state == 1) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                } else if(grid[y][x].state == 2) {
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                }
                SDL_RenderFillRect(renderer, &cellRect);
            }
        }

        grid = nextGrid;

        SDL_RenderPresent(renderer);
        SDL_Delay(1);
    }
}

void BriansBrainFfmpeg() {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Random Walk Visualization",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SpawnCells(grid);

    std::vector<uint8_t> pixels(WINDOW_WIDTH * WINDOW_HEIGHT * 4);

    const int total_frames = FPS * RECORD_SECONDS;
    for (int frame = 0; frame < total_frames; ++frame) {
        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) return;
        }

        for(int y=0;y<GRID_H;++y){
            for(int x=0;x<GRID_W;++x){
                nextGrid[y][x].state = 0;
            }
        }

        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);

        for(int y=0;y<GRID_H;++y) {
            for(int x=0;x<GRID_W;++x) {
                int onNeighbors = countNeighbors(grid, y, x);
                if(grid[y][x].state==0&&onNeighbors==2)nextGrid[y][x].state=1;
                else if(grid[y][x].state==1)nextGrid[y][x].state=2;
                else if(grid[y][x].state==2)nextGrid[y][x].state=0;

                SDL_Rect cellRect = {x * (WINDOW_WIDTH / GRID_W), y * (WINDOW_HEIGHT / GRID_H), (WINDOW_WIDTH / GRID_W), (WINDOW_HEIGHT / GRID_H)};
                if(grid[y][x].state == 0) {
                    SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
                } else if(grid[y][x].state == 1) {
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                } else if(grid[y][x].state == 2) {
                    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
                }
                SDL_RenderFillRect(renderer, &cellRect);

            }
        }

        grid = nextGrid;
        SDL_RenderPresent(renderer);

        SDL_RenderReadPixels(
            renderer,
            nullptr,
            SDL_PIXELFORMAT_RGBA32,
            pixels.data(),
            WINDOW_WIDTH * 4
        );

        write(STDOUT_FILENO, pixels.data(), pixels.size());
        SDL_Delay(1000 / FPS);
    }
}

int main(int argc, char** argv) {
    if (argc > 1 && std::string(argv[1]) == "ffmpeg") {
        BriansBrainFfmpeg();
    } else {
        BriansBrain();
    }
    return 0;
}
