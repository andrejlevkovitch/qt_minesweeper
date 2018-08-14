// main.cpp

#include "screen.hpp"
#include <QApplication>
#include <QFile>
#include <QLatin1String>

int main(int argc, char *argv[]) {
  ::QApplication app(argc, argv);

  ::QFile file(":/m_style");
  ::QString style;
  if (file.open(::QFile::ReadOnly)) {
    style = ::QLatin1String{file.readAll()};
    file.close();
    app.setStyleSheet(style);
  }

  minesweeper::screen screen;
  screen.setStyleSheet(style);
  screen.show();

  return app.exec();
}
