// button.hpp

#pragma once

#include "type.hpp"
#include <QPushButton>

namespace minesweeper {
class button : public ::QPushButton {
  Q_OBJECT
public:
  enum Status { ENABLED, DISABLED, PROTECTED, QUESTION };

public:
  static ::QSize MAX_SIZE();

private:
  ::QPoint pos_;
  Status status_;

public:
  button(::QPoint pos, ::QWidget *parent = nullptr);
  ::QSize sizeHint() const override;
  void resizeEvent(::QResizeEvent *event) override;
  void mouseReleaseEvent(::QMouseEvent *event) override;

public slots:
  void set_value(::QPoint pos, Type type);

signals:
  void pushed(::QPoint pos);
  void restore(::QPoint pos);
};
}; // namespace minesweeper
