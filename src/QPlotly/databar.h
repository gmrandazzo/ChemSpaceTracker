/*
 * QPlotly: Just a plotly.js wrapper
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
#ifndef DATABAR_H
#define DATABAR_H

#include <QtGlobal>
#include <QColor>
#include <QList>
#include <QVector>
#include <QStringList>
#include <QMetaType>


class DataBar
{
public:
  DataBar();
  DataBar(QStringList x, QVector<qreal> y);
  DataBar(QStringList x,  QVector<qreal> y, QStringList text);

  void setX(QStringList x);
  void setY(QVector<qreal> y);
  void setText(QStringList text);
  QStringList &getText();
  void setColor(const QColor &color);
  QColor &getColor();
  void setWidth(int width);

  void setSelection(bool selection);
  void setVisible(bool visible);
  void setTextVisible(bool tvisible);

  QStringList x() const;
  QVector<qreal> y() const;
  QStringList text() const;
  bool isSelected() const;
  bool isVisible() const;
  bool isTextVisible() const;

  QColor color() const;
  int width() const;

private:
  QStringList m_x;
  QVector<qreal> m_y;
  QStringList m_text;
  QColor m_color;
  int m_width;
  bool m_selection;
  bool m_visible, m_tvisible;
};

#endif // DATABAR_H
