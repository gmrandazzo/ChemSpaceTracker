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
#include "iodata.h"
#include <iostream>

#include <QBuffer>
#include <QByteArray>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QVariant>
#include <QStringList>
#include <QFile>
#include <QFileDevice>

void readSMI(QString fsmi, QString sep, int nameCol, int smiCol, SMILIST *smilst)
{
  QFile file(fsmi);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
     return;

  int i = 1;
  while (!file.atEnd()){
    QStringList a = QString(file.readLine().trimmed()).split(sep);
    if(nameCol == -1)
      (*smilst).append(SMI(QString("Molecule%1").arg(QString::number(i)), a[smiCol].trimmed()));
    else{
      if(a.size() >= 2){
        (*smilst).append(SMI(a[nameCol].trimmed(), a[smiCol].trimmed()));
      }
      else{
        std::cout << "Problem with string " << file.readLine().trimmed().toStdString() << std::endl;
      }
    }
  }
  file.close();
}

QStringList DeserializeQStringList(QString serialized_lst)
{
  if(serialized_lst.size() > 0)
    return serialized_lst.split(";");
  else
    return QStringList();
}

QString SerializeQStringList(QStringList lst)
{
  QString serialized_lst;
  for(int i = 0; i < lst.size()-1; i++)
    serialized_lst += lst[i] + ";";
  serialized_lst += lst.last() + ";";
  return serialized_lst;
}

void DeserializeMatrix(QString serialized_mx, matrix **mx)
{
  if(serialized_mx.size() > 0 && serialized_mx.compare("NULL") != 0){
    QList< QStringList > _mx_;
    QStringList row = serialized_mx.split("\n");
    for(int i = 0; i < row.size(); i++){
      _mx_.append(row[i].split(";"));
    }
    NewMatrix(mx, _mx_.size(), _mx_[0].size());
    for(int i = 0; i < _mx_.size(); i++)
      for(int j = 0; j < _mx_[i].size(); j++)
        (*mx)->data[i][j] = _mx_[i][j].toDouble();
  }
  else{
    initMatrix(mx);
  }
}

QString SerializeMatrix(matrix *mx)
{
  if(mx != NULL){
    if(mx->row > 0){
      QString serialized_mx;
      for(int i = 0; i < mx->row-1; i++){
        //start a row and concatenate values
        for(int j = 0; j < mx->col-1; j++){
          serialized_mx += QString::number(mx->data[i][j]) + ";";
        }
        serialized_mx += QString::number(mx->data[i][mx->col-1]);
        serialized_mx += "\n"; //end row
      }

      for(int j = 0; j < mx->col-1; j++){
        serialized_mx += QString::number(mx->data[mx->row-1][j]) + ";";
      }
      serialized_mx += QString::number(mx->data[mx->row-1][mx->col-1]);

      return serialized_mx;
    }
    else
      return "NULL";
  }
  else
    return "NULL";
}

void DeserializeDVector(QString serialized_v, dvector **v)
{
  if(serialized_v.size() > 0 && serialized_v.compare("NULL") != 0){
    QStringList _v_ = serialized_v.split(";");
    for(int i = 0; i < _v_.size(); i++)
      DVectorAppend(v, _v_[i].toDouble());
  }
  else{
    initDVector(v);
  }
}

QString SerializeDVector(dvector *v)
{
  QString serialized_v;
  for(int i = 0; i < v->size-1; i++){
    serialized_v += QString::number(v->data[i]) + ";";
  }
  serialized_v += QString::number(v->data[v->size-1]);
  return serialized_v;
}

void DeserializeSMIList(QString s_smilst, SMILIST *smilst)
{
  if(s_smilst.size() > 0 && s_smilst.compare("NULL") != 0){
    QStringList rows = s_smilst.split(";");
    for(int i = 0; i < rows.size(); i++){
      QStringList a = rows[i].split("\t");
      (*smilst).append(SMI(a[0], a[1]));
    }
  }
  else{
    return;
  }
}

