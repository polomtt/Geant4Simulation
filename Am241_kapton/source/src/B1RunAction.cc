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
//
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
#include "B1PrimaryGeneratorAction.hh"
#include "B1DetectorConstruction.hh"
// #include "B1Run.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

// #include "MyAnalysis.hh"

#include "Analysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction(const char *fN)
 : G4UserRunAction(),
   fGoodEvents(0)
{  
    filename=fN;
    	auto analysisManager = G4AnalysisManager::Instance();


   // Register accumulable to the accumulable manager
  G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
  accumulableManager->RegisterAccumulable(fGoodEvents);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void B1RunAction::BeginOfRunAction(const G4Run*)
// { 
//   // inform the runManager to save random number seed
//   G4RunManager::GetRunManager()->SetRandomNumberStore(false);
// 
//   // reset accumulables to their initial values
// //   G4AccumulableManager* accumulableManager = G4AccumulableManager::Instance();
// //   accumulableManager->Reset();
// 
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* run)
{
  G4int nofEvents = run->GetNumberOfEvent();
  if (nofEvents == 0) return;
    
  
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 
  // Write and close the output file 
  analysisManager->Write(); 
  analysisManager->CloseFile(); 
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run* run)  
{ 
  // Create/get analysis manager 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 
  analysisManager->SetVerboseLevel(1); 

  // analysisManager->OpenFile("histo.root");
  analysisManager->OpenFile();
  analysisManager->CreateNtuple("MyNtuple", "Edep");
  analysisManager->CreateNtupleDColumn("Eabs_cry_1");
  analysisManager->CreateNtupleDColumn("Eabs_cry_2");
  analysisManager->CreateNtupleDColumn("Eabs_tot");
  analysisManager->FinishNtuple();

}


