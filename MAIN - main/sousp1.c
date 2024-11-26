#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

// Fonction pour initialiser le plateau
void initialiserPlateau(char plateau[SIZE][SIZE][4]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            plateau[i][j][0] = '\0';

            if (i == 0 && j == 0) {
                strcpy(plateau[i][j], TOP_LEFT_CORNER);
            } else if (i == 0 && j == SIZE - 1) {
                strcpy(plateau[i][j], TOP_RIGHT_CORNER);
            } else if (i == SIZE - 1 && j == 0) {
                strcpy(plateau[i][j], BOTTOM_LEFT_CORNER);
            } else if (i == SIZE - 1 && j == SIZE - 1) {
                strcpy(plateau[i][j], BOTTOM_RIGHT_CORNER);
            } else if (i == 0 || i == SIZE - 1) {
                strcpy(plateau[i][j], HORIZONTAL_LINE);
            } else if (j == 0 || j == SIZE - 1) {
                strcpy(plateau[i][j], VERTICAL_LINE);
            } else if (i % 2 == 0 && j % 2 == 0) {
                strcpy(plateau[i][j], INTERSECTION);
            } else if (i % 2 == 0) {
                strcpy(plateau[i][j], HORIZONTAL_LINE);
            } else if (j % 2 == 0) {
                strcpy(plateau[i][j], VERTICAL_LINE);
            } else {
                strcpy(plateau[i][j], " ");
            }
        }
    }
}

// Fonction pour afficher le plateau
void afficherPlateau(char plateau[SIZE][SIZE][4], int GameMode, GameState *state, Joueurs pseudos[]) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
    printf("  ");
    for (int i = 0; i < SIZE / 2; i++) {
        printf("%2d  ", i);
    }
    printf("             INFORMATIONS JOUEURS\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char *c = plateau[i][j];
            int isBarrierTemp = 0; // Vérifier si une barrière est en cours de placement
            if ((state->modePlacement1 || state->modePlacement2 ||
                 state->modePlacement3 || state->modePlacement4)) {
                if (state->orientation == 'V') {
                    if ((i == state->barrierX - 1 || i == state->barrierX ||
                         i == state->barrierX + 1 || i == state->barrierX + 2) &&
                        j == state->barrierY) {
                        isBarrierTemp = 1;
                    }
                } else if (state->orientation == 'H') {
                    if (i == state->barrierX &&
                        (j == state->barrierY - 1 || j == state->barrierY ||
                         j == state->barrierY + 1 || j == state->barrierY + 2)) {
                        isBarrierTemp = 1;
                    }
                }
            }

            // Afficher les éléments du plateau avec des couleurs
            if (strcmp(c, "1") == 0) {
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, FOREGROUND_BLUE | FOREGROUND_INTENSITY);
                printf("o ");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
                printf("\033[34mo\033[0m ");
#endif
            } else if (strcmp(c, "2") == 0) {
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
                printf("o ");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
                printf("\033[31mo\033[0m ");
#endif
            } else if (strcmp(c, "3") == 0) {
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf("o ");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
                printf("\033[32mo\033[0m ");
#endif
            } else if (strcmp(c, "4") == 0) {
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("o ");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
                printf("\033[33mo\033[0m ");
#endif
            } else if (strcmp(c, "B") == 0) {
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                printf("%s ", c);
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
                printf("\033[32m%s\033[0m ", c);
#endif
            } else if (isBarrierTemp) {
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                printf("B ");
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
                printf("\033[32;1mB\033[0m ");
#endif
            } else if (i == 0 || i == SIZE - 1 || j == 0 || j == SIZE - 1) {
#ifdef _WIN32
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN);
                printf("%s ", c);
                SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
#else
                printf("\033[0;33m%s\033[0m ", c);