QString SerializeSMIList(SMILIST smilst)
{
  if(smilst.size() > 0){
    QString serialized_smilst;
    for(int i = 0; i < smilst.size()-1; i++){
      serialized_smilst += QString("%1\t%2").arg(smilst[i].name).arg(smilst[i].smiles) + ";";
    }
    serialized_smilst += QString("%1\t%2").arg(smilst.last().name).arg(smilst.last().smiles);
    return serialized_smilst;
  }
  else{
    return "NULL";
  }
}

void DeserializeLabels(QString s_labels, LABELS *labels)
{
  if(s_labels.size() > 0 && s_labels.compare("NULL") != 0){
    QStringList rows = s_labels.split(";");
    for(int i = 0; i < rows.size(); i++){
      QStringList a = rows[i].split("\t");
      LABEL l;
      l.name = a[0];
      QStringList b = a[1].split("/");
      for(int j = 0; j < b.size(); j++){
        l.ids.append(b[j].toInt());
      }
      (*labels).append(l);
    }
  }
  else{
    return;
  }
}

QString SerializeLabels(LABELS labels)
{
  if(labels.size() > 0){
    QString serialized_labels;
    for(int i = 0; i < labels.size()-1; i++){
      QString s_ids;
      for(int j = 0; j < labels[i].ids.size()-1; j++){
        s_ids += QString::number(labels[i].ids[j]) + "/";
      }
      s_ids += QString::number(labels[i].ids.last());
      serialized_labels += QString("%1\t%2").arg(labels[i].name).arg(s_ids) + ";";
    }

    QString s_ids;
    for(int j = 0; j < labels.last().ids.size()-1; j++){
      s_ids += QString::number(labels.last().ids[j]) + "/";
    }
    s_ids += QString::number(labels.last().ids.last());
    serialized_labels += QString("%1\t%2").arg(labels.last().name).arg(s_ids);
    return serialized_labels;
  }
  else{
    return "NULL";
  }
}

void SaveGPSProject(QString filename, PCAMODEL *pcamodel,  QStringList varnames, matrix *pscores, SMILIST smilist, matrix *pdescriptors, IMAGES pimages, LABELS labels)
{
  //QString dbName( "myDatabase.db3" );
  QFile::remove(filename);
  QSqlDatabase db = QSqlDatabase::addDatabase( "QSQLITE" );
  db.setDatabaseName(filename);
  db.open();
  QSqlQuery query = QSqlQuery(db);
  query.exec(QString("CREATE TABLE IF NOT EXISTS gpsTable (scores TEXT,  loadings TEXT, colaverage TEXT, colscaling TEXT, varnames TEXT, pred_scores TEXT, pred_smilst TEXT, pred_descriptors TEXT, labels TEXT)"));

  QString s_scores = SerializeMatrix(pcamodel->scores);
  QString s_loadings = SerializeMatrix(pcamodel->loadings);
  QString s_colavg = SerializeDVector(pcamodel->colaverage);
  QString s_colscaling = SerializeDVector(pcamodel->colscaling);
  QString s_varnames = SerializeQStringList(varnames);

  QString s_predscores;
  if(pscores != NULL)
    s_predscores = SerializeMatrix(pscores);
  else
    s_predscores = "NULL";
  QString s_smilst;
  if(smilist.size() > 0)
    s_smilst = SerializeSMIList(smilist);
  else
    s_smilst = "NULL";

  QString s_preddescriptors;
  if(pdescriptors != NULL){
    s_preddescriptors = SerializeMatrix(pdescriptors);
  }
  else
    s_preddescriptors = "NULL";

  QString s_labels;
  if(labels.size() > 0)
    s_labels = SerializeLabels(labels);
  else
    s_labels = "NULL";

  query.prepare("INSERT INTO gpsTable (scores, loadings, colaverage, colscaling, varnames, pred_scores, pred_smilst, pred_descriptors, labels) VALUES (:scores, :loadings, :colaverage, :colscaling, :varnames, :pred_scores, :pred_smilst, :pred_descriptors, :labels)");

  query.bindValue(0, s_scores);
  query.bindValue(1, s_loadings);
  query.bindValue(2, s_colavg);
  query.bindValue(3, s_colscaling);
  query.bindValue(4, s_varnames);
  query.bindValue(5, s_predscores);
  query.bindValue(6, s_smilst);
  query.bindValue(7, s_preddescriptors);
  query.bindValue(8, s_labels);
  if(!query.exec()){
    std::cout << "ERROR: " << std::endl;
    //qDebug() << "error:  "  << query.lastError();
  }

  query.exec(QString("CREATE TABLE IF NOT EXISTS imgTable (name TEXT,  imagedata BLOB)"));
  if(pimages.size() > 0){
    for(int i = 0; i < pimages.size(); i++){
      QByteArray inByteArray;
      QBuffer inBuffer(&inByteArray);
      inBuffer.open(QIODevice::WriteOnly);
      pimages[i].img.save(&inBuffer, "PNG");

      query.prepare( "INSERT INTO imgTable (name, imagedata) VALUES (:name, :imagedata)" );
      query.bindValue( ":name", pimages[i].name);
      query.bindValue( ":imagedata", inByteArray);
      query.exec();
    }
  }

  //query.exec(QString("INSERT INTO gpsTable (scores, loadings, colaverage, colscaling, varnames, pred_scores, pred_smilst) VALUES ('%2', %3', '%4', '%5', '%6', '%7', '%8')").arg(s_scores).arg(s_loadings).arg(s_colavg).arg(s_colscaling).arg(s_predscores).arg(s_smilst));
  db.close();
}

