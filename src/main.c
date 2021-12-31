//#include <iostream>
#include <SDL.h>

#include "init_sdl.h"
#include "validation.h"
#include "trace_segment.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>



int main() {

    SDL_Window * window = NULL;
    SDL_Renderer * renderer = NULL;
    SDL_Surface * picture = NULL;
    SDL_Texture * texture = NULL;


    SDL_Event event;
    SDL_bool quit = SDL_FALSE;


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) {
        printf("ERROR SDL");
        SDL_Log("ERREUR 1 > %s\n", SDL_GetError());
        clean_resources(NULL, NULL, NULL);
        exit(EXIT_FAILURE);
    }


    window = SDL_CreateWindow("Reconnaissance de chiffre.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 468, 530, 0); //Creation de la fenetre
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); //creation du renderer sur la fenetre window
    picture = SDL_LoadBMP("./src/images/backg.bmp");
    texture = SDL_CreateTextureFromSurface(renderer, picture);

    test_sdl(window, renderer, picture, texture);

    

    //positionnement de la texture sur le renderer
    SDL_Rect dest_rect = {0, 0, 1000, 1089};
    if(SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0) {
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        clean_resources(window,renderer,NULL);
        exit(EXIT_FAILURE); 
    }
    if(SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0) {
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        clean_resources(window,renderer,NULL);
        exit(EXIT_FAILURE); 
    }


    SDL_LockSurface(picture); 

    int pass_sanitaire = 1;

    SDL_Point * point = malloc(sizeof(SDL_Point));
    SDL_Point * LastPoint = malloc(sizeof(SDL_Point));

    SDL_Point * A = malloc(sizeof(SDL_Point));
    SDL_Point * B = malloc(sizeof(SDL_Point));


    int one_click = 0;

    size_t i = 0;

    Uint8 * keyb;

    SDL_RenderPresent(renderer);

    while(!quit)
    {

        SDL_WaitEvent(&event);
        one_click = 0;
        keyb = SDL_GetKeyboardState(NULL);
        if(keyb[SDL_SCANCODE_ESCAPE])
            quit = SDL_TRUE;
        else if(event.type == SDL_QUIT)  
            quit = SDL_TRUE;
        else if(event.type == SDL_MOUSEMOTION) {

            if(event.button.button != SDL_BUTTON_LEFT) {
                LastPoint->x = event.motion.x;
                LastPoint->y = event.motion.y;
            }


            if((event.button.button == SDL_BUTTON_LEFT) && (event.motion.x < 336) ) {

                    SDL_Rect rect = {event.motion.x, event.motion.y, 10, 10};

                    point->x = event.motion.x;
                    point->y = event.motion.y;

                    if( (sqrt( (pow(point->x - LastPoint->x, 2)) + (pow(point->y - LastPoint->y, 2)) ) > 1  ) && ( pass_sanitaire != 1 ) ) { 
                        A->x = LastPoint->x;
                        A->y = LastPoint->y;
                        B->x = point->x;
                        B->y = point->y;
                        trace_line(A,B,renderer);
                        SDL_RenderPresent(renderer);

                        LastPoint->x = event.motion.x;
                        LastPoint->y = event.motion.y;

                    }

                    SDL_RenderFillRect(renderer, &rect); 
                    SDL_RenderPresent(renderer);
                    pass_sanitaire = 0;
                }

            }



            if(event.type == SDL_MOUSEBUTTONUP){
                  pass_sanitaire = 1;
                  
            }

            if((event.motion.x > 345) && (event.motion.x < 390) && (event.motion.y > 168) && (event.motion.y < 207) && (event.button.button == SDL_BUTTON_LEFT) ) {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_Delay(10);
            }

            if((event.motion.x > 415) && (event.motion.x < 455) && (event.motion.y > 168) && (event.motion.y < 207) && (event.button.button == SDL_BUTTON_LEFT) ) {
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                SDL_Delay(10);
            }

            if((event.motion.x > 350) && (event.motion.x < 460) && (event.motion.y > 320) && (event.motion.y < 345) && (event.button.button == SDL_BUTTON_LEFT) ) {

                SDL_WaitEventTimeout(&event, 2000); //permet de ne recuperer qu'un seul évènement

                SDL_Texture * t_renderer = SDL_GetRenderTarget(renderer);
                SDL_Surface * surface = SDL_CreateRGBSurface(0, 345, 530, 32, 0, 0, 0, 0); 
                validation_process(renderer, texture, t_renderer, surface); //fonction qui lance le processus d'enregistrement de l'image dans validation.c
                

            }


           }
    clean_resources(window, renderer, texture);
    return EXIT_SUCCESS;
}

