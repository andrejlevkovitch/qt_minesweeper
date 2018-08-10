// game.hpp

#pragma once

#include "type.hpp"
#include <QObject>
#include <set>
#include <vector>

#include <QPoint>

class QMouseEvent;

namespace minesweeper {
class game : public ::QObject {
  Q_OBJECT
public:
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
  unsigned short mines_n_;
  bool first_pos_;

public:
  game(::QSize size, unsigned short mines_n, ::QObject *parent = nullptr);

private:
  void send_all_field() const;
  void create_field(::QPoint begin_pos);
  void compar_mines_protect() const;

public slots:
  void handle(::QPoint pos);
  void re_protect(::QPoint pos);

signals:
  void send_pos_value(::QPoint pos, Type value) const;
  void send_end_game() const;
  void send_lose_signal() const;
  void send_not_protect_capacity(int capacity) const;
};
}; // namespace minesweeper
