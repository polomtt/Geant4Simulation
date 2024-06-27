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
// $Id: B1EventAction.hh 75215 2013-10-29 16:07:06Z gcosmo $
//
/// \file B1EventAction.hh
/// \brief Definition of the B1EventAction class

#ifndef B1EventAction_h
#define B1EventAction_h 1

#include "G4UserEventAction.hh"
#include "G4THitsMap.hh"
#include "globals.hh"
//#include "Analysis.hh"

/// Event action class
///
/// In EndOfEventAction(), it prints the accumulated quantities of the energy 
/// deposit and track lengths of charged particles in Absober and Gap layers 
/// stored in the hits collections.

class B1EventAction : public G4UserEventAction
{
public:
  B1EventAction();
  virtual ~B1EventAction();
  
   // static access method
   // static B1EventAction* Instance();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void  EndOfEventAction(const G4Event* event);
  
 // G4double GetCounter(G4int q) const { return fCounter[q]; }
  
  //void AddEne_276(G4double ene);
  void AddLight_299(G4double p_ene);
 
private:	

	//G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
	//                          const G4Event* event) const;
	//G4double GetSum(G4THitsMap<G4double>* hitsMap) const;

	//static B1EventAction* fgInstance;    

	// data members                   
	//G4int  fCollID_detector[128];
	//G4double  fCounter[128];         
	//G4int  fPrintModulo;
	G4int fCollID_cryst;
	//G4double  fEnergy_276;
	G4double fLight;

	//G4AnalysisManager* analysisManager;
	
};

// inline functions

// EJ-276 scintillator
//inline void B1EventAction::AddEne_276(G4double ene)
//{	
	//fEnergy_276 += ene;
//}

inline void B1EventAction::AddLight_299(G4double p_ene) 
{
	//G4double ene_mevee = ( ( 0.75*p_ene ) - ( 3.2*(1-exp(-0.22*p_ene)) ) ); // Chris C. Lawrence et al (2014) for EJ299-33 3"x3";

	G4double ene_mevee = ( ( 0.68*p_ene ) - ( 1.7*(1-exp(-0.32*pow(p_ene,1.03))) ) ); // R. A. Weldon Jr et al (2020) for single-crystal organic scintillator trans-stilbene 1 cm3 cube
	
	fLight += ene_mevee;
	
	//G4cout << "p_ene: " << p_ene << G4endl;
	//G4cout << "ene_mevee: " << ene_mevee << G4endl;
	//G4cout << "fLight: " << fLight << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
