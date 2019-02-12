//
// Created by Gabriel Mitterrutzner on 2019-02-12.
//


#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>

#define SCREEN_W 640
#define SCREEN_H 380
#define SCREEN_SCALE 1
#define SCREEN_NAME "Fractal tree"

void game_init(void);

void game_quit(void);

struct {
    SDL_bool running;
    struct {
        unsigned int w;
        unsigned int h;
        const char *name;
        SDL_Window *window;
        SDL_Renderer *renderer;
    } screen;

    void (*init)(void);

    void (*quit)(void);
} Game = {
        SDL_FALSE,
        {
                SCREEN_SCALE * SCREEN_W,
                SCREEN_SCALE * SCREEN_H,
                SCREEN_NAME,
                NULL,
                NULL
        },
        game_init,
        game_quit
};

#define MAX_ITERATION 50

void game_init(void) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        printf("Error %s", SDL_GetError());
        exit(-1);
    }

    Game.screen.window = SDL_CreateWindow(Game.screen.name, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          Game.screen.w,
                                          Game.screen.h, SDL_WINDOW_SHOWN);
    if (Game.screen.window == NULL) {
        printf("Error %s", SDL_GetError());
        exit(-2);
    }

    Game.screen.renderer = SDL_CreateRenderer(Game.screen.window, -1, SDL_RENDERER_ACCELERATED);

    if (Game.screen.renderer == NULL) {
        printf("Error %s", SDL_GetError());
        SDL_DestroyWindow(Game.screen.window);
        exit(-3);
    }

    Game.running = SDL_TRUE;
}

void game_quit(void) {
    SDL_DestroyRenderer(Game.screen.renderer);
    SDL_DestroyWindow(Game.screen.window);

    Game.screen.renderer = NULL;
    Game.screen.window = NULL;

    SDL_Quit();
}

void hsv_to_rgb(float *r, float *g, float *b, float h, float s, float v) {
    int i;
    float f, p, q, t;
    if (s == 0) {
        *r = *g = *b = v;
        return;
    }
    h /= 60;
    i = (int) floorf(h);
    f = h - i;
    p = v * (1 - s);
    q = v * (1 - s * f);
    t = v * (1 - s * (1 - f));
    switch (i) {
        case 0:
            *r = v * 255;
            *g = t * 255;
            *b = p * 255;
            break;
        case 1:
            *r = q * 255;
            *g = v * 255;
            *b = p * 255;
            break;
        case 2:
            *r = p * 255;
            *g = v * 255;
            *b = t * 255;
            break;
        case 3:
            *r = p * 255;
            *g = q * 255;
            *b = v * 255;
            break;
        case 4:
            *r = t * 255;
            *g = p * 255;
            *b = v * 255;
            break;
        default:
            *r = v * 255;
            *g = p * 255;
            *b = q * 255;
            break;
    }
}

double deg_to_rad(double angle) {
    return angle * (M_PI / 180);
}

double map_d(double x, double in_min, double in_max, double out_min, double out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

int map(int x, int in_min, int in_max, int out_min, int out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void set_color(int color) {
    float r, g, b;
    hsv_to_rgb(&r, &g, &b, color, 1, 1);
    SDL_SetRenderDrawColor(Game.screen.renderer, (uint8_t) floorf(r), (uint8_t) floorf(g), (uint8_t) floorf(b), 255);
}

void mandelbroat(void){

    double x;
    double y;
    double x_temp;
    int iteration;
    double x_new;
    double  y_new;
    int color;

    for (int i = 0; i < Game.screen.w; ++i) {
        for (int j = 0; j < Game.screen.h; ++j) {
            x_new = map_d(i, 0, Game.screen.w, -2.5, 1);
            y_new = map_d(j, 0, Game.screen.h, -1, 1);

            x = 0.0;
            y = 0.0;
            iteration = 0;

            while (x * x + y * y <= 2 * 2 && iteration < MAX_ITERATION) {
                x_temp = x * x - y * y + x_new;
                y = 2 * x * y + y_new;
                x = x_temp;
                iteration++;
            }

            color = map(iteration, 0, MAX_ITERATION, 0, 255);
            set_color(color);
            SDL_RenderDrawPoint(Game.screen.renderer, i, j);
        }
    }

}

int main(void) {
    Game.init();

    while (Game.running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                Game.running = SDL_FALSE;
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case 'q':
                        Game.running = SDL_FALSE;
                        break;
                    default :
                        break;
                }
            }
        }
        SDL_RenderClear(Game.screen.renderer);
        mandelbroat();
        SDL_SetRenderDrawColor(Game.screen.renderer, 0, 0, 0, 255);
        SDL_RenderPresent(Game.screen.renderer);
        SDL_Delay(16);
    }

    Game.quit();

    return EXIT_SUCCESS;
}
