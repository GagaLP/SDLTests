//
// Created by Gabriel Mitterrutzner on 2019-02-16.
//

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

void draw_heart(SDL_Point* test, int size, int max) {
    //printf("%d\n", (size / max));
    if ((size / max) % 2) {
        for (int j = size % max + 1; j < max; ++j) {
            SDL_RenderDrawLine(Game.screen.renderer, test[j].x, test[j].y, test[(j + 1) % max].x, test[(j + 1) % max].y);
        }
    } else {
        for (int j = 0; j < size % max + 1; ++j) {
            SDL_RenderDrawLine(Game.screen.renderer, test[j].x, test[j].y, test[(j + 1) % max].x, test[(j + 1) % max].y);
        }
    }
}

SDL_Point* get_circle_points(int size) {
    SDL_Point* test = malloc(sizeof(SDL_Point) * size);
    double degree = (M_PI * 2) / (double)size;
    for (int i = 0; i < size; ++i) {
        test[i].x = (int)map_d(16 * pow(sin(degree * i), 3) , -16, 16, 10, Game.screen.w - 10);
        test[i].y = (int)map_d(13 * cos(degree * i) - 5 * cos(2 * degree * i) - 2 * cos(3 * degree * i) - cos(4 * degree * i), -17, 12, Game.screen.h - 10, 10);
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
    int num = 0;
    while (Game.running) {
        check_event();

        SDL_RenderClear(Game.screen.renderer);
        set_color(num % 330);
        draw_heart(point, num, SIZE);
        SDL_SetRenderDrawColor(Game.screen.renderer, 0, 0, 0, 255);
        SDL_RenderPresent(Game.screen.renderer);
        SDL_Delay(16);

        num = (num + 1);
    }

    free_data(point);

    Game.quit();

    return EXIT_SUCCESS;
}
