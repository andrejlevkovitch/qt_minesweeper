// screen.cpp

#include "screen.hpp"
#include "button.hpp"
#include "game.hpp"
#include "type.hpp"
#include <QBoxLayout>
#include <QLCDNumber>
#include <QLabel>
#include <QMenuBar>
#include <QMouseEvent>
#include <QStackedLayout>

minesweeper::screen::screen(::QWidget *parent)
    : ::QWidget{parent}, field_size_{9, 9}, mines_capacity_{10}, first_click_{
                                                                     true} {
  this->setFixedSize(0, 0);
  auto game = new minesweeper::game{this};

  auto stacked_layout = new ::QStackedLayout;
  {
    ::QFont font;
    font.setPixelSize(30);
    font.setBold(true);

    auto win_message = new ::QLabel{"YOU WIN!!!"};
    win_message->setFont(font);
    win_message->setAlignment(Qt::AlignCenter);
    win_message->setStyleSheet("color : blue");
    auto lose_message = new ::QLabel{"YOU LOSE"};
    lose_message->setFont(font);
    lose_message->setAlignment(Qt::AlignCenter);
    lose_message->setStyleSheet("color : red");

    stacked_layout->setStackingMode(::QStackedLayout::StackAll);
    stacked_layout->addWidget(win_message);
    stacked_layout->addWidget(lose_message);
    stacked_layout->addWidget(create_game_field(game));

    stacked_layout->setCurrentIndex(2);
    connect(game, &minesweeper::game::end_game_signal, stacked_layout,
            [=]() { stacked_layout->setCurrentIndex(0); });
    connect(game, &minesweeper::game::lose_signal, stacked_layout,
            [=]() { stacked_layout->setCurrentIndex(1); });
  }

  auto general_layout = new ::QVBoxLayout;
  {
    auto menu_bar = new ::QMenuBar;
    {
      auto game_menu = new ::QMenu{"Game"};
      {
        auto new_game = new ::QAction{"New", this};
        auto mini = new ::QAction{"Mini", this};
        auto midi = new ::QAction{"Midi", this};
        auto hard = new ::QAction{"Hard", this};
        game_menu->addAction(new_game);
        game_menu->addAction(mini);
        game_menu->addAction(midi);
        game_menu->addAction(hard);
        connect(new_game, SIGNAL(triggered()), game, SLOT(new_game()));
        connect(new_game, &::QAction::triggered, stacked_layout, [=]() {
          stacked_layout->setCurrentIndex(2);
          stacked_layout->removeWidget(stacked_layout->currentWidget());
          stacked_layout->addWidget(create_game_field(game));
          stacked_layout->setCurrentIndex(2);
        });
        connect(mini, &::QAction::triggered, game, [=]() {
          game->set_complexity(::QSize{9, 9}, 10);
          new_game->triggered();
        });
        connect(midi, &::QAction::triggered, game, [=]() {
          game->set_complexity(::QSize{16, 16}, 40);
          new_game->triggered();
        });
        connect(hard, &::QAction::triggered, game, [=]() {
          game->set_complexity(::QSize{30, 16}, 99);
          new_game->triggered();
        });
      }
      menu_bar->addMenu(game_menu);
    }

    auto lcd_layout = new ::QHBoxLayout;
    {
      auto mine_capacity = new ::QLCDNumber;
      mine_capacity->setDigitCount(2);
      mine_capacity->setMinimumSize(64, 32);
      connect(game, SIGNAL(not_protect_capacity_signal(int)), mine_capacity,
              SLOT(display(int)));

      lcd_layout->addStretch(1);
      lcd_layout->addWidget(mine_capacity);
      lcd_layout->addStretch(1);
    }

    general_layout->addWidget(menu_bar);
    general_layout->addLayout(lcd_layout);
    general_layout->addLayout(stacked_layout);
  }

  setLayout(general_layout);
}

::QWidget *
minesweeper::screen::create_game_field(minesweeper::game *game) const {
  auto field = new ::QWidget;
  auto field_layout = new ::QGridLayout;
  {
    field_layout->setSpacing(0);
    field_layout->setContentsMargins(0, 0, 0, 0);
    for (int i{}; i < game->height(); ++i) {
      for (int j{}; j < game->width(); ++j) {
        auto button = new minesweeper::button{::QPoint{j, i}};
        field_layout->addWidget(button, i, j);
        connect(button, SIGNAL(pushed(::QPoint)), game, SLOT(handle(::QPoint)));
        connect(button, SIGNAL(restore(::QPoint)), game,
                SLOT(re_protect(::QPoint)));
        connect(game, SIGNAL(pos_value_signal(::QPoint, Type)), button,
                SLOT(set_value(::QPoint, Type)));
      }
    }
  }
  field->setLayout(field_layout);
  field->setStyleSheet("background-color : rgba(250, 250, 250, 250)");
  return field;
}
