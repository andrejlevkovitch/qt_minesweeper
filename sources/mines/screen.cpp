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
  auto game = new minesweeper::game{field_size_, mines_capacity_, this};

  auto field_layout = new ::QGridLayout;
  {
    field_layout->setSpacing(0);
    field_layout->setContentsMargins(0, 0, 0, 0);
    for (int i{}; i < field_size_.height(); ++i) {
      for (int j{}; j < field_size_.width(); ++j) {
        auto button = new minesweeper::button{::QPoint{j, i}};
        field_layout->addWidget(button, i, j);
        connect(button, SIGNAL(pushed(::QPoint)), game, SLOT(handle(::QPoint)));
        connect(game, SIGNAL(send_pos_value(::QPoint, Type)), button,
                SLOT(set_value(::QPoint, Type)));
        connect(button, SIGNAL(restore(::QPoint)), game,
                SLOT(re_protect(::QPoint)));
      }
    }
  }

  auto lcd_layout = new ::QHBoxLayout;
  {
    auto mine_capacity = new ::QLCDNumber;
    mine_capacity->setDigitCount(2);
    mine_capacity->setMinimumSize(64, 32);
    mine_capacity->setMaximumSize(128, 64);
    connect(game, SIGNAL(send_not_protect_capacity(int)), mine_capacity,
            SLOT(display(int)));

    lcd_layout->addStretch(1);
    lcd_layout->addWidget(mine_capacity);
    lcd_layout->addStretch(1);
  }

  auto stacked_layout = new ::QStackedLayout;
  {
    auto screen_layout = new ::QVBoxLayout;
    screen_layout->addLayout(lcd_layout);
    screen_layout->addLayout(field_layout);

    ::QFont font;
    font.setPixelSize(30);
    font.setBold(true);

    auto win_message = new ::QLabel{"YOU WIN!!!"};
    win_message->setFont(font);
    win_message->setAlignment(Qt::AlignCenter);
    win_message->setPalette(::QPalette{::QPalette::Text, Qt::blue});
    auto lose_message = new ::QLabel{"YOU LOSE"};
    lose_message->setFont(font);
    lose_message->setAlignment(Qt::AlignCenter);
    lose_message->setPalette(::QPalette{::QPalette::Text, Qt::red});

    auto screen = new ::QWidget;
    screen->setLayout(screen_layout);

    stacked_layout->setStackingMode(::QStackedLayout::StackAll);
    stacked_layout->addWidget(win_message);
    stacked_layout->addWidget(lose_message);
    stacked_layout->addWidget(screen);

    stacked_layout->setCurrentIndex(2);
    connect(game, &minesweeper::game::send_end_game, stacked_layout,
            [=]() { stacked_layout->setCurrentIndex(0); });
    connect(game, &minesweeper::game::send_lose_signal, stacked_layout,
            [=]() { stacked_layout->setCurrentIndex(1); });
  }

  auto general_layout = new ::QVBoxLayout;
  {
    auto menu_bar = new ::QMenuBar;
    {
      auto game_menu = new ::QMenu{"Game"};
      menu_bar->addMenu(game_menu);
      auto new_game = new ::QAction{"New", this};
      game_menu->addAction(new_game);
      connect(game_menu, SIGNAL(), this, SLOT());
    }
    general_layout->addWidget(menu_bar);
    general_layout->addLayout(stacked_layout);
  }

  setLayout(general_layout);
}
