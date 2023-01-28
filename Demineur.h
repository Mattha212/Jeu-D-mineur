#ifndef DEMINEUR_H_INCLUDED
#define DEMINEUR_H_INCLUDED
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dirent.h>
#include <string.h>
#define NMAX 50

typedef struct difficulte {int l,L,nbBombes;} Difficulte;
typedef struct temps {time_t t1; double dftm;} Temps;

Difficulte choixDifficulte();
int creerMatrice(int mat[NMAX][NMAX],Difficulte diff);
int afficherMatAff(char mat[NMAX][NMAX],Difficulte diff);
int afficherMat(int mat[NMAX][NMAX],Difficulte diff);

int compteur(char mat[NMAX][NMAX],Difficulte diff);
int tour(char mat[NMAX][NMAX], int mat2[NMAX][NMAX],Difficulte diff, int x, int y,int flag,Temps tps);
void tourPartie(int mat[NMAX][NMAX],char matAff[NMAX][NMAX],Difficulte diff,Temps tps);

Difficulte recupererDiff(Difficulte diff,const char *nomFichier,Temps tps);
Temps recupererTemps(Difficulte diff,const char *nomFichier,Temps tps);
void recupererMatrice(Difficulte diff,int matt[NMAX][NMAX],char mattAff[NMAX][NMAX],const char *nomFichier);
void chargerPartie();
int ExistenceFictxt(const char *nomFichier);
void enregistrerMatrice(int mat[NMAX][NMAX],char matAff[NMAX][NMAX],Difficulte diff,const char *nomFichier,Temps tps);

void enregistrerPartie(int mat[NMAX][NMAX],char matAff[NMAX][NMAX],Difficulte diff,Temps tps);
void quitterPartie();
void lancerPartie();
void renommerPartie();
void supprimerPartie();
int accueil();
void aide();
void historiqueParties();


#endif // DEMINEUR_H_INCLUDED
