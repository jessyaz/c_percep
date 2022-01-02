#include <SDL.h>

void clean_resources(SDL_Window *w, SDL_Renderer *r, SDL_Texture *t) { //Fonction de nettoyage de la mémoire. Désalocation des pointeurs alloués dynamiquements.
    if(t != NULL) 
        SDL_DestroyTexture(t); 
    if(r != NULL) 
        SDL_DestroyRenderer(r);
    if(w != NULL) 
        SDL_DestroyWindow(w);
    SDL_Quit();
}

void test_sdl(SDL_Window * window, SDL_Renderer * renderer, SDL_Surface * texture, SDL_Texture * picture) { //fonction de test de l'initialisation des différents composants utilisés de la SDL

    SDL_version nb_version; //On crée une variable pous stocker la version de la librairie.

    SDL_VERSION(&nb_version); //On récupère le numéro de la version de la librairie dans la variable.
    printf("La version de la SDL est %d.%d.%d \n \n", nb_version.major, nb_version.minor, nb_version.patch); //On print la version de la SDL sur la console.

    if(window == NULL) { //Test d'initialisation de la fenetre.
        SDL_Log("ERREUR 2 > %s\n", SDL_GetError());
        clean_resources(NULL,NULL,NULL);
        exit(EXIT_FAILURE);
    }
    if(renderer == NULL) { //Test d'initialisation du rendu.
        SDL_Log("ERREUR 3 > %s\n", SDL_GetError());
        clean_resources(window,NULL,NULL);
        exit(EXIT_FAILURE);
    }
    if(texture == NULL) { //Test d'initialisation de la texture.
        SDL_Log("ERREUR 4 > %s\n", SDL_GetError());
        clean_resources(window,renderer,NULL);
        exit(EXIT_FAILURE); 
    }
    if(picture == NULL) { //Test d'initialisation de l'image.
        SDL_Log("ERREUR 5 > %s\n", SDL_GetError());
        clean_resources(window,renderer,NULL);
        exit(EXIT_FAILURE); 
    }
}

void test_target_component(SDL_Texture * t, SDL_Surface * s) {
	if(t == NULL) { //Test d'initialisation d'une texture t.
        SDL_Log("ERREUR 6 > %s\n", SDL_GetError());
        exit(EXIT_FAILURE); 
    }
    if(s == NULL) { //Test d'initialisation d'une surface s.
        SDL_Log("ERREUR 7 > %s\n", SDL_GetError());
        exit(EXIT_FAILURE); 
    }
}

