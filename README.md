Projet informatique S1 - ING1
- PROJET QUORIDOR

Composition du groupe:
GOSSELIN Loane
LE NILIAS HOUMEAU Mathieu
VUCHOT Evangeline
CAZAU Thomas

Objectif du projet :
Ecrire en langage C le code du jeu de société Quoridor en respectant certaine fonctionnalités pour le jeu de base.
-->Le quoridor est un jeu de stratégie combinatoire pour 2 à 4 joueurs.
   Le jeu prend place sur un plateau de 9x9 case, pour la partie à deux joueur les pions sont positionnés l'un en face de l'autre sur les bords du 
   plateau , pour la partie à quatre joueur les pions sont positionnés de sur chaque bord du plateau.
   Le but du jeu est d'être le premier à atteindre la ligne opposée.

Fonctionnalités de base à réspectées :
-Le plateau de jeu et de la partie droite d'information et d'interaction.
-Le menu 
-La possibilité de joué à 2 ou 4 joueur.
-Le déplacement des pions en tenant compte des contraintes.
-Le placement des barrières en tenant compte des contraintes.
-L'interuption de partie 

Le code est à réalisé en prenant en compte les contraintes suivantes :
-Le jeu se présente sous forme de console
-Le plateau du jeu de base a 9 lignes et 9 colonnes avec la place de positionner des murs entre eux.
-Pour les barrières :
      -Les barrières d'une longueur de 2 cases se place entre deux lignes ou deux 
      colonnes. 
      -Elles ne peuvent pas être utilisées pour bloquer totalement le chemin de son 
      adversaire
      -Pour placé une barrières le joueur peu saisir les coordonnées de 2 cases 
       voisines ou bien déplacer le début et fin de barrière, en respectant la longueur 
       des 2 cases
      -En cas d'impossiblitité le coup n'est pas pris enc compte et le joueur rejoue
      -Les barrières sont représentées par la letter 'B'
-La possibilité d'interompre la partie en cours, elle doit être sauvegardée pour être reprise ulterieurement.
-En fin d'opération toujours redirigée vers le menu principale.

