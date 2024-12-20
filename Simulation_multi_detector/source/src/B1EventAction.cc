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
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "B1RunAction.hh"
#include "B1DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

#include "MyAnalysis.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction(B1RunAction* runAction)
: G4UserEventAction(),
  fRunAction(runAction),
  fCollID_cryst(-1),
  fCollID_cryst2(-1)
{  
} 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event* event) 
{   

  // Ottieni l'istanza della classe B1DetectorConstruction
  const B1DetectorConstruction* detectorConstruction = 
  static_cast<const B1DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());
  // Accedi al membro non statico numDetectors
  int N_detector = detectorConstruction->numDetectors;

  // Get analysis manager 
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance(); 
  
  G4HCofThisEvent* HCE = event->GetHCofThisEvent();
  if(!HCE) return;

  //        _      _            _       _           
  //   __| | ___| |_     __ _| |_ __ | |__   __ _ 
  //  / _` |/ _ \ __|   / _` | | '_ \| '_ \ / _` |
  // | (_| |  __/ |_   | (_| | | |_) | | | | (_| |
  //  \__,_|\___|\__|___\__,_|_| .__/|_| |_|\__,_|
  //               |_____|     |_|                

  for(int det = 0;det<N_detector;det++) { 
  // Get hits collections IDs
  if (fCollID_cryst < 0) {
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();  
    fCollID_cryst   = SDMan->GetCollectionID("alfa_detector_" + std::to_string(det)+"/edep");  
  }
  
  // //Energy in crystals_1
  G4double edep_cry_1=0;
  G4THitsMap<G4double>* evtMap = (G4THitsMap<G4double>*)(HCE->GetHC(fCollID_cryst));       
  std::map<G4int,G4double*>::iterator itr;
  for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++) {
            edep_cry_1 = *(itr->second);
  } 
  analysisManager->FillNtupleDColumn(det,edep_cry_1);
  fCollID_cryst=-1;
  }
  
  //        _      _       _             
  //   __| | ___| |_    (_) ___  _ __  
  //  / _` |/ _ \ __|   | |/ _ \| '_ \ 
  // | (_| |  __/ |_    | | (_) | | | |
  //  \__,_|\___|\__|___|_|\___/|_| |_|
  //               |_____|           

  for(int det = 0;det<N_detector;det++) { 
  // Get hits collections IDs
  if (fCollID_cryst2 < 0) {
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();  
    fCollID_cryst2   = SDMan->GetCollectionID("ion_detector_" + std::to_string(det)+"/edep");  
  }
  
  // //Energy in crystals_1
  G4double edep_cry_2=0;
  G4THitsMap<G4double>* evtMap2 = (G4THitsMap<G4double>*)(HCE->GetHC(fCollID_cryst2));       
  std::map<G4int,G4double*>::iterator itr;
  for (itr = evtMap2->GetMap()->begin(); itr != evtMap2->GetMap()->end(); itr++) {
            edep_cry_2 = *(itr->second);
  } 
  analysisManager->FillNtupleDColumn(det+N_detector,edep_cry_2);
  fCollID_cryst2=-1;
  }

  analysisManager->AddNtupleRow();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
