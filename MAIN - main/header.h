#ifndef HEADER_H
#define HEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <conio.h>
#include <windows.h>

#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

#else
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#endif

// Taille du Plateau 9x9
#define SIZE 19
#define PLAYER1 '1'
#define PLAYER2 '2'
#define PLAYER3 '3'
#define PLAYER4 '4'
#define PSEUDO 50

// Caractères de dessin de boîte
#define VERTICAL_LINE       "║"
#define HORIZONTAL_LINE     "═"
#define TOP_LEFT_CORNER     "╔"
#define TOP_RIGHT_CORNER    "╗"
#define BOTTOM_LEFT_CORNER  "╚"
#define BOTTOM_RIGHT_CORNER "╝"
#define INTERSECTION        "╬"

// Structure pour l'état de jeu
typedef struct {
    int modePlacement1;
    int modePlacement2;
    int modePlacement3;
    int modePlacement4;
    int barrierX;
    int barrierY;
    char orientation;
    int nb_barrieres1;
    int nb_barrieres2;
    int nb_barrieres3;
    int nb_barrieres4;
} GameState;

// Structure pour les pseudos et les scores des joueurs
typedef struct {
    char pseudos[PSEUDO];
    int score;
} Joueurs;

// Déclarations des fonctions
void initialiserPlateau(char plateau[SIZE][SIZE][4]);

void afficherPlateau(char plateau[SIZE][SIZE][4], int Gamemode, GameState *state, Joueurs pseudos[]);
int lireTouche();
void clearConsole();

int barriereEntre(int x1, int y1, int x2, int y2, char plateau[SIZE][SIZE][4]);

void deplacerPion(char plateau[SIZE][SIZE][4], int *x, int *y, char direction, char joueur,
                  int x1, int y1, int x2, int y2,
                  int x3, int y3, int x4, int y4, int GameMode);

void deplacerBarriere(int *x, int *y, int direction, char joueur, GameState *state);
void placerBarriere(char plateau[SIZE][SIZE][4], int x, int y, char orientation, GameState *state, int joueur);
int estCheminValide(char plateau[SIZE][SIZE][4], int startX, int startY, int targetRow);

int peutPlacerBarriere(char plateau[SIZE][SIZE][4], int x, int y, char orientation,
                       int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int GameMode);

void afficherEcranVictoire(int joueur, Joueurs pseudos[4]);

void sauvegarderPartie(const char *nomFichier, char plateau[SIZE][SIZE][4],
                       int x1, int y1, int x2, int y2,
                       int x3, int y3, int x4, int y4,
                       int tour, int GameMode, GameState *state, Joueurs pseudos[4]);

void chargerPartie(const char *nomFichier, char plateau[SIZE][SIZE][4],
                   int *x1, int *y1, int *x2, int *y2,
                   int *x3, int *y3, int *x4, int *y4,
                   int *tour, int *GameMode, GameState *state, Joueurs pseudos[4]);

void initialiserDemo(char plateau[SIZE][SIZE][4], int *x1, int *y1,
                     int *x2, int *y2, int *x3, int *y3,
                     int *x4, int *y4, int *tour, int *GameMode, GameState *state);

// Déclarations des fonctions pour la gestion des scores
void chargerScores(const char *nomFichier, Joueurs scores[], int *nbScores);
void sauvegarderScores(const char *nomFichier, Joueurs scores[], int nbScores);
void mettreAJourScore(Joueurs scores[], int nbScores, const char *pseudo);
void afficherScores(Joueurs scores[], int nbScores);

// Modification de la déclaration de la fonction Pseudo
void Pseudo(Joueurs pseudosPartie[4], int *GameMode, Joueurs scores[], int *nbScores);

#endif // HEADER_H
