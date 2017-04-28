#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//#include <SDL2/SDL.h>
#include <stdbool.h>
//


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
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
        goto lib_uninit;
    }

    SDL_Window *window = SDL_CreateWindow("SDL Test",
                                          SDL_WINDOWPOS_UNDEFINED,
                                          SDL_WINDOWPOS_UNDEFINED,
                                          393,
                                          391,
                                          (SDL_WINDOW_SHOWN |
                                           SDL_WINDOW_ALLOW_HIGHDPI));
    if (!window) {
        fprintf(stderr, "SDL_CreateWindow failed: %s\n", SDL_GetError());
        status = 1;
        goto lib_uninit;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1,
                                                SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "SDL_CreateRenderer failed: %s\n", SDL_GetError());
        status = 1;
        goto lib_uninit;
    }
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, 393, 391);

    int flags = IMG_INIT_JPG | IMG_INIT_PNG;
    int initted = IMG_Init(flags);
    if ((initted & flags) != flags) {
        fprintf(stderr, "IMG_Init failed to init jpg and png support: %s\n",
                IMG_GetError());
        status = 1;
        goto lib_uninit;
    }

    char image_path[MAXPATHLEN];
    sprintf(image_path, "%s%s", SDL_GetBasePath(), "image.png");
    SDL_Surface *image_surface = IMG_Load(image_path);
    if (!image_surface) {
        fprintf(stderr, "IMG_Load failed: %s\n", IMG_GetError());
        status = 1;
        goto lib_uninit;
    }

    SDL_Texture *image_texture = SDL_CreateTextureFromSurface(renderer,
                                                              image_surface);
    SDL_FreeSurface(image_surface);
    if (!image_texture) {
        fprintf(stderr, "SDL_CreateTextureFromSurface failed: %s\n",
                SDL_GetError());
        status = 1;
        goto lib_uninit;
    }

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
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, image_texture, NULL, NULL);
        SDL_RenderPresent(renderer);
    } while (!done);

    SDL_DestroyTexture(image_texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    lib_uninit:
    IMG_Quit();
    SDL_Quit();

    return status;
}


