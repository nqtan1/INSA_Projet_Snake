#ifndef SNAKEWINDOW_HPP
#define SNAKEWINDOW_HPP

#include<QtWidgets/QtWidgets> 
#include<QtCore/QtCore> 

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
    QPixmap pixmapCorps, pixmapTete, pixmapMur,pixmapFood;

  public:
    SnakeWindow(QWidget *pParent=nullptr, Qt::WindowFlags flags=Qt::WindowFlags());
    void handleTimer();
    
    // Set new level value  
    void setLevel (Level);
    void setLevel (int);

    Level getLevel () const;
    QString getLevelString(Level);

  protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *);

};

#endif
