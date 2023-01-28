#include "Demineur.h"



int compteur(char mat[NMAX][NMAX],Difficulte diff){
    int n=0;
    for (int i=0;i<diff.l;i++){
        for (int j=0;j<diff.L;j++){
            if ((mat[i][j]!='|') && (mat[i][j]!='F')){      //compteur qui augmente pour chaque case découverte en dehors des cases marquées par un drapeau
                n+=1;
            }
        }
    }
    return n;
}

Difficulte choixDifficulte(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    int curDifficulte;
    Difficulte diff;
    printf("Choisir la difficulte :\n\n1. Facile (8x8,10 bombes)\n2. Moyen (16x16,40 bombes)\n3. Difficile (30x16,99 bombes)\n4. Impossible (10x10,99 bombes)\n5. Custom\n");
    scanf("%i",&curDifficulte);
    switch(curDifficulte){ // choix des paramètres à mettre dans la structure "diff"
    case 1: // Facile
        diff.l=8;
        diff.L=8;
        diff.nbBombes=10;
        break;
    case 2: // Moyen
        diff.l=16;
        diff.L=16;
        diff.nbBombes=40;
        break;
    case 3: // Difficile
        diff.l=30;
        diff.L=16;
        diff.nbBombes=99;
        break;
    case 4: // Impossible
        diff.l=10;
        diff.L=10;
        diff.nbBombes=99;
        break;
    case 5: //Custom
        do{
            printf("Saisir la largeur de la grille (plus petit que %i):\n",NMAX);
            scanf("%i",&diff.l);
        } while(diff.l>NMAX); // Choix de la largeur dans la limite de la taille maximale de la matrice
        do{
            printf("Saisir la longueur de la grille (plus petit que %i):\n",NMAX);
            scanf("%i",&diff.L);
        } while (diff.L>NMAX); // Choix de la longueur dans la limite de la taille maximale de la matrice
        int taille = diff.l*diff.L;
        do {
            printf("Saisir le nombre de bombes (plus petit que %i):\n",taille-1); // Le nombre de bombe ne peut pas excéder la taille de la matrice
            scanf("%i",&diff.nbBombes);                                           // Et il doit rester au moins un emplacement libre
        } while (diff.nbBombes>(taille-1));
        break;
    default:
        printf("\nMauvaise saisie. Reboot...\n");
        fflush(stdin);
        choixDifficulte(); // Recommence la demande en cas de problème
    }
    return diff;
}

int afficherMatAff(char mat[NMAX][NMAX],Difficulte diff){
    for (int i=0;i<diff.L;i++){ // Affichage des indications de position en haut
        if (i>=10){printf("%i ",i);}
        else {printf("%i  ",i);}
    }
    printf("\n\n");
    for (int i=0;i<diff.l;i++){
        for (int j=0;j<diff.L;j++){
            {printf("%c  ",mat[i][j]);} // Affichage du contenu de la matrice
        }
        printf(" %i\n",i); // Affichage des indications de position à droite
    }
    return 0;
}

int afficherMat(int mat[NMAX][NMAX],Difficulte diff){
    for (int i=0;i<diff.l;i++){
        for (int j=0;j<diff.L;j++){
            if (mat[i][j]>9){printf("9 ");} // Les bombes côte-à-côte dépassent 9 et sont ramenées à 9 pour un affichage propre
            else{printf("%i ",mat[i][j]);} // Affichage du contenu de la matrice
        }
    printf("\n");
    }
    return 0;
}

