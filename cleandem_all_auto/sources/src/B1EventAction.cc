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
// $Id: B1EventAction.cc 75604 2013-11-04 13:17:26Z gcosmo $
//
/// \file B1EventAction.cc
/// \brief Implementation of the B1EventAction class

#include "B1EventAction.hh"
#include "Analysis.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
//#include "G4HCofThisEvent.hh"
//#include "G4THitsMap.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//B1EventAction* B1EventAction::fgInstance = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

//B1EventAction* B1EventAction::Instance()
//{
// Static acces function via G4RunManager 

  //return fgInstance;
//}   

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::B1EventAction()
 : G4UserEventAction(),
   fCollID_cryst(-1),
   fLight(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1EventAction::~B1EventAction()
{
	//fgInstance = 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*G4THitsMap<G4double>* 
B1EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  G4THitsMap<G4double>* hitsCollection 
    = static_cast<G4THitsMap<G4double>*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("B1EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}*/

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/*G4double B1EventAction::GetSum(G4THitsMap<G4double>* hitsMap) const
{
  G4double sumValue = 0;
  std::map<G4int, G4double*>::iterator it;
  for ( it = hitsMap->GetMap()->begin(); it != hitsMap->GetMap()->end(); it++) {
    sumValue += *(it->second);
    
    //G4cout << "Linea del it: " << " " << sumValue << " " <<  *(it->second) << G4endl;
  }
  return sumValue;  
} */

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::BeginOfEventAction(const G4Event*)
{
	//G4int evtNb = event->GetEventID();
  
	//if (evtNb%fPrintModulo == 0) { 
	//	G4cout << G4endl << "---> begin of event: " << evtNb << G4endl;
	//}  
	
	// initialisation per event
	//fEnergy_276 = 0.;
	fLight = 0.;
    
	//G4cout << "---> I am here <---" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1EventAction::EndOfEventAction(const G4Event* evt)
{
	 //Hits collections
  //  
  G4HCofThisEvent* HCE = evt->GetHCofThisEvent();
  if(!HCE) return;
               
   // Get hits collections IDs
  if (fCollID_cryst < 0) {
    G4SDManager* SDMan = G4SDManager::GetSDMpointer();  
    fCollID_cryst = SDMan -> GetCollectionID("crystal/edep");    
  }
  
  //Energy in crystals : identify 'good events'
   
  G4THitsMap<G4double>* evtMap = 
                     (G4THitsMap<G4double>*)(HCE->GetHC(fCollID_cryst));
  
  G4double edep=0; 
             
  std::map<G4int,G4double*>::iterator itr;
  for (itr = evtMap->GetMap()->begin(); itr != evtMap->GetMap()->end(); itr++)
  {
    ///G4int copyNb  = (itr->first);
    edep = *(itr->second); 
  }  

  // Fill the N-tuple
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  if (edep > 0. || fLight > 0.)
  {
    analysisManager->FillNtupleDColumn(0,edep); // energy deposited on the CeBr3
    analysisManager->FillNtupleDColumn(1,fLight); // Stilbene light output
    analysisManager->AddNtupleRow();
  }

}
