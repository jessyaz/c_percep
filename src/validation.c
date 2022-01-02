#include <SDL.h> 
#include "init_sdl.h" 


Uint32 getPixel(SDL_Surface *surface, int x, int y) {
/*    """ int : x, y -> coordonnée du pixel voulu.
        SDL_Surface: * surface, pointeur sur structure de surface types SDL. Afin de choisir sur quel surface chercher le pixel aux coordonnées (x,y).

        nbOctetsParPixel : int , désigne le nombre d'octet par pixel, afin d'obtenir une structure format pour stocker le pointeur de pixel.
        Uint8 est une structure types de la SDL, utilisé pour les pixels à 1 octet. C'est le pointeur du pixel.

        Fonction qui recupère le format d'un pixel au coordonnée (x,y), ainsi que ces valeurs RGBa.
    """ */
    int nbOctetsParPixel = surface->format->BytesPerPixel;
    Uint8 *p = (Uint8 *)surface->pixels + y * surface->pitch + x * nbOctetsParPixel; //on recupère le nombre d'octet par pixel.
    switch(nbOctetsParPixel) //on on retourne le pointeur de structure en fonction du format nbOctetsParPixel correspondant.
    {
        case 1: //format 1 bits
            return *p; //on retourne le pointeur sur le pixel
        case 2: //format 2 bits
            return *(Uint16 *)p; //on retourne le pointeur sur le pixel
        case 3: //format 3 bits
            if(SDL_BYTEORDER == SDL_BIG_ENDIAN) //deux types de systèmes donnant deux formats de pixel différents, utilisé ici pour la compatibilité entre système d'exploitation.
                return p[0] << 16 | p[1] << 8 | p[2]; //on retourne le pointeur sur le pixel
            else
                return p[0] | p[1] << 8 | p[2] << 16; //on retourne le pointeur sur le pixel
        case 4: //format 32 bits
            return *(Uint32 *)p; //on retourne le pointeur sur le pixel
        default:
            return 0; //On retourne rien et on fait tout planter.
    }
}


int save_target_renderer(SDL_Renderer * renderer, SDL_Texture * texture, SDL_Texture * t_renderer, SDL_Surface * surface) { //fonction qui enregistre l'image dessinée.
  /*  """ Utilise les principales fonctions de la SDL
        Execute une suite d'instructions afin d'enregistrer un rendu, liberer un rendu, et revenir à un rendu.
        Retourne une valeur caractéristique arbitraire.
    """ */
    SDL_RenderReadPixels(renderer, NULL, surface->format->format, surface->pixels, surface->pitch); //on lit les pixels d'un rendu et on les copies dans des tableaux sur des structures TEXTURE et SURFACE.
    SDL_SaveBMP(surface, "./bin/temp.bmp"); //on sauvegarde l'image du tableau de pixel du pointeur de la structure SURFACE.
    SDL_FreeSurface(surface); //on vide le pointeur surface.
    SDL_SetRenderTarget(renderer, t_renderer); //on reprend renderer comme rendu principal.

    return 0; //on retourne 0 à la fin pour verrifier que toutes les instructions ont été éxécutés.
}



void load_pic() {
 /*   """ SDL_Surface est une structure types de la SDL. 
    Fonction qui lance le script python, et qui écrit la liste des pixels en une dimension dans un fichier CSV pour autre strategie de traittement des données.

    """ */
    SDL_Surface * picture = SDL_LoadBMP("./bin/temp.bmp"); //On recupère l'image enregistrer par la fonction save_target_renderer pour extraire les pixels dans le but de les enregistrer en une dimension dans un fichier CSV.

    if(picture == NULL) { //on test si l'image a bien été trouvé et ouverte.
        printf("Image non trouvé, problème lors de l'enregistrement de l'image, merci de donner les droits write au dossier ./bin/");
        exit(EXIT_FAILURE); 
    }

    Uint32 pixel; //parametre native pour lecture de pixel (format)
    Uint8 r,g,b,a; //parametre native pour lecture de pixel (couleur)
    int weight = picture->h*picture->w; //correspond au nombre de pixel total de l'image
    int *tab_of_pix = malloc(weight*sizeof(int)); 
    int coord = 0; //increment pour generer un vecteur.
    int moyenne = 0; //moyenne des couleurs RGB afin de d'obtenir une dimension par pixel
    
    for(int i = 0; i <= picture->w ; i++) { //boucle qui parcourt tout les pixels de l'image et les stocks dans un tableau.
        for(int j = 0; j <= picture->h ; j++) { 
            pixel = getPixel(picture, i, j); //fonction getPixel declarer dans ce même fichier validation.c
            SDL_GetRGBA(pixel, picture->format, &r, &g, &b, &a);
            moyenne = ((r + g + b)/3); //moyenne des couleurs pour avoir une dimension par pixel.
            tab_of_pix[coord] = moyenne; //on remplie le vecteur (pixel en une ligne et nombre de pixel de colonne)
            coord+=1; //increment de la boucle utilisé pour le tableau tab_of_pix.
        }
    }
    
    FILE *fpt = fopen("./bin/temp.csv", "w+"); //Fonction qui s'adresse à un fichier.
    for(int i = 0; i <= weight; i++) { //boucle qui permet d'executer les instructions ci dessous, à savoir écrire les valeurs d'un tableau dans un fichier.
        fprintf(fpt,"%d;", tab_of_pix[i]); //on écrit dans ce fichier les valeurs du tableau
        //le point virgule ( ; ) correspond au delimiteur utilisé par les fichiers CSV, c'est une norme arbitraire.
    }
    fclose(fpt); //on ferme le pointeur sur le fichier.
   

    //partie execution de commande système pour execution du script python.
    const char * command = "python ./src/predict.py"; //Commande pour executer python
    int cr = system( command ); //Execution de la commande grace à la fonction native system();
    if ( cr != 0 ) { //on verrifie si la commande a bien été executé.
        fprintf( stderr, "Impossible de lancer la commande : %s\n", command ); //Erreur si la commande n'a pas été executer pour une raison retourné. (exemple: erreur de syntaxe dans le script, droit d'execution, etc...)
    }
    free(tab_of_pix); //on libère le tableau de pixel à une dimesion.
}


void validation_process(SDL_Renderer * renderer, SDL_Texture * texture, SDL_Texture * t_renderer, SDL_Surface * surface) { 
 /*   """ Fonction principal de validation.c appelé depuis main.c
        execute les fonction défini ici dans validation.C -> save_target_renderer() et load_pic();

    """ */
    int res = save_target_renderer(renderer, texture, t_renderer, surface); //fonction sauvegarde de l'image
    load_pic(); //fonction qui lance le script python, et qui écrit la liste des pixels en une dimension dans un fichier CSV pour autre strategie de traittement des données.

}