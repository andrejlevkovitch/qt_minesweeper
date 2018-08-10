// screen.hpp

#pragma once

#include <QWidget>

namespace minesweeper {
class screen : public ::QWidget {
  Q_OBJECT
private:
  ::QSize field_size_;
  unsigned short mines_capacity_;
  bool first_click_;

public:
  screen(::QWidget *parent = nullptr);
};
}; // namespace minesweeper
