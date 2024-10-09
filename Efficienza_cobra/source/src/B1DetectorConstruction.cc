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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4VSensitiveDetector.hh"

#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"

#include "G4VSDFilter.hh"
#include <G4SDParticleFilter.hh>

#include "G4SDManager.hh"


B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }


B1DetectorConstruction::~B1DetectorConstruction()
{ }

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
// Get nist material manager
G4NistManager* nist = G4NistManager::Instance();
G4bool checkOverlaps = true;

/***************************/
// World
/***************************/
G4double world_sizeXY = 30.*cm;
G4double world_sizeZ  = 30.*cm;
G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
G4Box* solidWorld =  new G4Box("World",0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,world_mat,"World");
G4VPhysicalVolume* physWorld = new G4PVPlacement(0,G4ThreeVector(),logicWorld,"World",0,false,0,checkOverlaps);


/***************************/
// Detector
/***************************/

double xx = 0.4 * mm *0.5;
double yy = 0.4 * mm *0.5;
double zz = 0.1 * mm *0.5;

G4Box* solidEnv = new G4Box("Envelope",xx,yy,zz);
G4Material* detector_material = nist->FindOrBuildMaterial("G4_Si");

// G4Tubs* solidEnv = new G4Tubs("Envelope",0.*cm,0.5*radius,0.5*lenght,0*deg,360*deg);
// G4Material* detector_material = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");

G4LogicalVolume* Logic_detector = new G4LogicalVolume(solidEnv,detector_material,"Logic_detector");
new G4PVPlacement(0,G4ThreeVector(),Logic_detector,"crystal",logicWorld,false,0,checkOverlaps);
    
return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void B1DetectorConstruction::ConstructSDandField()
{
G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

G4MultiFunctionalDetector* cryst = new G4MultiFunctionalDetector("crystal");
G4SDManager::GetSDMpointer()->AddNewDetector(cryst);
G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");

// G4SDParticleFilter* alphaFilter = new G4SDParticleFilter("alphaFilter");

// bool gamma_source = true;
// bool neutron_source = false;

// if (gamma_source){
// alphaFilter->add("gamma");
// alphaFilter->add("e-");
// }
// if (neutron_source){
// alphaFilter->add("alpha");
// }

// primitiv1->SetFilter(alphaFilter);
cryst->RegisterPrimitive(primitiv1);

SetSensitiveDetector("Logic_detector",cryst);
}
