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
// \file exampleB1.cc
// \brief Main program of the B1 example

#include "B1DetectorConstruction.hh"
#include "B1ActionInitialization.hh"
//#include "B1PrimaryGeneratorAction.hh"
//#include "B1RunAction.hh"
//#include "B1EventAction.hh"
//#include "B1PrimaryGeneratorAction.hh"
//#include "B1SteppingAction.hh"
#include "Analysis.hh"

// #include "G4RunManager.hh"
#include "G4RunManagerFactory.hh"

#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "Randomize.hh"

#include "G4Threading.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
	// Detect interactive mode (if no arguments) and define UI session
	//
	G4UIExecutive* ui = 0;
	if ( argc == 1 ) 
	{
		ui = new G4UIExecutive(argc, argv);
	}
  
	// Choose the Random engine
	//
	G4Random::setTheEngine(new CLHEP::RanecuEngine);
    
    // set random seed wuth the system time
    G4long seed = time(NULL);
    CLHEP::HepRandom::setTheSeed(seed);
  
	// Construct the default run manager
	//
// 	G4RunManager* runManager = new G4RunManager;
    
//    G4RunManagerFactory* runManager = new G4RunManagerFactory;
auto* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
 runManager->SetNumberOfThreads(2);  //PER LANCIARE IN PARALLELO DECOMMENTARE

	// Set mandatory initialization classes
	//
	// Detector construction
	runManager->SetUserInitialization(new B1DetectorConstruction());

	// Physics list
	G4int verbose = 1;
	G4PhysListFactory factory;
	//G4VModularPhysicsList* physlist = factory.GetReferencePhysList("FTFP_BERT_EMV");
	G4VModularPhysicsList* physlist_neut = factory.GetReferencePhysList("QGSP_BERT_HP");
	//physlist->SetVerboseLevel(verbose);
	physlist_neut->SetVerboseLevel(verbose);
	//runManager->SetUserInitialization(physlist);
	runManager->SetUserInitialization(physlist_neut);
  
	// Set user action initialization
	//

	//runManager->SetUserAction(new B1PrimaryGeneratorAction());

	//auto eventAction = new B1EventAction;
	//runManager->SetUserAction(new B1RunAction(eventAction));

	//std::cout << "sono qui" << std::endl;

	//B1RunAction* runAction = new B1RunAction(eventAction);
	//runManager->SetUserAction(runAction);

	//runManager->SetUserAction(new B1SteppingAction(eventAction));

	runManager->SetUserInitialization(new B1ActionInitialization());

	G4cout << "EL NUMERO DE PROCESADORES ES: " << G4Threading::G4GetNumberOfCores() << G4endl; 
   
   // Initialize G4 kernel
  //
  	//runManager->Initialize();

	// Initialize visualization
	//
	G4VisManager* visManager = new G4VisExecutive;
	// G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
	// G4VisManager* visManager = new G4VisExecutive("Quiet");
	visManager->Initialize();

	// Get the pointer to the User Interface manager
	G4UImanager* UImanager = G4UImanager::GetUIpointer();

	// Process macro or start UI session
	//
	if ( ! ui ) 
	{ 
		// batch mode
		G4String command = "/control/execute ";
		G4String fileName = argv[1];
		UImanager->ApplyCommand(command+fileName);
	}
	else 
	{ 
		// interactive mode
		UImanager->ApplyCommand("/control/execute macro/init_vis.mac");
		ui->SessionStart();
		delete ui;
	}

	// Job termination
	// Free the store: user actions, physics_list and detector_description are
	// owned and deleted by the run manager, so they should not be deleted 
	// in the main() program !

	delete visManager;
	delete runManager;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