int tour(char mat[NMAX][NMAX], int mat2[NMAX][NMAX],Difficulte diff, int x, int y,int flag,Temps tps){
if (mat[x][y] == '|'){ // cas où on a pas encore testé la case
    if (flag==0){//cas où on n'est pas dans le mode drapeau
        if (mat2[x][y]>=9){ //cas où  la case est une bombe
            printf("Boom! Perdu");
            return 1;
        }
        if (mat2[x][y] >= 1 && mat2[x][y]<= 8){ //cas où une bombe est à coté de la case
            int c = mat2[x][y];
            mat[x][y] = c  + 48;

        }
        else{
            mat[x][y]='0';  //cas où la case est un 0, test des 8 cases autour
            int mat3[8][2]={{x-1,y},{x,y-1},{x-1,y-1},{x,y+1},{x+1,y},{x+1,y+1},{x-1,y+1},{x+1,y-1}};
            for (int i=0;i<8;i++){
                    if (((mat3[i][0]>=0)&&(mat3[i][0]<=diff.l))&&((mat3[i][1]>=0)&&(mat3[i][1]<=diff.L))){
                        tour(mat,mat2,diff,mat3[i][0],mat3[i][1],flag,tps);

                    }
            }
        }
    }
    else{//cas où on se trouve dans le mode drapeau
        mat[x][y]='F';
    }
}
else if(mat[x][y]=='F'){//si on tombe sur une case drapeau
    if(flag==1){
        mat[x][y]='|';
    }
    else{
        printf("Il y a un drapeau sur cette case !\n");
    }
}
int n = compteur(mat, diff);
if(n + diff.nbBombes == diff.l*diff.L){//test de condition de victoire
    printf("\nFelicitations! Vous avez gagne!\n");

    time_t t2 = time(NULL);
    tps.dftm = tps.dftm + difftime(t2,tps.t1);//mesure du temps de la partie
    printf("Vous avez termine avec un temps de %.2f secondes !\n",tps.dftm);

    FILE *fsv = fopen("Sauvegardes", "a");      //ouvre le fichier d'historique des parties
    printf("Veuillez entrez votre pseudo: ");       //propose à l'utilisateur son pseudo pour sauvegarder son temps dans l'historique de parties
    char pseudo[256];
    scanf("%s",pseudo);
    fprintf(fsv,"-> %s   |   Temps realise : %.2f secondes\n",pseudo,tps.dftm);
    fclose(fsv);

    return 1;
}
return 0;
}

int creerMatrice(int mat[NMAX][NMAX],Difficulte diff){
    int k,x,y,i;
    k=diff.nbBombes;
    //placement des bombes
    while(k>0){
        // Choix d'une position aléatoire dans la matrice
        x=rand()%diff.l;
        y=rand()%diff.L;
        // Vérifie qu'il n'y a pas déjà de bombe sur la case
        if (mat[x][y]<9){
            mat[x][y]=9;
            int mat2[8][2]={{x-1,y},{x,y-1},{x-1,y-1},{x,y+1},{x+1,y},{x+1,y+1},{x-1,y+1},{x+1,y-1}}; // définit les voisins de la case
            for (i=0;i<8;i++){
                if (((mat2[i][0]>=0)&&(mat2[i][0]<=diff.l))&&((mat2[i][1]>=0)||(mat2[i][1]<=diff.L))){ // Vérification des effets de bord
                    mat[mat2[i][0]][mat2[i][1]]+=1; // Incrémentation des cases voisines
                }
            }
            k--;
        }
    }
    return 0;
}


