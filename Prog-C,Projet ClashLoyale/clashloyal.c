#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#include <math.h>
#include "SDL.h"
#include "clashloyal.h"



/**************************************************************************/

                /// ALLOCATION, INITIALISATION, AFFICHAGE ///

/**************************************************************************/

TplateauJeu AlloueTab2D(int largeur, int hauteur) // Alloue le plateau en memoire
{
    TplateauJeu jeu;
    jeu = (Tunite***)malloc(sizeof(Tunite**)*largeur);
    for (int i=0;i<largeur;i++){
        jeu[i] = (Tunite**)malloc(sizeof(Tunite*)*hauteur);
    }
    return jeu;
}

void initPlateauAvecNULL(TplateauJeu jeu,int largeur, int hauteur) // Init chaque case de ce plateau a NULL, utilisation rare (debut et moments precis)
{
    for (int i=0;i<largeur;i++){
        for (int j=0;j<hauteur;j++){
            jeu[i][j] = NULL;
        }
    }
}

void affichePlateauConsole(TplateauJeu jeu, int largeur, int hauteur)
{
    //pour un affichage sur la console, en relation avec enum TuniteDuJeu
    const char* InitialeUnite[6]={"T", "R", "A", "C", "D", "G"};

    printf("\n|");
    for (int j=0;j<hauteur;j++){
        for (int i=0;i<largeur;i++){
            if (jeu[i][j] != NULL){
                    printf("%s",InitialeUnite[jeu[i][j]->nom]);
            }
            else printf(".");  //cad pas d'unité sur cette case
        }
        printf("|\n|");
    }
    printf("\r");
}



/**************************************************************************/

                          /// CREATIONS D'UNITE ///

/**************************************************************************/

// les fonctions basique de création des unités avec les statistiques données par le sujet
// a noter que nous avons changer certaines valeurs pour faire un peu d'equilibrage

Tunite *creeTour(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tour;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 150; //150
    nouv->vitesseAttaque = 1.0;
    nouv->degats = 40;
    nouv->portee = 3;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 0;
    return nouv;
}

Tunite *creeTourRoi(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = tourRoi;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 250; //250
    nouv->vitesseAttaque = 1.2;
    nouv->degats = 50;
    nouv->portee = 4;
    nouv->vitessedeplacement = 0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 0;
    return nouv;
}

Tunite *creeArcher(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = archer;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = sol;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.7;
    nouv->degats = 120;
    nouv->portee = 3;
    nouv->vitessedeplacement = 1.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 2;
    return nouv;
}

Tunite *creeGargouille(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = gargouille;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 80;
    nouv->vitesseAttaque = 0.6;
    nouv->degats = 90;
    nouv->portee = 1;
    nouv->vitessedeplacement = 3.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 1;
    return nouv;
}

Tunite *creeDragon(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = dragon;
    nouv->cibleAttaquable = solEtAir;
    nouv->maposition = air;
    nouv->pointsDeVie = 200;
    nouv->vitesseAttaque = 1.1;
    nouv->degats = 70;
    nouv->portee = 2;
    nouv->vitessedeplacement = 2.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 3;
    return nouv;
}

Tunite *creeChevalier(int posx, int posy)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = chevalier;
    nouv->cibleAttaquable = sol;
    nouv->maposition = sol;
    nouv->pointsDeVie = 400;
    nouv->vitesseAttaque = 1.5;
    nouv->degats = 250;
    nouv->portee = 1;
    nouv->vitessedeplacement = 2.0;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = 1;
    nouv->coutEnElixir = 4;
    return nouv;
}


/**************************************************************************/

                         /// BOOL DE FIN DE GAME ///

/**************************************************************************/

// Détection de la mort de la tour de roi
Bool tourRoiDetruite(TListePlayer player)
{
    TListePlayer liste = player; // on utilise une "copie" du player pour parcourir la liste pour détecter si la tour de roi est à 0 PV
    while(liste!=NULL)
    {
         if (liste->pdata->nom == tourRoi ) // Dans un premier temps on vérifie si c'est bien la tour de roi
        {
            if (liste->pdata->pointsDeVie <= 0) // Dans un second temps on vérifie si la tour de roi est bien à 0 PV ou en dessous pourquoi en dessous simplement parce que imaginons
                return True;                    // la tour est à 10 PV et on a une unité qui fait 20 de dégâts alors la tour va être à -10 PV si on laisse la condition à 0 alors la tour ne sera pas détecté
        }                                       // comme "morte" et donc il y'aura pas de fin de game
        liste = liste->suiv;
    }
    return False;
}




/**************************************************************************/

             /// ENVOIE LE JOUEUR SUR LE PLATEAU DE JEU ///

/**************************************************************************/


// Dans cette fonction on va récupérer les cartes du joueur et on va parcourir carte par carte dans un premier temp on récupère la position x et y de la carte
// qu'on va stocker dans des variables locales puis on utilise le plateau de jeu en utilisant ses variables pour placer la carte sur la map puis on passe à la
//carte suivante et ainsi de suite jusqu'à la fin.
void PositionnePlayerOnPlateau(TListePlayer player, TplateauJeu jeu)
{
    TListePlayer joueur = player;
    int positionX , positionY;
    while(joueur != NULL)
    {
        positionX = joueur->pdata->posX  ;
        positionY =  joueur->pdata->posY ;
        jeu[positionX][positionY] = joueur->pdata;
        joueur = joueur->suiv;
    }
}




/**************************************************************************/

  /// RECUPERE LA LISTE DES UNITES QUI PEUVENT ATTAQUER UNE AUTRE UNITE ///

/**************************************************************************/

Bool estDansRayonDAttaque(Tunite *uniteQuiAttaque, Tunite *uniteQuiEstAttaque)
{
    int ecart = sqrt(pow(uniteQuiAttaque->posX - uniteQuiEstAttaque->posX, 2)+pow(uniteQuiAttaque->posY - uniteQuiEstAttaque->posY,2)); //calcule la distance vectorielle entre les deux unites
    return (ecart <= uniteQuiAttaque->portee); //si l'unite qui est attaque est a portee de l'unite qui attaque on renvoie un booleen true
}

