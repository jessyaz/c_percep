#include <SDL.h>

void clean_resources(SDL_Window *w, SDL_Renderer *r, SDL_Texture *t) {
    if(t != NULL) 
        SDL_DestroyTexture(t);
    if(r != NULL) 
        SDL_DestroyRenderer(r);
    if(w != NULL) 
        SDL_DestroyWindow(w);
    SDL_Quit();
}

void test_sdl(SDL_Window * window, SDL_Renderer * renderer, SDL_Texture * texture, SDL_Texture * picture) { //fonction de test de l'initialisation des différents composants de la SDL
    SDL_version nb_version;


    SDL_VERSION(&nb_version);
    printf("La version de la SDL est %d.%d.%d \n \n", nb_version.major, nb_version.minor, nb_version.patch);

    if(window == NULL) {
        SDL_Log("ERREUR 2 > %s\n", SDL_GetError());
        clean_resources(NULL,NULL,NULL);
        exit(EXIT_FAILURE);
    }
    if(renderer == NULL) {
        SDL_Log("ERREUR 3 > %s\n", SDL_GetError());
        clean_resources(window,NULL,NULL);
        exit(EXIT_FAILURE);
    }
    if(texture == NULL) {
        SDL_Log("ERREUR 4 > %s\n", SDL_GetError());
        clean_resources(window,renderer,NULL);
        exit(EXIT_FAILURE); 
    }
    if(picture == NULL) {
        SDL_Log("ERREUR 5 > %s\n", SDL_GetError());
        clean_resources(window,renderer,NULL);
        exit(EXIT_FAILURE); 
    }
}

void test_target_component(SDL_Texture * t, SDL_Surface * s) {
	if(t == NULL) {
        SDL_Log("ERREUR 6 > %s\n", SDL_GetError());
        exit(EXIT_FAILURE); 
    }
    if(s == NULL) {
        SDL_Log("ERREUR 7 > %s\n", SDL_GetError());
        exit(EXIT_FAILURE); 
    }
}

