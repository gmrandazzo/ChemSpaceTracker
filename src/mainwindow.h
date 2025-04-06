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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <scientific.h>

#include <QMainWindow>
#include <QVBoxLayout>
#include <QStringList>
#include <QPixmap>
//#include "Chart/chart.h"
#include "QPlotly/qplotly.h"
#include "iodata.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void About();
    void NewChemSpace();
    void OpenChemSpace();
    void SaveChemSpaceAs();
    void ImportMolecules();
    void SimilaritySearch();
    void SearchCompound();
    void setAxis();
    void setGPSPointSize();
    void setMoleculePointSize();
    void ShowHidePoints();
    void SelectMolecules();

    void ShowContextMenu(const QPoint& pos);
    void SelectAll();
    void SelectByLabel();
    void invertSelection();
    void hideSelection();
    void hideGPSSelection();
    void clearSelection();
    void clearGPSSelection();
    void setSelectionStyle();
    void setSelectionLabel();
    void saveSelectionLabel();
    void removeSelectionLabel();
    void resetPlot();
    void UpdateImageView();

private:
    Ui::MainWindow *ui;
    QVBoxLayout *vbox;
    QPlotlyWindow *chart;
    PCAMODEL *m;
    SMILIST smilist;
    matrix *pscores;
    matrix *pdescriptors;
    IMAGES pimages;
    QStringList varnames;
    LABELS labels;
    QList<int> selectedPoints;
    void UpdateSelectedPoints();
};

#endif // MAINWINDOW_H