TListePlayer ajoutAPortee(TListePlayer player, Tunite *uneUnite) //fonction unique a cette partie qui ajoute directement une unite, si elle peut toucher une unite adverse donnee, a une liste
{
    TListePlayer nouv =(TListePlayer)malloc(sizeof(struct T_cell)); // rien de special dans ce code, fonction ajoutEnTete basique
    nouv->pdata = uneUnite;

    if (player == NULL)
    {
       nouv->suiv = NULL;
    }
    else
    {
        nouv->suiv = player;
    }
    return nouv;
}

TListePlayer quiEstAPortee(TListePlayer player, Tunite *uneUniteDeLautreJoueur) //on pourrait mettre le code ci-dessus directement dans cette fonction là mais j'ai prefere mettre un appel par soucis de lisibilite
{
    TListePlayer liste = player;
    TListePlayer aPortee = NULL; //on initialise une liste vide qui sera la liste de toutes les unites a portee de l'unite adverse, utile pour la fonction attaque et combat
    while (liste != NULL) //on va check toutes les unites de la liste player même si elles semblent trop eloignees
    {
        if (uneUniteDeLautreJoueur->maposition == sol)
        {
            if (liste->pdata->cibleAttaquable == sol || liste->pdata->cibleAttaquable == solEtAir)
            {
                if (estDansRayonDAttaque(liste->pdata, uneUniteDeLautreJoueur))
                {
                   aPortee= ajoutAPortee(aPortee, liste->pdata); //une unite a portee est donc une unite qui est capable d'attaquer l'unite cible et qui est a portee
                }
            }
        }
        else if (uneUniteDeLautreJoueur->maposition == air)
        {
            if (liste->pdata->cibleAttaquable == air || liste->pdata->cibleAttaquable == solEtAir)
            {
                if (estDansRayonDAttaque(liste->pdata, uneUniteDeLautreJoueur))
                {
                      aPortee= ajoutAPortee(aPortee, liste->pdata);
                }
            }
        }
        liste = liste->suiv;
    }

    return aPortee;
}

//// TRI DE LA LISTE ////

// Premiere fonction pour recup la longueur
// Deuxieme fonction pour allouer un tableau (important)
// Troisieme fonction qui injecte dans un tableau les pointeurs vers les cellules de la liste
// Quatrieme fonction qui trie le tableau (attention a recuperer les valeurs pointees)
// Cinquieme fonction qui supprime les liens entre les cellules pour pouvoir les relier avec la fonction suivante
// Sixieme fonction qui va creer une liste en enfilant les pointeurs a la chaine qui nous donnera la liste originale triee
// Derniere fonction qui fait appel a tout ca et qui sera appelee par le reste du jeu

int getLongueur(TListePlayer player){ // pour recuperer la longueur de la liste et avoir un tableau pas trop grand
    if (player == NULL) return 0;
    else return 1+getLongueur(player->suiv);
}

TListePlayer* allocation_tab (int longueur) // alloue un tableau car notre tableau sera utilise a plusieurs reprises
{
    TListePlayer* res = (TListePlayer*)malloc(longueur*sizeof(TListePlayer));
    return res;
}

TListePlayer* ListToTab (TListePlayer player, int taille) // fonction qui met dans un tableau les adresses des cellules de la liste donnee en parametre
{
    TListePlayer* tab = allocation_tab(taille);
    TListePlayer pointeur = player; //pointeur pour parcourir la liste
    int ind = 0;
    while (player != NULL){ //tant que la liste n'est pas vide
        tab[ind] = pointeur; //chaque case du tableau recupere un pointeur sur une cellule de la liste
        player = player->suiv;
        pointeur = player;
        ind++;
    }
    return tab; // on retourne ce tableau, on a donc une conversion de liste en tableau sans passer par une copie
}

float getSpeedAttack (Tunite unite)
{
    return unite.vitesseAttaque;
}

TListePlayer* TriDuTableau (TListePlayer* tabplayer, int taille) // fonction qui triera le tableau donnee en parametres, ici cas special : on recuperera un champ du pdata du pointeur de chaque case du taleau
{
    //On doit trier selon la vitesse d'attaque des differentes unites
    TListePlayer echange; //Variable de stockage pour la permutation de deux cellules
    int ind_stop = 0; //arretera le tri lorsque ind_stop sera egale a la taille du tableau
    Bool flag = False; //flag pour le cas ou si aucune permutation faite dans un parcours du tableau alors arret du tri car tableau trie

    while (ind_stop < taille && flag == False){//Si on peut tjrs permuter deux cases ou si lors du precedent parcours on a permute
        flag = True;
        for (int i = 0 ; i < taille - 1 ; i++){ //Tri a bulles optimise
            if (getSpeedAttack(*tabplayer[i]->pdata) > getSpeedAttack(*tabplayer[i+1]->pdata)) //on recupere donc la valeur pointee par le pointeur qui est dans la case du tableau, c'est une cellule d'une liste
            {
                flag = False;
                echange = tabplayer[i];
                tabplayer[i] = tabplayer[i+1];
                tabplayer[i+1] = echange;
            }
        }
        ind_stop++;
    }


    return tabplayer; //on retourne le tableau qui aura les cellules de la liste dans le bon ordre
}

// On peut continuer a operer avec un tableau mais pour eviter de changer la fonction combat (qui etait deja faite avant le tri) on va renfiler la liste correctement a l'aide du tableau
// C'est bien evidemment la solution la moins optimale puisque je pense qu'avec les fonction ci dessous je devrais etre capable de pouvoir trier une liste sans tableau directement
// Mais n'étant pas sur de moi, je prefere garder le tableau (surtout pour eviter des erreurs qui me prendront des heures a corriger ^^)

void DeleteList (TListePlayer player) //on coupe juste les liens qui relient les cellules, fonction importante car même si on remodele ces liens en renfilant, on aura des cellules dont le champ suiv sera toujours
                                      // attache a une cellule anterieure ce qui provoquerait une sorte de liste circulaire pas tres propre, donc autant tout couper maintenant
{
    TListePlayer player_suiv = player->suiv;
    while (player_suiv != NULL){ //tant que la cellule suivante n'est pas NULL
        player->suiv = NULL; //on coupe le lien qui relie la cellule a la suivante
        player = player_suiv; //on passe a la cellule suivante
        player_suiv = player_suiv->suiv; //la cellule suivante fait idem
    }
}

