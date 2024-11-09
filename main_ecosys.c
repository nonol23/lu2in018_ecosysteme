#include <assert.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <strings.h>
#include "ecosys.h"



#define NB_PROIES 20
#define NB_PREDATEURS 20
#define T_WAIT 40000





int main(void) {
  srand(time(NULL));
  int i=0;
  int energieproie=10;
  int energiepredateur=10;
  Animal *liste_proie = NULL;
  Animal *liste_predateur = NULL;

  int monde[SIZE_X][SIZE_Y];
  for(int i=0;i<SIZE_X;i++){
    for(int j=0;j<SIZE_Y;j++){
      monde[i][j]=0;  
    }
  }
  
  for(int i=0;i<NB_PROIES;i++){
        int x = rand()%(SIZE_X);
        int y = rand()%(SIZE_Y);
        ajouter_animal(x,y,energieproie,&liste_proie);
  }
  for(int i=0;i<NB_PREDATEURS;i++){
        int x = rand()%(SIZE_X);
        int y = rand()%(SIZE_Y);
        ajouter_animal(x,y,energiepredateur,&liste_predateur);
  }
  
  clear_screen();

  FILE *f = fopen("data.txt", "w");
  
  while(liste_proie && liste_predateur){
    rafraichir_proies(&liste_proie,monde);
    afficher_ecosys(liste_proie,liste_predateur);
    usleep(T_WAIT);
    clear_screen();
    
    rafraichir_predateurs(&liste_predateur,&liste_proie);
    afficher_ecosys(liste_proie,liste_predateur);
    usleep(T_WAIT);
    clear_screen();

    rafraichir_monde(monde);

    i++;
    fprintf(f,"%d %d %d\n",i,compte_animal_it(liste_proie),compte_animal_it(liste_predateur));

  }
  
  fclose(f);

  liste_predateur = liberer_liste_animaux(liste_predateur);
  liste_proie = liberer_liste_animaux(liste_proie);
  return 0;
}