void ReadGPSProject(QString filename, PCAMODEL **pcamodel,  QStringList *varnames, matrix **pscores, SMILIST *smilst, matrix **descriptors, IMAGES *pimages, LABELS *labels)
{
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(filename);
  db.open();
  QSqlQuery query = QSqlQuery(db);
  query.exec("SELECT * from gpsTable");
  query.first();
  /*int idscores  = query.record().indexOf("scores");
  QString s_scores = query.value(idscores).toString();
  int idloadings  = query.record().indexOf("loadings");
  QString s_loadings = query.value(idloadings).toString();
  int idcolaverage  = query.record().indexOf("colaverage");
  QString s_colavg = query.value(idcolaverage).toString();
  int idcolscaling  = query.record().indexOf("colscaling");
  QString s_colscaling = query.value(idcolscaling).toString();
  int idvarnames  = query.record().indexOf("varnames");
  QString s_varnames = query.value(idvarnames).toString();
  int idpredscores  = query.record().indexOf("pred_scores");
  QString s_predscores = query.value(idpredscores).toString();
  int idsmilst  = query.record().indexOf("pred_smilst");
  QString s_smilst = query.value(idsmilst).toString();
  int iddescriptors  = query.record().indexOf("pred_descriptors");
  QString s_preddescriptors = query.value(iddescriptors).toString();
  int idlabels  = query.record().indexOf("labels");
  QString s_labels = query.value(idlabels).toString();*/

  QString s_scores = query.value(0).toString();
  QString s_loadings = query.value(1).toString();
  QString s_colavg = query.value(2).toString();
  QString s_colscaling = query.value(3).toString();
  QString s_varnames = query.value(4).toString();
  QString s_predscores = query.value(5).toString();
  QString s_smilst = query.value(6).toString();
  QString s_preddescriptors = query.value(7).toString();
  QString s_labels = query.value(8).toString();


  NewPCAModel(pcamodel);
  DeserializeMatrix(s_scores, &((*pcamodel)->scores));
  DeserializeMatrix(s_loadings, &((*pcamodel)->loadings));
  DeserializeDVector(s_colavg, &((*pcamodel)->colaverage));
  DeserializeDVector(s_colscaling, &((*pcamodel)->colscaling));
  DeserializeDVector(s_colscaling, &((*pcamodel)->colscaling));
  (*varnames) = DeserializeQStringList(s_varnames);
  DeserializeMatrix(s_predscores, pscores);
  DeserializeSMIList(s_smilst, smilst);
  DeserializeMatrix(s_preddescriptors, descriptors);
  DeserializeLabels(s_labels, labels);

  query.exec("SELECT * from imgTable");
  while (query.next()){
    QString name = query.value(0).toString();
    QByteArray outByteArray = query.value(1).toByteArray();
    QPixmap img = QPixmap();
    img.loadFromData(outByteArray);
    (*pimages).append(IMAGE(name, img));
  }

  //PrintPCA((*pcamodel));
  db.close();
}
