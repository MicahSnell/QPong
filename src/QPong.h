/**
 * @file QPong.h
 * definition file for QPong
 */

#ifndef QPONG_H__
#define QPONG_H__

#include <QFrame>
#include <QMouseEvent>
#include <QDialog>
#include <QTimer>

/**
 * @class PongArea performs painting and calculations to provide Pong like game
 */
class PongArea : public QFrame {
  Q_OBJECT
public:
  /**
   * @brief initilizes the area that the game is played in
   * @param parent the parent widget of this object, nullptr if not given
   */
  PongArea (QWidget *parent = nullptr);

  /**
   * @brief sets the position of paddles and ball when resized
   */
  void resizeEvent (QResizeEvent *event) override;

  /**
   * @brief handles drawing of the PongArea
   */
  void paintEvent (QPaintEvent *event) override;

public slots:
  /**
   * @brief toggles the game between active and inactive
   */
  void toggleGameActive ();

private slots:
  /**
   * @brief updates the position of the ball, user and computer paddle
   */
  void updatePositions ();

private:
  /** margin around the frame in pixels */
  static const int MARGIN_WIDTH_PX;
  /** refresh rate for ball drawing */
  static const int REFRESH_RATE_HZ;
  /** width of paddles */
  static const int PADDLE_WIDTH_PX;
  /** height of paddles */
  static const int PADDLE_HEIGHT_PX;
  /** width of net */
  static const int NET_WIDTH_PX;
  /** width of ball */
  static const int BALL_WIDTH_PX;

  /** drives frame updates at REFRESH_RATE_HZ */
  QTimer *gameTimer;
  /** current position of users paddle */
  QRectF userPaddle;
  /** current position of computers paddle */
  QRectF compPaddle;
  /** current position of the ball */
  QPointF ballPos;
  /** users score */
  unsigned userScore;
  /** computers score */
  unsigned compScore;
  /** x direction of the ball */
  int ballDirectionX;
  /** y direction of the ball */
  int ballDirectionY;
};

/**
 * @class QPong base dialog widget that contains the PongArea and control buttons
 */
class QPong : public QDialog {
  Q_OBJECT
public:
  /**
   * @brief initializes the pong area and control buttons
   * @param parent the parent widget of this object, nullptr if not given
   */
  QPong (QWidget *parent = nullptr);

  /**
   * @brief shows pause menu/exits when ESC key is single/double pressed
   */
  void keyPressEvent (QKeyEvent *event) override;

private:
  /** custom widget that implements pong */
  PongArea *pongArea;
  /** overlay that shows when first launched */
  QFrame *startMenu;
  /** overlay that shows when the game is paused */
  QFrame *pauseMenu;
};

#endif
