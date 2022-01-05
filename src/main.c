#include <SDL.h>

#include "init_sdl.h" 
#include "validation.h"
#include "trace_segment.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>



int main() {

    SDL_Window * window = NULL; //Initialisation du pointeur de fenêtre
    SDL_Renderer * renderer = NULL; //Initialisation du pointeur de rendu
    SDL_Surface * picture = NULL; //Initialisation du pointeur d'image pour le background de l'application
    SDL_Texture * texture = NULL; //Initilisation d'une Texture 

    SDL_Event event; //Initialisation de la variable élément pour récuperer les évènements.
    SDL_bool quit = SDL_FALSE; //Initialisation de la variable de sortie du programme. //Si true, le programme quitte.


    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) != 0) { //Test de l'initialisation de (SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS)
        printf("ERROR SDL");
        SDL_Log("ERREUR 1 > %s\n", SDL_GetError());
        clean_resources(NULL, NULL, NULL); //Fonction de nettoyage des pointeurs mémoires.
        exit(EXIT_FAILURE);
    }


    window = SDL_CreateWindow("Reconnaissance de chiffres.", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 468, 530, 0); //Creation de la fenetre
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE); //Creation du rendu sur la fenetre window.
    picture = SDL_LoadBMP("./src/images/backg.bmp"); //On recupère l'image background.
    texture = SDL_CreateTextureFromSurface(renderer, picture); //On crée la texture incluant le rendu et l'image.

    test_sdl(window, renderer, picture, texture); //on test que tout s'est bien initalisé correctement.

    //positionnement de la texture sur le renderer en (0,0)
    SDL_Rect dest_rect = {0, 0, 1000, 1089};
    if(SDL_QueryTexture(texture, NULL, NULL, &dest_rect.w, &dest_rect.h) != 0) { //On test que texture s'est bien initalisé en recupérant la taille de texture.
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        clean_resources(window,renderer,NULL); //Nettoyage de la memoire.
        exit(EXIT_FAILURE); 
    }
    if(SDL_RenderCopy(renderer, texture, NULL, &dest_rect) != 0) { //On test d'accéder au rendu et à la texture
        SDL_Log("ERREUR > %s\n", SDL_GetError());
        clean_resources(window,renderer,NULL); //Nettoyage de la memoire.
        exit(EXIT_FAILURE); 
    }

    SDL_Point * point = malloc(sizeof(SDL_Point)); //Initalisation d'un point point.
    SDL_Point * LastPoint = malloc(sizeof(SDL_Point)); //Initalisation d'un point LastPoint.

    SDL_Point * A = malloc(sizeof(SDL_Point)); //Initalisation d'un point A .
    SDL_Point * B = malloc(sizeof(SDL_Point)); //Initalisation d'un point B.

    size_t i = 0; //Initialisation d'une variable d'incrémentation.
    int pass_sanitaire = 1; //Variable avec nom arbitraire, permet de determiné si le clique de la souris a été levé afin de ne pas tracer de segment entre deux cliques.

    const Uint8 * keyb; //Initialisation du pointeur de clavier.
    SDL_RenderPresent(renderer); //Actualisation du rendu.

    while(!quit)
    {

        SDL_WaitEvent(&event); //Listener d'evenements.
        keyb = SDL_GetKeyboardState(NULL); //On récupère les évènements du clavier.

        if(keyb[SDL_SCANCODE_ESCAPE]) //On detecte si l'évènement clavier est de type, appuie sur echap.
            quit = SDL_TRUE; //Si echap, on quitte le programme.
        else if(event.type == SDL_QUIT)  
            quit = SDL_TRUE; //On quitte le programme si on a un evenement de type SDL_Quit
        else if(event.type == SDL_MOUSEMOTION) { //On récupère tout les évènement où la souris est en mouvement.

            if(event.button.button != SDL_BUTTON_LEFT) { //On récupère l'évènement de non-clique gauche de la souris et on stock les coordonnées de la souris sur un point LastPoint. 
                LastPoint->x = event.motion.x; 
                LastPoint->y = event.motion.y;
            }

            if((event.button.button == SDL_BUTTON_LEFT) && (event.motion.x < 336) ) { //On écoute les cliques gauches étant dans la partie de dessin.
                    SDL_Rect rect = {event.motion.x, event.motion.y, 10, 10}; //On initialise un carré.
                    point->x = event.motion.x; //On stock les coordonnées de la souris sur un point.
                    point->y = event.motion.y;

                    if( (sqrt( (pow(point->x - LastPoint->x, 2)) + (pow(point->y - LastPoint->y, 2)) ) > 1  ) && ( pass_sanitaire != 1 ) ) { //On calcul la distance entre deux points, si elle dépasse 1, et que on a pas laché le clique de la souris entre temps (pass_sanitaire), alors on appelle le tracage d'un segment entre les deux points.
                        A->x = LastPoint->x; //On restock les coordonnées des deux points de distance > 1 dans des points A et B.
                        A->y = LastPoint->y;
                        B->x = point->x;
                        B->y = point->y;
                        trace_line(A,B,renderer); //On appelle la fonction de tracage du fichier trace_segment.c
                        SDL_RenderPresent(renderer); //On actualise le rendu.
                        LastPoint->x = event.motion.x; //On stock les coordonnées de la souris dans LastPoint.
                        LastPoint->y = event.motion.y;
                    } 
                    SDL_RenderFillRect(renderer, &rect);  //On dessine les renctangles sur le rendu.
                    SDL_RenderPresent(renderer); // On actualise le rendu.
                    pass_sanitaire = 0; 
                }
            }

            if(event.type == SDL_MOUSEBUTTONUP){ //On écoute si le clique de la souris est laché.
                  pass_sanitaire = 1;
            }

            if((event.motion.x > 345) && (event.motion.x < 390) && (event.motion.y > 168) && (event.motion.y < 207) && (event.button.button == SDL_BUTTON_LEFT) ) { //On écoute le clique sur une zone précise, ceci permet d'avoir un bouton
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); //Couleur noir, le crayon.
                SDL_Delay(10);
            } 

            if((event.motion.x > 415) && (event.motion.x < 455) && (event.motion.y > 168) && (event.motion.y < 207) && (event.button.button == SDL_BUTTON_LEFT) ) { //On écoute le clique sur une zone précise, ceci permet d'avoir un bouton
                SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); //Couleur blanche, la gomme.
                SDL_Delay(10);
            }

            if((event.motion.x > 350) && (event.motion.x < 460) && (event.motion.y > 320) && (event.motion.y < 345) && (event.button.button == SDL_BUTTON_LEFT) ) { //On écoute le clique sur une zone précise, ceci permet d'avoir un bouton
                // Boutton valider
                SDL_WaitEventTimeout(&event, 2000); //permet de ne recuperer qu'un seul évènement

                SDL_Texture * t_renderer = SDL_GetRenderTarget(renderer); //On crée le rendu du dessin effectué par l'utilisateur.
                SDL_Surface * surface = SDL_CreateRGBSurface(0, 345, 530, 32, 0, 0, 0, 0); 
                validation_process(renderer, texture, t_renderer, surface); //fonction qui lance le processus d'enregistrement de l'image dans validation.c
                
            }
        }
    clean_resources(window, renderer, texture); //Fin de programmme et nettoyage de la mémoire.
    return EXIT_SUCCESS; //Fermeture du programme.
}

