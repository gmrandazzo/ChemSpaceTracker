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
#include "AboutDialog.h"
#include <QString>
#include "scientific.h"
AboutDialog::AboutDialog(int major, int minor, int patch)
{
  ui.setupUi(this);

  QString scientificversion = QString::fromLatin1(GetScientificVersion());
  ui.label_version->setText(QString("Version: %1.%2.%3\nVersion libscientific: %4").arg(QString::number(major)).arg(QString::number(minor)).arg(QString::number(patch)).arg(scientificversion));

  connect(ui.closeButton, SIGNAL(clicked(bool)), SLOT(accept()));
}
