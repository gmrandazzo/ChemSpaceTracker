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
 #ifndef IMPORTSMIDIALOG_H
#define IMPORTSMIDIALOG_H

#include <QList>
#include <QDialog>
#include <QStandardItemModel>

#include "ui_importsmidialog.h"
#include "iodata.h"

class ImportSMIDialog : public QDialog
{
    Q_OBJECT

public:
    ImportSMIDialog(QDialog *parent = 0);
    ~ImportSMIDialog();
    SMILIST &getSmilesList() { return smilst; };
    QString getLabel();
private slots:
    void OpenSmilesFile();
    void Import();
    void UpdatePreview();

private:
  Ui::ImportSMIDialog *ui;
  QStandardItemModel *preview;
  QList<SMI> smilst;
  QString getSeparator();
  int getFileNCols();
};

#endif
