//NOM prénom étudiant 1 : CAILLAUD TOM
//NOM prénom étudiant 2 : AL NATOUR MAZEN

//NOTE : Tous ce qui se trouve dans le main.c entre des commentaires de ce type a été modifié par nous.
                                                /*****DEBUT MODIF**********************/
                                                /*****FIN MODIF***********************/

// NOTE2 : Il se peut que si vous recompilez le projet cela ne marche pas c'est simplement à cause de la musique, je vous invite à aller à la ligne 98 pour régler ce problème ou si vous ne souhaitez pas avoir l'expérience sonore ,la ligne 101 est à mettre en commentaire. Merci pour votre compréhension.


#include "SDL.h"
#include "maSDL.h"
#include "clashloyal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>

/*--------- Main ---------------------*/
int main(int argc, char* argv[])
{
    SDL_Window *pWindow;
    SDL_Init(SDL_INIT_VIDEO);

    pWindow = SDL_CreateWindow(
        "Appuyez sur ECHAP pour quitter, S/C ET D/V pour gerer les sauvegardes",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        LARGEURJEU*40,
        HAUTEURJEU*40,
        SDL_WINDOW_SHOWN
    );

     /*****DEBUT MODIF*************************************************************/

    //ouvrir une Fênetre
    SDL_Surface* pWinSurf = SDL_GetWindowSurface(pWindow);

    //les sprites du terrain (nouveaux sprites pour ameliorer l'apparence du jeu)
    SDL_Surface* pSpriteEau = SDL_LoadBMP("./data/Textures/Map/Eau/Eau.bmp"); //indice 0  Ne figure pas dans l'enum TuniteDuJeu
    SDL_Surface* pSpriteHerbe = SDL_LoadBMP("./data/Textures/Map/Herbes/Herbe.bmp"); //indice 1 idem
    SDL_Surface* pSpritePont = SDL_LoadBMP("./data/Textures/Map/Pont.bmp"); //indice 2 idem
    SDL_Surface* pSpriteTerre = SDL_LoadBMP("./data/Textures/Map/Terre.bmp"); //indice 3 idem
    SDL_Surface* pSpriteHerbeHaut = SDL_LoadBMP("./data/Textures/Map/Herbes/HerbeHaut.bmp"); //indice 4 idem
    SDL_Surface* pSpriteHerbeBas = SDL_LoadBMP("./data/Textures/Map/Herbes/HerbeBas.bmp"); //indice 5 idem
    SDL_Surface* pSpriteHerbeCoteG = SDL_LoadBMP("./data/Textures/Map/Herbes/HerbeCoteG.bmp"); //indice 6 idem
    SDL_Surface* pSpriteHerbeCoteD = SDL_LoadBMP("./data/Textures/Map/Herbes/HerbeCoteD.bmp"); //indice 7 idem
    SDL_Surface* pSpriteHerbeCoinHautG = SDL_LoadBMP("./data/Textures/Map/Herbes/HerbeCoinHautG.bmp"); //indice 8 idem
    SDL_Surface* pSpriteHerbeCoinHautD = SDL_LoadBMP("./data/Textures/Map/Herbes/HerbeCoinHautD.bmp"); //indice 9 idem
    SDL_Surface* pSpriteHerbeCoinBasG = SDL_LoadBMP("./data/Textures/Map/Herbes/HerbeCoinBasG.bmp"); //indice 10 idem
    SDL_Surface* pSpriteHerbeCoinBasD = SDL_LoadBMP("./data/Textures/Map/Herbes/HerbeCoinBasD.bmp"); //indice 11 idem
    SDL_Surface* pSpriteEauG = SDL_LoadBMP("./data/Textures/Map/Eau/EauG.bmp"); //indice 12 idem
    SDL_Surface* pSpriteEauD = SDL_LoadBMP("./data/Textures/Map/Eau/EauD.bmp"); //indice 13 idem

    // l'Eau, l''herbe et le pont apparaitront en l absence d'unité (NULL dans le plateau) et en foction de certains indices x,y définissant le chemin central
    SDL_Surface* TabSpriteMap[14]={pSpriteEau,pSpriteHerbe,pSpritePont,pSpriteTerre,pSpriteHerbeHaut,pSpriteHerbeBas,pSpriteHerbeCoteG,pSpriteHerbeCoteD ,pSpriteHerbeCoinHautG,pSpriteHerbeCoinHautD,pSpriteHerbeCoinBasG,pSpriteHerbeCoinBasD,pSpriteEauG,pSpriteEauD };


    // ASTUCE : on stocke le sprite d'une unité à l'indice de son nom dans le type enum TuniteDuJeu, dans le tableau TabSprite

    //les spirtes de l'equipe Bleu
    SDL_Surface* pSpriteTourBleu = SDL_LoadBMP("./data/Textures/Joueurs/Bleu/Tour.bmp");  //indice 0 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourRoiBleu = SDL_LoadBMP("./data/Textures/Joueurs/Bleu/TourRoi.bmp"); //indice 1
    SDL_Surface* pSpriteArcherBleu = SDL_LoadBMP("./data/Textures/Joueurs/Bleu/Archer.bmp"); //indice 2
    SDL_Surface* pSpriteChevalierBleu = SDL_LoadBMP("./data/Textures/Joueurs/Bleu/Chevalier.bmp"); //indice 3
    SDL_Surface* pSpriteDragonBleu = SDL_LoadBMP("./data/Textures/Joueurs/Bleu/Dragon.bmp"); //indice 4
    SDL_Surface* pSpriteGargouilleBleu = SDL_LoadBMP("./data/Textures/Joueurs/Bleu/Gargouille.bmp"); //indice 5

    SDL_Surface* TabSpriteplayer1[6]={pSpriteTourBleu,pSpriteTourRoiBleu,pSpriteArcherBleu,pSpriteChevalierBleu,pSpriteDragonBleu,pSpriteGargouilleBleu};

    //les spirtes de l'equipe rouge
    SDL_Surface* pSpriteTourRouge = SDL_LoadBMP("./data/Textures/Joueurs/Rouge/Tour.bmp");  //indice 0 dans tabSprite (via l'enum TuniteDuJeu)
    SDL_Surface* pSpriteTourRoiRouge = SDL_LoadBMP("./data/Textures/Joueurs/Rouge/TourRoi.bmp"); //indice 1
    SDL_Surface* pSpriteArcherRouge = SDL_LoadBMP("./data/Textures/Joueurs/Rouge/Archer.bmp"); //indice 2
    SDL_Surface* pSpriteChevalierRouge = SDL_LoadBMP("./data/Textures/Joueurs/Rouge/Chevalier.bmp"); //indice 3
    SDL_Surface* pSpriteDragonRouge = SDL_LoadBMP("./data/Textures/Joueurs/Rouge/Dragon.bmp"); //indice 4
    SDL_Surface* pSpriteGargouilleRouge = SDL_LoadBMP("./data/Textures/Joueurs/Rouge/Gargouille.bmp"); //indice 5

    SDL_Surface* TabSpriteplayer2[6]={pSpriteTourRouge,pSpriteTourRoiRouge,pSpriteArcherRouge,pSpriteChevalierRouge,pSpriteDragonRouge,pSpriteGargouilleRouge};

    /*****FIN MODIF*********************************************************************/

    if ( pSpriteEau  )  //si le permier sprite a bien été chargé, on suppose que les autres aussi
    {
        TplateauJeu jeu = AlloueTab2D(LARGEURJEU,HAUTEURJEU);
        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);
        affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);

        prepareAllSpriteJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSpriteMap,pWinSurf);
        maj_fenetre(pWindow);

        message("Welcome in ClashLoyal","ECHAP->quitter, S/D pour sauvegarder et C/V pour charger");

        /*****DEBUT MODIF*****************************************************/

        /*------------------- initialisation de la Musique--------------------*/
        // jouer la musique
        // A noter : pour que cela fonctionne l'option à ajouter dans le linker du compiler (Settings/Compiler/Global Compiler Settings/linker Settings/Other linkers options)
        // est : -lwinmm en plus d'inclure les mmsystem.h et windows.h (mais c'est deja fait pour ces 2 bibliothèques)
          //  PlaySound(TEXT("./data/Song/ClashRoyale.wav"),NULL,SND_ASYNC);


        /*---------------------- initialisation Joueur 1 ---------------------*/
            TListePlayer joueur1=NULL;
            AjouterUnite(&joueur1,creeTourRoi(5,1));
            AjouterUnite(&joueur1,creeTour(5,3));
            int elixirJ1 =10;
            PositionnePlayerOnPlateau(joueur1,jeu);
            affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);

        /*---------------------- initialisation Joueur 2 ---------------------*/
            TListePlayer joueur2=NULL;
            AjouterUnite(&joueur2,creeTourRoi(5,17));
            AjouterUnite(&joueur2,creeTour(5,15));
            int elixirJ2 =10;
            PositionnePlayerOnPlateau(joueur2,jeu);
            affichePlateauConsole(jeu,LARGEURJEU,HAUTEURJEU);


        /*****FIN MODIF********************************************************/


        int tourdejeu = 0; //init des tour de jeu

        // boucle principale du jeu
        int cont = 1;
        while ( cont != 0 ){
                SDL_PumpEvents(); //do events

                /*****DEBUT MODIF*******************************************************/

                if(tourdejeu < 10) // on fait tourner le jeu qu'une fois sur 10 pour que cela soit plus simple a suivre
                {
                    tourdejeu++;
                }
                else
                {
                    tourdejeu = 0;
                }

                if(tourdejeu == 0)
                {
                    // on affiche des infos sur les 2 joueurs dans la console
                    // A noter qu'il y aura un decalage entre la console et la map car afficheconsoleplayers
                    // est placer juste avant les combats et donc ne sera que update au prochain tour de boucle
                    afficheconsoleplayers(joueur1,joueur2,&elixirJ1,&elixirJ2);

                    // on reset l'attaque des troupes, ceux qui étaient entrain d'attaquer car on suppose qu'ils ont terminé leur attaque
                    resetattaque(&joueur1);
                    resetattaque(&joueur2);

                    int probaquijoue=(rand()%2); // ce n'est pas toujours le même joueurs qui va jouer en premier , ça sera tiré de façon aleatoire
                    if(probaquijoue == 0 )
                    {
                        // -------------------------------- Joueur 1 ---------------------------------

                            // -----------------pour ajouter/acheter une unite-----------------------
                                createcard(&joueur1,jeu,&elixirJ1,0);
                            // -------------------------le combat----------------------------
                                attaque(joueur1,&joueur2);
                            // -------------Detection si l'autre joueur est mort -------------
                                if(tourRoiDetruite(joueur2))
                                {
                                    cont = 0; // fin du jeu , le joueur 2 a perdu
                                    affichegagnant(0);
                                }
                                else // si ce n'est pas le cas alors on continue avec le joueur 2
                                {
                                    attaque(joueur2,&joueur1);
                                    if(tourRoiDetruite(joueur1))
                                    {
                                        cont = 0; // fin du jeu , le joueur 1 a perdu
                                        affichegagnant(1);
                                    }
                                    Tunite* unite = uniteaDetruire(&joueur1);
                                    supprimerUnite(&joueur1, unite,jeu);
                                }
                            // -----------------pour suprimer une unite-----------------------
                                Tunite* unite = uniteaDetruire(&joueur2);
                                supprimerUnite(&joueur2, unite,jeu);
                    }
                    else if (probaquijoue == 1 )
                    {
                        // -------------------------------- Joueur 2 ---------------------------------

                            // -----------------pour ajouter/acheter une unite-----------------------
                                createcard(&joueur2,jeu,&elixirJ2,1);
                            // -------------------------le combat----------------------------
                                attaque(joueur2,&joueur1);
                            // -------------Detection si l'autre joueur est mort -------------
                                if(tourRoiDetruite(joueur1))
                                {
                                    cont = 0; // fin du jeu , le joueur 1 a perdu
                                    affichegagnant(1);
                                }
                                else // si ce n'est pas le cas alors on continue avec le joueur 1
                                {
                                    attaque(joueur1,&joueur2);
                                    if(tourRoiDetruite(joueur2))
                                    {
                                        cont = 0; // fin du jeu , le joueur 2 a perdu
                                        affichegagnant(0);
                                    }
                                    Tunite* unite = uniteaDetruire(&joueur2);
                                    supprimerUnite(&joueur2, unite,jeu);
                                }
                            // -----------------pour suprimer une unite----------------------
                                Tunite* unite = uniteaDetruire(&joueur1);
                                supprimerUnite(&joueur1, unite,jeu);
                    }
                    // on fera bouger les troupes de chaque camp à chaque tour de jeu s'ils pouvent bouger bien sur
                    moveoncase(jeu,&joueur1,0);
                    moveoncase(jeu,&joueur2,1);
                }

                /*****FIN MODIF*********************************************************/


                //affichage du jeu à chaque tour
                efface_fenetre(pWinSurf);
                /*****DEBUT MODIF********************************/
                prepareAllSpriteJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSpriteMap,pWinSurf);
                prepareAllSpriteplayer(TabSpriteplayer1,pWinSurf,joueur1);
                prepareAllSpriteplayer(TabSpriteplayer2,pWinSurf,joueur2);
                /*****FIN MODIF**********************************/
                maj_fenetre(pWindow);
                SDL_Delay(100);  //valeur du délai

                //LECTURE DE CERTAINES TOUCHES POUR LANCER LES RESTAURATIONS ET SAUVEGARDES
                const Uint8* pKeyStates = SDL_GetKeyboardState(NULL);
                if ( pKeyStates[SDL_SCANCODE_V] ){

                        /*****DEBUT MODIF********************************/
                        joueur1 = DestroyplayerDeck(joueur1);
                        joueur2 = DestroyplayerDeck(joueur2);
                        chargersauvgardeseq(&joueur1,&joueur2);
                        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);
                        /*****FIN MODIF**********************************/

                        message("chargement","Le chargement du fichier sequentiel a bien ete effectue !");

                        efface_fenetre(pWinSurf);
                        /*****DEBUT MODIF********************************/
                        prepareAllSpriteJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSpriteMap,pWinSurf);
                        prepareAllSpriteplayer(TabSpriteplayer1,pWinSurf,joueur1);
                        prepareAllSpriteplayer(TabSpriteplayer2,pWinSurf,joueur2);
                        /*****FIN MODIF**********************************/
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_C] ){

                        /*****DEBUT MODIF****************************************************/
                        joueur1 = DestroyplayerDeck(joueur1);
                        joueur2 = DestroyplayerDeck(joueur2);
                        chargerBnr(&joueur1,&joueur2);
                        initPlateauAvecNULL(jeu,LARGEURJEU,HAUTEURJEU);
                        /*****FIN MODIF******************************************************/

                        message("chargement","Le chargement du fichier binaire a bien ete effectue !");

                        efface_fenetre(pWinSurf);
                        /*****DEBUT MODIF********************************/
                        prepareAllSpriteJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSpriteMap,pWinSurf);
                        prepareAllSpriteplayer(TabSpriteplayer1,pWinSurf,joueur1);
                        prepareAllSpriteplayer(TabSpriteplayer2,pWinSurf,joueur2);
                        /*****FIN MODIF**********************************/
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_D] ){


                        /*****DEBUT MODIF********************************/
                        sauvegardeseq(joueur1,joueur2);
                        /*****FIN MODIF**********************************/

                        message("Sauvegarde","La sauvegarde du fichier sequentiel a bien ete effectue !");

                        efface_fenetre(pWinSurf);
                        /*****DEBUT MODIF********************************/
                        prepareAllSpriteJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSpriteMap,pWinSurf);
                        prepareAllSpriteplayer(TabSpriteplayer1,pWinSurf,joueur1);
                        prepareAllSpriteplayer(TabSpriteplayer2,pWinSurf,joueur2);
                        /*****FIN MODIF**********************************/
                        maj_fenetre(pWindow);
                        SDL_Delay(300);

                }
                if ( pKeyStates[SDL_SCANCODE_S] ){

                        /*****DEBUT MODIF****************************************************/
                        sauvegarderBnr(joueur1,joueur2);
                        /*****FIN MODIF******************************************************/

                        message("Sauvegarde","La sauvegarde du fichier binaire a bien ete effectue !");

                        efface_fenetre(pWinSurf);
                        /*****DEBUT MODIF********************************/
                        prepareAllSpriteJeu(jeu,LARGEURJEU,HAUTEURJEU,TabSpriteMap,pWinSurf);
                        prepareAllSpriteplayer(TabSpriteplayer1,pWinSurf,joueur1);
                        prepareAllSpriteplayer(TabSpriteplayer2,pWinSurf,joueur2);
                        /*****FIN MODIF**********************************/
                        maj_fenetre(pWindow);
                        SDL_Delay(300);
                }
                if ( pKeyStates[SDL_SCANCODE_ESCAPE] ){
                        cont = 0;  //sortie de la boucle
                }

        }
        //fin boucle du jeu

        /*****DEBUT MODIF********************************/
        // Libération des ressources occupées par les 2 listes du joueurs
        joueur1 = DestroyplayerDeck(joueur1);
        joueur2 = DestroyplayerDeck(joueur2);

        // Libération des ressources occupées par les sprites Rouge
        SDL_FreeSurface(pSpriteTourRouge);
        SDL_FreeSurface(pSpriteTourRoiRouge);
        SDL_FreeSurface(pSpriteArcherRouge);
        SDL_FreeSurface(pSpriteChevalierRouge);
        SDL_FreeSurface(pSpriteDragonRouge);
        SDL_FreeSurface(pSpriteGargouilleRouge);

        // Libération des ressources occupées par les sprites Bleu
        SDL_FreeSurface(pSpriteTourBleu);
        SDL_FreeSurface(pSpriteTourRoiBleu);
        SDL_FreeSurface(pSpriteArcherBleu);
        SDL_FreeSurface(pSpriteChevalierBleu);
        SDL_FreeSurface(pSpriteDragonBleu);
        SDL_FreeSurface(pSpriteGargouilleBleu);

        // Libération des ressources occupées par les sprites Du Jeu
        SDL_FreeSurface(pSpritePont);
        SDL_FreeSurface(pSpriteTerre);
        //Herbes
        SDL_FreeSurface(pSpriteHerbe);
        SDL_FreeSurface(pSpriteHerbeHaut);
        SDL_FreeSurface(pSpriteHerbeBas);
        SDL_FreeSurface(pSpriteHerbeCoteG);
        SDL_FreeSurface(pSpriteHerbeCoteD);
        SDL_FreeSurface(pSpriteHerbeCoinHautG);
        SDL_FreeSurface(pSpriteHerbeCoinHautD);
        SDL_FreeSurface(pSpriteHerbeCoinBasG);
        SDL_FreeSurface(pSpriteHerbeCoinBasD);
        //Eau
        SDL_FreeSurface(pSpriteEau);
        SDL_FreeSurface(pSpriteEauG);
        SDL_FreeSurface(pSpriteEauD);

        SDL_FreeSurface(pWinSurf);
        /*****FIN MODIF**********************************/
    }
    else
    {
        fprintf(stdout,"Échec de chargement du sprite (%s)\n",SDL_GetError());
    }

    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    return 0;
}
