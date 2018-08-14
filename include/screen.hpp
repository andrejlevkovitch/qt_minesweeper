// screen.hpp

#pragma once

#include <QWidget>

namespace minesweeper {
class game;
};

namespace minesweeper {
class screen : public ::QWidget {
  Q_OBJECT
private:
  ::QSize field_size_;
  unsigned short mines_capacity_;
  bool first_click_;

public:
  screen(::QWidget *parent = nullptr);
  ::QWidget *create_game_field(minesweeper::game *game) const;
};
}; // namespace minesweeper
