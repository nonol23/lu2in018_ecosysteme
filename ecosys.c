#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ecosys.h"

float p_ch_dir=0.1;
float p_reproduce_proie=0.3;
float p_reproduce_predateur=0.2;
int temps_repousse_herbe=-10;

/* PARTIE 1*/
/* Fourni: Part 1, exercice 4, question 2 */
Animal *creer_animal(int x, int y, float energie) {
  Animal *na = (Animal *)malloc(sizeof(Animal));
  assert(na);

  na->x = x;
  na->y = y;
  na->energie = energie;
  na->dir[0] = rand() % 3 - 1;
  na->dir[1] = rand() % 3 - 1;
  na->suivant = NULL;
  return na;
}


/* Fourni: Part 1, exercice 4, question 3 */
Animal *ajouter_en_tete_animal(Animal *liste, Animal *animal) {
  assert(animal);
  assert(!animal->suivant);
  animal->suivant = liste;
  return animal;
}

/* A faire. Part 1, exercice 6, question 2 */
void ajouter_animal(int x, int y,  float energie, Animal **liste_animal) {
  assert(x>=0 && y >=0);
  assert(x < SIZE_X);
  assert(y<SIZE_Y);
  Animal *nv = creer_animal(x,y,energie);
  assert(nv);
  assert (nv -> suivant == NULL);
  *liste_animal = ajouter_en_tete_animal(*liste_animal,nv);
}
/* A Faire. Part 1, exercice 5, question 5 */
void enlever_animal(Animal **liste, Animal *animal) {
  Animal *courant = *liste;
  Animal *precedent = NULL;
  while(courant != animal && courant != NULL){
    precedent = courant;
    courant = courant->suivant;
  }
  if(courant != NULL){
    if(precedent==NULL){
      *liste=courant->suivant;
    }
    else{
      precedent->suivant = courant->suivant;
    }
    free(courant);
  }
}

