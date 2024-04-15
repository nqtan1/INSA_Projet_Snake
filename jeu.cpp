#include <iostream>
#include <cassert>
#include "jeu.hpp"

using namespace std;

/*--------------------------------------------Position--------------------------------------------*/
Position::Position()
{
}

Position::Position(int a, int b)
{
    x = a;
    y = b;
}

bool Position::operator==(const Position &pos) const
{
    return (x == pos.x && y == pos.y);
}

bool Position::operator!=(const Position &pos) const
{
    return (x != pos.x || y != pos.y);
}

/*--------------------------------------------Jeu--------------------------------------------*/
Jeu::Jeu()
{
    terrain = nullptr;
    largeur = 0;
    hauteur = 0;
    dirSnake = BAS;
}

Jeu::Jeu(const Jeu &jeu) : snake(jeu.snake)
{
    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;

    if (jeu.terrain != nullptr)
    {
        terrain = new Case[largeur * hauteur];
        for (int c = 0; c < largeur * hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
}

Jeu::~Jeu()
{
    if (terrain != nullptr)
        delete[] terrain;
}

Jeu &Jeu::operator=(const Jeu &jeu)
{
    if (terrain != nullptr)
        delete[] terrain;

    largeur = jeu.largeur;
    hauteur = jeu.hauteur;
    dirSnake = jeu.dirSnake;
    snake = jeu.snake;

    if (jeu.terrain != nullptr)
    {
        terrain = new Case[largeur * hauteur];
        for (int c = 0; c < largeur * hauteur; c++)
            terrain[c] = jeu.terrain[c];
    }
    else
        terrain = nullptr;
    return *this;
}

bool Jeu::init()
{
    int x, y;
    // list<Position>::iterator itSnake;

    const char terrain_defaut[15][21] = {
        "####################",
        "#........##........#",
        "#.#####..##...####.#",
        "#........##........#",
        "#..................#",
        "#..................#",
        "#..................#",
        "#..................#",
        "#..................#",
        "#..................#",
        "#..................#",
        "#..................#",
        "#.....#......#.....#",
        "#.....#......#.....#",
        "####################"};

    largeur = 20;
    hauteur = 15;

    terrain = new Case[largeur * hauteur];

    for (y = 0; y < hauteur; ++y)
        for (x = 0; x < largeur; ++x)
            if (terrain_defaut[y][x] == '#')
                terrain[y * largeur + x] = MUR;
            else
                terrain[y * largeur + x] = VIDE;

    int longueurSerpent = 5;
    snake.clear();

    Position posTete;
    posTete.x = 15;
    posTete.y = 8;
    for (int i = 0; i < longueurSerpent; i++)
    {
        snake.push_back(posTete);
        posTete.x--;
    }

    foodPosition.x = 5;
    foodPosition.y = 5;

    return true;
}

void Jeu::evolue()
{
    Position posTest;
    list<Position>::iterator itSnake;

    int depX[] = {-1, 1, 0, 0};
    int depY[] = {0, 0, -1, 1};

    posTest.x = snake.front().x + depX[dirSnake];
    posTest.y = snake.front().y + depY[dirSnake];

    if (posValide(posTest))
    {
        snake.pop_back();
        snake.push_front(posTest);
    }

    if (isFoodCollision())
    {
        // Increase snake size
        snake.push_back(foodPosition);
        // Create new food position
        generateFoodPosition();
    }
}

int Jeu::getNbCasesX() const
{
    return largeur;
}

int Jeu::getNbCasesY() const
{
    return hauteur;
}

Case Jeu::getCase(const Position &pos) const
{
    assert(pos.x >= 0 && pos.x < largeur && pos.y >= 0 && pos.y < hauteur);
    return terrain[pos.y * largeur + pos.x];
}

const list<Position> &Jeu::getSnake() const
{
    return snake;
}

bool Jeu::posValide(const Position &pos) const
{
    if (pos.x >= 0 && pos.x < largeur && pos.y >= 0 && pos.y < hauteur && terrain[pos.y * largeur + pos.x] == VIDE)
    {
        list<Position>::const_iterator itSnake;
        itSnake = snake.begin();
        while (itSnake != snake.end() && *itSnake != pos)
            itSnake++;
        return (itSnake == snake.end());
    }
    else
        return false;
}

void Jeu::setDirection(Direction dir)
{
    dirSnake = dir;
}

/*-----------------Ajouter-----------------*/
void Jeu::generateFoodPosition()
{
    std::srand(std::time(nullptr));

    bool check = false;
    int attempts = 0;
    while (!check && attempts < 100)
    { // Limit attempts to prevent infinite loop
        foodPosition.x = std::rand() % largeur;
        foodPosition.y = std::rand() % hauteur;
        // Food position must satisfy the condition of
        // being different from the wall and the snake
        if (getCase(foodPosition) == VIDE && !isSnakePosition(foodPosition, snake))
            check = true;
        attempts++;
    }
}

Position Jeu::getFoodPosition() const
{
    return foodPosition;
}

bool Jeu::isFoodCollision() const
{
    return (snake.front() == foodPosition);
}

bool Jeu::isSnakePosition(const Position &newPos, const std::list<Position> &snake)
{
    for (const auto &pos : snake)
    {
        if (pos == newPos)
            return true;
    }
    return false;
}

bool Jeu::isWallCollision() const
{
    switch (dirSnake)
    {
    case GAUCHE:
        if (terrain[snake.front().y * largeur + (snake.front().x - 1)] == MUR)
            cout << "GAUCHE" << endl;
        return true;
        break;
    case DROITE:
        if (terrain[snake.front().y * largeur + (snake.front().x + 1)] == MUR)
            cout << "DROITE" << endl;
        return true;
        break;
    case HAUT:
        if (terrain[(snake.front().y - 1) * largeur + (snake.front().x)] == MUR)
            cout << "HAUT" << endl;
        return true;
        break;
    case BAS:
        if (terrain[(snake.front().y + 1) * largeur + (snake.front().x)] == MUR)
            cout << "BAS" << endl;
        return true;
        break;
    default:
        cout << "NON" << endl;
        return false;
        break;
    }
}

int Jeu::getScore() const
{
    return 10*(snake.size() - 5); 
}