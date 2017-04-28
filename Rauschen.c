//
// Created by Gabriel Mitterrutzner on 28.04.17.
//

#include <SDL2/SDL.h>
#include <time.h>

static SDL_Surface* init_sdl_surface() {
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    return SDL_CreateRGBSurface(0, 64, 32, 32, rmask, gmask, bmask, amask);
}

int main(int argc, char** argv) {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Surface *surface;
    SDL_Texture* texture;
    SDL_Event event;
    SDL_bool is_active = SDL_TRUE;
    int last_delta = SDL_GetTicks();
    Uint32 pixels[2048];
    srand((unsigned int) time(NULL));
    for (int i = 0; i < 2048; i++) {
        pixels[i] = (rand() & 0x01 ? -1 : 0);
    }

    // Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("Rauschen", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 320, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    surface = init_sdl_surface();
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, 64, 32);

    while (is_active) {
        while (SDL_WaitEventTimeout(&event, 10)) {
            switch (event.type) {
                case SDL_QUIT:
                    is_active = SDL_FALSE;
                    break;
            }
        }

        if (SDL_GetTicks() - last_delta > (1000 / 60)) {
            SDL_LockTexture(texture, NULL, &surface->pixels, &surface->pitch);
            memcpy(surface->pixels, pixels, 2048 * sizeof(Uint32));
            SDL_UnlockTexture(texture);
            for (int i = 0; i < 2048; i++) {
                pixels[i] = (rand() & 0x01 ? -1 : 0);
            }
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
            last_delta = SDL_GetTicks();
        }
    }

    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
