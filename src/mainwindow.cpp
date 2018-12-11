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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "AboutDialog.h"
#include "progressdialog.h"
#include "setlabeldialog.h"
#include "removelabeldialog.h"
#include "selectbylabeldialog.h"
#include "selectionstyledialog.h"
#include "importsmidialog.h"
#include "calculations.h"
#include "iodata.h"

#include <iostream>
#include <vector>

#include <scientific.h>
#include <QApplication>
#include <QFileInfo>
#include <QFileDialog>
#include <QList>
#include <QPainter>
#include <QPixmap>
#include <QSvgRenderer>
#include <QTemporaryDir>
#include <QTextStream>

void MainWindow::About()
{
  AboutDialog ad(1, 0, 0);
  ad.exec();
}

void MainWindow::NewChemSpace()
{
  if(varnames.size() > 0){
    DelPCAModel(&m);
    DelMatrix(&pscores);
    DelMatrix(&pdescriptors);
    smilist.clear();
    varnames.clear();
  }
  // ReadGPSProject automatically initialize all the structures!
  //printf("GPS PATH %s\n", QString(QFileInfo(QCoreApplication::applicationFilePath()).absolutePath()+"/gps.cst").toStdString().c_str());
  ReadGPSProject(QFileInfo(QCoreApplication::applicationFilePath()).absolutePath()+"/gps.cst", &m,  &varnames, &pscores, &smilist, &pdescriptors, &pimages, &labels);
  resetPlot();
}

void MainWindow::OpenChemSpace()
{
  QString fname = QFileDialog::getOpenFileName( this, tr("Open ChemSpaceTracker GPS files "), QDir::currentPath(), tr("ChemSpaceTracker GPS Files (*.cst)"), 0, QFileDialog::DontUseNativeDialog);
  QFileInfo check_file(fname);
  if(check_file.exists() && check_file.isFile()){
    if(varnames.size() > 0){
      DelPCAModel(&m);
      DelMatrix(&pscores);
      smilist.clear();
      varnames.clear();
      DelMatrix(&pdescriptors);
    }
    // ReadGPSProject automatically initialize all the structures!
    ReadGPSProject(QFileInfo(fname).absoluteFilePath(), &m,  &varnames, &pscores, &smilist, &pdescriptors, &pimages, &labels);
    resetPlot();
  }
  /*else{
    QMessageBox::warning(this, tr("ChemSpaceTracker"),
                               tr("The ChemSpaceTracker GPS File was not found!\n"
                                  "PleaseSelect"),
                               QMessageBox::Ok);
  }*/
}

void MainWindow::SaveChemSpaceAs()
{
  QString fname = QFileDialog::getSaveFileName( this, tr("Save ChemSpaceTracker GPS files As"), QDir::currentPath(), tr("ChemSpaceTracker GPS Files (*.cst)"), 0, QFileDialog::DontUseNativeDialog);
  if(fname.contains(".cst", Qt::CaseInsensitive) == false)
    fname += ".cst";
  SaveGPSProject(fname, m,  varnames, pscores, smilist, pdescriptors, pimages, labels);
}

QPixmap SVG2QPixmap(const QSize &imgsize, const QString &svgfile)
{
  QSvgRenderer svgr(svgfile);
  QPixmap img(imgsize);
  QPainter p;
  img.fill(Qt::transparent);
  p.begin(&img);
  svgr.render(&p);
  p.end();
  return img;
}