#endif
            } else {
                printf("%s ", c);
            }
        }
        
        // Affichage des informations sur les joueurs et barrières
        if (i == 1) {
            printf("   Joueur 1 : %s (Pion BLEU)", pseudos[0].pseudos);
        } else if (i == 2) {
            printf("   Barrières restantes: %d", state->nb_barrieres1);
        } else if (i == 5) {
            printf("   Joueur 2 : %s (Pion ROUGE)", pseudos[1].pseudos);
        } else if (i == 6) {
            printf("   Barrières restantes: %d", state->nb_barrieres2);
        } else if (i == 9 && GameMode == 4) {
            printf("   Joueur 3 : %s (Pion VERT)", pseudos[2].pseudos);
        } else if (i == 10 && GameMode == 4) {
            printf("   Barrières restantes: %d", state->nb_barrieres3);
        } else if (i == 13 && GameMode == 4) {
            printf("   Joueur 4 : %s (Pion JAUNE)", pseudos[3].pseudos);
        } else if (i == 14 && GameMode == 4) {
            printf("   Barrières restantes: %d", state->nb_barrieres4);
        }
        printf("\n");
    }
}

// Fonction pour lire une touche sans attendre l'appui sur Entrée
int lireTouche() {
#ifdef _WIN32
    // Code spécifique à Windows
    int ch = _getch();
    if (ch == 0 || ch == 224) {
        ch = _getch();
        switch (ch) {
            case 72: return 'U'; // Flèche haut
            case 80: return 'D'; // Flèche bas
            case 75: return 'L'; // Flèche gauche
            case 77: return 'R'; // Flèche droite
            default: return 0;
        }
    } else {
        return ch;
    }
#else
    // Code spécifique à Unix/Linux
    struct termios oldt, newt;
    int ch;

    // Désactiver l'écho et la mise en tampon
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    ch = getchar();

    if (ch == '\033') {
        getchar(); // Ignorer le caractère '['
        switch(getchar()) {
            case 'A': ch = 'U'; break; // Flèche haut
            case 'B': ch = 'D'; break; // Flèche bas
            case 'C': ch = 'R'; break; // Flèche droite
            case 'D': ch = 'L'; break; // Flèche gauche
            default: ch = 0;
        }
    }

    // Restaurer les paramètres du terminal
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
#endif
}

