//
// Created by Gabriel Mitterrutzner on 28.04.17.
//

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>

int main(int argc, char ** argv)
{
    bool quit = false;
    SDL_Event event;

    SDL_Init(SDL_INIT_VIDEO);
    IMG_Init(IMG_INIT_PNG);

    SDL_Window * window = SDL_CreateWindow("SDL2 Sprite Sheets",
                                           SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
    SDL_Renderer * renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Surface * image = IMG_Load("spritesheet.png");
    SDL_Texture * texture = SDL_CreateTextureFromSurface(renderer, image);

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);

    int count = -1;
    int before = 5;

    while (!quit)
    {
        Uint32 ticks = SDL_GetTicks();
        Uint32 sprite = (ticks / 30) % 6; //Calculates the column

        if (sprite == 0 && before == 5){ //Calculates the row
            count = (count + 1) % 5;
        }
        before = sprite;

        SDL_Rect srcrect = { sprite * 240, count * 296, 240, 296 };
        SDL_Rect dstrect = { 10, 10, 240, 296 };

        while (SDL_PollEvent(&event) != NULL)
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    if (event.key.keysym.mod == 'q'){
                        quit = true;
                    }
                case SDL_QUIT:
                    quit = true;
                    break;
            }
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, &srcrect, &dstrect);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(image);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();

    return 0;
}