void MainWindow::ImportMolecules()
{
  ImportSMIDialog importsmi;
  if(importsmi.exec() ==  QDialog::Accepted){
    SMILIST smilst_ = importsmi.getSmilesList();

    ProgressDialog progress;
    progress.setMin(0);
    progress.setMax(smilst_.size());
    progress.show();

    matrix *mx;
    initMatrix(&mx);
    int prev_smilist_size = smilist.size();
    QTemporaryDir tdir;
    for(int i = 0; i < smilst_.size(); i++){
      if(progress.wasCanceled()){
        break;
      }
      else{
        //progress.setValue(i);
        progress.updateProgressBar(QString("Processing: %1").arg(smilst_[i].name), i);
        std::vector<double> desc;
        std::vector<std::string> varnames_; // UNUSED VARIABLE!!!
        calcDescriptors(smilst_[i].smiles.toStdString().c_str(), &desc, &varnames_);
        QString imgname = tdir.path()+"/"+smilst_[i].name.replace("/","_").replace("\\", "_")+".svg";
        gen2DDepiction(smilst_[i].smiles.toStdString().c_str(), 300, 300, imgname.toStdString().c_str());
        pimages.append(IMAGE(smilst_[i].name, SVG2QPixmap(QSize(300, 300), imgname)));
        if(desc.size() > 0){
          dvector *v;
          NewDVector(&v, desc.size());
          for(int j = 0; j < desc.size(); j++)
            v->data[j] = desc[j];
          MatrixAppendRow(&mx, v);
          MatrixAppendRow(&pdescriptors, v);
          DelDVector(&v);
          //EXTEND DATA STRUCTURE
          smilist.append(smilst_[i]);
        }
        else{
          continue;
        }
      }
    }
    tdir.remove();
    if(mx->row > 0){
      matrix *pscores_;
      initMatrix(&pscores_);
      PCAScorePredictor(mx, m, 8, &pscores_);
      // EXTEND DATA STRUCTURE
      int x = ui->spinBox->value()-1;
      int y = ui->spinBox_2->value()-1;
      for(int i = 0; i < pscores_->row; i++){
        dvector *v = getMatrixRow(pscores_, i);
        chart->addPoint(pscores_->data[i][x], pscores_->data[i][y], smilist[prev_smilist_size+i].name, Qt::red, ui->moleculePointSize->value());
        MatrixAppendRow(&pscores, v);
        DelDVector(&v);
      }
      chart->Refresh();

      if(importsmi.getLabel().isEmpty() == false){
        LABEL l;
        l.name = importsmi.getLabel();
        QList<int> lids;
        for(int i = 0; i < pscores_->row; i++){
          lids.append(i+m->scores->row+prev_smilist_size);
        }
        l.ids = lids;
        labels.append(l);
      }

      DelMatrix(&pscores_);
    }
    DelMatrix(&mx);
  }
  else{
    return;
  }
}

void MainWindow::SimilaritySearch()
{

}

void MainWindow::SelectMolecules()
{
  /*MLocalizer*/
}

void MainWindow::SearchCompound()
{
  QString str = ui->lineEdit->text();
  if(str.size() > 0){
    for(int i = m->scores->row; i < chart->PointSize(); i++){
      if(chart->getPoint(i)->getName().contains(str) == true){
        chart->getPoint(i)->setSelection(true);
        selectedPoints.append(i);
      }
      else{
        continue;
      }
    }
  }
  else{
    return;
  }
  chart->Refresh();
  UpdateImageView();
}

/* PRIVATE METHODS */
void MainWindow::ShowContextMenu(const QPoint& pos)
{
  //getPointSelected();
  QPoint globalPos = chart->weview()->mapToGlobal(pos);
  QMenu menu;
  menu.addAction("&Select All", this, SLOT(SelectAll()));
  menu.addAction("&Hide All Selection", this, SLOT(hideSelection()));
  menu.addAction("&Hide GPS Selection", this, SLOT(hideGPSSelection()));
  menu.addAction("&Invert Selection", this, SLOT(invertSelection()));
  menu.addAction("&Clear All Selection", this, SLOT(clearSelection()));
  menu.addAction("&Clear GPS Selection", this, SLOT(clearGPSSelection()));
  menu.addSeparator();
  menu.addAction("&Set Selection Label", this, SLOT(setSelectionLabel()));
  menu.addAction("&Select by Label", this, SLOT(SelectByLabel()));
  menu.addAction("&Save Label As..", this, SLOT(saveSelectionLabel()));
  menu.addAction("&Remove Selection Label", this, SLOT(removeSelectionLabel()));
  menu.addSeparator();
  menu.addAction("&Set Selection Style", this, SLOT(setSelectionStyle()));
  menu.addAction("&Show Molecular Depiction", this, SLOT(UpdateImageView()));
  menu.addSeparator();
  menu.addAction("&Reset Plot", this, SLOT(resetPlot()));
  menu.exec(globalPos);
}

void MainWindow::setAxis()
{
  unsigned int x, y;
  x = (unsigned int) ui->spinBox->value()-1;
  y = (unsigned int) ui->spinBox_2->value()-1;

  chart->setXaxisName(QString("PC %1").arg(x+1));
  chart->setYaxisName(QString("PC %1").arg(y+1));


  for(int i = 0; i < m->scores->row; i++){
    chart->getPoint(i)->setX(m->scores->data[i][x]);
    chart->getPoint(i)->setY(m->scores->data[i][y]);
  }

  int ngps = m->scores->row;
  for(int i = 0; i < pscores->row; i++){
    chart->getPoint(ngps+i)->setX(pscores->data[i][x]);
    chart->getPoint(ngps+i)->setY(pscores->data[i][y]);
  }

  chart->Refresh();
}


