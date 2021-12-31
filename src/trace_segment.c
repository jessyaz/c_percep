#include <SDL.h>

void trace_line(SDL_Point * A, SDL_Point * B, SDL_Renderer * renderer) {

    int dx, dy, x, y, reste, absdx, absdy, pasx, pasy, i;
    dx = B->x - A->x;
    dy = B->y - A->y;
    reste = 0;
    x = A->x;
    y = A->y;
    SDL_Rect rect = {x, y, 10, 10};
    SDL_RenderFillRect(renderer, &rect);

    if(dx > 0) {
        pasx = 1;
    } else { 
        pasx = -1; 
    }


    if(dy > 0) { 
        pasy = 1;
    } else {
        pasy = -1;
    }

    absdx = abs(dx);
    absdy = abs(dy);

    if(dx == 0) { //asymptote
        for(i = 0; i < absdy; i++) {
            y+=pasy;
            SDL_Rect rect = {x, y, 10, 10};
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    else if(dy == 0) { //asymptote
        for(i = 0; i < absdx; i++) {
            x+=pasx;
            SDL_Rect rect = {x, y, 10, 10};
            SDL_RenderFillRect(renderer, &rect);
                       
        }
    }
    else if(absdx == absdy) {
        for(i = 0; i < absdx ; i++) {
            x+=pasx;
            y+=pasy;
            SDL_Rect rect = {x, y, 10, 10};
            SDL_RenderFillRect(renderer, &rect);
            
        }
    } 
    else if(absdx > absdy) {
        for(i = 0; i < absdx ; i++) {
            x+=pasx;
            reste+=absdy;
            if(reste >= absdx) {
                reste-=absdx;
                y+=pasy;
            }   
            SDL_Rect rect = {x, y, 10, 10};
            SDL_RenderFillRect(renderer, &rect);
            
        }
        
        } else {
            for(i = 0; i < absdy ; i++) {
                y+=pasy;
                reste+=absdx;
                if(reste >= absdy) {
                    reste = reste - absdy;
                    x+=pasx;
                }
                SDL_Rect rect = {x, y, 10, 10};
                SDL_RenderFillRect(renderer, &rect);
            }
        }
        SDL_RenderPresent(renderer); 
    }