TListePlayer TabToList (TListePlayer* tabplayer, int taille) // Inverse de ListToTab, vu que notre tableau est un tableau de pointeurs vers des cellules, on renfile donc une nouvelle liste a l'aide de ces pointeurs
{
    TListePlayer res = NULL;
    TListePlayer ptr = NULL; // Variable importante! Avec seulement res et en retournant res, on aura uniquement la derniere cellule de la liste triee, ptr pointera donc sur la premiere cellule ajoutee

    res = tabplayer[0]; //pas de cas ou tabplayer est vide car verifiee dans la fonction generale
    ptr = res;

    for (int i = 1 ; i < taille ; i++){
        res->suiv = tabplayer[i]; //le champ suiv de la derniere cellule ajoutee pointe sur la cellule i
        res = res->suiv; //et ainsi de suite jusqu'a arriver au bout du tableau
    }

    return ptr; //on retourne cette variable et donc le pointeur sur la premiere cellule de notre liste remodelee
}

TListePlayer TriDeListe (TListePlayer player) // Fonction generale qui appelle toutes les fonctions au dessus
{
    if (player == NULL || player->suiv == NULL){
        return player; // si player vide ou avec un seul element, pas besoin de trier. Gagner optimisation : enlever cette partie et ajouter dans attaque pour eviter un appel de fonction
    }
    else {
        int tailleliste = getLongueur(player); // on recupere la taille de notre liste qui nous aidera beaucoup
        TListePlayer* tab_list = ListToTab(player, tailleliste); // on fait un tableau avec des cases de pointeurs sur les cellules de notre liste
        DeleteList(player); // on coupe les liens entre les cellules de la liste
        tab_list = TriDuTableau(tab_list, tailleliste); // on trie notre tableau
        player = TabToList(tab_list, tailleliste); // on renfile notre liste
        free(tab_list); //on delete notre tab qui ne nous servira plus pour eviter des fuites de memoire
        return player; // on retourne notre même liste mais remodelee
    }
}



/**************************************************************************/

                              /// ATTAQUE ///

/**************************************************************************/

//le cœur du combat
void combat(TListePlayer player, Tunite *uneUniteDeLautreJoueur)
{
    TListePlayer joueur = player; // on utilise une "copie" du player pour parcourir la liste pour le combat
    while(joueur != NULL)
    {
        int degat = joueur->pdata->degats ; ; // on récupère les dégâts de chaque carte dans une variable locale par soucis de lisibilité

        if (joueur->pdata->peutAttaquer == 1) // on vérifie si cette carte peutattaquer l'unité de l'autre joueur via le champ peutattaquer 1=il peut , 0 = ne peut pas
        {          // si la condition est vérifiée
             uneUniteDeLautreJoueur->pointsDeVie -= degat; ; // on soustrait le nbr de dégât au PV de l'unité de l'autre joueur
             joueur->pdata->peutAttaquer = 0; ; // et on oublie pas de mettre le peutattaquer à 0 pour l'unité qui vient d'attaquer
        }
        if(uneUniteDeLautreJoueur->pointsDeVie <= 0) // on vérifie si l'unité de l'autre joueur est morte ou pas si oui pas besoin de continuer le combat avec les autres troupes on sort directement de la boucle avec un break
        {
            break;
        }
        joueur = joueur->suiv;
    }
}

// la phase d'attaque où cette fonction appelle les autres fonctions combat/quiestaportee/trideliste pour bien effectuer la phase de combat entre 2 joueurs
void attaque(TListePlayer playerattaquant,TListePlayer *playerdefenseur) // en paramètre on récupère le joueur qui attaque et l'adresse du joueur qui va se faire attaquer(playerdefenseur) car c'est ce joueurs qui va prendre des dégâts et donc ses cartes vont être modifiées
{
    TListePlayer defense = *playerdefenseur; // on va parcourir la liste du joueur qui va se faire attaquer
    while(defense != NULL)
    {
        TListePlayer aportee = quiEstAPortee(playerattaquant,defense->pdata); // dans un premier temps on récupère la liste des troupes du joueurs attaquant qui sont à portée de l'unité du joueur défenseur
        aportee = TriDeListe(aportee); // on va les triée avec la fonction tri de liste
        combat(aportee,defense->pdata); //on appelle la fonction combat avec la liste qui a été produite pour bien effectué le cœur de l'action
        defense = defense->suiv; // et on refait ça avec chaque unité du joueur défenseur
    }
}

void resetattaque(TListePlayer *player) // dans cette fonction on récupère la liste d'un joueur pour remettre à 1 les unités qui ont déjà attaqué au précèdent tour
{
    TListePlayer joueur = *player;  // on parcourra la liste du joueur avec "joueur"
    while(joueur  != NULL)
    {
        if (joueur->pdata->peutAttaquer == 0) // pour éviter de modifie le peutattaquer de toutes les unités on va simplement vérifié si celle-ci est déjà à 0 ou pas
            joueur->pdata->peutAttaquer = 1; // si la condition est vérifiée alors on modifie le peutattaque comme ça la troupe peut attaquer de nouveau
        joueur = joueur ->suiv;
    }
}



/**************************************************************************/

                              /// AFFICHAGE ///

/**************************************************************************/

void afficheListeplayer(TListePlayer p)// fonction d'affichage pour chaque carte dans la liste du joueur
{
   const char* InitialeUnite[6]={"T", "R", "A", "C", "D", "G"};
   TListePlayer courant = p;
   if (p == NULL)  // si la liste est vide on affiche rien
   {
       return;
   }
   else // sinon on parcourt la liste avec courant et on affiche les infos nécessaire sur chaque carte
   {
       while (courant!=NULL)  // peutattaquer peut être compris d'une autre façon comme si l'entité pouvait ou pas attaquer à un moment donné or
        {                    //ce n'est pas vraiment le cas c'est plustôt est-ce que l'entité est en train d'attaquer ou pas c'est pour cela je mets en train d'attaquer pour afficher peutattaquer
            printf(" %s -> PV: %d, Degats: %d, EntrainAttaquer: %d\n", InitialeUnite[courant->pdata->nom], courant->pdata->pointsDeVie, courant->pdata->degats,courant->pdata->peutAttaquer);
            courant=courant->suiv;
        }
        printf("\n");
   }
}

