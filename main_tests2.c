#include <assert.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "ecosys.h"

int main(void){
    Animal *liste_proie = NULL;
    Animal *liste_predateur = NULL;
    Animal *liste_proieTEST = NULL;
    Animal *liste_predateurTEST = NULL;
    srand(time(NULL));
    int energie = 10;
    for(int i=0;i<= 19;i++){
        int x = rand()%(SIZE_X);
        int y = rand()%(SIZE_Y);
        ajouter_animal(x,y,energie,&liste_proie);
    }
    for(int i=0;i<= 19;i++){
        int x = rand()%(SIZE_X);
        int y = rand()%(SIZE_Y);
        ajouter_animal(x,y,energie,&liste_predateur);
    }
    afficher_ecosys(liste_predateur,liste_proie);

    printf("Après lecture et écriture:\n");
    ecrire_ecosys("ecrire_eco.txt",liste_predateur,liste_proie); // ecrit dans le fichier eco.txt
    lire_ecosys("ecrire_eco.txt",&liste_predateurTEST,&liste_proieTEST);
    ecrire_ecosys("lire_ecosys.txt",liste_predateurTEST,liste_proieTEST); //lis dans eco.txt
    afficher_ecosys(liste_predateurTEST,liste_proieTEST); // affiche l'ecosystème lu


    liberer_liste_animaux(liste_predateur);
    liberer_liste_animaux(liste_proie);
    liberer_liste_animaux(liste_predateurTEST);
    liberer_liste_animaux(liste_proieTEST);
    return 0;
}