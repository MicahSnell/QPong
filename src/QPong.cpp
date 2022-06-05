/**
 * @file QPong.cpp
 * implementation file for QPong
 */

#include <iostream>

#include <QPainter>
#include <QDateTime>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>

#include "QPong.h"

// constant initializations
const int PongArea::MARGIN_WIDTH_PX = 16;
const int PongArea::REFRESH_RATE_HZ = 60;
const int PongArea::PADDLE_WIDTH_PX = 10;
const int PongArea::PADDLE_HEIGHT_PX = 100;
const int PongArea::NET_WIDTH_PX = 4;
const int PongArea::BALL_WIDTH_PX = 10;

PongArea::PongArea (QWidget *parent)
  : QFrame (parent),
    gameTimer (nullptr),
    userPaddle (0, 0, PADDLE_WIDTH_PX, PADDLE_HEIGHT_PX),
    compPaddle (0, 0, PADDLE_WIDTH_PX, PADDLE_HEIGHT_PX),
    ballPos (0, 0),
    userScore (0),
    compScore (0),
    ballDirectionX (1),
    ballDirectionY (0)
{
  setCursor (Qt::BlankCursor);
  
  // update the ball and paddle positions at the given refresh rate
  gameTimer = new QTimer (this);
  connect (gameTimer, &QTimer::timeout, this, &PongArea::updatePositions);
}

void PongArea::resizeEvent (QResizeEvent *event)
{
  QPointF paddleCenter (rect ().right () - MARGIN_WIDTH_PX - (PADDLE_WIDTH_PX / 2),
                        rect ().center ().y ());
  userPaddle.moveCenter (paddleCenter);
  paddleCenter = QPointF (rect ().left () + MARGIN_WIDTH_PX + (PADDLE_WIDTH_PX / 2),
                          rect ().center ().y ());
  compPaddle.moveCenter (paddleCenter);

  ballPos = rect ().center ();
  QFrame::resizeEvent (event);
}

void PongArea::paintEvent (QPaintEvent *event)
{
  QPainter painter (this);
  painter.setRenderHint (QPainter::Antialiasing);
  QFont font = painter.font ();
  font.setPixelSize (rect ().height () / 10);
  painter.setFont (font);
  QFontMetrics metrics (painter.font ());

  QPen pen (Qt::SolidLine);
  pen.setColor (Qt::white);
  painter.setPen (pen);
  painter.setBrush (Qt::white);

  // draw user paddles
  painter.drawRect (userPaddle);
  painter.drawRect (compPaddle);

  // draw the scores
  QString score (QString ("%1").arg (QString::number (userScore), 2, '0'));
  QRectF textRect (painter.boundingRect (QRectF (), Qt::AlignCenter, score));
  QPointF scorePos (rect ().right () - (rect ().width () / 4) - (textRect.width () / 2),
                    rect ().top () + MARGIN_WIDTH_PX);
  textRect.moveTopLeft (scorePos);
  painter.drawText (textRect, Qt::AlignCenter, score);

  score = QString ("%1").arg (QString::number (compScore), 2, '0');
  textRect = (painter.boundingRect (QRectF (), Qt::AlignCenter, score));
  scorePos = QPointF (rect ().left () + (rect ().width () / 4) - (textRect.width () / 2),
                      rect ().top () + MARGIN_WIDTH_PX);
  textRect.moveTopLeft (scorePos);
  painter.drawText (textRect, Qt::AlignCenter, score);

  // draw the ball
  painter.drawRect (ballPos.x () - (BALL_WIDTH_PX / 2),
                    ballPos.y () - (BALL_WIDTH_PX / 2), BALL_WIDTH_PX, BALL_WIDTH_PX);

  // draw center line
  pen.setWidth (NET_WIDTH_PX);
  pen.setStyle (Qt::DashLine);
  painter.setPen (pen);
  painter.drawLine (rect ().center ().x (), rect ().top () + pen.width (),
                    rect ().center ().x (), rect ().bottom ());

  QFrame::paintEvent (event);
}

void PongArea::toggleGameActive ()
{
  gameTimer->isActive () ?
    gameTimer->stop () : gameTimer->start (1000 / REFRESH_RATE_HZ);
}

