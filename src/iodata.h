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
#ifndef IODATA_H
#define IODATA_H

#include <QList>
#include <QString>
#include <QStringList>
#include <QPixmap>
#include <scientific.h>

struct SMI{
  SMI(QString name_, QString smiles_) : name(name_), smiles(smiles_){}
  QString name;
  QString smiles;
};

#define SMILIST QList<SMI>

struct LABEL{
  QList<int> ids;
  QString name;
};

#define LABELS QList<LABEL>

struct IMAGE{
  IMAGE(QString name_, QPixmap img_) : name(name_), img(img_) {}
  QString name;
  QPixmap img;
};

#define IMAGES QList<IMAGE>

void readSMI(QString fsmi, QString sep, int nameCol, int smiCol, SMILIST *smilst);
/* Methods to serialize/deserialize data structures and store/retrieve into sqlite models*/
QStringList DeserializeQStringList(QString serialized_lst);
QString SerializeQStringList(QStringList lst);
void DeserializeMatrix(QString serialized_mx, matrix **mx);
QString SerializeMatrix(matrix *mx);
void DeserializeSMIList(QString s_smilst, SMILIST *smilst);
QString SerializeSMIList(SMILIST smilst);
void DeserializeLabels(QString s_smilst, SMILIST *smilst);
QString SerializeLabels(LABELS smilst);


void SaveGPSProject(QString filename, PCAMODEL *pcamodel,  QStringList varnames, matrix *pscores, SMILIST smilist, matrix *pdescriptors, IMAGES pimages, LABELS labels);
void ReadGPSProject(QString filename, PCAMODEL **pcamodel,  QStringList *varnames, matrix **pscores, SMILIST *smilst, matrix **descriptors, IMAGES *pimages, LABELS *labels);

#endif