void MainWindow::setGPSPointSize()
{
  int sz = ui->gpsPointSize->value();
  for(int i = 0; i < m->scores->row; i++){
    chart->getPoint(i)->setRadius(sz);
  }
  chart->Refresh();
}

void MainWindow::setMoleculePointSize()
{
  int sz = ui->moleculePointSize->value();
  int ngps = m->scores->row;
  for(int i = 0; i < pscores->row; i++){
    chart->getPoint(ngps+i)->setRadius(sz);
  }
  chart->Refresh();
}

void MainWindow::ShowHidePoints()
{
  if(ui->viewGPS->isChecked()){
    for(int i = 0; i < m->scores->row; i++){
      chart->getPoint(i)->setVisible(true);
    }
  }
  else{
    for(int i = 0; i < m->scores->row; i++){
      chart->getPoint(i)->setVisible(false);
    }
  }

  if(ui->viewMolecules->isChecked()){
    int ngps = m->scores->row;
    for(int i = 0; i < pscores->row; i++){
      chart->getPoint(i+ngps)->setVisible(true);
    }
  }
  else{
    int ngps = m->scores->row;
    for(int i = 0; i < pscores->row; i++){
      chart->getPoint(i+ngps)->setVisible(false);
    }
  }
  chart->Refresh();
}

void MainWindow::UpdateSelectedPoints()
{
  selectedPoints.clear();
  int ngps = m->scores->row;
  for(int i = 0; i < chart->PointSize(); i++){
    if(i >= ngps){ // TO AVOID THE GPS SELECTION
      if(chart->getPoint(i)->isSelected() == true){
        selectedPoints.append(i);
      }
      else{
        continue;
      }
    }
    else{
      continue;
    }
  }
}

void MainWindow::SelectAll()
{
  chart->SelectAll();
  chart->Refresh();
  UpdateSelectedPoints();
}

void MainWindow::SelectByLabel()
{
  QStringList labels_;
  for(int i = 0; i < labels.size(); i++)
    labels_.append(labels[i].name);
  if(labels_.size() > 0){
    SelectByLabelDialog selbylabel(labels_);
    if(selbylabel.exec() == QDialog::Accepted){
      int indx = selbylabel.getLabelID();
      for(int i = 0; i < labels[indx].ids.size(); i++){
        chart->getPoint(labels[indx].ids[i])->setSelection(true);
      }
      chart->Refresh();
    }
    else{
      return;
    }
  }
  else{
    //label not found
    return;
  }
}

void MainWindow::invertSelection()
{
  QList<int> selectedPoints_;
  for(int i = 0; i < chart->PointSize(); i++){
    if(chart->getPoint(i)->isSelected() == true){
      chart->getPoint(i)->setSelection(false);
    }
    else{
      chart->getPoint(i)->setSelection(true);
      selectedPoints_.append(i);
    }
  }
  selectedPoints = selectedPoints_;
  chart->Refresh();
}

void MainWindow::hideSelection()
{
  for(int i = 0; i < selectedPoints.size(); i++){
    chart->getPoint(selectedPoints[i])->setVisible(false);
    chart->getPoint(i)->setSelection(false);
  }
  chart->Refresh();
  UpdateSelectedPoints();
}

void MainWindow::hideGPSSelection()
{
  for(int i = 0; i < m->scores->row; i++){
    chart->getPoint(i)->setVisible(false);
    chart->getPoint(i)->setSelection(false);
  }
  chart->Refresh();
  UpdateSelectedPoints();
}


void MainWindow::clearSelection()
{
  selectedPoints.clear();
  chart->ClearSelection();
  chart->Refresh();
  UpdateImageView();
}

void MainWindow::clearGPSSelection()
{
  for(int i = 0; i < m->scores->row; i++){
    chart->getPoint(i)->setSelection(false);
  }
  chart->Refresh();
  UpdateSelectedPoints();
}

QColor makeGradientColor(double val, double min, double max, QColor &color1, QColor &color2)
{
/*
colorA = [0, 0, 255] # blue
colorB = [255, 0, 0] # red
function get_gradient_color(val):
# 'val' must be between 0 and 1
for i in [1,2,3]:
color[i] = colorA[i] + val * (colorB[i] - colorA[i])
return color1
*/
  double normval = (val - min) / (max -min);
  QColor ncolor;
  ncolor.setRed(color1.red() + (normval * (color2.red() - color1.red())));
  ncolor.setGreen(color1.green() + (normval * (color2.green() - color1.green())));
  ncolor.setBlue(color1.blue() + (normval * (color2.blue() - color1.blue())));
  return ncolor;
}

