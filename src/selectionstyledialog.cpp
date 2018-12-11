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
#include "selectionstyledialog.h"
#include <QColorDialog>

void SelectionStyleDialog::SetMaxColor()
{
  color3 = QColorDialog::getColor(color3, this);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color3.red()).arg(color3.green()).arg(color3.blue()));
}

void SelectionStyleDialog::SetMinColor()
{
  color2 = QColorDialog::getColor(color2, this);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color2.red()).arg(color2.green()).arg(color2.blue()));
}

void SelectionStyleDialog::SetColor()
{
  color1 = QColorDialog::getColor(color1, this);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));
}

void SelectionStyleDialog::Cancel()
{
  reject();
}

void SelectionStyleDialog::OK()
{
  accept();
}

int SelectionStyleDialog::getVarIDColor()
{
  return ui.colorvarlist->currentIndex();
}

QColor& SelectionStyleDialog::getMaxColor()
{
  return color3;
}

QColor& SelectionStyleDialog::getMinColor()
{
  return color2;
}

QColor& SelectionStyleDialog::getColor()
{
  return color1;
}

SelectionStyleDialog::ChangeType SelectionStyleDialog::getColorationType()
{
  if(ui.solidcolor->isChecked())
    return ONECOLOR;
  else
    return GRADIENT;
}

SelectionStyleDialog::SelectionStyleDialog(QStringList &varnames): QDialog()
{
  ui.setupUi(this);

  ui.solidcolor->setChecked(true);
  color1 = QColor(Qt::black);
  ui.colorLabel->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color1.red()).arg(color1.green()).arg(color1.blue()));
  color2 = QColor(Qt::green);
  ui.actionSelectColorMin->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color2.red()).arg(color2.green()).arg(color2.blue()));
  color3 = QColor(Qt::blue);
  ui.actionSelectColorMax->setStyleSheet(QString("background-color: rgb(%1, %2, %3);").arg(color3.red()).arg(color3.green()).arg(color3.blue()));
  ui.colorvarlist->clear();

  for(int i = 0; i < varnames.size(); i++){
    ui.colorvarlist->addItem(varnames[i]);
  }

  connect(ui.actionSelectColor, SIGNAL(clicked(bool)), SLOT(SetColor()));
  connect(ui.actionSelectColorMin, SIGNAL(clicked(bool)), SLOT(SetMinColor()));
  connect(ui.actionSelectColorMax, SIGNAL(clicked(bool)), SLOT(SetMaxColor()));
  connect(ui.okButton, SIGNAL(clicked(bool)), SLOT(OK()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(Cancel()));
}
