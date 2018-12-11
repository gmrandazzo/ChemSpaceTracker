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

#ifndef MOLOCALIZER_H
#define MOLOCALIZER_H
#include <scientific.h>

/* Algorithm
 * cc <- calculate the active centroid or select a molecule as centroid
 * dabd <- 50/50, 1/10, 1/100, 1/1000
 * for each distance from cc to the max distance:
 *     dsets build a dataset  n times
 *     for each dataset in dsets:
 *         ntp <- count how many tp molecules are in this distance
 *         nfn <- count how many tn molecules are in this distance
 *         probability <- ntp/ (ntp+nfn) = ntp / tot molecule in this distance
 *
 *  plot dist, probability, dabd
 *
 */
class Localizer
{
public:
  Localizer(matrix *gps, matrix *tp, matrix *tn, int grid_size);
  ~Localizer();
private:
  HyperGridMap *hgm;
  std::vector <int> allbins;
  uivector *tp_bins, *tn_bins;
  size_t niters;
  size_t n_tp;
};

#endif
