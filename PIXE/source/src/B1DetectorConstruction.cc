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
G4double world_sizeXY = 100.*cm;
G4double world_sizeZ  = 1000.*cm;
G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
G4Box* solidWorld =  new G4Box("World",0.5*world_sizeXY, 0.5*world_sizeXY, 0.5*world_sizeZ);
G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,world_mat,"World");
G4VPhysicalVolume* physWorld = new G4PVPlacement(0,G4ThreeVector(),logicWorld,"World",0,false,0,checkOverlaps);

/***************************/
// Fascio in vuoto
/***************************/
float lunhezza_pipe = 4*cm;
G4Tubs* tubo_fascio = new G4Tubs("beam_pipe",0,1*cm,lunhezza_pipe,0*deg,360*deg);
G4Material* tubo_fascio_mat = nist->FindOrBuildMaterial("G4_Galactic");
G4LogicalVolume* tubo_fascio_log = new G4LogicalVolume(tubo_fascio,tubo_fascio_mat,"Logic_beam_pipe");
G4VisAttributes* Color_Kpt = new G4VisAttributes(G4Colour::Gray());
tubo_fascio_log->SetVisAttributes(Color_Kpt);
G4RotationMatrix* rotationMatrix_tubo = new G4RotationMatrix();
rotationMatrix_tubo->rotateZ(90.*deg);
new G4PVPlacement(rotationMatrix_tubo,G4ThreeVector(0,0,lunhezza_pipe*(-1)),tubo_fascio_log,"Logic_beam_pipe",logicWorld,false,0,checkOverlaps);

/***************************/
// Foglio kapton
/***************************/

G4Tubs* foglio_kapton = new G4Tubs("kapton_disk",0,1*cm,7*0.5*um,0*deg,360*deg);
G4Material* foglio_kapton_mat = nist->FindOrBuildMaterial("G4_KAPTON");
// G4Material* foglio_kapton_mat = nist->FindOrBuildMaterial("G4_AIR");
G4LogicalVolume* foglio_kapton_log = new G4LogicalVolume(foglio_kapton,foglio_kapton_mat,"Logic_kapton");
G4VisAttributes* Color_Pipe = new G4VisAttributes(G4Colour::Red());
foglio_kapton_log->SetVisAttributes(Color_Pipe);
G4RotationMatrix* rotationMatrix_kpt = new G4RotationMatrix();
rotationMatrix_kpt->rotateZ(90.*deg);
new G4PVPlacement(rotationMatrix_tubo,G4ThreeVector(0,0,3.5*um),foglio_kapton_log,"Logic_beam_pipe",logicWorld,false,0,checkOverlaps);

/***************************/
// Foglio oro
/***************************/

G4Box* gold_foil = new G4Box("gold_foil",5.*cm,5.*cm,25*um);
G4Material* gold_foil_mat = nist->FindOrBuildMaterial("G4_Au");
G4LogicalVolume* gold_foil_log = new G4LogicalVolume(gold_foil,gold_foil_mat,"Logic_gold");
G4VisAttributes* Color_gold = new G4VisAttributes(G4Colour::Yellow());
gold_foil_log->SetVisAttributes(Color_gold);
new G4PVPlacement(0,G4ThreeVector(0,0,5*cm),gold_foil_log,"Logic_beam_gold",logicWorld,false,0,checkOverlaps);

/***************************/
// Detector
/***************************/

float radius =      5.  * cm;
float thickness =   500. * 0.5 * um;
float lunghezza_tubo = 50. * cm * 0.5;
// G4Sphere* solidEnv = new G4Sphere("Envelope",radius,radius+thickness,90*deg,270*deg,asin(10./50.),360*deg);
G4Sphere* solidEnv = new G4Sphere("Envelope",radius,radius+thickness,90*deg,180*deg,0,360*deg);
// G4Tubs* solidEnv = new G4Tubs("Envelope",radius,radius+thickness,lunghezza_tubo,0*deg,360*deg);
G4Material* detector_material = nist->FindOrBuildMaterial("G4_Si");

G4LogicalVolume* Logic_detector = new G4LogicalVolume(solidEnv,detector_material,"Logic_detector");
G4RotationMatrix* rotationMatrix_shp = new G4RotationMatrix();
// rotationMatrix_shp->rotateY(90.*deg);
rotationMatrix_shp->rotateX(90.*deg);
rotationMatrix_shp->rotateZ(90.*deg);
new G4PVPlacement(rotationMatrix_shp,G4ThreeVector(0,0,-100.0*mm),Logic_detector,"crystal",logicWorld,false,0,checkOverlaps);
    
return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void B1DetectorConstruction::ConstructSDandField()
{
G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

G4MultiFunctionalDetector* cryst = new G4MultiFunctionalDetector("crystal");
G4SDManager::GetSDMpointer()->AddNewDetector(cryst);
G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");

G4SDParticleFilter* alphaFilter = new G4SDParticleFilter("alphaFilter");

bool gamma_source = true;
bool neutron_source = false;

if (gamma_source){
alphaFilter->add("gamma");
alphaFilter->add("e-");
}
if (neutron_source){
alphaFilter->add("alpha");
}

// primitiv1->SetFilter(alphaFilter);
cryst->RegisterPrimitive(primitiv1);

SetSensitiveDetector("Logic_detector",cryst);
}