/* A Faire. Part 1, exercice 6, question 7 */
Animal* liberer_liste_animaux(Animal *liste) {
  Animal *tmp = liste;
  while(tmp){
    Animal *suiv = tmp->suivant;
    free(tmp);
    tmp=suiv;
  }

  return NULL;
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_rec(Animal *la) {
  if (!la) return 0;
  return 1 + compte_animal_rec(la->suivant);
}

/* Fourni: part 1, exercice 4, question 4 */
unsigned int compte_animal_it(Animal *la) {
  int cpt=0;
  while (la) {
    ++cpt;
    la=la->suivant;
  }
  return cpt;
}



/* Part 1. Exercice 5, question 1, ATTENTION, ce code est susceptible de contenir des erreurs... */
void afficher_ecosys(Animal *liste_proie, Animal *liste_predateur) {
  unsigned int i, j;
  char ecosys[SIZE_X][SIZE_Y];
  Animal *pa=NULL;

  /* on initialise le tableau */
  for (i = 0; i < SIZE_X; ++i) {
    for (j = 0; j < SIZE_Y; ++j) {
      ecosys[i][j]=' ';
    }
  }

  /* on ajoute les proies */
  pa = liste_proie;
  while (pa) {
    ecosys[pa->x][pa->y] = '*';
    pa=pa->suivant;
  }

  /* on ajoute les predateurs */
  pa = liste_predateur;
  while (pa) {
      if ((ecosys[pa->x][pa->y] == '@') || (ecosys[pa->x][pa->y] == '*')) { /* proies aussi present */
        ecosys[pa->x][pa->y] = '@';
      } else {
        ecosys[pa->x][pa->y] = 'O';
      }
    pa = pa->suivant;
  }

  /* on affiche le tableau */
  printf("+");
  for (j = 0; j < SIZE_Y; ++j) {
    printf("-");
  }  
  printf("+\n");
  for (i = 0; i < SIZE_X; ++i) {
    printf("|");
    for (j = 0; j < SIZE_Y; ++j) {
      putchar(ecosys[i][j]);
    }
    printf("|\n");
  }
  printf("+");
  for (j = 0; j<SIZE_Y; ++j) {
    printf("-");
  }
  printf("+\n");
  int nbproie=compte_animal_it(liste_proie);
  int nbpred=compte_animal_it(liste_predateur);
  
  printf("Nb proies : %5d\tNb predateurs : %5d\n", nbproie, nbpred);

}


void clear_screen() {
  printf("\x1b[2J\x1b[1;1H");  /* code ANSI X3.4 pour effacer l'ecran */
}

/* PARTIE 2*/

/* Part 2. Exercice 4, question 1 */
void bouger_animaux(Animal *la) {
    Animal *tmp=la;
    while(tmp){
      if(rand() / (float)RAND_MAX < p_ch_dir){
        tmp->dir[0]=rand()%3-1;
        tmp->dir[1]=rand()%3-1;
      }
      tmp->x = (tmp->x + tmp->dir[0] + SIZE_X) % SIZE_X;
      tmp->y = (tmp->y + tmp->dir[1] + SIZE_Y) % SIZE_Y;
      tmp = tmp->suivant;

    }

}

/* Part 2. Exercice 4, question 3 */
void reproduce(Animal **liste_animal, float p_reproduce) {
  Animal *tmp = *liste_animal;
  while(tmp){
    if(rand() / (float)RAND_MAX < p_reproduce){
      ajouter_animal(tmp->x,tmp->y,(tmp->energie/2),liste_animal);
      //printf("Reproduction!\n");
      tmp->energie /= 2;
    }
    tmp = tmp->suivant;
  }
}




/* Part 2. Exercice 6, question 1 */
void rafraichir_proies(Animal **liste_proie, int monde[SIZE_X][SIZE_Y]) {
    Animal *tmp = *liste_proie;
    bouger_animaux(tmp);
    //printf("Les proies se déplacent.\n");
    while(tmp != NULL){
      tmp->energie -= 1;
      int herbe =  monde[tmp->x][tmp->y];
      if(herbe>0){
        tmp->energie+= herbe;
        monde[tmp->x][tmp->y]=temps_repousse_herbe;
      }
      if(tmp->energie < 0){
        Animal *suppr=tmp;
        tmp=tmp->suivant;
        enlever_animal(liste_proie,suppr);
        //printf("Une proie est morte.\n");
      }
      else{
      tmp=tmp->suivant;
      }
    }
    reproduce(liste_proie,p_reproduce_proie);

}

/* Part 2. Exercice 7, question 1 */
Animal *animal_en_XY(Animal *l, int x, int y) {
    Animal *tmp = l;
    while(tmp){
      if(tmp->x==x && tmp->y==y){
        return tmp;
      }
      tmp=tmp->suivant;
    }

  return NULL;
} 

/* Part 2. Exercice 7, question 2 */
void rafraichir_predateurs(Animal **liste_predateur, Animal **liste_proie) {
  Animal *tmp = *liste_predateur;
  Animal *proie = *liste_proie;
    bouger_animaux(tmp);
    //printf("Les predateurs se déplacent.\n");
    while(tmp){
      tmp->energie -= 1;
      while(animal_en_XY(proie,tmp->x,tmp->y)!=NULL){
        Animal *manger = animal_en_XY(proie,tmp->x,tmp->y);
        tmp->energie+=manger->energie;
        //printf("Une proie est mangée. +%d energie, energie preda:%d\n",manger->energie,tmp->energie);
        enlever_animal(liste_proie,manger);
      }
      if(tmp->energie < 0){
        Animal *suppr=tmp;
        tmp=tmp->suivant;
        enlever_animal(liste_predateur,suppr);
        //printf("Une predateur est mort.\n");
      }
      else{
        tmp=tmp->suivant;
      }
    }
    reproduce(liste_predateur,p_reproduce_predateur);

}

/* Part 2. Exercice 5, question 2 */
void rafraichir_monde(int monde[SIZE_X][SIZE_Y]){

  for(int i=0;i<SIZE_X;i++){
    for(int j=0;j<SIZE_Y;j++){
      monde[i][j]++;;  
    }
  }


}

void ecrire_ecosys(const char *nom_fichier, Animal *liste_predateur, Animal *liste_proie){
  FILE *f = fopen(nom_fichier, "w");
    if(f==NULL){
      printf("Erreur lors de l'ouverture de %s\n",nom_fichier);
      return;
    }
    fprintf(f,"<proies>\n");
    Animal *tmp = liste_proie;
    while(tmp){
      fprintf(f,"x=%d y=%d dir=[%d %d] e=%.6f\n",tmp->x,tmp->y,tmp->dir[0],tmp->dir[1],tmp->energie);
      tmp = tmp->suivant;
    }
    fprintf(f,"</proies>\n");
    fprintf(f,"<predateurs>\n");
    tmp = liste_predateur;
    while(tmp){
      fprintf(f,"x=%d y=%d dir=[%d %d] e=%.6f\n",tmp->x,tmp->y,tmp->dir[0],tmp->dir[1],tmp->energie);
      tmp = tmp->suivant;
    }
    fprintf(f,"</predateurs>\n");
  fclose(f);
  return;
}

void lire_ecosys(const char *nom_fichier, Animal **liste_predateur,Animal **liste_proie){
  FILE *f = fopen(nom_fichier,"r");
  char buffer[256];
  fgets(buffer,256,f);
  printf("%s",buffer);
  assert(strncmp(buffer,"<proies>",8)==0);
  fgets(buffer,256,f);
  printf("%s",buffer);
  int x_lu,y_lu;
  int dir_lu[2];
  float e_lu;
  while(strncmp(buffer,"</proies>",9)!=0){
    sscanf(buffer, "x=%d y=%d dir=[%d %d] e=%f\n",&x_lu,&y_lu,&dir_lu[0],&dir_lu[1],&e_lu);
    Animal *a_lu = creer_animal(x_lu,y_lu,e_lu);
    a_lu->dir[0]= dir_lu[0];
    a_lu->dir[1]= dir_lu[1];
    a_lu->suivant = *liste_proie;
    *liste_proie=a_lu;
    fgets(buffer,256,f);
    printf("%s",buffer);
  }
  fgets(buffer,256,f);
  printf("%s",buffer);
  assert(strncmp(buffer,"<predateurs>",12)==0);
  fgets(buffer,256,f);
  printf("%s",buffer);
  while(strncmp(buffer,"</predateurs>",13)!=0){
    sscanf(buffer, "x=%d y=%d dir=[%d %d] e=%f\n",&x_lu,&y_lu,&dir_lu[0],&dir_lu[1],&e_lu);
    Animal *a_lu = creer_animal(x_lu,y_lu,e_lu);
    a_lu->dir[0]= dir_lu[0];
    a_lu->dir[1]= dir_lu[1];
    a_lu->suivant = *liste_predateur;
    *liste_predateur=a_lu;
    fgets(buffer,256,f);
    printf("%s",buffer);
  }
  fclose(f);
}

