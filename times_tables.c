//
// Created by Gabriel Mitterrutzner on 2019-02-15.
//

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdlib.h>

#define SCREEN_W 500
#define SCREEN_H 500
#define SCREEN_SCALE 1
#define SCREEN_NAME "Times Table"

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

#define SIZE 100

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

void check_event(void) {
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
}

void draw_times_table(SDL_Point* test, int size, double times) {
    for (int j = 0; j < size; ++j) {
        SDL_RenderDrawLine(Game.screen.renderer, test[j].x, test[j].y, test[(int)(j * times) % size].x, test[(int)(j * times) % size].y);
    }
}

SDL_Point* get_circle_points(int size) {
    SDL_Point* test = malloc(sizeof(SDL_Point) * size);
    double degree = (M_PI * 2) / (double)size;
    for (int i = 0; i < size; ++i) {
        test[i].x = (int)map_d(cos(degree * i), -1, 1, 10, Game.screen.w - 10);
        test[i].y = (int)map_d(sin(degree * i), -1, 1, 10, Game.screen.h - 10);
    }
    return test;
}

void free_data(SDL_Point* points) {
    free(points);
}

double mod_d(double num, double mod) {
    return num - (mod * (int)(num / mod));
}

int main(void) {
    Game.init();

    SDL_Point* point = get_circle_points(SIZE);
//    uint32_t last_time = SDL_GetTicks();
//    uint32_t cur_time;
    double num = 0;
    while (Game.running) {
        //printf("%lf\n", mod_d(num, 2));
        check_event();

        SDL_RenderClear(Game.screen.renderer);
        set_color((int)map_d(mod_d(num, 3), 0, 2, 255, 0));
        draw_times_table(point, SIZE, num);
        SDL_SetRenderDrawColor(Game.screen.renderer, 0, 0, 0, 255);
        SDL_RenderPresent(Game.screen.renderer);
        SDL_Delay(16);

        if (num < 2) {
            num = (num + 0.01);
        }
    }

    free_data(point);

    Game.quit();

    return EXIT_SUCCESS;
}