void affichegagnant(int Joueur) // fonction d'affichage du gagnant dépendant du numéro entrée si 0 = joueur Haut , 1 = joueur du Bas
{
    /* la coloration de l'ecran : 0= noir / 1= bleu foncé / 2= vert / 3= bleu gris / 4= marron / 5= pourpre / 6= kaki / 7= gris claire / 8= gris / 9= bleu claire / B= cyan / C= rouge / D= rose / E= jaune / F= blanc  */
    int b = 219;
    if (Joueur == 0) // joueur 0 = joueur du HAUT
    {
        system("color 9");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);
        printf("\t\t\t\t\t %c%c LE JOUEUR BLEU A GAGNE, BRAVO A LUI %c%c\n",b,b,b,b);
        printf("\t\t\t\t\t %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }
    else if(Joueur == 1) // joueur 1 = joueur du BAS
    {
        system("color C");
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
        printf("\t\t\t\t\t %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);
        printf("\t\t\t\t\t %c%c LE JOUEUR ROUGE A GAGNE,BRAVO A LUI %c%c\n",b,b,b,b);
        printf("\t\t\t\t\t %c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);
        printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
    }
}

void afficheconsoleplayers(TListePlayer player1,TListePlayer player2,int *elixirplayer1, int *elixirplayer2) // fonction d'affichage de chaque joueur dans la console d'une façon propre
{
    TListePlayer p1 = player1;
    TListePlayer p2 = player2;
    int b = 205;
    system("color F"); // coloration de l'ecran de la console en blanc pour plus de visibilité
    printf("\n\n\n\n\n\n\n\n\n\n\n\n\n");
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);
    printf(" \tJoueurs Bleu \n\n");
    printf(" * Son Elixir -> %d\n\n",*(elixirplayer1)); // on affiche l'elixir
    printf(" * Ses Cartes :\n");
    afficheListeplayer(p1); // on affiche les cartes de chaque joueur en utilisant la fonction
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);
    printf(" \tJoueurs Rouge \n\n");
    printf(" * Son Elixir -> %d\n\n",*(elixirplayer2));
    printf(" * Ses Cartes : \n");
    afficheListeplayer(p2);
    printf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c\n",b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b,b);
    printf("\n\n\n\n\n");
}



/**************************************************************************/

                         /// SUPPRIMER UNE UNITE ///

/**************************************************************************/

void supprimerUnite(TListePlayer *player, Tunite *UniteDetruite,TplateauJeu J) // fonction de suppression d'une unité
{
    TListePlayer tmp = *player; // on utilise tmp pour supprimer par soucis de lisibilité
    if( tmp == NULL) // si la liste est vide alors on supprime rien
    {
        return;
    }
    else if (tmp->suiv == NULL && tmp->pdata == UniteDetruite) //la liste contient une seule unité et c'est cette unité qu'il faut supprimer
    {
        J[tmp->pdata->posX][tmp->pdata->posY]= NULL; // on met null sur le plateau à la position de l'unité pour pas avoir un fantôme sur la map
        free(tmp->pdata); // on libère cette unité
        free(tmp); // on libere la cellule
        *player = NULL; // et comme y'avait qu'une seule unité alors la liste devient null

    }
    else if ( tmp->pdata == UniteDetruite) // l'unité qu'on veut détruire se trouve directement à la 1er position de la liste
    {
        *player = tmp->suiv; // alors on change le pointeur player sur la 2eme cellule
        J[tmp->pdata->posX][tmp->pdata->posY]= NULL; // on met null sur le plateau à la position de l'unité pour pas avoir un fantôme sur la map
        free(tmp->pdata); // on libère cette unité
        free( tmp); // on libere la cellule
    }
    else  // l'unité se trouve à la Nieme position de la liste
    {
      TListePlayer precedent=tmp;       // on va se déplacer sur la liste avec 2 pointeurs 1 sur la 1ère cellule
      TListePlayer courant=tmp->suiv;   // et un autre sur la cellule qui suit juste après et on les déplace d'une carte à chaque fois
      while(courant != NULL)
      {
          if ( courant->pdata == UniteDetruite)  // si c'est l'unité qu'on veut supprimer
          {
                precedent->suiv = courant->suiv;        // on fait le branchement entre la carte avant l'unité et la carte apres cette unité ex: 1,5,3 on veut supprimer la cellule contenant 5 alors on va faire le branchement de la cellule de 1 avec la cellule 3
                J[courant->pdata->posX][courant->pdata->posY]= NULL; // on met null sur le plateau à la position de l'unité pour pas avoir un fantôme sur la map
                free(courant->pdata); // on libère cette unité
                free(courant); // on libere la cellule
                break;
          }
        precedent = courant; // on update precedent
        courant = courant->suiv; // on update courant
      }
    }
}

Tunite *uniteaDetruire(TListePlayer *player) // la fonction qui va chercher l'unité à détruire pour la détruire avec la fonction supprimerUnite
{
     TListePlayer tmp = *player; // on va parcourir la liste avec tmp
     Tunite *unitemorte = NULL; // on va créer une unité qu'on va renvoyer pour la détruire
     while(tmp != NULL) // on parcourt la liste
     {
        if(tmp->pdata->pointsDeVie <= 0) // si il y a une unité qui a 0 ou moins de PV alors on la récupère dans unitemorte
            unitemorte = tmp->pdata;  // on va récupérer que la dernière qui est morte par ex si à la cellule 1 et 3 y'a des unités à 0 ou moins de PV alors la fonction va récupérer l'unité se trouvant à la cellule 3 , en réalité cela ne posera pas de problème car il y'a qu'une seule unité qui meurt à la fois
        tmp = tmp->suiv;
     }
      return unitemorte; // on retourne cette unité
}


/**************************************************************************/

                         /// AJOUTER UNE UNITE ///

/**************************************************************************/
Tunite *AcheteUnite(int *elixirEnStockduJoueur)  // achat d'une unité
{
    Tunite *unite=NULL; // on crée cette variable pour stocker l'unité qu'on va acheter
    int random_unite;
    random_unite = (rand() %4);  // on va créer de l'aléatoire pour pas avoir toujours la même unité qui va spawn
    if (  (*elixirEnStockduJoueur) > 0 ) // on achètera seulement si l'elixir du joueur est strictement supérieur à 0
    {
        switch(random_unite)
        {
            case 0:
                if ((*elixirEnStockduJoueur) >= 2) // avant l'achat on vérifie si le joueur a assez de l'elixir pour acheter cette unité
                {
                    unite = creeArcher(1,0); // on va créer cette unité avec la fonction de création correspondante , a noter que lors de l'achat on place l'unité en 1,0 sur la map cela ne change rien car il y'a une fonction spécifique (placetroupeplayer) qui va placer cette troupe au bon endroit dépendant du quel joueur l'achet a été effectue
                    *elixirEnStockduJoueur -= 2; // on décrémente le prix de l'achat à l'elixir du joueur
                    break;
                }
            case 1:
                if ((*elixirEnStockduJoueur) >= 3)
                {
                    unite = creeDragon(1,0);
                    *elixirEnStockduJoueur -= 3;
                    break;
                }
            case 2:
                if ((*elixirEnStockduJoueur) >= 4)
                {
                    unite = creeChevalier(1,0);
                     *elixirEnStockduJoueur -= 4;
                     break;
                }
            case 3:
                if ((*elixirEnStockduJoueur) >= 1)
                {
                    unite = creeGargouille(1,0);
                    *elixirEnStockduJoueur -= 1;
                    break;
                }
        }
    }
    else if( (*elixirEnStockduJoueur) == 0 ) // si l'elixir du joueur est égale à 0 alors on renvoi NULL
    {
        return unite = NULL;
    }
    return unite;
}

Bool estvideposition(TplateauJeu J, int x , int y) // fonction de verification de position
{
    if( J[x][y] == NULL ) // on vérifie si une position est vide ou pas sur un plateau , utile pour la fonction placetroupeplayer et moveoncase
        return True;
    return False;
}

void placetroupeplayer(TplateauJeu J,Tunite *unite, int Joueur) //fonction de placement des troupes pour le bon joueur
{
    int posY=0;
    if (Joueur == 0)// 0 = joueur du haut
    {
        posY = 3; // alors le y de chaque troupe va être à 3
    }
    else if (Joueur == 1) // 1 = joueur du bas
    {
        posY = 15; // alors le y de chaque troupe va être à 15
    }
    unite->posY = posY;  //on place le bon y à l'unité achetée qui a été donnée en paramètre
    int positionrand = (rand()%6); // on va créer de l'aléatoire pour pas avoir toujours la même position où l'unité va spawn
    switch(positionrand)
    {
            case 0:
                if(estvideposition(J,2,posY)) // on vérifie si cette position est vide ou pas à l'aide de la fonction précédente
                {
                    unite->posX = 2 ; // si oui alors on attribue le bon x à l'unité et on break ainsi on a modifié les coordonnées de cette unité
                    break;
                }
            case 1:
                if(estvideposition(J,3,posY))
                {
                    unite->posX = 3 ;
                    break;
                }
            case 2:
                if(estvideposition(J,4,posY))
                {
                    unite->posX = 4 ;
                    break;
                }
            case 3:
                if(estvideposition(J,6,posY))
                {
                    unite->posX = 6 ;
                    break;
                }
            case 4:
                if(estvideposition(J,7,posY))
                {
                    unite->posX = 7 ;
                    break;
                }
            case 5:
                if(estvideposition(J,8,posY))
                {
                    unite->posX = 8 ;
                    break;
                }
        }
}

void AjouterUnite(TListePlayer *player, Tunite *nouvelleUnite) //fonction d'ajout de l'unité à la liste du joueur
{               // maintenant que l'achat et le placement de l'unité acheté s'est effectué il faut l'ajouter dans la liste du joueur
    if(nouvelleUnite != NULL) // on ajoutera seulement s’il y a une unité créer
    {
        TListePlayer nouv =(TListePlayer)malloc(sizeof(struct T_cell)); // on réserve de la place mémoire pour la cellule
        nouv->pdata = nouvelleUnite; // on ajoute cette nouvelle unité dans le champ pdata de la cellule

        nouv->suiv =*player; // on branche cette cellule au reste de la liste , à noter qu'on fait un ajout en tête pour la liste
        *player = nouv ; // on update le pointeur sur la 1ère cellule
    }
    else // sinon on fait rien
        return;
}



/**************************************************************************/

                        /// DEPLACER UNE UNITE ///

/**************************************************************************/

void moveoncase(TplateauJeu J,TListePlayer *player , int Joueur ) // fonction de déplacement
{
    TListePlayer liste = *player; // pour déplacer les unités on sera obligé de parcourir la liste pour modifier les coordonnées à chaque tour
    while(liste != NULL)
    {                   // à noter qu'on va utiliser des variables locales par soucis de lisibilité
        float vitessedeplacement = liste->pdata->vitessedeplacement; // on va récupérer à chaque tour la vitesse de déplacement de l'unité qu'on va déplacer
        int peutattaquer = liste->pdata->peutAttaquer;  // on va récupérer à chaque tour le champ peutAttaquer de l'unité qu'on va déplacer
        int posX = liste->pdata->posX;  // on va récupérer à chaque tour la positon X de l'unité qu'on va déplacer
        int posY = liste->pdata->posY;  // on va récupérer à chaque tour la positon Y de l'unité qu'on va déplacer
       if( (vitessedeplacement > 0.0 ) && (peutattaquer != 0 )  ) // on vérifie si la vitesse de déplacement est supérieure à 0 pour éviter de déplacer les tours , et on vérifie le peut attaquer s'il est différent de 0 car si c'est égal à 0 alors l'unité est en train d'attaquer et donc on ne va pas la bouger
       {
           if (Joueur == 0 )   // on vérifie quel joueur , 0 = joueur du haut pour effectuer le bon déplacement pour le joueur du haut on va déplacer les unités vers le bas
           {
                if( posX > 5 && (estvideposition(J,posX-1,posY+1)) )  // on vérifie si le X est supérieur à 5 cela veut dire on est à droite de la route centrale et on vérifie si la position x-1 et y+1 est vide pour pouvoir se déplacer
                {
                    posX -= 1;
                    posY += 1;
                }
                else if( posX < 5 && (estvideposition(J,posX+1,posY+1)) ) // on vérifie si le X est inférieur à 5 cela veut dire qu'on est à gauche de la route centrale et on vérifie si la position x+1 et y+1 est vide pour pouvoir se déplacer
                {
                    posX += 1;
                    posY += 1;
                }
                else if(posX == 5 && (estvideposition(J,posX,posY+1)) ) // on vérifie si le X est égal à 5 cela veut dire on est sur la route centrale et on vérifie si la position y+1 est vide pour pouvoir se déplacer
                {
                    posY += 1;
                }
           }
           else if (Joueur == 1  ) // 1 = joueur du bas , idem pour le joueur du bas à la seule différence que les unités se déplacent vers le haut
           {
                if( posX > 5  && (estvideposition(J,posX-1,posY-1)))
                {
                    posX -= 1;
                    posY -= 1;
                }
                if( posX < 5   && (estvideposition(J,posX+1,posY-1)) )
                {
                    posX += 1;
                    posY -= 1;
                }

                if(posX == 5 && (estvideposition(J,posX,posY-1)) )
                {
                    posY -= 1;
                }
           }
            J[liste->pdata->posX][liste->pdata->posY] = NULL; // on va mettre null à l'ancienne position de l'unité ou sinon le risque d'avoir des unité fantômes
            liste->pdata->posX = posX; // on update la position x de l'unité
            liste->pdata->posY = posY; // on update la position y de l'unité
            PositionnePlayerOnPlateau(liste,J); // et on la place sur la map
       }
        liste = liste->suiv;
    }
}



/**************************************************************************/

             /// FONCTION FINALE POUR LA CREATION D'UNITE ///

/**************************************************************************/

void createcard(TListePlayer *player,TplateauJeu J,int *elixirEnStockduJoueur,int Joueur) // la fonction principale qui va gérer l'achat le placement et l'ajout des unités
{
        int probacreation = (rand()%5); // on va créer de l'aléatoire pour créer une unité, 1 chance sur 5 est le bon ratio pour pas avoir trop de troupes sur la map
        if(probacreation == 1)
        {
            if ((*elixirEnStockduJoueur) >= 15) // on va réguler l'elixir du joueur pour pas qu'il dépasse le 15
            {
                (*elixirEnStockduJoueur) = 10;
            }
            else if( (*elixirEnStockduJoueur) > 0 ) // cette condition est seulement par sécurité vu que dans les autres fonction notamment l'achat on revérifie l'élixir ça évitera d'appeler les autres fonctions si on a pas assez de l'élixir
            {
                (*elixirEnStockduJoueur) += (rand()%5); // si on a vérifié la condition alors on va forcément acheté une unité et donc on redonne de l'elixir de façon aléatoire au joueur
                Tunite *unitealea = AcheteUnite(elixirEnStockduJoueur); // on appelle achete unite
                placetroupeplayer(J,unitealea,Joueur); // on place l'unité acheté par rapport au INT Joueur 0 = Haut , 1 = Bas
                AjouterUnite(player,unitealea); // on ajoute cette unité à la liste du joueur
            }
            else if( (*elixirEnStockduJoueur) <= 0 ) // on va réguler  l'elixir du joueur pour pas qu'il passe sous le 0
            {
                (*elixirEnStockduJoueur) += 10;
            }
        }
}



/**************************************************************************/

                  /// CALCULE LA TAILLE DE LA LISTE ///

/**************************************************************************/

int taillelistejoueur(TListePlayer player)
{
    int res=0;
    while(player != NULL) // on va parcourir la liste et incrémenter la variable res à chaque cellule pour nous donner la taille de cette liste
    {
        res++;
        player = player->suiv;
    }
    return res ;
}



/**************************************************************************/

             /// CREER UNE UNITE POUR LES CHARGEMENTS ///

/**************************************************************************/

// fonction de création d'une unité pour les fonctions de rechargement en récupérant les informations nécessaires à la création d'une unité
Tunite *creeunite(TuniteDuJeu nom,Tcible cibleAttaquable,Tcible maposition,int pointsDeVie,float vitesseAttaque,int degats,int portee,float vitessedeplacement,int posx, int posy, int peutAttaquer, int coutEnElixir)
{
    Tunite *nouv = (Tunite*)malloc(sizeof(Tunite));
    nouv->nom = nom;
    nouv->cibleAttaquable = cibleAttaquable;
    nouv->maposition = maposition;
    nouv->pointsDeVie = pointsDeVie;
    nouv->vitesseAttaque = vitesseAttaque;
    nouv->degats = degats;
    nouv->portee = portee;
    nouv->vitessedeplacement = vitessedeplacement;
    nouv->posX = posx;
    nouv->posY = posy;
    nouv->peutAttaquer = peutAttaquer;
    nouv->coutEnElixir = coutEnElixir;
    return nouv;
}



/**************************************************************************/

        /// SUPPRIMER DES LISTES (/!\ POUR LES CHARGEMENTS !) /////

/**************************************************************************/

TListePlayer DestroyplayerDeck(TListePlayer player) //détruire entièrement un deck d'un joueur pour libérer de la mémoire avant les chargements sinon dédoublement de liste...
{
    if(player == NULL) // si la liste est vide alors on retourne NULL
        return NULL;
    else
    {
        TListePlayer deuxiemeCell = NULL; // on initialise deuxiemecell
        while(player != NULL)// on parcourt la liste avec player
        {
            deuxiemeCell = player->suiv;
            free(player->pdata);
            free(player);
            player = deuxiemeCell;
        }
        return player;
    }
}



/**************************************************************************/

             /// SAUVEGARDE/CHARGEMENT VERSION SEQUENTIEL ///

/**************************************************************************/

void sauvegardeseq(TListePlayer player1,TListePlayer player2) // fonction de sauvegarde d'un fichier séquentiel
{
    FILE *F = fopen("./Sauvegardes/PartieSeq.clc","w"); // on ouvre un fichier pour écrire dedans

    if (F==NULL) // on vérifie si l'ouverture est bien passé
    {
        printf("\n\tLa Sauvegarde n'as pas pu etre Faite, Sauvegarde impossible !\n");
        exit(1);
    }

    //on imprime l'entête du fichier
    fprintf(F,"ClashLoyale\n");

    fprintf(F,"FichierSequentiel\n");

     //on imprime la longueur de la liste du joueur cela sera utile pour parcourir la liste lors du chargement
    fprintf(F,"Joueur1,len:%d\n",taillelistejoueur(player1));
    while(player1 != NULL ) // on imprime la liste du 1er joueur
    {
      fprintf(F,"nom:%d ",player1->pdata->nom);
      fprintf(F,"cibleAttaquable:%d ",player1->pdata->cibleAttaquable);
      fprintf(F,"maposition:%d ",player1->pdata->maposition);
      fprintf(F,"pointsDeVie:%d ",player1->pdata->pointsDeVie);
      fprintf(F,"vitesseAttaque:%f ",player1->pdata->vitesseAttaque);
      fprintf(F,"degats:%d ",player1->pdata->degats);
      fprintf(F,"portee:%d ",player1->pdata->portee);
      fprintf(F,"vitessedeplacement:%f ",player1->pdata->vitessedeplacement);
      fprintf(F,"posX:%d ",player1->pdata->posX);
      fprintf(F,"posY:%d ",player1->pdata->posY);
      fprintf(F,"peutAttaquer:%d ",player1->pdata->peutAttaquer);
      fprintf(F,"coutEnElixir:%d ",player1->pdata->coutEnElixir);
      fprintf(F,"\n");
      player1 = player1->suiv;
    }

    //on imprime la longueur de la liste du joueur cela sera utile pour parcourir la liste lors du chargement
    fprintf(F,"Joueur2,len:%d\n",taillelistejoueur(player2));
    while(player2 != NULL )// on imprime la liste du 2eme joueur
    {
      fprintf(F,"nom:%d ",player2->pdata->nom);
      fprintf(F,"cibleAttaquable:%d ",player2->pdata->cibleAttaquable);
      fprintf(F,"maposition:%d ",player2->pdata->maposition);
      fprintf(F,"pointsDeVie:%d ",player2->pdata->pointsDeVie);
      fprintf(F,"vitesseAttaque:%f ",player2->pdata->vitesseAttaque);
      fprintf(F,"degats:%d ",player2->pdata->degats);
      fprintf(F,"portee:%d ",player2->pdata->portee);
      fprintf(F,"vitessedeplacement:%f ",player2->pdata->vitessedeplacement);
      fprintf(F,"posX:%d ",player2->pdata->posX);
      fprintf(F,"posY:%d ",player2->pdata->posY);
      fprintf(F,"peutAttaquer:%d ",player2->pdata->peutAttaquer);
      fprintf(F,"coutEnElixir:%d ",player2->pdata->coutEnElixir);
      fprintf(F,"\n");
      player2 = player2->suiv;
    }

    fclose(F); // on ferme le fichier
}

void chargersauvgardeseq(TListePlayer *player1,TListePlayer *player2) // fonction de chargement d'un fichier séquentiel
{
    // on met les 2 listes des joueurs à null par sécurité , car dans le main avant d'appeler cette fonction on a supprimé la liste de chaque joueur
    *player1 = NULL;
    *player2 = NULL;

    FILE *F = fopen("./Sauvegardes/PartieSeq.clc","r");

    if (F==NULL) // on vérifie si l'ouverture est bien passé
    {
        printf("\n\tLe Chargement n'as pas pu etre Faite, Chargement  impossible !\n");
        exit(1);
    }

    //on lit l'entête du fichier
    fscanf(F,"ClashLoyale\n");

    fscanf(F,"FichierSequentiel\n");

    //on lit la longueur pour pouvoir utiliser une boucle for pour créer la liste
    int len=1;
    fscanf(F,"Joueur1,len:%d\n",&len);
    for(int i=1;i<=len;i++)
    {
      // on créer des variables locales oὺ on va stocker chaque info de l’unité qu’on veut créer
      int nom=0,cibleAttaquable=0, maposition=0,pointsDeVie=0,degats=0,portee=0,posX=0,posY=0,peutAttaquer=0,coutEnElixir=0;
      float vitesseAttaque=0.0,vitessedeplacement=0.0;

      fscanf(F,"nom:%d ",&nom);
      fscanf(F,"cibleAttaquable:%d ",&cibleAttaquable);
      fscanf(F,"maposition:%d ",&maposition);
      fscanf(F,"pointsDeVie:%d ",&pointsDeVie);
      fscanf(F,"vitesseAttaque:%f ",&vitesseAttaque);
      fscanf(F,"degats:%d ",&degats);
      fscanf(F,"portee:%d ",&portee);
      fscanf(F,"vitessedeplacement:%f ",&vitessedeplacement);
      fscanf(F,"posX:%d ",&posX);
      fscanf(F,"posY:%d ",&posY);
      fscanf(F,"peutAttaquer:%d ",&peutAttaquer);
      fscanf(F,"coutEnElixir:%d ",&coutEnElixir);
      // on va utiliser ses variables pour créer chaque unité avec la fonction creeunite
      Tunite *unitecree =creeunite(nom,cibleAttaquable,maposition,pointsDeVie,vitesseAttaque,degats,portee,vitessedeplacement,posX,posY,peutAttaquer,coutEnElixir);
      // ensuite on l'ajoute à la liste du joueur
      AjouterUnite(player1,unitecree);
    }

    // idem pour le joueur 2 , même principe que le joueur 1
    int len2=1;
    fscanf(F,"Joueur2,len:%d\n",&len2);
    for(int i=1;i<=len2;i++)
    {
      int nom,cibleAttaquable, maposition,pointsDeVie,degats,portee,posX,posY,peutAttaquer,coutEnElixir;
      float vitesseAttaque,vitessedeplacement;

      fscanf(F,"nom:%d ",&nom);
      fscanf(F,"cibleAttaquable:%d ",&cibleAttaquable);
      fscanf(F,"maposition:%d ",&maposition);
      fscanf(F,"pointsDeVie:%d ",&pointsDeVie);
      fscanf(F,"vitesseAttaque:%f ",&vitesseAttaque);
      fscanf(F,"degats:%d ",&degats);
      fscanf(F,"portee:%d ",&portee);
      fscanf(F,"vitessedeplacement:%f ",&vitessedeplacement);
      fscanf(F,"posX:%d ",&posX);
      fscanf(F,"posY:%d ",&posY);
      fscanf(F,"peutAttaquer:%d ",&peutAttaquer);
      fscanf(F,"coutEnElixir:%d ",&coutEnElixir);
      Tunite *unitecree =creeunite(nom,cibleAttaquable,maposition,pointsDeVie,vitesseAttaque,degats,portee,vitessedeplacement,posX,posY,peutAttaquer,coutEnElixir);
      AjouterUnite(player2,unitecree);
    }
    fclose(F);
}



/**************************************************************************/

              /// SAUVEGARDE/CHARGEMENT VERSION BINAIRE ///

/**************************************************************************/

void sauvegarderBnr_bis(Tunite uniteASauv, FILE *F) //Pouvait etre directement mis dans le code principal mais par souci de lisibilite, passe par une fonction aux
{
    fwrite(&uniteASauv.nom, sizeof(int), 1, F); //ecrit chaque champ de l'unite en parametre dans le fichier
    fwrite(&uniteASauv.cibleAttaquable, sizeof(int), 1, F);
    fwrite(&uniteASauv.maposition, sizeof(int), 1, F);
    fwrite(&uniteASauv.pointsDeVie, sizeof(int), 1, F);
    fwrite(&uniteASauv.vitesseAttaque, sizeof(float), 1, F);
    fwrite(&uniteASauv.degats, sizeof(int), 1, F);
    fwrite(&uniteASauv.portee, sizeof(int), 1, F);
    fwrite(&uniteASauv.vitessedeplacement, sizeof(float), 1, F);
    fwrite(&uniteASauv.posX, sizeof(int), 1, F);
    fwrite(&uniteASauv.posY, sizeof(int), 1, F);
    fwrite(&uniteASauv.peutAttaquer, sizeof(int), 1, F);
    fwrite(&uniteASauv.coutEnElixir, sizeof(int), 1, F);
}

void sauvegarderBnr(TListePlayer player1, TListePlayer player2) //Fonction de sauvegarde en binaire
{
    FILE *f_out = fopen("./Sauvegardes/PartieBin.clb","w+b");
    TListePlayer player1_b = player1;
    TListePlayer player2_b = player2;
    int taille_player1_b = taillelistejoueur(player1_b); // Recupere les tailles des deux listes de joueur (important pour le chargement pour savoir quand s'arreter
    int taille_player2_b = taillelistejoueur(player2_b);

    if (f_out == NULL){
        printf("ouverture impossible ! \n");
        exit(1);
    }

    fwrite(&taille_player1_b, sizeof(int), 1, f_out); // on ecrit donc la taille de la liste du joueur 1 et on enchaine direct avec chaque champ de chaque unite

    while (player1_b != NULL){
        sauvegarderBnr_bis(*(player1_b->pdata), f_out);
        player1_b = player1_b->suiv;
    }

    fwrite(&taille_player2_b, sizeof(int), 1, f_out); // on ecrit donc la taille de la liste du joueur 2 et on enchaine direct avec chaque champ de chaque unite

    while (player2_b != NULL){
        sauvegarderBnr_bis(*(player2_b->pdata), f_out);
        player2_b = player2_b->suiv;
    }

    fclose(f_out);
}

void chargerBnr(TListePlayer *player1, TListePlayer *player2) // Fonction de chargement en binaire
{
    FILE *f_in = fopen("./Sauvegardes/PartieBin.clb","r+b");
    int taille_player1 = 0; // on init chaque champ possible pour les utiliser apres dans la creation d'une unite correspondante
    int taille_player2 = 0;
    int unite_nom = 0;
    int unite_cible = 0;
    int unite_position = 0;
    int unite_pv = 0;
    float unite_vitA = 0.0;
    int unite_dgts = 0;
    int unite_port = 0;
    float unite_vitD = 0.0;
    int unite_posX = 0;
    int unite_posY = 0;
    int unite_peutAttk = 0;
    int unite_coutElx = 0;

    if (f_in == NULL){
        printf("ouverture impossible ! \n");
        exit(1);
    }

    fread(&taille_player1, sizeof(int), 1, f_in); // lire le premier champ pour recup la taille de la liste du joueur 1 (donc savoir quand s'arreter pour passer au joueur 2)

    for (int i = 0 ; i < taille_player1 ; i++){ // on parcourt le fichier jusqu'a la derniere unite du joueur 1 sauvegarde
        fread(&unite_nom, sizeof(int), 1, f_in); // on recupere chaque champ de l'unite
        fread(&unite_cible, sizeof(int), 1, f_in);
        fread(&unite_position, sizeof(int), 1, f_in);
        fread(&unite_pv, sizeof(int), 1, f_in);
        fread(&unite_vitA, sizeof(float), 1, f_in);
        fread(&unite_dgts, sizeof(int), 1, f_in);
        fread(&unite_port, sizeof(int), 1, f_in);
        fread(&unite_vitD, sizeof(float), 1, f_in);
        fread(&unite_posX, sizeof(int), 1, f_in);
        fread(&unite_posY, sizeof(int), 1, f_in);
        fread(&unite_peutAttk, sizeof(int), 1, f_in);
        fread(&unite_coutElx, sizeof(int), 1, f_in); // puis on va creer l'unite avec toutes ces variables
        Tunite *unitecree =creeunite(unite_nom,unite_cible, unite_position, unite_pv,unite_vitA, unite_dgts,unite_port,unite_vitD,unite_posX,unite_posY,unite_peutAttk,unite_coutElx);
        AjouterUnite(player1,unitecree); // on ajoute cette unite au player1
    }

    fread(&taille_player2, sizeof(int), 1, f_in); // a la fin de cette boucle, on est cense arriver a ce qui nous indique la taille de la liste du joueur 2 (donc idem pour s'arreter)

    for (int i = 0 ; i < taille_player2 ; i++){ // on parcourt le fichier jusqu'a la derniere unite du joueur 2 sauvegarde
        fread(&unite_nom, sizeof(int), 1, f_in); // on recupere chaque champ de l'unite
        fread(&unite_cible, sizeof(int), 1, f_in);
        fread(&unite_position, sizeof(int), 1, f_in);
        fread(&unite_pv, sizeof(int), 1, f_in);
        fread(&unite_vitA, sizeof(float), 1, f_in);
        fread(&unite_dgts, sizeof(int), 1, f_in);
        fread(&unite_port, sizeof(int), 1, f_in);
        fread(&unite_vitD, sizeof(float), 1, f_in);
        fread(&unite_posX, sizeof(int), 1, f_in);
        fread(&unite_posY, sizeof(int), 1, f_in);
        fread(&unite_peutAttk, sizeof(int), 1, f_in);
        fread(&unite_coutElx, sizeof(int), 1, f_in); // puis on va creer l'unite avec toutes ces variables
        Tunite *unitecree =creeunite(unite_nom,unite_cible, unite_position, unite_pv,unite_vitA, unite_dgts,unite_port,unite_vitD,unite_posX,unite_posY,unite_peutAttk,unite_coutElx);
        AjouterUnite(player2,unitecree); // on ajoute cette unite au player1
    }

    fclose(f_in);
}