void tourPartie(int mat[NMAX][NMAX],char matAff[NMAX][NMAX],Difficulte diff,Temps tps){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    afficherMatAff(matAff,diff);
    printf("100 100 -> quitter\n101 101 -> sauvegarder\n102 102 -> mode drapeau\n");
    printf("donnez une case: ");
    int x,y,flag=0;
    scanf("%i %i", &y, &x);         //saisie des coordonnees de la case voulue par l'utilisateur
    if ((x == 100) && (y == 100)) {     //taper 100 100 fait retourner à l'acceuil
            accueil();
            exit(0);
        }
    if ((x ==101) && (y == 101))        //taper 101 101 débute l'enregistrement de la partie
            enregistrerPartie(mat,matAff,diff,tps);
    if ((x ==102) && (y == 102)){       //taper 102 102 active le mode drapeau qui permet de placer des drapeaux
            if (flag==0){           //place un drapeau si la case n'est pas decouverte
                flag=1;
            }
            else {flag=0;}          //enlève un drapeau s'il y a deja un drapeau
        }
    int var;
    var=tour(matAff, mat,diff, x, y,flag,tps); //retourne 1 si la condition de victoire est remplie
    while (var!=1){        //tant que var = 0, le tour recommence
        #ifdef _WIN32
        system("cls");
        #else
        system("clear");
        #endif
        afficherMatAff(matAff,diff);    //la partie qui suit est semblable à la partie supérieure qui est en dehors de la boucle
        if (flag==1){
            printf("mode drapeau actif !\n");
        }
        printf("100 100 -> quitter\n101 101 -> sauvegarder\n102 102 -> mode drapeau\n");
        printf("donnez une case: ");
        scanf("%i %i", &x, &y);
        if ((x == 100) && (y == 100)) {
            accueil();
            exit(0);
        }
        if ((x ==101) && (y == 101))
            enregistrerPartie(mat,matAff,diff,tps);
        if ((x ==102) && (y == 102)){
            if (flag==0){
                flag=1;
            }
            else {flag=0;}
        }
        var=tour(matAff, mat,diff, y, x,flag,tps);
    }
}


Difficulte recupererDiff(Difficulte diff,const char *nomFichier,Temps tps){
    FILE *fsv = fopen(nomFichier, "r");

    char texte[1999];

    while(fgets(texte, sizeof(texte), fsv))             //place le curseur à la derniere ligne (tant que le fgets() = 1 reste dans la boucle)
    {
        fscanf(fsv, "%d %d %d %lf", &diff.l, &diff.L, &diff.nbBombes, &tps.dftm);   //récupère le nombre de lignes,colonnes, de bombes et le temps dans le fichier texte
    }

    return diff; //renvoie diff
}

Temps recupererTemps(Difficulte diff,const char *nomFichier,Temps tps){
    FILE *fsv = fopen(nomFichier, "r");

    char texte[1999];

    while(fgets(texte, sizeof(texte), fsv))             //place le curseur à la derniere ligne (tant que le fgets() = 1 reste dans la boucle)
    {
        fscanf(fsv, "%d %d %d %lf", &diff.l, &diff.L, &diff.nbBombes, &tps.dftm);   //récupère le nombre de lignes,colonnes, de bombes et le temps dans le fichier texte
    }

    return tps; //renvoie le temps
}

void recupererMatrice(Difficulte diff,int matt[NMAX][NMAX],char mattAff[NMAX][NMAX],const char *nomFichier){
    FILE *fsv = fopen(nomFichier, "r");

    int a = 0;                      //récupère la mat du fichier dans une nvlle mat
    for(int i=0;i<diff.l;i++) {
        for(int j=0;j<diff.L;j++) {
            fseek(fsv, a, SEEK_SET);
            fscanf(fsv, "%i", &matt[i][j]);
            a++;
            a++;
        }
    a++;
    a++;
    }

                                        //récupère la mataff
    for(int i=0;i<diff.l;i++) {
        for(int j=0;j<diff.L;j++) {
            fseek(fsv, a, SEEK_SET);
            fscanf(fsv, "%c", &mattAff[i][j]);
            a++;
            a++;
        }
    a++;
    a++;
    }

}



