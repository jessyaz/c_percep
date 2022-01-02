#include <SDL.h>

void trace_line(SDL_Point * A, SDL_Point * B, SDL_Renderer * renderer) { //Trace_line ne trace pas de ligne, mais pour chaque pixel, cette fonction trace des carrés.

    //Pour plus d'information sur le fonctionnement de cet algo, voir le rapport.

    int dx, dy, x, y, reste, absdx, absdy, pasx, pasy, i; //On initialise les variables necessaires à la recherche de la direction et le tracage du segment.
    dx = B->x - A->x; //Calcul de dx
    dy = B->y - A->y; //Calcul de dy
    reste = 0;
    x = A->x; //Point initial ou x_0
    y = A->y;//Point initial ou y_0

    SDL_Rect rect = {x, y, 10, 10}; //On trace le premier point.
    SDL_RenderFillRect(renderer, &rect); //On associe "rect" sur rendu.

    //Etape des différents cas de direction du tracage de segment.

    if(dx > 0) { 
        pasx = 1; //On avance vers x croissant
    } else { 
        pasx = -1; //On avance vers x décroissant
    }


    if(dy > 0) { 
        pasy = 1; //On avance vers y croissant
    } else {
        pasy = -1; //On avance vers y décroissant
    }

    absdx = abs(dx); //Calcule de la valeur absolue des deux variables de direction afin de determiné la quelle de deux est la plus forte et de s'en servir..
    absdy = abs(dy);

    if(dx == 0) { //X constant, ou asymptote en y
        for(i = 0; i < absdy; i++) { //Tracage
            y+=pasy;
            SDL_Rect rect = {x, y, 10, 10}; //On trace un carré de coté 10 en (x,y)
            SDL_RenderFillRect(renderer, &rect); //On associe "rect" sur rendu.
        }
    }

    else if(dy == 0) { //Y constant, ou asymptote en x
        for(i = 0; i < absdx; i++) { //Tracage
            x+=pasx;
            SDL_Rect rect = {x, y, 10, 10}; //On trace un carré de coté 10 en (x,y)
            SDL_RenderFillRect(renderer, &rect); //On associe "rect" sur rendu.              
        }
    }

    else if(absdx == absdy) { //Cas diagonaux
        for(i = 0; i < absdx ; i++) { //Tracage
            x+=pasx;
            y+=pasy;
            SDL_Rect rect = {x, y, 10, 10}; //On trace un carré de coté 10 en (x,y)
            SDL_RenderFillRect(renderer, &rect); //On associe "rect" sur rendu.
            
        }
    } 

    else if(absdx > absdy) { //Cas d'une pente douce, les deplacements en x sont plus fort que ceux y
        for(i = 0; i < absdx ; i++) { //Tracage
            x+=pasx;
            reste+=absdy;
            if(reste >= absdx) {
                reste-=absdx;
                y+=pasy;
            }   
            SDL_Rect rect = {x, y, 10, 10}; //On trace un carré de coté 10 en (x,y)
            SDL_RenderFillRect(renderer, &rect); //On associe "rect" sur rendu.
            
        }
        
        } else { //Cas d'une pente forte, les deplacements en y sont plus fort que ceux x
            for(i = 0; i < absdy ; i++) { //Tracage
                y+=pasy;
                reste+=absdx;
                if(reste >= absdy) {
                    reste = reste - absdy;
                    x+=pasx;
                }
                SDL_Rect rect = {x, y, 10, 10}; //On trace un carré de coté 10 en (x,y)
                SDL_RenderFillRect(renderer, &rect); //On associe "rect" sur rendu.
            }
        }
        SDL_RenderPresent(renderer); //Actualisation du rendu
    }