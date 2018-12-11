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
#include <iostream>
#include <fstream>
#include <exception>

#include <GraphMol/RDKitBase.h>
#include <GraphMol/GraphMol.h>

#include <GraphMol/RWMol.h>

#include <GraphMol/SmilesParse/SmilesParse.h>
#include <GraphMol/SmilesParse/SmilesWrite.h>
#include <GraphMol/FileParsers/FileParsers.h>
#include <GraphMol/FileParsers/MolSupplier.h>

#include <GraphMol/Descriptors/MolDescriptors.h>
#include <GraphMol/Descriptors/Crippen.h>
#include <GraphMol/Descriptors/Property.h>
#include <GraphMol/Descriptors/MolSurf.h>
#include <GraphMol/Descriptors/Lipinski.h>

#include <GraphMol/PeriodicTable.h>
#include <GraphMol/atomic_data.h>

/*Depictor headers */
#include <GraphMol/Depictor/RDDepictor.h>
#include <GraphMol/MolDraw2D/MolDraw2D.h>
#include <GraphMol/MolDraw2D/MolDraw2DSVG.h>
#include <GraphMol/MolDraw2D/MolDraw2DUtils.h>

using namespace RDKit;
using namespace RDKit::Descriptors;


void calcDescriptors(const char *smi, std::vector<double> *descriptors, std::vector<std::string> *varnames)
{
  RDKit::ROMol *mol = (RDKit::ROMol*)0;
  try {
    //rdErrorLog->df_enabled = false;  Do not report parsing errors
    mol = RDKit::SmilesToMol(smi, 0, 1);
    if(mol){
      double MolLogP, MolMR;
      RDKit::Descriptors::calcCrippenDescriptors(*mol, MolLogP, MolMR, true, false);
      (*descriptors).push_back(MolLogP); (*varnames).push_back("MolLogP");
      (*descriptors).push_back(MolMR); (*varnames).push_back("MolMR");
      double LabuteASA = RDKit::Descriptors::calcLabuteASA(*mol); (*descriptors).push_back(LabuteASA); (*varnames).push_back("LabuteASA");
      double TPSA = RDKit::Descriptors::calcTPSA(*mol); (*descriptors).push_back(TPSA); (*varnames).push_back("TPSA");

      std::vector<double> SlogP_VSA = calcSlogP_VSA(*mol);
      for(int i = 0; i < SlogP_VSA.size(); i++){
        (*descriptors).push_back(SlogP_VSA[i]);
        (*varnames).push_back("SlogP_VSA_"+std::to_string(i+1));
      }
      std::vector<double> SMR_VSA = calcSMR_VSA(*mol);
      for(int i = 0; i < SMR_VSA.size(); i++){
        (*descriptors).push_back(SMR_VSA[i]);
        (*varnames).push_back("SMR_VSA_"+std::to_string(i+1));
      }
      std::vector<double> PEOPE_VSA = calcPEOE_VSA(*mol);
      for(int i = 0; i < PEOPE_VSA.size(); i++){
        (*descriptors).push_back(PEOPE_VSA[i]);
        (*varnames).push_back("PEOPE_VSA_"+std::to_string(i+1));
      }

      unsigned int  NumRotatableBonds = calcNumRotatableBonds(*mol); (*descriptors).push_back(NumRotatableBonds); (*varnames).push_back("NumRotatableBonds");
      unsigned int NumHDonors = calcNumHBD(*mol); (*descriptors).push_back(NumHDonors); (*varnames).push_back("NumHDonors");
      unsigned int NumHAcceptors = calcNumHBA(*mol); (*descriptors).push_back(NumHAcceptors); (*varnames).push_back("NumHAcceptors");
      unsigned int NumHeteroatoms = calcNumHeteroatoms(*mol); (*descriptors).push_back(NumHeteroatoms); (*varnames).push_back("NumHeteroatoms");
      unsigned int nambonds = calcNumAmideBonds(*mol); (*descriptors).push_back(nambonds); (*varnames).push_back("NumAmideBonds");
      double FracCSP3 = calcFractionCSP3(*mol); (*descriptors).push_back(FracCSP3); (*varnames).push_back("FractionCSP3");
      unsigned int RingCount = calcNumRings(*mol); (*descriptors).push_back(RingCount); (*varnames).push_back("NumRings");
      unsigned int NumAromaticRings = calcNumAromaticRings(*mol); (*descriptors).push_back(NumAromaticRings); (*varnames).push_back("NumAromaticRings");
      unsigned int NumAliphaticRings = calcNumAliphaticRings(*mol); (*descriptors).push_back(NumAliphaticRings); (*varnames).push_back("NumAliphaticRings");
      unsigned int NumSaturatedRings = calcNumSaturatedRings(*mol); (*descriptors).push_back(NumSaturatedRings); (*varnames).push_back("NumSaturatedRings");
      unsigned int nhetcycl = calcNumHeterocycles(*mol); (*descriptors).push_back(nhetcycl); (*varnames).push_back("NumHeterocycles");
      unsigned int NumAromaticHeterocycles = calcNumAromaticHeterocycles(*mol); (*descriptors).push_back(NumAromaticHeterocycles); (*varnames).push_back("NumAromaticHeterocycles");
      unsigned int NumAromaticCarbocycles = calcNumAromaticCarbocycles(*mol); (*descriptors).push_back(NumAromaticCarbocycles); (*varnames).push_back("NumAromaticCarbocycles");
      unsigned int NumSaturatedHeterocycles = calcNumSaturatedHeterocycles(*mol); (*descriptors).push_back(NumSaturatedHeterocycles); (*varnames).push_back("NumSaturatedHeterocycles");
      unsigned int NumSaturatedCarbocycles = calcNumSaturatedCarbocycles(*mol); (*descriptors).push_back(NumSaturatedCarbocycles); (*varnames).push_back("NumSaturatedCarbocycles");
      unsigned int NumAliphaticHeterocycles = calcNumAliphaticHeterocycles(*mol); (*descriptors).push_back(NumAliphaticHeterocycles); (*varnames).push_back("NumAliphaticHeterocycles");
      unsigned int NumAliphaticCarbocycles = calcNumAliphaticCarbocycles(*mol); (*descriptors).push_back(NumAliphaticCarbocycles); (*varnames).push_back("NumAliphaticCarbocycles");

      double MolWt = calcAMW(*mol, false); (*descriptors).push_back(MolWt); (*varnames).push_back("MolWt");
      double HeavyAtomMolWt = calcAMW(*mol, true); (*descriptors).push_back(HeavyAtomMolWt); (*varnames).push_back("HeavyAtomMolWt");
      double ExactMolWt = calcExactMW(*mol, false); (*descriptors).push_back(ExactMolWt); (*varnames).push_back("ExactMolWt");

      double Chi0v = calcChi0v(*mol); (*descriptors).push_back(Chi0v); (*varnames).push_back("Chi0v");
      double Chi1v = calcChi1v(*mol); (*descriptors).push_back(Chi1v); (*varnames).push_back("Chi1v");
      double Chi2v = calcChi2v(*mol); (*descriptors).push_back(Chi2v); (*varnames).push_back("Chi2v");
      double Chi3v = calcChi3v(*mol); (*descriptors).push_back(Chi3v); (*varnames).push_back("Chi3v");
      double Chi4v = calcChi4v(*mol); (*descriptors).push_back(Chi4v); (*varnames).push_back("Chi4v");

      double Chi0n = calcChi0n(*mol); (*descriptors).push_back(Chi0n); (*varnames).push_back("Chi0n");
      double Chi1n = calcChi1n(*mol); (*descriptors).push_back(Chi1n); (*varnames).push_back("Chi1n");
      double Chi2n = calcChi2n(*mol); (*descriptors).push_back(Chi2n); (*varnames).push_back("Chi2n");
      double Chi3n = calcChi3n(*mol); (*descriptors).push_back(Chi3n); (*varnames).push_back("Chi3n");
      double Chi4n = calcChi4n(*mol); (*descriptors).push_back(Chi4n); (*varnames).push_back("Chi4n");

      double HallKierAlpha = calcHallKierAlpha(*mol); (*descriptors).push_back(HallKierAlpha); (*varnames).push_back("HallKierAlpha");
      double Kappa1 = calcKappa1(*mol); (*descriptors).push_back(Kappa1); (*varnames).push_back("Kappa1");
      double Kappa2 = calcKappa2(*mol); (*descriptors).push_back(Kappa2); (*varnames).push_back("Kappa2");
      double Kappa3 = calcKappa3(*mol); (*descriptors).push_back(Kappa3); (*varnames).push_back("Kappa3");

      std::vector<unsigned int> MQNs = calcMQNs(*mol);
      for(int i = 0; i < MQNs.size(); i++){
        (*descriptors).push_back(MQNs[i]);
        (*varnames).push_back("MQNs_"+std::to_string(i+1));
      }
    }
    else{
      std::cout << "[calcDescriptors Error!] " << smi << " error no. " << std::endl;
    }
    delete mol;
  }
  catch (...){ // RDKit::MolSanitizeException and friends
    delete mol;
    std::cout << "[calcDescriptors Error!] " << smi << " error no. " << std::endl;
  }
 }

void gen2DDepiction(const char *smi, int width=300, int height=300, const char *outimagefile=NULL)
{
  RDKit::ROMol *mol = (RDKit::ROMol*)0;
  try {
    //rdErrorLog->df_enabled = false;  Do not report parsing errors
    mol = RDKit::SmilesToMol(smi, 0, 1);
    if(mol){
      RDDepict::compute2DCoords((*mol));
      std::ofstream o(outimagefile);
      MolDraw2DSVG d(width, height, o);
      d.drawMolecule(*mol);
      d.finishDrawing();
      o.flush();
    }
    else{
      std::cout << "[calcDescriptors Error!] " << smi << " error no. " << std::endl;
    }
    delete mol;
  }
  catch (...){ // RDKit::MolSanitizeException and friends
    delete mol;
    std::cout << "[calcDescriptors Error!] " << smi << " error no. " << std::endl;
  }
}
