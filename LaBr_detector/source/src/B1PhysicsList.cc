//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file B3PhysicsList.cc
/// \brief Implementation of the B3PhysicsList class

#include "B1PhysicsList.hh"

//#include "G4DecayPhysics.hh"

//#include "G4EmStandardPhysics.hh"

//#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"

//#include "G4RadioactiveDecayPhysics.hh"

// #include "QGSP_BERT_HP.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PhysicsList::B1PhysicsList() 
: G4VModularPhysicsList(){
  SetVerboseLevel(2);

  // Default physics
  //RegisterPhysics(new G4DecayPhysics());

  // EM physics standard
  //RegisterPhysics(new G4EmStandardPhysics());

    // EM physics PENELOPE PHYSICS
  RegisterPhysics(new G4EmPenelopePhysics());

    // EM physics LIVERMORE PHYSICS
  //RegisterPhysics(new G4EmLivermorePhysics());

  // Radioactive decay
  //RegisterPhysics(new G4RadioactiveDecayPhysics());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1PhysicsList::~B1PhysicsList()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1PhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}  