void MainWindow::setSelectionStyle()
{
  UpdateSelectedPoints();
  if(selectedPoints.size() > 0){
    //Selection of variables... only few... easy... and interpretables!!
    QStringList varselections;
    QList<int>  varselections_indx;
    varselections << "Polar surface area" << "Accessible surface area";
    varselections_indx << 3 << 2;
    varselections << "Log P" << "Molecular Refractivity" << "MW";
    varselections_indx << 0 << 1 << 57;
    varselections << "No. Hydrogen-Bond Donors" << "No. Hydrogen-Bond Acceptors";
    varselections_indx << 41 << 42;
    varselections << "No. Aromatic Rings" << "No. Aliphatic Rings";
    varselections_indx << 47 << 48;
    varselections << "No. Rotable bonds" << "No. Heteroatoms";
    varselections_indx << 40 << 43;

    SelectionStyleDialog sstyle(varselections);
    if(sstyle.exec() == QDialog::Accepted){
      if(sstyle.getColorationType() == SelectionStyleDialog::ONECOLOR){
        QColor color = sstyle.getColor();
        for(int i = 0; i < selectedPoints.size(); i++){
          chart->getPoint(selectedPoints[i])->setColor(color);
        }
      }
      else{ //GRADIENT
        int col = varselections_indx[sstyle.getVarIDColor()];
        int ngps = m->scores->row;
        double min, max;
        min = max = pdescriptors->data[selectedPoints[0]-ngps][col];
        for(int i = 1; i < selectedPoints.size(); i++){
          int row = selectedPoints[i]-ngps;
          if(pdescriptors->data[row][col] < min){
            min = pdescriptors->data[row][col];
          }

          if(pdescriptors->data[row][col] > max){
            max = pdescriptors->data[row][col];
          }
        }

        for(int i = 0; i < selectedPoints.size(); i++){
          int row = selectedPoints[i] - ngps;
          chart->getPoint(selectedPoints[i])->setColor(makeGradientColor(pdescriptors->data[row][col], min, max, sstyle.getMinColor(), sstyle.getMaxColor()));
        }
      }
    }
    chart->Refresh();
  }
}

void MainWindow::setSelectionLabel()
{
  SetLabelDialog setlabel;
  if(setlabel.exec() == QDialog::Accepted){
    UpdateSelectedPoints();
    LABEL l;
    l.name = setlabel.getLabel();
    l.ids = selectedPoints;
    labels.append(l);
  }
}

void MainWindow::removeSelectionLabel()
{
  QStringList labels_;
  for(int i = 0; i < labels.size(); i++)
    labels_.append(labels[i].name);
  if(labels_.size() > 0){
    RemoveLabelDialog rmlabel(labels_);
    if(rmlabel.exec() == QDialog::Accepted){
      labels.removeAt(rmlabel.getLabelID());
    }
    else{
      return;
    }
  }
  else{
    //label not found
    return;
  }
}


void MainWindow::saveSelectionLabel()
{
  QStringList labels_;
  for(int i = 0; i < labels.size(); i++)
    labels_.append(labels[i].name);
  if(labels_.size() > 0){
    SelectByLabelDialog selbylabel(labels_);
    if(selbylabel.exec() == QDialog::Accepted){
      int indx = selbylabel.getLabelID();
      QString fname = QFileDialog::getSaveFileName( this, tr("Save Selection As.."), QDir::currentPath(), tr("CSV file(*.csv)"), 0, QFileDialog::DontUseNativeDialog);
      if(fname.contains(".csv", Qt::CaseInsensitive) == false)
        fname += ".csv";
      QFile file(fname);
      if(file.open(QIODevice::ReadWrite)){
        QTextStream stream(&file);
        stream << "Molecule Name;SMILES;log P;MW\n";
        int ngps = m->scores->row;
        for(int i = 0; i < labels[indx].ids.size(); i++){
          int id = labels[indx].ids[i]-ngps;
          stream << "\"" <<  smilist[id].name << "\";" <<  smilist[id].smiles << ";" << pdescriptors->data[id][0] << ";" << pdescriptors->data[id][57] << "\n";
        }
        file.close();
      }
    }
    else{
      return;
    }
  }
  else{
    //label not found
    return;
  }
}

