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
#include "importsmidialog.h"

#include <QList>
#include <QFile>
#include <QFileDialog>
#include <QStandardItemModel>

QString ImportSMIDialog::getLabel(){
  return ui->lineEdit_2->text();
}

QString ImportSMIDialog::getSeparator(){
  QString sep;
  if(ui->separatorBox->currentText() == "Blank")
    return QString(" ");
  else if(ui->separatorBox->currentText() == "TAB")
    return QString("\t");
  else
    return ui->separatorBox->currentText();
}

int ImportSMIDialog::getFileNCols()
{
  QFile file(ui->lineEdit->text());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     return 0;

  QString sep = getSeparator();

  int ncol = 0;
  while (!file.atEnd()){
    QStringList a = QString(file.readLine()).split(sep);
    if(a.size() > ncol)
      ncol = a.size();
    else
      continue;
  }
  return ncol;
}

/*
void ImportSMIDialog::readSMI()
{
  QFile file(ui->lineEdit->text());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     return;

  QString sep = getSeparator();

  int namecol;
  if(ui->ordinalno->isChecked()){
    namecol = -1;
  }
  else{
    namecol = ui->nameColumn->currentIndex();;
  }

  int smicol = ui->smileColumn->currentIndex();

  int i = 1;
  while (!file.atEnd()) {
    QStringList a = QString(file.readLine()).split(sep);
    if(namecol == -1)
      smilst.append(SMI(QString("Molecule%1").arg(QString::number(i)), a[smicol] ));
    else
      smilst.append(SMI(a[namecol], a[smicol]));
  }
  file.close();
}*/

void ImportSMIDialog::OpenSmilesFile()
{
  ui->lineEdit->setText(QFileDialog::getOpenFileName( this, tr("Open SMILES list"), QDir::currentPath(), tr("SMILES Files (*.smi *.smiles);;All files (*.*)"), 0, QFileDialog::DontUseNativeDialog));
  UpdatePreview();
}

void ImportSMIDialog::Import()
{
  int namecol;
  if(ui->ordinalno->isChecked()){
    namecol = -1;
  }
  else{
    namecol = ui->nameColumn->currentIndex();;
  }

  int smicol = ui->smileColumn->currentIndex();

  readSMI(ui->lineEdit->text(), getSeparator(), namecol, smicol, &smilst);
  accept();
}


void ImportSMIDialog::UpdatePreview()
{
  int ncols = getFileNCols();
  ui->smileColumn->clear();
  ui->nameColumn->clear();
  for(int i = 0; i < ncols; i++){
    ui->smileColumn->addItem(QString::number(i+1));
    ui->nameColumn->addItem(QString::number(i+1));
  }

  preview->clear();
  QFile file(ui->lineEdit->text());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     return;

  QString sep = getSeparator();

  int i = 1;
  while (!file.atEnd()) {
    if(i < 10){
      QStringList a = QString(file.readLine()).split(sep);
      QList<QStandardItem*> row;
      for(int j = 0; j < a.size(); j++)
        row.append(new QStandardItem(a[j]));
      preview->appendRow(row);
    }
    else{
      break;
    }
  }
  file.close();
}

ImportSMIDialog::ImportSMIDialog(QDialog *parent) :
  QDialog(parent),
  ui(new Ui::ImportSMIDialog)
{
  ui->setupUi(this);

  preview = new QStandardItemModel();
  ui->tableView->setModel(preview);

  connect(ui->openSmiButton, SIGNAL(clicked(bool)), SLOT(OpenSmilesFile()));
  connect(ui->separatorBox, SIGNAL(currentIndexChanged(int)), SLOT(UpdatePreview()));
  connect(ui->lineEdit, SIGNAL(textChanged(QString)), SLOT(UpdatePreview()));
  connect(ui->cancelButton, SIGNAL(clicked(bool)), SLOT(reject()));
  connect(ui->importButton, SIGNAL(clicked(bool)), SLOT(Import()));
}


ImportSMIDialog::~ImportSMIDialog()
{
  delete preview;
  delete ui;
}
