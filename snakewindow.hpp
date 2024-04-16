#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include <QtWidgets/QtWidgets>
#include <QtCore/QtCore>
#include "jeu.hpp"

// Change level by change time variable in SnakeWindow
typedef enum
{
  EASY = 200,
  AVERAGE = 100,
  HARD = 80 
} Level;

class SnakeWindow : public QFrame
{
protected:
  Jeu jeu;
  Level level = EASY;
  QPixmap pixmapCorps, pixmapTete, pixmapMur, pixmapFood;

public:
  SnakeWindow(QWidget *pParent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    // Set up label to display game over message
  QLabel *gameOverLabel = new QLabel(this);
  void handleTimer();

  // Set new level value
  void setLevel(Level);

  Level getLevel() const;
  QString getLevelString(Level);
  void activateGameOver();
  void handleButtonAdd();
  void handleButtonDelete();

  // Display gameover 
  void displayGameOver(int);

protected:
  void paintEvent(QPaintEvent *);
  void keyPressEvent(QKeyEvent *);
};

// Using code in the TP subject to create the button add and remove wall
class SnakeButton : public QPushButton
{
  public:
    SnakeButton(QWidget *pParent=0):QPushButton(pParent) {}

  protected:
    void keyPressEvent(QKeyEvent *event)
    {
        if (parent()!=NULL)
            QCoreApplication::sendEvent(parent(), event);
    }
};
#endif