// Fonction pour effacer la console
void clearConsole() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[H\033[J");
#endif
}

// Fonction pour vérifier s'il y a une barrière entre deux positions
int barriereEntre(int x1, int y1, int x2, int y2,
                  char plateau[SIZE][SIZE][4]) {
    int midX = (x1 + x2) / 2;
    int midY = (y1 + y2) / 2;
    if (strcmp(plateau[midX][midY], "B") == 0) {
        return 1;
    }
    return 0;
}

// Fonction pour déplacer un pion avec gestion du saut
void deplacerPion(char plateau[SIZE][SIZE][4], int *x, int *y,
                  char direction, char joueur, int x1, int y1, int x2, int y2,
                  int x3, int y3, int x4, int y4, int GameMode) {
    int newX = *x, newY = *y;
    int opponentX = -1, opponentY = -1;
    int opponentFound = 0;
    int oppX[3], oppY[3]; // Variables pour les positions potentielles des adversaires
    int oppCount = 0;
    if (GameMode == 2) { // Remplir les positions des adversaires en fonction du joueur actuel
        if (joueur == PLAYER1) {
            oppX[0] = x2; oppY[0] = y2; oppCount = 1;
        } else if (joueur == PLAYER2) {
            oppX[0] = x1; oppY[0] = y1; oppCount = 1;
        }
    } else if (GameMode == 4) {
        int idx = 0;
        if (joueur != PLAYER1) { oppX[idx] = x1; oppY[idx++] = y1; }
        if (joueur != PLAYER2) { oppX[idx] = x2; oppY[idx++] = y2; }
        if (joueur != PLAYER3) { oppX[idx] = x3; oppY[idx++] = y3; }
        if (joueur != PLAYER4) { oppX[idx] = x4; oppY[idx++] = y4; }
        oppCount = idx;
    }
    // Calcul du déplacement souhaité
    if (joueur == PLAYER1) {
        if (direction == 'z' && *x > 1) {
            newX -= 2;
        } else if (direction == 's' && *x < SIZE - 2) {
            newX += 2;
        } else if (direction == 'q' && *y > 1) {
            newY -= 2;
        } else if (direction == 'd' && *y < SIZE - 2) {
            newY += 2;
        } else {
            return;
        }
    } else if (joueur == PLAYER2) {
        if (direction == 'U' && *x > 1) {
            newX -= 2;
        } else if (direction == 'D' && *x < SIZE - 2) {
            newX += 2;
        } else if (direction == 'L' && *y > 1) {
            newY -= 2;
        } else if (direction == 'R' && *y < SIZE - 2) {
            newY += 2;
        } else {
            return;
        }
    } else if (joueur == PLAYER3) {
        if (direction == 't' && *x > 1) {
            newX -= 2;
        } else if (direction == 'g' && *x < SIZE - 2) {
            newX += 2;
        } else if (direction == 'f' && *y > 1) {
            newY -= 2;
        } else if (direction == 'h' && *y < SIZE - 2) {
            newY += 2;
        } else {
            return;
        }
    } else if (joueur == PLAYER4) {
        if (direction == 'i' && *x > 1) {
            newX -= 2;
        } else if (direction == 'k' && *x < SIZE - 2) {
            newX += 2;
        } else if (direction == 'j' && *y > 1) {
            newY -= 2;
        } else if (direction == 'l' && *y < SIZE - 2) {
            newY += 2;
        } else {
            return;
        }
    }

    for (int i = 0; i < oppCount; i++) { // Vérifier s'il y a un pion adverse à la position cible
        if (newX == oppX[i] && newY == oppY[i]) {
            opponentX = oppX[i];
            opponentY = oppY[i];
            opponentFound = 1;
            break;
        }
    }
    if (opponentFound) { // Tentative de saut par-dessus l'adversaire
        int jumpX = opponentX + (opponentX - *x);
        int jumpY = opponentY + (opponentY - *y);
        // Vérifier les limites du plateau
        if (jumpX >= 1 && jumpX <= SIZE - 2 && jumpY >= 1 && jumpY <= SIZE - 2) {
            // Vérifier s'il n'y a pas de barrière entre l'adversaire et la case de saut
            if (!barriereEntre(opponentX, opponentY, jumpX, jumpY, plateau)) {
                // Vérifier si la case derrière l'adversaire est libre
                if (strcmp(plateau[jumpX][jumpY], " ") == 0) {
                    // Effectuer le saut
                    strcpy(plateau[*x][*y], " ");
                    *x = jumpX;
                    *y = jumpY;
                    char strJoueur[2] = {joueur, '\0'};
                    strcpy(plateau[*x][*y], strJoueur);
                    return;
                }
            }
        }
        
        // Si on ne peut pas sauter, vérifier les déplacements diagonaux
        int deltaX = opponentX - *x;
        int deltaY = opponentY - *y;

        int diagX1 = opponentX - deltaY; // Déterminer les mouvements diagonaux possibles
        int diagY1 = opponentY + deltaX;
        int diagX2 = opponentX + deltaY;
        int diagY2 = opponentY - deltaX;

        // Vérifier les deux diagonales possibles
        if (diagX1 >= 1 && diagX1 <= SIZE - 2 && diagY1 >= 1 && diagY1 <= SIZE - 2 &&
            strcmp(plateau[diagX1][diagY1], " ") == 0 &&
            !barriereEntre(*x, *y, diagX1, diagY1, plateau)) {
            // Déplacement en diagonale
            strcpy(plateau[*x][*y], " ");
            *x = diagX1;
            *y = diagY1;
            char strJoueur[2] = {joueur, '\0'};
            strcpy(plateau[*x][*y], strJoueur);
            return;
        }
        if (diagX2 >= 1 && diagX2 <= SIZE - 2 && diagY2 >= 1 && diagY2 <= SIZE - 2 &&
            strcmp(plateau[diagX2][diagY2], " ") == 0 &&
            !barriereEntre(*x, *y, diagX2, diagY2, plateau)) {
            // Déplacement en diagonale
            strcpy(plateau[*x][*y], " ");
            *x = diagX2;
            *y = diagY2;
            char strJoueur[2] = {joueur, '\0'};
            strcpy(plateau[*x][*y], strJoueur);
            return;
        } // Si aucun déplacement n'est possible, annuler le mouvement
        return;
    } // Vérifier s'il y a une barrière entre l'ancienne et la nouvelle position
    if (barriereEntre(*x, *y, newX, newY, plateau)) {
        return;
    }
    if (strcmp(plateau[newX][newY], " ") == 0) { // Vérifier si la nouvelle case est libre
        strcpy(plateau[*x][*y], " ");
        *x = newX;
        *y = newY;
        char strJoueur[2] = {joueur, '\0'};
        strcpy(plateau[*x][*y], strJoueur);
    }
}

// Fonction pour déplacer une barrière (changement de position lors de la sélection)
void deplacerBarriere(int *x, int *y, int direction, char joueur,
                      GameState *state) {
    int deltaX = 0, deltaY = 0;

    if (joueur == PLAYER1) {
        if (direction == 'z') {
            deltaX = -1;
        } else if (direction == 's') {
            deltaX = 1;
        } else if (direction == 'q') {
            deltaY = -1;
        } else if (direction == 'd') {
            deltaY = 1;
        }
    } else if (joueur == PLAYER2) {
        if (direction == 'U') {
            deltaX = -1;
        } else if (direction == 'D') {
            deltaX = 1;
        } else if (direction == 'L') {
            deltaY = -1;
        } else if (direction == 'R') {
            deltaY = 1;
        }
    } else if (joueur == PLAYER3) {
        if (direction == 't') {
            deltaX = -1;
        } else if (direction == 'g') {
            deltaX = 1;
        } else if (direction == 'f') {
            deltaY = -1;
        } else if (direction == 'h') {
            deltaY = 1;
        }
    } else if (joueur == PLAYER4) {
        if (direction == 'i') {
            deltaX = -1;
        } else if (direction == 'k') {
            deltaX = 1;
        } else if (direction == 'j') {
            deltaY = -1;
        } else if (direction == 'l') {
            deltaY = 1;
        }
    }
    *x += deltaX;
    *y += deltaY;
    if (*x < 1) *x = 1; // Garder x et y dans les limites du plateau
    if (*x > SIZE - 3) *x = SIZE - 3; // Ajusté pour barrière de taille 4
    if (*y < 1) *y = 1;
    if (*y > SIZE - 3) *y = SIZE - 3; // Ajusté pour barrière de taille 4
}

void placerBarriere(char plateau[SIZE][SIZE][4], int x, int y, char orientation, GameState *state, int joueur) {
    int *nb_barrieres_joueur;
    switch (joueur) { // Sélection du compteur de barrières en fonction du joueur
        case 1: nb_barrieres_joueur = &state->nb_barrieres1; break;
        case 2: nb_barrieres_joueur = &state->nb_barrieres2; break;
        case 3: nb_barrieres_joueur = &state->nb_barrieres3; break;
        case 4: nb_barrieres_joueur = &state->nb_barrieres4; break;
        default: return;
    }
    if (*nb_barrieres_joueur <= 0) {
        printf("Vous n'avez plus de barrières à placer !\n");
        return;
    }
    if (orientation == 'V') {
        if (x > 0 && x < SIZE - 3) {
            strcpy(plateau[x - 1][y], "B");
            strcpy(plateau[x][y], "B");
            strcpy(plateau[x + 1][y], "B");
            strcpy(plateau[x + 2][y], "B");
            (*nb_barrieres_joueur)--;
        }
    } else if (orientation == 'H') {
        if (y > 0 && y < SIZE - 3) {
            strcpy(plateau[x][y - 1], "B");
            strcpy(plateau[x][y], "B");
            strcpy(plateau[x][y + 1], "B");
            strcpy(plateau[x][y + 2], "B");
            (*nb_barrieres_joueur)--;
        }
    }
}

// Fonction pour afficher l'écran de victoire
void afficherEcranVictoire(int joueur, Joueurs pseudos[4]) {
    printf("\n\n");
    printf("********************************\n");
    printf("*                              *\n");
    printf("*    Le joueur %s a gagné !    *\n", pseudos[joueur-1].pseudos);
    printf("*                              *\n");
    printf("********************************\n");
    printf("\n\n");
}

// Fonction pour sauvegarder la partie
void sauvegarderPartie(const char *nomFichier,
                       char plateau[SIZE][SIZE][4], int x1, int y1, int x2, int y2,
                       int x3, int y3, int x4, int y4, int tour, int GameMode,
                       GameState *state, Joueurs pseudos[4]) {

    FILE *f = fopen(nomFichier, "wb");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        return;
    }

    fwrite(plateau, sizeof(char), SIZE * SIZE * 4, f);
    fwrite(&x1, sizeof(int), 1, f);
    fwrite(&y1, sizeof(int), 1, f);
    fwrite(&x2, sizeof(int), 1, f);
    fwrite(&y2, sizeof(int), 1, f);
    fwrite(&x3, sizeof(int), 1, f);
    fwrite(&y3, sizeof(int), 1, f);
    fwrite(&x4, sizeof(int), 1, f);
    fwrite(&y4, sizeof(int), 1, f);
    fwrite(&tour, sizeof(int), 1, f);
    fwrite(&GameMode, sizeof(int), 1, f);
    fwrite(state, sizeof(GameState), 1, f);
    fwrite(pseudos, sizeof(Joueurs), 4, f);

    fclose(f);
    printf("Partie sauvegardée dans le fichier '%s'.\n", nomFichier);
}

