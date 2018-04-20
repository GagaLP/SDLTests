//
// Created by Gabriel Mitterrutzner on 30.04.17.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <SDL2/SDL.h>
#include <stdbool.h>
//


#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/param.h>

int main(int argc, char *argv[])
{
    int status = 0;

    SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "0");
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
        status = 1;
        SDL_Quit();
        return status;
    }

    SDL_Window *window = SDL_CreateWindow("SDL Test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          400,
                                          400,
                                          (SDL_WINDOW_SHOWN |
                                           SDL_WINDOW_ALLOW_HIGHDPI));
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        status = 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        status = 1;
        SDL_Quit();
        return status;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, 400, 400);


    bool done = false;
    do {
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_KEYDOWN:
                    if (e.key.keysym.sym == 'f') {
                        uint32_t flags = SDL_GetWindowFlags(window);
                        flags ^= SDL_WINDOW_FULLSCREEN_DESKTOP;
                        if (!SDL_SetWindowFullscreen(window, flags)) {
                            //fprintf(stderr, "Toggle window mode failed: %s\n", SDL_GetError());
                        }
                    } else if (e.key.keysym.sym == 'm') {
                        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION,
                                                 "This is a message box",
                                                 "Forfee I got off the freeway!",
                                                 window);
                    } else if (e.key.keysym.sym == 'q') {
                        done = true;
                    }
                    break;

                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_TEXTINPUT:

                    break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderPresent(renderer);
    } while (!done);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return status;
}


