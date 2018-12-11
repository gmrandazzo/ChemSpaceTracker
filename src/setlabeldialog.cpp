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
#include "setlabeldialog.h"

QString SetLabelDialog::getLabel()
{
  return ui.lineEdit->text();
}

SetLabelDialog::SetLabelDialog(): QDialog()
{
  ui.setupUi(this);
  connect(ui.addButton, SIGNAL(clicked(bool)), SLOT(accept()));
  connect(ui.cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
}
