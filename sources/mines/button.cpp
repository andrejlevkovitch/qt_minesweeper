// button.cpp

#include "button.hpp"
#include <QResizeEvent>

::QSize minesweeper::button::MAX_SIZE() {
  static const ::QSize retval{64, 64};
  return retval;
}

minesweeper::button::button(::QPoint pos, ::QWidget *parent)
    : ::QPushButton{parent}, pos_{pos}, protected_{false}, disabled_{false} {
  setSizePolicy(::QSizePolicy::Policy::Minimum, ::QSizePolicy::Policy::Minimum);
  setCheckable(true);

  connect(this, &::QPushButton::released, this, [=]() {
    if (!protected_ && !disabled_) {
      emit pushed(pos_);
    } else {
      setChecked(false);
    }
  });
}

::QSize minesweeper::button::sizeHint() const { return ::QSize{32, 32}; }

void minesweeper::button::set_value(::QPoint pos, Type type) {
  if (pos == pos_) {
    switch (type) {
    case NONE:
      setDisabled(true);
      break;
    case ONE:
      pixmap_ = ::QPixmap{":/1"};
      break;
    case TWO:
      pixmap_ = ::QPixmap{":/2"};
      break;
    case THREE:
      pixmap_ = ::QPixmap{":/3"};
      break;
    case FOUR:
      pixmap_ = ::QPixmap{":/4"};
      break;
    case FIVE:
      pixmap_ = ::QPixmap{":/5"};
      break;
    case SIX:
      pixmap_ = ::QPixmap{":/6"};
      break;
    case SEVEN:
      pixmap_ = ::QPixmap{":/7"};
      break;
    case EIGHT:
      pixmap_ = ::QPixmap{":/8"};
      break;
    case MINE:
      pixmap_ = ::QPixmap{":/mine"};
      break;
    default:
      break;
    };

    disabled_ = true;
    setIconSize(size());
    setIcon(pixmap_);
    if (!isChecked()) {
      setChecked(true);
    }
  }
}

void minesweeper::button::resizeEvent(::QResizeEvent *event) {
  setIconSize(event->size());
  ::QPushButton::resizeEvent(event);
}

void minesweeper::button::mouseReleaseEvent(::QMouseEvent *event) {
  if (event->button() == Qt::MouseButton::RightButton && !disabled_) {
    if (protected_) {
      protected_ = false;
      setIcon(::QPixmap{});
      emit restore(pos_);
    } else {
      protected_ = true;
      setIconSize(sizeHint());
      setIcon(::QPixmap{":/flag"});
      emit restore(pos_);
    }
  }
  ::QPushButton::mouseReleaseEvent(event);
}
