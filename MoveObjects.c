//
// Created by Gabriel Mitterrutzner on 28.04.17.
//

#include <SDL2/SDL.h>
#include <stdbool.h>

int main(int argc, char ** argv){
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Rect playerPos;

    int width = 500;
    int height = 500;
    int step = 5;

    playerPos.h = 20;
    playerPos.w = 20;
    playerPos.x = 20;
    playerPos.y = 20;

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        printf("Error %s", SDL_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Move objects width arrow keys", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL){
        printf("Error %s", SDL_GetError());
        return -2;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL){
        printf("Error %s", SDL_GetError());
        return -3;
    }

    if (SDL_RenderSetLogicalSize(renderer, width, height) == -1){
        printf("Error %s", SDL_GetError());
        return -4;
    }

    if (SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255) == -1){
        printf("Error %s", SDL_GetError());
        return -5;
    }

    if (SDL_RenderClear(renderer) == -1){
        printf("Error %s", SDL_GetError());
        return -6;
    }

    bool loop = true;

    while ( loop )
    {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) )
        {
            if ( event.type == SDL_QUIT )
                loop = false;
            else if ( event.type == SDL_KEYDOWN )
            {
                switch ( event.key.keysym.sym )
                {
                    case 'q':
                        loop = false;
                        break;
                    case SDLK_RIGHT:
                        playerPos.x += step;
                        break;
                    case SDLK_LEFT:
                        playerPos.x -= step;
                        break;
                        // Remeber 0,0 in SDL is left-top. So when the user pressus down, the y need to increase
                    case SDLK_DOWN:
                        playerPos.y += step;
                        break;
                    case SDLK_UP:
                        playerPos.y -= step;
                        break;
                    default :
                        break;
                }
            }
        }

        SDL_RenderClear(renderer);

        // Change color to yellow
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255 );

        // Render our "player"
        SDL_RenderFillRect(renderer, &playerPos );

        SDL_RenderDrawLine(renderer, 0, 0, 500, 500);

        SDL_RenderDrawLine(renderer, 500, 0, 0, 500);

        // Change color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );

        // Render the changes above
        SDL_RenderPresent(renderer);

        // Add a 16msec delay to make our game run at ~60 fps
//        SDL_Delay( 16 );
    }




    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}