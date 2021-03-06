#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_w 640
#define SCREEN_H 480
#define SCREEN_SCALE 1
#define SCREEN_NAME "Test"


void game_quit(void);
void game_init(void);

struct {
    // define "attributes"
    SDL_bool running;
    struct {
        unsigned int w;
        unsigned int h;
        const char* name;
        SDL_Window* window;
        SDL_Renderer* renderer;
    } screen;

    // define "methods"
    void (*init)(void);
    void (*quit)(void);
} Game = {
        SDL_FALSE,
        {
                SCREEN_SCALE * SCREEN_w,
                SCREEN_SCALE * SCREEN_H,
                SCREEN_NAME,
                NULL,
                NULL
        },
        game_init,
        game_quit
};

void game_init(void) {
    if(SDL_Init(SDL_INIT_EVERYTHING)!=0) {
        printf("SDL error -> %sn", SDL_GetError());
        exit(1);
    }

    unsigned int w = Game.screen.w;
    unsigned int h = Game.screen.h;
    const char* name = Game.screen.name;

    Game.screen.window = SDL_CreateWindow(
            name,
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            w, h, 0
    );
    Game.screen.renderer = SDL_CreateRenderer(
            Game.screen.window, -1,
            SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC
    );

    Game.running = SDL_TRUE;
}


void game_quit(void) {
    SDL_DestroyRenderer(Game.screen.renderer);
    SDL_DestroyWindow(Game.screen.window);
    Game.screen.window = NULL;
    Game.screen.renderer = NULL;

    SDL_Quit();
}

int main(int argc, char* argv[]) {

    Game.init();

    SDL_Event event;
    while(Game.running) {
        while(SDL_PollEvent(&event)) {
            switch(event.type) {
                case SDL_KEYDOWN:
                    if (event.key.keysym.sym == 'q'){
                        Game.running = SDL_FALSE;
                    }
                    break;
                case SDL_QUIT: {
                    Game.running = SDL_FALSE;
                } break;
                default:break;
            }
        }

        SDL_RenderClear(Game.screen.renderer);
        SDL_RenderPresent(Game.screen.renderer);
    }

    Game.quit();

    return 0;
}