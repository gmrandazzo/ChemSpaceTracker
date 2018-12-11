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
#include "mlocalizer.h"
#include <scientific.h>
#include <vector>

void Localizer::ProbCalc()
{
  size_t i, j, k;
  std::vector<int> tp_id, tn_id;
  std::vector<int> v = {1, 10, 100, 1000, 10000}

  for(i = 0; i < v.size(); i++){
    size_t n_tn = n_tp*v[i];
    if(n_tn > tn_bins->size){
      n_tn = tn_bins->size;
    }

    std::cout << " N. TP: " << n_tp << " N. TN: " << n_tn << std::endl;

    for(j = 0; j < allbins.size(); j++){
      for(k = 0; k < niters; k++){

      }
    }
  }
}

Localizer::Localizer(matrix *gps, matrix *tp, matrix *tn, int grid_size)
{
  /* Initialize some variables */
  niters = 1000;
  n_tp = tp->row;

  /*Create the hyper grid map */
  NewHyperGridMap(&hgm);
  HyperGridMap(gps, grid_size, NULL, &hgm);
  initUIVector(&tp_bins);
  initUIVector(&tn_bins);
  /*Project the true positive and get the covered bins */
  HyperGridMapObjects(tp, hgm, &tp_bins);
  /*Project the true negative and get the covered bins */
  HyperGridMapObjects(tn, hgm, &tn_bins);

  /* Add into a set s all the bins to avoid repeated bins */
  std::set<int> s;
  for(size_t i = 0; i < tp_bins->size; i++)
    s.insert(tp_bins->data[i]);

  for(i = 0; i < tn_bins->size; i++)
    s.insert(tn_bins->data[i]);
  //Remove duplicates
  allbins.assign(s.begin(), s.end());


  /* For each bin calculate the ratio between the tp and tn which correspond
   * to the probability to find an active. Do this iterativelly!
   */

}

Localizer::~Localizer()
{
  DelHyperGridMap(&hgm);
  DelUIVector(&tp_bins);
  DelUIVector(&tn_bins);
}
