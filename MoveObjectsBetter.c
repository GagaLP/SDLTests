//
// Created by Gabriel Mitterrutzner on 19.04.18.
//

//
// Created by Gabriel Mitterrutzner on 28.04.17.
//

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdlib.h>

int width = 500;
int height = 500;
int step;
int playerXVel;
int playerYVel;
int const length = 4;
Uint32 sleep;
SDL_Rect playerPos;
SDL_Surface *surface[length];
SDL_Texture *texture[length];
SDL_Renderer *renderer;
int texW = 0;
int texH = 0;
TTF_Font *font;

void restart(){
    playerPos.h = 50;
    playerPos.w = 50;
    playerPos.x = (width - playerPos.h) / 2;
    playerPos.y = (width - playerPos.w) / 2;
    playerXVel = 0;
    playerYVel = 0;
    step = 1;
    sleep = 0;
}

void write(int position, char *text){
    SDL_Color color = { 255, 255, 255 };
    surface[position] = TTF_RenderText_Solid(font, text, color);
    texture[position] = SDL_CreateTextureFromSurface(renderer, surface[position]);

    SDL_QueryTexture(texture[position], NULL, NULL, &texW, &texH);
    SDL_Rect dstrect = { 2, position * texH + 2, texW, texH };
    SDL_RenderCopy(renderer, texture[position], NULL, &dstrect);
}

int main(int argc, char ** argv){
    char buffer[50];
    SDL_Window* window;

    restart();

    if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
        printf("Error %s", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0) {
        printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());

        exit(1);
    }

    font = TTF_OpenFont("arial.ttf", 15);

    window = SDL_CreateWindow("Move objects width arrow keys", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_ALLOW_HIGHDPI);
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
    bool debugInfo = false;

    while ( loop ) {
        SDL_Event event;
        while ( SDL_PollEvent( &event ) ) {
            switch ( event.type ) {
                case SDL_QUIT:
                    loop = false;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case 'q':
                            loop = false;
                            break;
                        case 'x':
                            restart();
                            break;
                        case 'd':
                        case SDLK_RIGHT:
                            playerXVel = step;
                            break;
                        case 'a':
                        case SDLK_LEFT:
                            playerXVel = -step;
                            break;
                            // Remeber 0,0 in SDL is left-top. So when the user pressus down, the y need to increase
                        case 's':
                        case SDLK_DOWN:
                            playerYVel = step;
                            break;
                        case 'w':
                        case SDLK_UP:
                            playerYVel = -step;
                            break;
                        case 'r':
                            step += 1;
                            break;
                        case 'f':
                            step -= 1;
                            break;
                        case 'e':
                            sleep = sleep == 0 ? 16 : 0;
                            break;
                        case 'y':
                            debugInfo = debugInfo == true ? false : true;
                            break;
                        default :
                            break;
                    }
                    break;
                case SDL_KEYUP:
                    switch (event.key.keysym.sym) {
                        case 'd':
                        case SDLK_RIGHT:
                            if( playerXVel > 0 )
                                playerXVel = 0;
                            break;
                        case 'a':
                        case SDLK_LEFT:
                            if( playerXVel < 0 )
                                playerXVel = 0;
                            break;
                            // Remeber 0,0 in SDL is left-top. So when the user pressus down, the y need to increase
                        case 's':
                        case SDLK_DOWN:
                            if( playerYVel > 0 )
                                playerYVel = 0;
                            break;
                        case 'w':
                        case SDLK_UP:
                            if( playerYVel < 0 )
                                playerYVel = 0;
                            break;
                        default :
                            break;
                    }
                default:
                    break;

            }
        }

        playerPos.x += playerXVel;
        playerPos.y += playerYVel;

        SDL_RenderClear(renderer);

        // Change color to yellow
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255 );

        // Render our "player"
        SDL_RenderFillRect(renderer, &playerPos);

//        SDL_RenderDrawLine(renderer, 0, 0, 500, 500);

//        SDL_RenderDrawLine(renderer, 500, 0, 0, 500);
        if (debugInfo){
            sprintf(buffer, "Playerpos x: %d y: %d", playerPos.x , playerPos.y);
            write(0, buffer);
            sprintf(buffer, "Velocity: %d", step);
            write(2, buffer);
            sprintf(buffer, "Sleep: %d", sleep);
            write(3, buffer);
            if (playerXVel < 0 && playerYVel < 0){
                sprintf(buffer, "Direction: left + up");
            } else if (playerXVel > 0 && playerYVel < 0){
                sprintf(buffer, "Direction: right + up");
            } else if (playerXVel < 0 && playerYVel > 0){
                sprintf(buffer, "Direction: left + down");
            } else if (playerXVel > 0 && playerYVel > 0){
                sprintf(buffer, "Direction: right + down");
            } else if (playerXVel == 0 && playerYVel > 0){
                sprintf(buffer, "Direction: down");
            } else if (playerXVel == 0 && playerYVel < 0){
                sprintf(buffer, "Direction: up");
            } else if (playerXVel < 0 && playerYVel == 0){
                sprintf(buffer, "Direction: left");
            } else if (playerXVel > 0 && playerYVel == 0){
                sprintf(buffer, "Direction: right");
            } else if (playerXVel == 0 && playerYVel == 0){
                sprintf(buffer, "Direction: none");
            }
            write(1, buffer);
        }

//        SDL_SetClipboardText("Gabriel");
        
        // Change color to black
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255 );

        // Render the changes above
        SDL_RenderPresent(renderer);

        // Add a 16msec delay to make our game run at ~60 fps
        SDL_Delay( sleep );
        if (debugInfo){
            for (int i = 0; i < length; ++i) {
                SDL_DestroyTexture(texture[i]);
                SDL_FreeSurface(surface[i]);
            }
        }
    }

//    for (int i = 0; i < 5; ++i) {
//        SDL_DestroyTexture(texture[i]);
//        SDL_FreeSurface(surface[i]);
//    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}