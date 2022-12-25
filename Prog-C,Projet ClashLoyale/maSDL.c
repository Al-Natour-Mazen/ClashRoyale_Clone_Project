#include "SDL.h"
#include "maSDL.h"
#include "clashloyal.h"

void message(char *myTitle, char *myMessage){
        const SDL_MessageBoxButtonData buttons[] = {
        { /* .flags, .buttonid, .text */        0, 0, "ok" },
 //       { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "yes" },
 //       { SDL_MESSAGEBOX_BUTTON_ESCAPEKEY_DEFAULT, 2, "cancel" },
    };
    const SDL_MessageBoxColorScheme colorScheme = {
        { /* .colors (.r, .g, .b) */
            /* [SDL_MESSAGEBOX_COLOR_BACKGROUND] */
            { 255,   0,   0 },
            /* [SDL_MESSAGEBOX_COLOR_TEXT] */
            {   0, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BORDER] */
            { 255, 255,   0 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_BACKGROUND] */
            {   0,   0, 255 },
            /* [SDL_MESSAGEBOX_COLOR_BUTTON_SELECTED] */
            { 255,   0, 255 }
        }
    };
    const SDL_MessageBoxData messageboxdata = {
        SDL_MESSAGEBOX_INFORMATION, /* .flags */
        NULL, /* .window */
        myTitle, /* .title */
        myMessage, /* .message */
        SDL_arraysize(buttons), /* .numbuttons */
        buttons, /* .buttons */
        &colorScheme /* .colorScheme */
    };
    int buttonid;
    if (SDL_ShowMessageBox(&messageboxdata, &buttonid) < 0) {
        SDL_Log("error displaying message box");
        //return 1;
    }
    if (buttonid == -1) {
        SDL_Log("no selection");
    } else {
        SDL_Log("selection was %s", buttons[buttonid].text);
    }

}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    SDL_Rect offset;

    offset.x = x;
    offset.y = y;

    //On blitte la surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

void clear_surface(SDL_Surface *psurf){
        Uint32 color = SDL_MapRGB(psurf->format,0,0,0);
        SDL_FillRect(psurf,NULL, color);
}

void prepare_sprite( int x, int y, SDL_Surface* source, SDL_Surface* destination ){
    apply_surface( x, y, source, destination );
}

void efface_fenetre(SDL_Surface *psurf){
    clear_surface(psurf);
}

void maj_fenetre(SDL_Window *pWindow){
    SDL_UpdateWindowSurface(pWindow);
}

void prepareAllSpriteJeu(TplateauJeu jeu, int largeur, int hauteur, SDL_Surface **TabSprite, SDL_Surface* destination ){
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){
            if (i ==0 && j ==0)
                prepare_sprite(i*40,j*40,TabSprite[8],destination); // Herbe CoinHautGauche
            else if (i ==10 && j ==0)
                prepare_sprite(i*40,j*40,TabSprite[9],destination); // Herbe CoinHautDroite
            else if (i ==0 && j ==18)
                prepare_sprite(i*40,j*40,TabSprite[10],destination); // Herbe CoinBasGauche
            else if (i ==10 && j ==18)
                prepare_sprite(i*40,j*40,TabSprite[11],destination); // Herbe CoinBasDroite
            else if ( j == 0)
                prepare_sprite(i*40,j*40,TabSprite[4],destination); // Herbe Haut
            else if ( j == 18)
                prepare_sprite(i*40,j*40,TabSprite[5],destination); // Herbe Bas
            else if (i == 5 && j == 9)
                prepare_sprite(i*40,j*40,TabSprite[2],destination); // Pont
            else if (i== 0 && j == 9)
                prepare_sprite(i*40,j*40,TabSprite[12],destination); // Eau extremité Gauche
            else if (i== 10 && j == 9)
                prepare_sprite(i*40,j*40,TabSprite[13],destination); // Eau extremité Droite
            else if ( j == 9)
                prepare_sprite(i*40,j*40,TabSprite[0],destination); // Eau
            else if ( (i==5 && j!=0 && j!=18) )
                prepare_sprite(i*40,j*40,TabSprite[3],destination); // Terre
            else if ( i == 0)
                prepare_sprite(i*40,j*40,TabSprite[6],destination); // Herbe Gauche
            else if ( i == 10)
                prepare_sprite(i*40,j*40,TabSprite[7],destination); // Herbe Droite
            else
                prepare_sprite(i*40,j*40,TabSprite[1],destination); // Herbe
            }
        }
}

void prepareAllSpriteplayer(SDL_Surface **TabSprite, SDL_Surface* destination,TListePlayer player)
{
    TListePlayer joueur = player; // on va parcourire la liste du joueur et on va placer chaque entité au bon endroit sur la map avec la focntion prepare_sprite
    while(joueur != NULL)
    {
        prepare_sprite(joueur->pdata->posX*40,joueur->pdata->posY*40,TabSprite[joueur->pdata->nom],destination);  //remarque les sprites font 40*40 pixels
        joueur = joueur->suiv;
    }
}
