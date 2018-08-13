// game.hpp

#pragma once

#include "type.hpp"
#include <QObject>
#include <QPoint>
#include <set>
#include <vector>
#include <QSize>

class QMouseEvent;

namespace minesweeper {
class game : public ::QObject {
  Q_OBJECT
private:
  struct less {
    bool operator()(::QPoint lhs, ::QPoint rhs) const {
      if (lhs.y() < rhs.y()) {
        return true;
      } else if (lhs.y() == rhs.y()) {
        if (lhs.x() < rhs.x()) {
          return true;
        }
      }
      return false;
    };
  };

private:
  std::vector<std::vector<Type>> field_;
  std::set<::QPoint, less> mines_;
  std::set<::QPoint, less> protected_;
  ::QSize size_;
  unsigned short mines_n_;
  bool first_pos_;

public:
  game(::QObject *parent = nullptr);

private:
  void send_all_field() const;
  void create_field(::QPoint begin_pos);
  void compar_mines_protect() const;

public:
  int height() const;
  int width() const;

public slots:
  void handle(::QPoint pos);
  void re_protect(::QPoint pos);
  void new_game();
  void set_complexity(::QSize size, unsigned short mines_n);

signals:
  void pos_value_signal(::QPoint pos, Type value) const;
  void end_game_signal() const;
  void lose_signal() const;
  void not_protect_capacity_signal(int capacity) const;
};
}; // namespace minesweeper