void MainWindow::resetPlot()
{
  chart->RemoveAllPoints();
  chart->RemoveAllCurves();

  ui->spinBox->setValue(1);
  ui->spinBox_2->setValue(2);

  unsigned int x, y;
  x = (unsigned int) ui->spinBox->value()-1;
  y = (unsigned int) ui->spinBox_2->value()-1;

  chart->setXaxisName(QString("PC %1").arg(x+1));
  chart->setYaxisName(QString("PC %1").arg(y+1));

  for(int i = 0; i < m->scores->row; i++){
    chart->addPoint(m->scores->data[i][x], m->scores->data[i][y], QString("GPSOBJ%1").arg(i+1));
  }

  int ngps = m->scores->row;
  for(int i = 0; i < pscores->row; i++){
    chart->addPoint(pscores->data[i][x], pscores->data[i][y], smilist[i].name);
    chart->getPoint(ngps+i)->setColor(Qt::red);
  }

  if(ui->viewGPS->isChecked() == false){
    for(int i = 0; i < m->scores->row; i++){
      chart->getPoint(i)->setVisible(false);
    }
  }

  if(ui->viewMolecules->isChecked() == false){
    int ngps = m->scores->row;
    for(int i = 0; i < pscores->row; i++){
      chart->getPoint(i+ngps)->setVisible(false);
    }
  }


  chart->Refresh();
}

void MainWindow::UpdateImageView()
{
  UpdateSelectedPoints();
  if(selectedPoints.size() > 0){
    while(ui->tabWidget->count() > 0){
      int indx = ui->tabWidget->currentIndex();
      delete ui->tabWidget->widget(indx);
      ui->tabWidget->removeTab(indx);
    }
    ui->tabWidget->clear();

    int ngps = m->scores->row;
    for(int i = 0; i < selectedPoints.size(); i++){
      int indx = selectedPoints[i]-ngps;
      if(indx > -1){
        QLabel *img = new QLabel();
        img->setMinimumSize(342, 313);
        img->resize(342,313);
        img->setPixmap(pimages[indx].img);
        img->setScaledContents(true);
        img->pixmap()->scaled(342, 313, Qt::KeepAspectRatioByExpanding);
        ui->tabWidget->addTab(img, pimages[indx].name);
      }
      else{
        //GPS molecule
        continue;
      }
    }
  }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->setUsesScrollButtons(true);

    vbox = new QVBoxLayout;
    chart = new QPlotlyWindow();
    chart->setWindowTitle(QObject::tr("ChemSpace Tracker"));
    vbox->addWidget(chart);
    ui->plotwidget->setLayout(vbox);

    NewChemSpace();
    resetPlot();
    chart->show();

    ui->similaritySearchButton->hide();
    ui->localizeButton->hide();
    connect(ui->actionAbout, SIGNAL(triggered()), SLOT(About()));
    connect(ui->importMoleculeButton, SIGNAL(clicked(bool)), SLOT(ImportMolecules()));
    connect(ui->similaritySearchButton, SIGNAL(clicked(bool)), SLOT(SimilaritySearch()));
    connect(ui->localizeButton, SIGNAL(clicked(bool)), SLOT(SelectMolecules()));
    connect(ui->actionNew_Chem_Space, SIGNAL(triggered()), SLOT(NewChemSpace()));
    connect(ui->actionOpen_Chem_Space, SIGNAL(triggered()), SLOT(OpenChemSpace()));
    connect(ui->actionSave_Chem_Space_As, SIGNAL(triggered()), SLOT(SaveChemSpaceAs()));
    connect(ui->actionClose, SIGNAL(triggered()), SLOT(close()));
    connect(ui->spinBox,  SIGNAL(valueChanged(int)), SLOT(setAxis()));
    connect(ui->spinBox_2,  SIGNAL(valueChanged(int)), SLOT(setAxis()));
    connect(ui->gpsPointSize,  SIGNAL(valueChanged(int)), SLOT(setGPSPointSize()));
    connect(ui->moleculePointSize,  SIGNAL(valueChanged(int)), SLOT(setMoleculePointSize()));
    connect(ui->viewGPS,  SIGNAL(stateChanged(int)), SLOT(ShowHidePoints()));
    connect(ui->viewMolecules,  SIGNAL(stateChanged(int)), SLOT(ShowHidePoints()));
    connect(ui->searchButton,  SIGNAL(clicked(bool)), SLOT(SearchCompound()));

    chart->weview()->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(chart->weview(), SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(ShowContextMenu(const QPoint&)));
}

MainWindow::~MainWindow()
{
  DelPCAModel(&m);
  DelMatrix(&pscores);
  delete chart;
  delete vbox;
  delete ui;
}
