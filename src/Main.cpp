/**
 * @file Main.cpp
 * Main file for QPong
 */

#include <QApplication>

#include "QPong.h"

int main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  QFont font;
  font.setPixelSize (18);
  app.setFont (font);

  QFile qssFile ("stylesheet.qss");
  qssFile.open (QFile::ReadOnly);
  app.setStyleSheet (QString (qssFile.readAll ()));

  QPong pong;
  pong.resize (800, 600);
  pong.show ();

  return app.exec ();
}
