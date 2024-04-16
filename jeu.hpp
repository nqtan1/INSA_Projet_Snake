#ifndef JEU_HPP
#define JEU_HPP
#include <cstdlib>
#include <ctime>
#include <list>

typedef enum
{
  VIDE,
  MUR
} Case;
typedef enum
{
  GAUCHE,
  DROITE,
  HAUT,
  BAS
} Direction;

class Position
{
public:
  int x, y;
  Position();
  Position(int, int);
  bool operator==(const Position &) const;
  bool operator!=(const Position &) const;
};

class Jeu
{
protected:
  Case *terrain;
  int largeur, hauteur; // Nombre de cases en largeur et en hauteur
  std::list<Position> snake;
  Direction dirSnake;
  Position foodPosition;

public:
  Jeu();
  Jeu(const Jeu &);
  ~Jeu();

  Jeu &operator=(const Jeu &);

  bool init();
  void evolue();

  // Retourne les dimensions (en nombre de cases)
  int getNbCasesX() const;
  int getNbCasesY() const;

  // Retourne la case a une position donn�e
  Case getCase(const Position &) const;

  // Retourne la liste des elements du serpent en lecture seule
  const std::list<Position> &getSnake() const;

  // Indique si la case a une position donn�e existe et est libre
  bool posValide(const Position &) const;

  // Modifie la direction
  void setDirection(Direction);

  /*-----------------Ajouter-----------------*/
  // Add and remove walls
  void addWall();
  void deleteWall();

  /*----Food-----*/
  // Generate food position
  void generateFoodPosition();

  // Return food position
  Position getFoodPosition() const;

  /*-----Collision-----*/
  bool isFoodCollision() const;
  bool isWallCollision() const;
  bool isSnakePosition(const Position &, const std::list<Position> &);
  bool isBorderCollision(const Position&) const;
  // Get Score 
  int getScore() const;
};
#endif
