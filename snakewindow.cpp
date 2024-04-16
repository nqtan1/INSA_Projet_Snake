#include <iostream>
#include "snakewindow.hpp"
#include "jeu.hpp"
#include <cstdlib>
#include <ctime>
using namespace std;

SnakeWindow::SnakeWindow(QWidget *pParent, Qt::WindowFlags flags) : QFrame(pParent, flags)
{
    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    if (pixmapCorps.load("./data/snake_corps.png") == false)
    {
        cout << "Impossible d'ouvrir snake_corps.png" << endl;
        exit(-1);
    }

    if (pixmapTete.load("./data/snake_tete.png") == false)
    {
        cout << "Impossible d'ouvrir snake_tete.png" << endl;
        exit(-1);
    }

    if (pixmapMur.load("./data/mur.bmp") == false)
    {
        cout << "Impossible d'ouvrir mur.bmp" << endl;
        exit(-1);
    }

    if (pixmapFood.load("./data/food.bmp") == false)
    {
        cout << "Impossible d'ouvrir food.bmp" << endl;
        exit(-1);
    }

    jeu.init();

    // Set title of window
    setWindowTitle("Snake Hunting");

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &SnakeWindow::handleTimer);
    timer->start(level);

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Put a randomly wall
    SnakeButton *btnAdd = new SnakeButton(this);

    btnAdd->setText("Add Wall");
    btnAdd->setFixedSize(120, 30);
    btnAdd->move(200, 490);

    // Remove a randomly wall
    SnakeButton *btnDelete = new SnakeButton(this);
    btnDelete->setText("Delete Wall");
    btnDelete->setFixedSize(120, 30);
    btnDelete->move(325, 490);

    // Show information:
    SnakeButton *btnInfo = new SnakeButton(this);
    btnInfo->setText("Info");
    btnInfo->setFixedSize(120, 30);
    btnInfo->move(325, 530);

    connect(btnAdd, &QPushButton::clicked, this, &SnakeWindow::handleButtonAdd);
    connect(btnDelete, &QPushButton::clicked, this, &SnakeWindow::handleButtonDelete);
    connect(btnInfo, &QPushButton::clicked, this, &SnakeWindow::handleButtonDelete);

    resize(jeu.getNbCasesX() * largeurCase, (jeu.getNbCasesY() + 3) * hauteurCase);
}

void SnakeWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    Position pos;

    // Taille des cases en pixels
    int largeurCase, hauteurCase;

    largeurCase = pixmapMur.width();
    hauteurCase = pixmapMur.height();

    // Dessine les cases
    for (pos.y = 0; pos.y < jeu.getNbCasesY(); pos.y++)
        for (pos.x = 0; pos.x < jeu.getNbCasesX(); pos.x++)
            if (jeu.getCase(pos) == MUR)
                painter.drawPixmap(pos.x * largeurCase, pos.y * hauteurCase, pixmapMur);

    // Dessine le serpent
    const list<Position> &snake = jeu.getSnake();
    if (!snake.empty())
    {
        list<Position>::const_iterator itSnake;
        const Position &posTete = snake.front();
        painter.drawPixmap(posTete.x * largeurCase, posTete.y * hauteurCase, pixmapTete);

        for (itSnake = ++snake.begin(); itSnake != snake.end(); itSnake++)
            painter.drawPixmap(itSnake->x * largeurCase, itSnake->y * hauteurCase, pixmapCorps);
    }
    // Dessine la nourriture
    for (pos.y = 0; pos.y < jeu.getNbCasesY(); pos.y++)
        for (pos.x = 0; pos.x < jeu.getNbCasesX(); pos.x++)
            painter.drawPixmap(jeu.getFoodPosition().x * largeurCase, jeu.getFoodPosition().y * hauteurCase, pixmapFood);

    // Display score
    QString scoreText = "Score: " + QString::number(jeu.getScore());
    painter.drawText(QRect(10, 500, 100, 520), Qt::AlignLeft, scoreText);

    // Display current level
    Level currentLevel = getLevel();
    QString levelText = "Level: " + getLevelString(currentLevel);
    painter.drawText(QRect(width() - 110, 500, 100, 520), Qt::AlignRight, levelText);

    displayGameOver(jeu.getScore());
}

void SnakeWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left)
        jeu.setDirection(GAUCHE);
    else if (event->key() == Qt::Key_Right)
        jeu.setDirection(DROITE);
    else if (event->key() == Qt::Key_Up)
        jeu.setDirection(HAUT);
    else if (event->key() == Qt::Key_Down)
        jeu.setDirection(BAS);
    update();
}

void SnakeWindow::handleTimer()
{
    jeu.evolue();
    update();
}

Level SnakeWindow::getLevel() const
{
    return level;
}

// Use string to display the level
QString SnakeWindow::getLevelString(Level level)
{
    switch (level)
    {
    case EASY:
        return "Easy";
    case AVERAGE:
        return "Average";
    case HARD:
        return "Hard";
    default:
        return "Unknown!";
    }
}

void SnakeWindow::setLevel(Level l)
{
    level = l;
}

void SnakeWindow::activateGameOver()
{
    update();
}

void SnakeWindow::handleButtonAdd()
{
    jeu.addWall();
    update();
}

void SnakeWindow::handleButtonDelete()
{
    jeu.deleteWall();
    update();
}

void SnakeWindow::displayGameOver(int score)
{
    QFont gameOverFont;
    gameOverFont.setPointSize(15);
    gameOverLabel->setFont(gameOverFont);

    gameOverLabel->setText("GAME OVER!\n Score: " + QString::number(score));
    // Center the text in the label
    gameOverLabel->setAlignment(Qt::AlignCenter);
    // Set margins for borders
    gameOverLabel->setMargin(15);
    gameOverLabel->move(250, 200);
    if (jeu.isBorderCollision(jeu.getSnake().front()) == true || jeu.posValide(jeu.getSnake().front()) == true || jeu.isWallCollision() == true)
    {
        gameOverLabel->show();
    }
    else
    {
        gameOverLabel->hide();
    }
}