// Fonction pour charger une partie
void chargerPartie(const char *nomFichier,
                   char plateau[SIZE][SIZE][4], int *x1, int *y1, int *x2, int *y2,
                   int *x3, int *y3, int *x4, int *y4, int *tour, int *GameMode,
                   GameState *state, Joueurs pseudos[4]) {

    FILE *f = fopen(nomFichier, "rb");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier de sauvegarde");
        exit(EXIT_FAILURE);
    }

    fread(plateau, sizeof(char), SIZE * SIZE * 4, f);
    fread(x1, sizeof(int), 1, f);
    fread(y1, sizeof(int), 1, f);
    fread(x2, sizeof(int), 1, f);
    fread(y2, sizeof(int), 1, f);
    fread(x3, sizeof(int), 1, f);
    fread(y3, sizeof(int), 1, f);
    fread(x4, sizeof(int), 1, f);
    fread(y4, sizeof(int), 1, f);
    fread(tour, sizeof(int), 1, f);
    fread(GameMode, sizeof(int), 1, f);
    fread(state, sizeof(GameState), 1, f);
    fread(pseudos, sizeof(Joueurs), 4, f);

    fclose(f);
    printf("Partie chargée depuis le fichier '%s'.\n", nomFichier);
}

// Fonction pour initialiser une partie de démonstration
void initialiserDemo(char plateau[SIZE][SIZE][4], int *x1, int *y1,
                     int *x2, int *y2, int *x3, int *y3, int *x4, int *y4,
                     int *tour, int *GameMode, GameState *state) {

    
    // Initialiser le plateau
    initialiserPlateau(plateau);

    // Définir le mode de jeu à 2 joueurs
    *GameMode = 2;

    // Positions avancées des pions
    *x1 = 7;  *y1 = 9;   // Position avancée pour le joueur 1
    *x2 = 11; *y2 = 9;   // Position avancée pour le joueur 2

    // Placer les pions sur le plateau
    strcpy(plateau[*x1][*y1], "1");
    strcpy(plateau[*x2][*y2], "2");

    // Initialiser l'état du jeu
    *tour = 1; // C'est au tour du joueur 1 de jouer
    *state = (GameState){0, 0, 0, 0, 0, 0, 'V', 5, 5, 5, 5};

    // Placer des barrières pour simuler une partie avancée
    placerBarriere(plateau, 6, 9, 'H', state, 1);
    placerBarriere(plateau, 8, 7, 'V', state, 2);
    placerBarriere(plateau, 10, 9, 'H', state, 1);
    placerBarriere(plateau, 12, 11, 'V', state, 2);
}

