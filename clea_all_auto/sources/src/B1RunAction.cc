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
/// \file B1RunAction.cc
/// \brief Implementation of the B1RunAction class

#include "B1RunAction.hh"
//#include "B1PrimaryGeneratorAction.hh"
//#include "B1EventAction.hh"
//#include "B1SteppingAction.hh"
  // use of other actions 
  // - primary generator: to get info for printing about the primary
  // - event action: to get and reset accumulated energy sums
  // - stepping action: to get info about accounting volume 

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "Analysis.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::B1RunAction(/*B1EventAction* eventAction*/)
: G4UserRunAction() //,
  //fEventAction(eventAction)
{
	auto analysisManager = G4AnalysisManager::Instance();
	//G4cout << "Using " << analysisManager->GetType() << G4endl;

	analysisManager->SetVerboseLevel(1);
	//analysisManager->SetNtupleMerging(true);
  
	// Creating ntuple
	//
	analysisManager->CreateNtuple("results","resultsG4");
	analysisManager->CreateNtupleDColumn("CeBr3");
	analysisManager->CreateNtupleDColumn("stilbene");
	analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1RunAction::~B1RunAction()
{
	delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::BeginOfRunAction(const G4Run* /*aRun*/)
{ 
	//G4cout << "### Run " << aRun->GetRunID() << " start." << G4endl;

	//inform the runManager to save random number seed
	G4RunManager::GetRunManager()->SetRandomNumberStore(false);

	// Create analysis manager
	// The choice of analysis technology is done via selection of a namespace
	// in Analysis.hh
	
	G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

	// Open an output file
	//
	//G4String fileName = "results_cllb";
	analysisManager->OpenFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1RunAction::EndOfRunAction(const G4Run* aRun)
{
	G4int nofEvents = aRun->GetNumberOfEvent();
	if (nofEvents == 0) return;

	auto analysisManager = G4AnalysisManager::Instance();

	analysisManager->Write();
	analysisManager->CloseFile();

	// Contador
	//G4double minEnergy, maxEnergy;

	//std::ofstream text_File ("flux.csv");

	//text_File 
	//G4cout << "Energy Min (MeV)\t" << "Energy Max (MeV)\t" << "Neutron Flux\t" << G4endl;

	//for (G4int i=0; i<128; i++) 
	//{
	//  minEnergy = pow(1.19709, i)*1.0e-9*MeV;
	//  maxEnergy = pow(1.19709, i + 1)*1.0e-9*MeV;
		  
	  // G4double contador_final  = fEventAction->GetCounter(i);
	   
	   //text_File 
	  // G4cout << minEnergy << "\t" << maxEnergy << "\t" << contador_final << G4endl;
	//}

	//text_File.close();


	// Print results
	//
	if (IsMaster())
	{
		G4cout
		<< G4endl
		<< "--------------------End of Global Run-----------------------"
		<< G4endl
		<< "  The run was " << nofEvents << " events " << G4endl;
	}
	else
	{
		G4cout
		<< G4endl
		<< "--------------------End of Local Run------------------------"
		<< G4endl
		<< "  The run was " << nofEvents << " events " << G4endl; //<< partName;
	}      

	// save histograms 
	//
	std::cout << "sono qui al final" << std::endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
