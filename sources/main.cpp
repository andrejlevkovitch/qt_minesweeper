// main.cpp

#include <QApplication>
#include "screen.hpp"

int main(int argc, char *argv[]) {
  ::QApplication app(argc, argv);

  minesweeper::screen screen;
  screen.show();

  return app.exec();
}