// Fonction pour saisir et afficher les pseudos des joueurs
void Pseudo(Joueurs pseudosPartie[4], int *GameMode, Joueurs scores[], int *nbScores) {
    for (int i = 0; i < *GameMode; i++) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {} // Consommer le caractère '\n' restant dans le tampon
        printf("\n");
        printf("\n");
        printf("                                            Saisir le pseudo du joueur %d (50 caractères maximum) :", i + 1);
        fgets(pseudosPartie[i].pseudos, PSEUDO, stdin); // Supprimer le caractère '\n' si présent
        size_t len = strlen(pseudosPartie[i].pseudos);
        if (len > 0 && pseudosPartie[i].pseudos[len - 1] == '\n') {
            pseudosPartie[i].pseudos[len - 1] = '\0';
        }
        int found = 0; // Vérifier si le pseudonyme existe déjà dans les scores
        for (int j = 0; j < *nbScores; j++) {
            if (strcmp(scores[j].pseudos, pseudosPartie[i].pseudos) == 0) { // Pseudonyme trouvé, récupérer le score
                pseudosPartie[i].score = scores[j].score;
                found = 1;
                break;
            }
        }
        if (!found) { // Nouveau joueur, l'ajouter à la liste des scores
            strcpy(scores[*nbScores].pseudos, pseudosPartie[i].pseudos);
            scores[*nbScores].score = 0;
            pseudosPartie[i].score = 0;
            (*nbScores)++;
        }
    }
    printf("\n");
    printf("\n                                                        Pseudos enregistrés :\n");  // Vérification pour savoir si les pseudos sont bien enregistrés
    for (int i = 0; i < *GameMode; i++) {
        printf("                                                        Joueur %d: %s\n", i + 1, pseudosPartie[i].pseudos);
    }
    printf("\n");
    printf("                                                        Appuyez sur entrer pour commencer à jouer.\n");
    getchar(); // Attendre que l'utilisateur appuie sur Entrée
}

