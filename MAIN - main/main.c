#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "header.h"

int main() {
    char plateau[SIZE][SIZE][4];
    int x1, y1, x2, y2, x3, y3, x4, y4;
    int tour;
    int GameMode;
    GameState state;
    Joueurs pseudos[4];
    Joueurs scores[100]; // Tableau pour stocker jusqu'à 100 joueurs
    int nbScores = 0;    // Nombre actuel de joueurs dans les scores

    srand(time(NULL));
#ifdef _WIN32
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hOut, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(hOut, dwMode);
    SetConsoleOutputCP(CP_UTF8);
#endif

    int choix;

    do {
        // Affichage du menu
        printf("\n");
        printf("\n");
        printf("                              *******      **     **     *******     *******     **   *******       *******     *******  \n");
        printf("                             **/////**    /**    /**    **/////**   /**////**   /**  /**////**     **/////**   /**////** \n");
        printf("                            **     //**   /**    /**   **     //**  /**   /**   /**  /**    /**   **     //**  /**   /** \n");
        printf("                           /**      /**   /**    /**  /**      /**  /*******    /**  /**    /**  /**      /**  /*******  \n");
        printf("                           /**    **/**   /**    /**  /**      /**  /**///**    /**  /**    /**  /**      /**  /**///**  \n");
        printf("                           //**  // **    /**    /**  //**     **   /**  //**   /**  /**    **   //**     **   /**  //** \n");
        printf("                            //******* **  //*******    //*******    /**   //**  /**  /*******     //*******    /**   //**\n");
        printf("                             /////// //    ///////      ///////     //     //   //   ///////       ///////     //     // \n");
        printf("\n");
        printf("\n");
        printf("                                                        Choisissez une option :\n");
        printf("\n");
        printf("                                                        0. COMMENCER UNE NOUVELLE PARTIE\n");
        printf("                                                        1. CHARGER UNE PARTIE SAUVEGARDEE\n");
        printf("                                                        2. CHARGER LA PARTIE DE DEMONSTRATION\n");
        printf("                                                        3. AFFICHER L'AIDE\n");
        printf("                                                        4. AFFICHER LES SCORES\n");
        printf("                                                        5. QUITTER\n");
        printf("\n");
        printf("                                                                  Votre choix : ");
        scanf("%d", &choix);

        int c;
        while ((c = getchar()) != '\n' && c != EOF);

        // Gestion des choix
        switch (choix) {
            case 0:
                // Initialisation des variables pour une nouvelle partie
                x1 = 1; y1 = SIZE / 2;
                x2 = SIZE - 2; y2 = SIZE / 2;
                x3 = SIZE / 2; y3 = 1;
                x4 = SIZE / 2; y4 = SIZE - 2;
                GameMode = 0;
                state = (GameState){0, 0, 0, 0, 0, 0, 'V', 5, 5, 5, 5};

                // Charger les scores depuis le fichier
                chargerScores("scores.dat", scores, &nbScores);

                // Demander le mode de jeu
                do {
                    printf("\n");
                    printf("                                                        Choisissez le mode de jeu (2 ou 4) :\n");
                    printf("\n");
                    printf("                                                        Mode 2 - 2 Joueurs\n");
                    printf("                                                        Mode 4 - 4 Joueurs\n");
                    printf("\n");
                    printf("                                                                  Votre choix : ");
                    scanf("%d", &GameMode);
                    if (GameMode != 2 && GameMode != 4) {
                        printf("Erreur ! Veuillez choisir un mode de jeu valide.\n\n");
                    } else if (GameMode == 2 || GameMode == 4){
                        Pseudo(pseudos, &GameMode, scores, &nbScores);
                    }
                } while (GameMode != 2 && GameMode != 4);

                // Consommer le caractère '\n' restant dans le tampon
                while ((c = getchar()) != '\n' && c != EOF) {}

                // Initialisation du plateau
                initialiserPlateau(plateau);
                strcpy(plateau[x1][y1], "1");
                strcpy(plateau[x2][y2], "2");

                if (GameMode == 4) {
                    strcpy(plateau[x3][y3], "3");
                    strcpy(plateau[x4][y4], "4");
                }
                break;
            
            case 1:
                // Charger les scores depuis le fichier
                chargerScores("scores.dat", scores, &nbScores);
                chargerPartie("sauvegarde.dat", plateau, &x1, &y1, &x2, &y2,
                              &x3, &y3, &x4, &y4, &tour, &GameMode, &state);
                break;
            
            case 2:
                // Charger les scores depuis le fichier
                chargerScores("scores.dat", scores, &nbScores);
                initialiserDemo(plateau, &x1, &y1, &x2, &y2,
                                &x3, &y3, &x4, &y4, &tour, &GameMode, &state);
                break;
            
            case 3:
                clearConsole(); // Effacer la console
                printf("\n QUORIDOR\n");
                printf("\n1. BUT DU JEU\n");
                printf("\nLe jeu du Quoridor est un jeu de réflexion où le but est d'atteindre le premier la ligne opposée.\nLes joueurs jouent chacun leur tour et ont la possibilité d'effectuer une des deux actions possibles : Avancer son pion ou poser une barrière.\n");
                printf("\n2. RÈGLES DU JEU\n");
                printf("\n");
                printf("A. RÈGLE POUR 2 JOUEURS\nEn début de partie, les barrières sont remisées dans leur zone de stockage : 10 barrières par joueur.\nChaque joueur pose son pion au centre de sa ligne de départ.\nUn tirage au sort détermine qui commence.\n");
                printf("\n");
                printf("B. RÈGLE POUR 4 JOUEURS\nEn début de partie, les 4 pions sont disposés au centre de chacun des 4 côtés du plateau et chaque joueur dispose de 5 barrières.\nLes règles sont strictement identiques, mais on ne peut sauter plus d’un pion à la fois.\n");
                printf("\n");
                printf("3. DÉROULEMENT DE LA PARTIE\nÀ tour de rôle, chaque joueur choisit de déplacer son pion ou de poser une de ses barrières.\nLorsqu’il n’a plus de barrières, un joueur est obligé de déplacer son pion.\n");
                printf("\n");
                printf("1. Déplacement des pions:\nLes pions se déplacent d’une case, horizontalement ou verticalement, en avant ou en arrière, les barrières doivent être contournées.\n");
                printf("\n2. Pose des barrières:\nUne barrière doit être posée exactement entre 2 blocs de 2 cases.\nLa pose des barrières a pour but de se créer son propre chemin ou de ralentir l’adversaire, mais il est interdit de lui fermer totalement l’accès à sa ligne de but: il faut toujours lui laisser une solution.\n");
                printf("\n3. Face à face:\nQuand les 2 pions se retrouvent en vis-à-vis sur 2 cases voisines non séparées par une barrière, le joueur dont c’est le tour peut sauter son adversaire et se placer derrière lui.\nSi une barrière est située derrière le pion, le joueur peut choisir de bifurquer à droite ou à gauche.\n");
                printf("\nFIN DE PARTIE\nLe premier joueur qui atteint une des 9 cases de la ligne opposée à sa ligne de départ gagne la partie.\n");
                printf("\nDURÉE DE PARTIE\nLe jeu dure de 10 à 20 minutes.\n");
                do {
                    printf("\nAPPUYEZ SUR M POUR RETOURNER AU MENU\n");
                    scanf(" %c", (char *)&choix);
                    if (choix == 'm' || choix == 'M') {
                        break;
                    }
                } while (choix != 'm' && choix != 'M');
                break;
            
            case 4:
                // Charger les scores depuis le fichier
                chargerScores("scores.dat", scores, &nbScores);
                // Afficher les scores
                afficherScores(scores, nbScores);
                printf("Appuyez sur Entrée pour revenir au menu principal...");
                getchar();
                getchar(); // Attendre que l'utilisateur appuie sur Entrée
                break;
            
            case 5:
                exit(0);
            default:
                printf("Choix invalide. Veuillez réessayer.\n");
                break;
        }

        if (choix == 0 || choix == 1 || choix == 2) {
            tour = (rand() % GameMode) + 1;

            // Boucle principale du jeu
            while (1) {
                clearConsole(); // Effacer la console

                afficherPlateau(plateau, GameMode, &state, pseudos);

                int input;
                printf("C'est à %s de jouer.\n", pseudos[tour-1].pseudos);

                // Afficher les actions possibles selon le joueur et le mode placement
                if (tour == 1) {
                    if (state.modePlacement1 == 0) {
                        printf("\nActions possibles :\n");
                        printf("- Déplacez votre pion avec : z/q/s/d.\n");
                        printf("- Appuyez sur 'a' pour placer une barrière.\n");
                        printf("- Appuyez sur 'T' pour sauvegarder.\n");
                    } else {
                        printf("\nPlacement barrières :\n");
                        printf("- Déplacez la barrière avec z/q/s/d.\n");
                        printf("- Changez l'orientation avec 'o'.\n");
                        printf("- Appuyez sur Entrée pour la placer.\n");
                        printf("- Le nombre de barrières restantes pour %s est de : %d barrières.\n", pseudos[tour-1].pseudos, state.nb_barrieres1);
                    }
                } else if (tour == 2) {
                    if (state.modePlacement2 == 0) {
                        printf("\nActions possibles :\n");
                        printf("- Déplacez votre pion avec les flèches du clavier.\n");
                        printf("- Appuyez sur 'a' pour placer une barrière.\n");
                        printf("- Appuyez sur 'T' pour sauvegarder.\n");
                    } else {
                        printf("\nPlacement barrières :\n");
                        printf("- Déplacez la barrière avec les flèches du clavier.\n");
                        printf("- Changez l'orientation avec 'o'.\n");
                        printf("- Appuyez sur Entrée pour la placer.\n");
                        printf("- Le nombre de barrières restantes pour %s est de : %d barrières.\n", pseudos[tour-1].pseudos, state.nb_barrieres2);
                    }
                } else if (tour == 3) {
                    if (state.modePlacement3 == 0) {
                        printf("\nActions possibles :\n");
                        printf("- Déplacez votre pion avec : t/f/g/h.\n");
                        printf("- Appuyez sur 'a' pour placer une barrière.\n");
                        printf("- Appuyez sur 'T' pour sauvegarder.\n");
                    } else {
                        printf("\nPlacement barrières :\n");
                        printf("- Déplacez la barrière avec t/f/g/h.\n");
                        printf("- Changez l'orientation avec 'o'.\n");
                        printf("- Appuyez sur Entrée pour la placer.\n");
                        printf("- Le nombre de barrières restantes pour %s est de : %d barrières.\n", pseudos[tour-1].pseudos, state.nb_barrieres3);
                    }
                } else if (tour == 4) {
                    if (state.modePlacement4 == 0) {
                        printf("\nActions possibles :\n");
                        printf("- Déplacez votre pion avec : i/j/k/l.\n");
                        printf("- Appuyez sur 'a' pour placer une barrière.\n");
                        printf("- Appuyez sur 'T' pour sauvegarder.\n");
                    } else {
                        printf("\nPlacement barrières :\n");
                        printf("- Déplacez la barrière avec i/j/k/l.\n");
                        printf("- Changez l'orientation avec 'o'.\n");
                        printf("- Appuyez sur Entrée pour la placer.\n");
                        printf("- Le nombre de barrières restantes pour %s est de : %d barrières.\n", pseudos[tour-1].pseudos, state.nb_barrieres4);
                    }
                }

                input = lireTouche();

                if (input == 'T') {
                    sauvegarderPartie("sauvegarde.dat", plateau, x1, y1, x2, y2,
                                      x3, y3, x4, y4, tour, GameMode, &state);
                    printf("Partie sauvegardée ! Appuyez sur Entrée pour continuer...");
                    getchar(); // Consommer le '\n' restant
                    getchar(); // Attendre que l'utilisateur appuie sur Entrée
                    continue;
                }

                // Gestion des actions selon le joueur courant
                if (tour == 1) {
                    if (state.modePlacement1 == 0 && input == 'a') {
                        if (state.nb_barrieres1 > 0) {
                            state.modePlacement1 = 1;
                            state.barrierX = x1;
                            state.barrierY = y1;
                            state.orientation = 'V';
                        } else {
                            printf("Vous n'avez plus de barrières à placer !\n");
                            printf("Appuyez sur Entrée pour continuer...");
                            getchar();
                        }
                    } else if (state.modePlacement1 == 1) {
                        if (input == '\n' || input == '\r') { // Touche Entrée
                            // Placer la barrière sans vérifier
                            placerBarriere(plateau, state.barrierX, state.barrierY, state.orientation, &state, 1);
                            state.modePlacement1 = 0;
                            tour = 2;
                        } else if (input == 'o' || input == 'O') {
                            state.orientation = (state.orientation == 'V') ? 'H' : 'V';
                        } else {
                            deplacerBarriere(&state.barrierX, &state.barrierY, input, PLAYER1, &state);
                        }
                    } else {
                        deplacerPion(plateau, &x1, &y1, (char)input, PLAYER1,
                                     x1, y1, x2, y2, x3, y3, x4, y4, GameMode);

                        if (x1 == SIZE - 2) {
                            clearConsole();
                            afficherPlateau(plateau, GameMode, &state, pseudos);
                            afficherEcranVictoire(1, pseudos);

                            // Mettre à jour le score du joueur gagnant
                            mettreAJourScore(scores, nbScores, pseudos[0].pseudos);
                            // Sauvegarder les scores mis à jour
                            sauvegarderScores("scores.dat", scores, nbScores);

                            printf("Appuyez sur Entrée pour quitter...");
                            getchar();
                            getchar();
                            break;
                        }

                        tour = 2;
                    }
                } else if (tour == 2) {
                    if (state.modePlacement2 == 0 && input == 'a') {
                        if (state.nb_barrieres2 > 0) {
                            state.modePlacement2 = 1;
                            state.barrierX = x2;
                            state.barrierY = y2;
                            state.orientation = 'V';
                        } else {
                            printf("Vous n'avez plus de barrières à placer !\n");
                            printf("Appuyez sur Entrée pour continuer...");
                            getchar();
                        }
                    } else if (state.modePlacement2 == 1) {
                        if (input == '\n' || input == '\r') { // Touche Entrée
                            // Placer la barrière sans vérifier
                            placerBarriere(plateau, state.barrierX, state.barrierY, state.orientation, &state, 2);
                            state.modePlacement2 = 0;
                            tour = (GameMode == 2) ? 1 : 3;
                        } else if (input == 'o' || input == 'O') {
                            state.orientation = (state.orientation == 'V') ? 'H' : 'V';
                        } else {
                            deplacerBarriere(&state.barrierX, &state.barrierY, input, PLAYER2, &state);
                        }
                    } else {
                        deplacerPion(plateau, &x2, &y2, (char)input, PLAYER2,
                                     x1, y1, x2, y2, x3, y3, x4, y4, GameMode);

                        if (x2 == 1) {
                            clearConsole();
                            afficherPlateau(plateau, GameMode, &state, pseudos);
                            afficherEcranVictoire(2, pseudos);

                            // Mettre à jour le score du joueur gagnant
                            mettreAJourScore(scores, nbScores, pseudos[1].pseudos);
                            // Sauvegarder les scores mis à jour
                            sauvegarderScores("scores.dat", scores, nbScores);

                            printf("Appuyez sur Entrée pour quitter...");
                            getchar();
                            getchar();
                            break;
                        }

                        tour = (GameMode == 2) ? 1 : 3;
                    }
                } else if (tour == 3) {
                    if (state.modePlacement3 == 0 && input == 'a') {
                        if (state.nb_barrieres3 > 0) {
                            state.modePlacement3 = 1;
                            state.barrierX = x3;
                            state.barrierY = y3;
                            state.orientation = 'V';
                        } else {
                            printf("Vous n'avez plus de barrières à placer !\n");
                            printf("Appuyez sur Entrée pour continuer...");
                            getchar();
                        }
                    } else if (state.modePlacement3 == 1) {
                        if (input == '\n' || input == '\r') { // Touche Entrée
                            // Placer la barrière sans vérifier
                            placerBarriere(plateau, state.barrierX, state.barrierY, state.orientation, &state, 3);
                            state.modePlacement3 = 0;
                            tour = 4;
                        } else if (input == 'o' || input == 'O') {
                            state.orientation = (state.orientation == 'V') ? 'H' : 'V';
                        } else {
                            deplacerBarriere(&state.barrierX, &state.barrierY, input, PLAYER3, &state);
                        }
                    } else {
                        deplacerPion(plateau, &x3, &y3, (char)input, PLAYER3,
                                     x1, y1, x2, y2, x3, y3, x4, y4, GameMode);

                        if (y3 == SIZE - 2) {
                            clearConsole();
                            afficherPlateau(plateau, GameMode, &state, pseudos);
                            afficherEcranVictoire(3, pseudos);

                            // Mettre à jour le score du joueur gagnant
                            mettreAJourScore(scores, nbScores, pseudos[2].pseudos);
                            // Sauvegarder les scores mis à jour
                            sauvegarderScores("scores.dat", scores, nbScores);

                            printf("Appuyez sur Entrée pour quitter...");
                            getchar();
                            getchar();
                            break;
                        }

                        tour = 4;
                    }
                } else if (tour == 4) {
                    if (state.modePlacement4 == 0 && input == 'a') {
                        if (state.nb_barrieres4 > 0) {
                            state.modePlacement4 = 1;
                            state.barrierX = x4;
                            state.barrierY = y4;
                            state.orientation = 'V';
                        } else {
                            printf("Vous n'avez plus de barrières à placer !\n");
                            printf("Appuyez sur Entrée pour continuer...");
                            getchar();
                        }
                    } else if (state.modePlacement4 == 1) {
                        if (input == '\n' || input == '\r') { // Touche Entrée
                            // Placer la barrière sans vérifier
                            placerBarriere(plateau, state.barrierX, state.barrierY, state.orientation, &state, 4);
                            state.modePlacement4 = 0;
                            tour = 1;
                        } else if (input == 'o' || input == 'O') {
                            state.orientation = (state.orientation == 'V') ? 'H' : 'V';
                        } else {
                            deplacerBarriere(&state.barrierX, &state.barrierY, input, PLAYER4, &state);
                        }
                    } else {
                        deplacerPion(plateau, &x4, &y4, (char)input, PLAYER4,
                                     x1, y1, x2, y2, x3, y3, x4, y4, GameMode);

                        if (y4 == 1) {
                            clearConsole();
                            afficherPlateau(plateau, GameMode, &state, pseudos);
                            afficherEcranVictoire(4, pseudos);

                            // Mettre à jour le score du joueur gagnant
                            mettreAJourScore(scores, nbScores, pseudos[3].pseudos);
                            // Sauvegarder les scores mis à jour
                            sauvegarderScores("scores.dat", scores, nbScores);

                            printf("Appuyez sur Entrée pour quitter...");
                            getchar();
                            getchar();
                            break;
                        }

                        tour = 1;
                    }
                }
            }
        }
        clearConsole();
    } while (choix != 5); // Quitte lorsque l'utilisateur choisit l'option 5.

    return 0;
}