void PongArea::updatePositions ()
{
  // update paddle positions
  float xPx = userPaddle.center ().x ();
  float yPx = (mapFromGlobal (QCursor::pos ())).y ();
  userPaddle.moveCenter (QPointF (xPx, yPx));

  float yDeltaPx = (BALL_WIDTH_PX / 2);
  xPx = compPaddle.center ().x ();
  if ((compPaddle.center ().y () + yDeltaPx) < ballPos.y ()) {
    yPx = compPaddle.center ().y () + yDeltaPx;
    compPaddle.moveCenter (QPointF (xPx, yPx));
  } else if ((compPaddle.center ().y () - yDeltaPx) > ballPos.y ()) {
    yPx = compPaddle.center ().y () - yDeltaPx;
    compPaddle.moveCenter (QPointF (xPx, yPx));
  }

  // check if ball hit users/computers paddle, or top/bottom of widget
  if ((userPaddle.contains (ballPos)) || compPaddle.contains (ballPos)) {
    ballDirectionX *= -1;
    ballDirectionY = 2 * (((rand () % 2) == 0) ? -1 : 1);
  } else if (((ballPos.y () - MARGIN_WIDTH_PX) < rect ().top ()) ||
             ((ballPos.y () + MARGIN_WIDTH_PX) > rect ().bottom ())) {
    ballDirectionY *= -1;
  }

  // if ball is left/right of paddle add point and reset
  if (ballPos.x () < compPaddle.left ()) {
    ballPos = rect ().center ();
    ballDirectionY = 0;
    ++userScore;
  } else if (ballPos.x () > userPaddle.right ()) {
    ballPos = rect ().center ();
    ballDirectionY = 0;
    ++compScore;
  }

  // calculate number of pixels to travel per refresh and update ball position
  float deltaPxPerHz = (rect ().width () / 3) / REFRESH_RATE_HZ;
  ballPos.rx () += (deltaPxPerHz * ballDirectionX);
  ballPos.ry () += (deltaPxPerHz * ballDirectionY);

  // repaint the widget
  update ();
}

QPong::QPong (QWidget *parent)
  : QDialog (parent),
    pongArea (nullptr),
    pauseMenu (nullptr)
{
  setWindowTitle ("QPong");
  srand (QDateTime::currentMSecsSinceEpoch ());

  pongArea = new PongArea;
  pongArea->setVisible (false);

  // setup start menu
  startMenu = new QFrame;

  QLabel *startLbl = new QLabel ("QPong");
  QFont lblFont (font ());
  lblFont.setPixelSize (font ().pixelSize () * 4);
  startLbl->setFont (lblFont);

  QPushButton *startBtn = new QPushButton ("Start");
  connect (startBtn, &QPushButton::clicked, startMenu, &QFrame::hide);
  connect (startBtn, &QPushButton::clicked, pongArea, &QDialog::show);
  connect (startBtn, &QPushButton::clicked, pongArea, &PongArea::toggleGameActive);

  QGridLayout *startMenuLayout = new QGridLayout (startMenu);
  startMenuLayout->addWidget (startLbl, 0, 1, 1, 1, Qt::AlignCenter | Qt::AlignBottom);
  startMenuLayout->addWidget (startBtn, 1, 1, 1, 1, Qt::AlignCenter | Qt::AlignTop);

  // setup pause menu
  pauseMenu = new QFrame;
  pauseMenu->setVisible (false);

  QLabel *pauseLbl = new QLabel ("Game Paused");
  pauseLbl->setFont (lblFont);

  QPushButton *resumeBtn = new QPushButton ("Resume");
  connect (resumeBtn, &QPushButton::clicked, pauseMenu, &QFrame::hide);
  connect (resumeBtn, &QPushButton::clicked, pongArea, &PongArea::toggleGameActive);

  QGridLayout *pauseMenuLayout = new QGridLayout (pauseMenu);
  pauseMenuLayout->addWidget (pauseLbl, 0, 1, 1, 1, Qt::AlignCenter | Qt::AlignBottom);
  pauseMenuLayout->addWidget (resumeBtn, 1, 1, 1, 1, Qt::AlignCenter | Qt::AlignTop);

  // setup close button
  QPushButton *closeBtn = new QPushButton ("Close");
  connect (closeBtn, &QPushButton::clicked, this, &QDialog::accept);

  // add components to main layout
  QGridLayout *mainLayout = new QGridLayout (this);
  mainLayout->setVerticalSpacing ((mainLayout->contentsMargins ()).top ());
  mainLayout->addWidget (startMenu, 0, 0, 1, 3);
  mainLayout->addWidget (pongArea, 0, 0, 1, 3);
  mainLayout->addWidget (pauseMenu, 0, 0, 1, 3);
  mainLayout->addWidget (closeBtn, 1, 2, Qt::AlignRight);
  for (int col = 0; col < mainLayout->columnCount (); ++col) {
    mainLayout->setColumnStretch (col, 1);
  }
}

void QPong::keyPressEvent (QKeyEvent *event)
{
  if (event->key () == Qt::Key_Escape) {
    if ((startMenu->isVisible ()) || (pauseMenu->isVisible ())) {
      accept ();
    }
    pauseMenu->setVisible (true);
    pongArea->toggleGameActive ();
    return;
  }
  QDialog::keyPressEvent (event);
}