// Fonction pour charger les scores depuis le fichier
void chargerScores(const char *nomFichier, Joueurs scores[], int *nbScores) {
    FILE *f = fopen(nomFichier, "rb");
    if (f == NULL) {
        // Le fichier n'existe pas encore, initialiser nbScores à 0
        *nbScores = 0;
        return;
    }

    fread(nbScores, sizeof(int), 1, f);
    fread(scores, sizeof(Joueurs), *nbScores, f);

    fclose(f);
}

// Fonction pour sauvegarder les scores dans le fichier
void sauvegarderScores(const char *nomFichier, Joueurs scores[], int nbScores) {
    FILE *f = fopen(nomFichier, "wb");
    if (f == NULL) {
        perror("Erreur lors de l'ouverture du fichier de scores");
        return;
    }

    fwrite(&nbScores, sizeof(int), 1, f);
    fwrite(scores, sizeof(Joueurs), nbScores, f);

    fclose(f);
}

// Fonction pour mettre à jour le score d'un joueur
void mettreAJourScore(Joueurs scores[], int nbScores, const char *pseudo) {
    for (int i = 0; i < nbScores; i++) {
        if (strcmp(scores[i].pseudos, pseudo) == 0) {
            scores[i].score += 1; // Incrémenter le score du joueur
            return;
        }
    }
    // Si le pseudo n'est pas trouvé, l'ajouter
    // (cela ne devrait pas arriver si nous gérons bien les pseudos)
}

// Fonction pour afficher les scores
void afficherScores(Joueurs scores[], int nbScores) {
    printf("\n\n=== Scores des joueurs ===\n");
    for (int i = 0; i < nbScores; i++) {
        printf("%s : %d point(s)\n", scores[i].pseudos, scores[i].score);
    }
    printf("==========================\n\n");
}

// Fonction pour vérifier si un chemin est valide pour un joueur
int estCheminValide(char plateau[SIZE][SIZE][4], int startX, int startY, int targetRow) {
    return 1; // Désactivation de la vérification du chemin
}

// Fonction pour vérifier si une barrière peut être placée (désactivée)
int peutPlacerBarriere(char plateau[SIZE][SIZE][4], int x, int y, char orientation,
                       int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GameMode) {
    // Autoriser toujours le placement de la barrière
    return 1;
}
