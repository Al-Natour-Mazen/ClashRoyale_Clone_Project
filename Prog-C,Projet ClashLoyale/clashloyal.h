#ifndef CLASHLOYAL_H_INCLUDED
#define CLASHLOYAL_H_INCLUDED

#include <stdio.h>
#define LARGEURJEU 11
#define HAUTEURJEU 19

typedef enum{tour, tourRoi, archer, chevalier, dragon, gargouille} TuniteDuJeu;
typedef enum{sol, solEtAir, air } Tcible;

/// un type enumeré pour un booleen //
typedef enum {False,True} Bool ;

typedef struct {
    TuniteDuJeu nom;
    Tcible cibleAttaquable;	//indique la position des unités que l’on peut attaquer
    Tcible maposition;		//indique soit « air » soit « sol », utile pour savoir
                            //qui peut nous attaquer
    int pointsDeVie;
    float vitesseAttaque;	//en seconde, plus c’est petit plus c’est rapide
    int degats;
    int portee ;			//en mètre, distance sur laquelle on peut atteindre une
                            //cible

    float vitessedeplacement;	//en m/s
    int posX, posY;			//position sur le plateau de jeu
    int peutAttaquer;		//permet de gérer le fait que chaque unité attaque une
                            //seule fois par tour ;
                            //0 = a déjà attaqué, 1 = peut attaquer ce tour-ci
                            // à remettre à 1 au début de chaque tour
    int coutEnElixir;
} Tunite;

typedef struct T_cell{
    struct T_cell *suiv;
    Tunite *pdata; //pointeur vers une unité
} *TListePlayer;


typedef Tunite* ** TplateauJeu;  ////tableau a deux dimensions de largeur 11 et hauteur 19 contenant des pointeurs (Tunite*)

/// ALLOCATION, INITIALISATION, AFFICHAGE ///

TplateauJeu AlloueTab2D(int largeur, int hauteur);
void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur);
void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur);

/// CREATIONS D'UNITE ///

Tunite *creeTour(int posx, int posy);
Tunite *creeTourRoi(int posx, int posy);
Tunite *creeArcher(int posx, int posy);
Tunite *creeGargouille(int posx, int posy);
Tunite *creeDragon(int posx, int posy);
Tunite *creeChevalier(int posx, int posy);

/// BOOL DE FIN DE GAME ///

Bool tourRoiDetruite(TListePlayer player);

/// ENVOIE LE JOUEUR SUR LE PLATEAU DE JEU ///

void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu);

/// RECUPERE LA LISTE DES UNITES QUI PEUVENT ATTAQUER UNE AUTRE UNITE ///

Bool estDansRayonDAttaque(Tunite *uniteQuiAttaque, Tunite *uniteQuiEstAttaque);
TListePlayer ajoutAPortee(TListePlayer player, Tunite *uneUnite);
TListePlayer quiEstAPortee(TListePlayer player, Tunite *uneUniteDeLautreJoueur);

/// TRI DE LA LISTE ///

int getLongueur(TListePlayer player);
TListePlayer* allocation_tab (int longueur);
TListePlayer* ListToTab (TListePlayer player, int taille);
float getSpeedAttack (Tunite unite);
TListePlayer* TriDuTableau (TListePlayer* tabplayer, int taille);
void DeleteList (TListePlayer player);
TListePlayer TabToList (TListePlayer* tabplayer, int taille);
TListePlayer TriDeListe (TListePlayer player);

/// ATTAQUE ///

void combat(TListePlayer player, Tunite *uneUniteDeLautreJoueur);
void attaque(TListePlayer playerattaquant,TListePlayer *playerdefenseur);
void resetattaque(TListePlayer *player);

/// AFFICHAGE ///

void afficheListeplayer(TListePlayer p);
void affichegagnant(int Joueur);
void afficheconsoleplayers(TListePlayer player1,TListePlayer player2,int *elixirplayer1, int *elixirplayer2);

/// SUPPRIMER UNE UNITE ///

void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite,TplateauJeu J);
Tunite *uniteaDetruire(TListePlayer *player);

/// AJOUTER UNE UNITE ///

Tunite *AcheteUnite(int *elixirEnStockduJoueur);
Bool estvideposition(TplateauJeu J, int x , int y);
void placetroupeplayer(TplateauJeu J,Tunite *unite, int Joueur);
void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite);

/// DEPLACER UNE UNITE ///

void moveoncase(TplateauJeu J,TListePlayer *player,  int Joueur);

/// FONCTION FINALE POUR LA CREATION D'UNITE ///

void createcard(TListePlayer *player,TplateauJeu J,int *elixirEnStockduJoueur,int Joueur);

/// CALCULE LA TAILLE DE LA LISTE ///

int taillelistejoueur(TListePlayer player);

/// CREER UNE UNITE POUR LES CHARGEMENTS ///

Tunite *creeunite(TuniteDuJeu nom,Tcible cibleAttaquable,Tcible maposition,int pointsDeVie,float vitesseAttaque,int degats,int portee,float vitessedeplacement,int posx, int posy, int peutAttaquer, int coutEnElixir);

/// SUPPRIMER DES LISTES (/!\ POUR LES CHARGEMENTS !) ///

TListePlayer DestroyplayerDeck(TListePlayer player);

/// SAUVEGARDE/CHARGEMENT VERSION SEQUENTIEL ///

void sauvegardeseq(TListePlayer player1,TListePlayer player2);

void chargersauvgardeseq(TListePlayer *player1,TListePlayer *player2);

/// SAUVEGARDE/CHARGEMENT VERSION BINAIRE ///

void sauvegarderBnr_bis(Tunite uniteASauv, FILE *F);
void sauvegarderBnr(TListePlayer player1, TListePlayer player2);

void chargerBnr(TListePlayer *player1, TListePlayer *player2);


#endif // CLASHLOYAL_H_INCLUDED