void chargerPartie(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    Difficulte diff;int matt[NMAX][NMAX];char mattAff[NMAX][NMAX];Temps tps;

    printf("Liste des parties sauvegardees:\n");
                        //affiche tous les fichiers texte du répertoir
    struct dirent *dir;
    // opendir() renvoie un pointeur de type DIR.
    DIR *d = opendir(".");              //ouvre le dossier
    if (d){
        while ((dir = readdir(d)) != NULL){         //parcourt le dossier
            if (strstr(dir->d_name,".txt")){        //affiche le fichier si c'est un fichier texte
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);                    //ferme le dossier
    }

    printf("\nretour -> retourner au menu");
    char nomFichier[256];
    char ret[]= "retour";
    printf("\nQuelle partie voulez-vous charger ?: ");
    scanf("%s",nomFichier);                         //saisie de l'utilisateur
    if (strcmp(nomFichier,ret) == 0){                   //test pour proposer à l'utilisateur de retourner au menu
            accueil();
            exit(0);
    }
    if (strstr(nomFichier,".txt") == 0){            //ajoute la terminaison ".txt" si la saisie ne se termine pas par ".txt"
        strcat(nomFichier,".txt");
    }

    switch(ExistenceFictxt(nomFichier))              //si le fichier existe...
    {
    case 1:                                            //récupère la difficulte, les matrices et le temps
        diff = recupererDiff(diff,nomFichier,tps);
        tps = recupererTemps(diff,nomFichier,tps);
        recupererMatrice(diff,matt,mattAff,nomFichier);

        printf("Temps de partie de %f secondes. Reprise du timer...\n", tps.dftm);
        tps.t1 = time(NULL);                    //reprise du timer
        tourPartie(matt,mattAff,diff,tps);
        break;
    case 0:                                             //sinon relance la fonction "chargerFichier"
        printf("Le fichier n'existe pas... \n\n");
        return chargerPartie();
        break;
    }
}


int ExistenceFictxt(const char *nomFichier)    //fonction qui vérifie si un fichier existe déjà
{
    FILE *file=fopen(nomFichier, "r");
    if (file)       //retourne 1 si le fichier s'ouvre
    {
        fclose(file);
        return 1;
    }
    return 0;
}

void enregistrerMatrice(int mat[NMAX][NMAX],char matAff[NMAX][NMAX],Difficulte diff,const char *nomFichier,Temps tps){

    FILE *fsv = fopen(nomFichier, "w+");

    for(int i=0;i<diff.l;i++) {              //écrit la mat sur le fichier txt de sauvegarde
        for(int j=0;j<diff.L;j++) {
            if (mat[i][j]>9){fprintf(fsv,"9 ");}
            else{fprintf(fsv,"%i ", mat[i][j]);}
        }
    fprintf(fsv, "\n");
    }

    for(int i=0;i<diff.l;i++) {              //écrit la matAff sur le fichier txt de sauvegarde
        for(int j=0;j<diff.L;j++) {
            fprintf(fsv,"%c ", matAff[i][j]);
        }
    fprintf(fsv, "\n");
    }

    fseek(fsv, 0, SEEK_END);            //érit les l, c , nb_bombes et le temps à la fin du fichier
    fprintf(fsv, "%d %d %d %f", diff.l, diff.L, diff.nbBombes, tps.dftm);

    if(fsv == NULL)                     //ferme le fichier s'il est vide
        exit(1);

    fclose(fsv);
}


void enregistrerPartie(int mat[NMAX][NMAX],char matAff[NMAX][NMAX],Difficulte diff,Temps tps)
{


    time_t t2 = time(NULL);
    tps.dftm = tps.dftm + difftime(t2,tps.t1);          //arret du temps et stockage dans une variable
    printf("Temps de partie de %2.2f secondes.\n", tps.dftm);

    char nomFichier[256];
    printf("Veuillez entrer un nom de sauvegarde:\n");
    scanf("%s", nomFichier);                            //demande à l'utilisateur de nommer le fichier

    strcat(nomFichier,".txt");      //donne le type "texte" au fichier
    printf("%s\n",nomFichier);

    if (ExistenceFictxt(nomFichier))    //si le fichier existe, demande à l'utilisateur s'il souhaite écraser le premier fichier
    {
        printf("Une sauvegarde du meme nom existe deja. Voulez-vous ecraser cette sauvegarde ?\n   1. oui\n   2. non\n");
        int rep;
        scanf("%d", &rep);
        switch(rep)
        {
        case 1:
            enregistrerMatrice(mat,matAff,diff,nomFichier,tps);   //ecrase le fichier existant et enregistrement Matrice
            break;
        case 2:
            enregistrerPartie(mat,matAff,diff,tps);       //relance si l'utilisateur refuse d'écraser le fichier existant
            break;
        }
    }
    else
    {
        enregistrerMatrice(mat,matAff,diff,nomFichier,tps);   //enregistrement Matrice
    }
}



void quitterPartie(){
}



void lancerPartie(){
    Difficulte diff = choixDifficulte();
    int mat[NMAX][NMAX] = {0}; // Création matrice de 0
    char matAff[NMAX][NMAX];
    for(int i=0; i<diff.l; i++){
        for(int j=0; j<diff.L; j++){
            matAff[i][j] = '|'; // Création matrice d'affichage non dévoilée
        }
    }

    creerMatrice(mat,diff); // Remplissage de la matrice
    Temps tps;
    tps.t1 = time(NULL);    //début du timer
    tps.dftm = 0;
    tourPartie(mat,matAff,diff,tps); // Lancement de la partie
}

void renommerPartie(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    printf("Liste des parties sauvegardees:\n");
                        //affiche tous les fichiers texte du répertoir
    struct dirent *dir;
    // opendir() renvoie un pointeur de type DIR.
    DIR *d = opendir(".");              //ouvre le dossier
    if (d){
        while ((dir = readdir(d)) != NULL){         //parcourt le dossier
            if (strstr(dir->d_name,".txt")){        //affiche le fichier si c'est un fichier texte
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);                    //ferme le dossier
    }

    printf("\nretour -> retourner au menu");
    char nomFichier[256];
    char nomNVFichier[256];
    char ret[]= "retour";
    printf("\nQuelle partie voulez-vous renommer ?: ");
    scanf("%s",nomFichier);                         //saisie de l'utilisateur
    if (strcmp(nomFichier,ret) == 0){                   //test pour proposer à l'utilisateur de retourner au menu
            accueil();
            exit(0);
    }
    if (strstr(nomFichier,".txt") == 0){            //ajoute la terminaison ".txt" si la saisie ne se termine pas par ".txt"
        strcat(nomFichier,".txt");
    }

    switch(ExistenceFictxt(nomFichier))              //si le fichier existe...
    {
    case 1:
        printf("\nEntrez un nouveau nom: ");
        scanf("%s",nomNVFichier);
        if (strstr(nomNVFichier,".txt") == 0){            //ajoute la terminaison ".txt" si la saisie ne se termine pas par ".txt"
        strcat(nomNVFichier,".txt");}
        rename(nomFichier,nomNVFichier);
        accueil();
        break;
    case 0:                                             //sinon relance la fonction "chargerFichier"
        printf("Le fichier n'existe pas... \n\n");
        return renommerPartie();
        break;
    }
}

void supprimerPartie(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    printf("Liste des parties sauvegardees:\n");
                        //affiche tous les fichiers texte du répertoir
    struct dirent *dir;
    // opendir() renvoie un pointeur de type DIR.
    DIR *d = opendir(".");              //ouvre le dossier
    if (d){
        while ((dir = readdir(d)) != NULL){         //parcourt le dossier
            if (strstr(dir->d_name,".txt")){        //affiche le fichier si c'est un fichier texte
                printf("%s\n", dir->d_name);
            }
        }
        closedir(d);                    //ferme le dossier
    }

    printf("\nretour -> retourner au menu");
    char nomFichier[256];
    char ret[]= "retour";
    printf("\nQuelle partie voulez-vous supprimer ?: ");
    scanf("%s",nomFichier);                         //saisie de l'utilisateur
    if (strcmp(nomFichier,ret) == 0){                   //test pour proposer à l'utilisateur de retourner au menu
            accueil();
            exit(0);
    }
    if (strstr(nomFichier,".txt") == 0){            //ajoute la terminaison ".txt" si la saisie ne se termine pas par ".txt"
        strcat(nomFichier,".txt");
    }

    switch(ExistenceFictxt(nomFichier))              //si le fichier existe...
    {
    case 1:
        remove(nomFichier);
        accueil();
        break;
    case 0:                                             //sinon relance la fonction "chargerFichier"
        printf("Le fichier n'existe pas... \n\n");
        return supprimerPartie();
        break;
    }
}

int accueil(){
    #ifdef _WIN32
    system("cls"); //Efface l'écran sous windows
    #else
    system("clear"); //Efface l'écran sous linux
    #endif
    srand(time(NULL)); //Initialisation du temps (pour random notamment)
    int curEcranAccueil;
    printf("---------------Demineur---------------\n\n1. Nouvelle partie\n2. Charger partie\n3. Quitter\n4. Renommer partie\n5. Supprimer partie\n6. Aide\n7. Historique Parties\n");
    scanf("%i",&curEcranAccueil);
    switch(curEcranAccueil){ // Choix du menu
    case 1:
        lancerPartie();
        break;
    case 2:
        chargerPartie();
        break;
    case 3:
        quitterPartie();
        break;
    case 4:
        renommerPartie();
        break;
    case 5:
        supprimerPartie();
        break;
    case 6:
        aide();
        break;
    case 7:
        historiqueParties();
        break;
    default:
        printf("\nMauvaise saisie. Reboot...\n");
        fflush(stdin); // Sert à ne pas surcharger le buffer
        accueil(); // Relance la fonction en cas de problème
        break;
    }
    return 0;
}

void aide(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    printf("---------------Aide---------------\n");
    printf("Le demineur comprend une grille ou les cases sont cachees. Le but est de devoiler les cases sans tomber sur une bombe,\n");
    printf("en sachant que les cases devoilees revelent le nombre de bombes adjacentes a la case.\n\n");
    printf("Pour devoiler une case, il suffit de rentrer les coordonees de la case a devoiler au format \"longueur hauteur\".\n");
    printf("Exemple:\nSi je rentre \"2 6\" dans la console, la case marquee d'un X sera devoilee.\n\n");
    printf("0  1  2  3  4  5  6  7\n");
    printf("|  |  |  |  |  |  |  |   0\n");
    printf("|  |  |  |  |  |  |  |   1\n");
    printf("|  |  |  |  |  |  |  |   2\n");
    printf("|  |  |  |  |  |  |  |   3\n");
    printf("|  |  |  |  |  |  |  |   4\n");
    printf("|  |  |  |  |  |  |  |   5\n");
    printf("|  |  X  |  |  |  |  |   6\n");
    printf("|  |  |  |  |  |  |  |   7\n\n");
    printf("Vous pouvez quitter la partie a tout moment en rentrant le code \"100 100\"\nou la sauvegarder en rentrant le code \"101 101\".\n");
    printf("Les choix du menu se font en rentrant le numero marque a cote du choix voulu.\n");
    printf("(ex: ecrire 2 ouvre la liste des parties sauvegardees,\npuis il suffit d'ecrire le nom de la partie voulue pour la lancer)\n");
    printf("\nAppuyer sur une touche pour continuer : ");
    while(getchar()=='\n'); // Finit quelque soit le contenu envoyé par l'utilisateur
    accueil();
}

void historiqueParties(){
    #ifdef _WIN32
    system("cls");
    #else
    system("clear");
    #endif
    FILE *fsv = fopen("Sauvegardes", "r");  //ouvre un fichier de sauvegarde qui n'est pas un fichier texte et donc qui ne sera pas lister lors des chargements de parties
    char stats[2999];
    while (fgets(stats, 2998, fsv) != NULL){    //parcourt le fichier et affiche chaque ligne
        printf("%s",stats);
    }
    fclose(fsv);
    printf("\nAppuyer sur une touche pour continuer : ");
    while(getchar()=='\n');
    accueil();
}
