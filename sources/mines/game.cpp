// game.cpp

#include "game.hpp"
#include <QPoint>
#include <QSize>
#include <random>

minesweeper::game::game(::QObject *parent)
    : ::QObject{parent}, size_{9, 9}, mines_n_{10}, first_pos_{true} {
  new_game();
}

void minesweeper::game::create_field(::QPoint beg_pos) {
#ifdef __linux__
  static std::random_device rd;
  static std::default_random_engine engine{rd()};
#endif
  std::vector<::QPoint> field_line;
  field_line.reserve(field_.size() * field_[0].size());
  for (int i = 0; i < field_.size(); ++i) {
    for (int j = 0; j < field_[i].size(); ++j) {
      if (beg_pos != ::QPoint{j, i}) {
        field_line.push_back(::QPoint{j, i});
      }
    }
  }

  int last(field_line.size() - 1);
  for (int i = 0; i < mines_n_; ++i) {
    std::uniform_int_distribution<int> dist(0, last);
    std::swap(field_line[dist(engine)], field_line[last]);
    mines_.insert(field_line[last]);
    --last;
  }

  for (const auto &i : mines_) {
    int y = i.y();
    int x = i.x();
    field_[y][x] = Type::MINE;
    for (int l = -1; l < 2; ++l) {
      for (int m = -1; m < 2; ++m) {
        if (!((!l && !m) || y + l < 0 || x + m < 0 || y + l >= field_.size() ||
              x + m >= field_[0].size() || field_[y + l][x + m] == MINE)) {
          field_[y + l][x + m] =
              static_cast<Type>(static_cast<int>(field_[y + l][x + m] + 1));
        }
      }
    }
  }
}

void minesweeper::game::send_all_field() const {
  for (int i = 0; i < field_.size(); ++i) {
    for (int j = 0; j < field_[i].size(); ++j) {
      if (field_[i][j] != OPEN) {
        emit pos_value_signal(::QPoint{j, i}, field_[i][j]);
      }
    }
  }
}

void minesweeper::game::handle(::QPoint pos) {
  if (first_pos_) {
    first_pos_ = false;
    create_field(pos);
    emit not_protect_capacity_signal(mines_.size());
  }
  int y = pos.y();
  int x = pos.x();
  auto value = field_[y][x];
  if (value == OPEN) {
    return;
  }
  pos_value_signal(pos, value);
  field_[y][x] = OPEN;
  if (value == NONE) {
    for (int i = -1; i < 2; ++i) {
      for (int j = -1; j < 2; ++j) {
        if (!(y + i < 0 || x + j < 0 || y + i >= field_.size() ||
              x + j >= field_[0].size() || field_[y + i][x + j] == OPEN)) {
          handle(::QPoint{x + j, y + i});
        }
      }
    }
  } else if (value == MINE) {
    send_all_field();
    emit lose_signal();
  }
}

void minesweeper::game::re_protect(::QPoint pos) {
  if (protected_.find(pos) == protected_.end()) {
    protected_.insert(pos);
  } else {
    protected_.erase(pos);
  }
  if (protected_.size() == mines_.size()) {
    compar_mines_protect();
  }
  emit not_protect_capacity_signal(mines_.size() - protected_.size());
}

void minesweeper::game::new_game() {
  first_pos_ = true;
  field_ = decltype(field_)(
      size_.height(), decltype(field_)::value_type(size_.width(), Type::NONE));
  protected_.clear();
  mines_.clear();
}

void minesweeper::game::compar_mines_protect() const {
  if (protected_ == mines_) {
    send_all_field();
    emit end_game_signal();
  }
}

int minesweeper::game::height() const { return size_.height(); }

int minesweeper::game::width() const { return size_.width(); }

void minesweeper::game::set_complexity(::QSize size, unsigned short mines_n) {
  size_ = size;
  mines_n_ = mines_n;
}
