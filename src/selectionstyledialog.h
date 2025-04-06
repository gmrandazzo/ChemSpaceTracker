/*
 * ChemSpaceTracker: The Chemical Space Navigator!
 * Copyright (C) 2018 Giuseppe Marco Randazzo
 * Contact: https://www.gmrandazzo.net
 * Email: gmrandazzo@gmail.com
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#ifndef SELECTIONSTYLEDIALOG_H
#define SELECTIONSTYLEDIALOG_H

#include <scientific.h>
#include <QDialog>
#include <QColor>
#include "ui_selectionstyledialog.h"

class SelectionStyleDialog: public QDialog
{
  Q_OBJECT

public:
  enum ChangeType { ONECOLOR, GRADIENT };

  SelectionStyleDialog(QStringList &varname);

  ChangeType getColorationType();
  QColor& getColor();
  QColor& getMinColor();
  QColor& getMaxColor();
  int getVarIDColor();

public slots:
  void Cancel();
  void OK();
  void SetColor();
  void SetMinColor();
  void SetMaxColor();

private:
  Ui::SelectionStyleDialog ui;
  QColor color1, color2, color3;
};

#endif
