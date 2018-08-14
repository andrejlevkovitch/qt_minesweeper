// main.cpp

#include "screen.hpp"
#include <QApplication>

int main(int argc, char *argv[]) {
  ::QApplication app(argc, argv);

  minesweeper::screen screen;
  screen.show();

  return app.exec();
}
