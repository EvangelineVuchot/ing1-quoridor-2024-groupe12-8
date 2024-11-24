#include "header.h"

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
void afficherPlateau(char plateau[SIZE][SIZE][4], GameState *state) {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            char *c = plateau[i][j];
            int isBarrierTemp = 0;

            // Vérifier si une barrière est en cours de placement
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

// Fonction pour déplacer un pion
void deplacerPion(char plateau[SIZE][SIZE][4], int *x, int *y,
                  int direction, char joueur) {
    int newX = *x, newY = *y;

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

    // Vérifier s'il y a une barrière entre l'ancienne et la nouvelle position
    if (barriereEntre(*x, *y, newX, newY, plateau)) {
        return;
    }

    // Vérifier si la nouvelle case est libre ou un objectif
    if (strcmp(plateau[newX][newY], " ") == 0 || strchr("1234", plateau[newX][newY][0]) != NULL) {
        strcpy(plateau[*x][*y], " ");
        *x = newX;
        *y = newY;
        char strJoueur[2] = {joueur, '\0'};
        strcpy(plateau[*x][*y], strJoueur);
    }
}

// Fonction pour déplacer une barrière (changement de position lors de la sélection)
void deplacerBarriere(char plateau[SIZE][SIZE][4], int *x, int *y, int direction, char joueur,
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

    // Calculer les nouvelles coordonnées
    int newX = *x + deltaX;
    int newY = *y + deltaY;

    // Vérifier les limites du plateau
    if (newX < 1 || newX > SIZE - 3 || newY < 1 || newY > SIZE - 3) {
        printf("Déplacement hors limites. Impossible de déplacer la barrière.\n");
        return;
    }

    // Vérifier si la nouvelle position est un espace
    if (strcmp(plateau[newX][newY], " ") == 0) {
        printf("Déplacement sur un espace vide interdit. Choisissez une autre direction.\n");
        return;
    }

    // Vérifier si la nouvelle position est occupée par un autre élément (barrière, joueur, etc.)
    if (strcmp(plateau[newX][newY], "B") == 0 ||
        strchr("1234", plateau[newX][newY][0]) != NULL) {
        printf("Déplacement sur une position occupée par un autre élément. Choisissez une autre direction.\n");
        return;
    }

    // Si toutes les vérifications passent, déplacer la barrière
    *x = newX;
    *y = newY;
}

// Fonction pour placer définitivement une barrière
void placerBarriere(char plateau[SIZE][SIZE][4], int x, int y,
                    char orientation, GameState *state) {
    // Vérifier que la barrière ne chevauche aucun joueur
    if ((strcmp(plateau[x][y], "1") == 0) || (strcmp(plateau[x][y], "2") == 0) ||
        (strcmp(plateau[x][y], "3") == 0) || (strcmp(plateau[x][y], "4") == 0)) {
        printf("Erreur : Impossible de placer une barrière sur la position d'un joueur.\n");
        return;
    }

    // Vérifier que la barrière ne chevauche pas une autre barrière
    if (strcmp(plateau[x][y], "B") == 0) {
        printf("Erreur : Une barrière est déjà présente à cette position.\n");
        return;
    }

    // Vérifier le nombre maximal de barrières
    if (state->nb_barrieres >= 20) {
        printf("Le nombre maximal de barrières placées est atteint !\n");
        return;
    }

    // Placer la barrière selon l'orientation
    if (orientation == 'V') {
        if (x > 0 && x < SIZE - 3) {
            strcpy(plateau[x - 1][y], "B");
            strcpy(plateau[x][y], "B");
            strcpy(plateau[x + 1][y], "B");
            strcpy(plateau[x + 2][y], "B");
            state->nb_barrieres++;
        } else {
            printf("Erreur : Orientation verticale hors des limites du plateau.\n");
        }
    } else if (orientation == 'H') {
        if (y > 0 && y < SIZE - 3) {
            strcpy(plateau[x][y - 1], "B");
            strcpy(plateau[x][y], "B");
            strcpy(plateau[x][y + 1], "B");
            strcpy(plateau[x][y + 2], "B");
            state->nb_barrieres++;
        } else {
            printf("Erreur : Orientation horizontale hors des limites du plateau.\n");
        }
    }
}

// Fonction pour afficher l'écran de victoire
void afficherEcranVictoire(int joueur, Pseudos pseudos[4]) {
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
                       GameState *state) {

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

    fclose(f);
    printf("Partie sauvegardée dans le fichier '%s'.\n", nomFichier);
}

// Fonction pour charger une partie
void chargerPartie(const char *nomFichier,
                   char plateau[SIZE][SIZE][4], int *x1, int *y1, int *x2, int *y2,
                   int *x3, int *y3, int *x4, int *y4, int *tour, int *GameMode,
                   GameState *state) {

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

    // Placer des barrières pour simuler une partie avancée
    placerBarriere(plateau, 6, 9, 'H', state);
    placerBarriere(plateau, 8, 7, 'V', state);
    placerBarriere(plateau, 10, 9, 'H', state);
    placerBarriere(plateau, 12, 11, 'V', state);

    // Initialiser l'état du jeu
    *tour = 1; // C'est au tour du joueur 1 de jouer
    *state = (GameState){0, 0, 0, 0, 0, 0, 'V', 4}; // Initialisation de state
}

// Fonction pour saisir et afficher les pseudos des joueurs
void Pseudo(Pseudos pseudo[4], int joueur, int *GameMode) {
    for (int i = 0; i < *GameMode; i++) {
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {
            // Consommer le caractère '\n' restant dans le tampon
        }
        printf("\n");
        printf("\n");
        printf("                                            Saisir le pseudo du joueur %d (50 caractères maximum) :", i + 1);
        fgets(pseudo[i].pseudos, PSEUDO, stdin);
        // Supprimer le caractère '\n' si présent
        size_t len = strlen(pseudo[i].pseudos);
        if (len > 0 && pseudo[i].pseudos[len - 1] == '\n') {
            pseudo[i].pseudos[len - 1] = '\0';
        }
    }
    printf("\n");
    printf("\n                                                        Pseudos enregistrés :\n");  // Vérification pour savoir si les pseudos sont bien enregistrés
    for (int i = 0; i < *GameMode; i++) {

        printf("                                                        Joueur %d: %s\n", i + 1, pseudo[i].pseudos);
    }
    printf("\n");
    printf("                                                        Appuyez sur entrer pour commencer à jouer.\n");
    getchar(); // Attendre que l'utilisateur appuie sur Entrée
}

// Fonction pour vérifier si un chemin est valide pour un joueur
int estCheminValide(char plateau[SIZE][SIZE][4], int startX, int startY, int targetRow) {
    int visited[SIZE][SIZE];
    memset(visited, 0, sizeof(visited));
    int queue[SIZE * SIZE][2];
    int front = 0, rear = 0;

    // Enfiler la position de départ
    queue[rear][0] = startX;
    queue[rear][1] = startY;
    rear++;
    visited[startX][startY] = 1;

    while (front < rear) {
        int x = queue[front][0];
        int y = queue[front][1];
        front++;

        // Vérifier si on a atteint la ligne cible
        if (x == targetRow) {
            return 1; // Chemin valide trouvé
        }

        // Vérifier les voisins (haut, bas, gauche, droite)
        int directions[4][2] = {{-2, 0}, {2, 0}, {0, -2}, {0, 2}};
        for (int i = 0; i < 4; i++) {
            int newX = x + directions[i][0];
            int newY = y + directions[i][1];

            // Vérifier si le mouvement est valide
            if (newX >= 0 && newX < SIZE && newY >= 0 && newY < SIZE &&
                !visited[newX][newY] &&
                strcmp(plateau[(x + newX) / 2][(y + newY) / 2], "B") != 0 && // Pas de barrière entre les cases
                (strcmp(plateau[newX][newY], " ") == 0 || strchr("1234", plateau[newX][newY][0]) != NULL)) { // Case libre ou objectif
                queue[rear][0] = newX;
                queue[rear][1] = newY;
                rear++;
                visited[newX][newY] = 1;
            }
        }
    }

    return 0; // Aucun chemin trouvé
}
