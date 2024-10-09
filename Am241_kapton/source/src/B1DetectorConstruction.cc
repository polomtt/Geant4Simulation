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

//  __  __       _            _       _ _ 
// |  \/  | __ _| |_ ___ _ __(_) __ _| (_)
// | |\/| |/ _` | __/ _ \ '__| |/ _` | | |
// | |  | | (_| | ||  __/ |  | | (_| | | |
// |_|  |_|\__,_|\__\___|_|  |_|\__,_|_|_|
                                       
    // if(act_material_choice == 2)
    // {
    // G4Isotope* iso_B10 = new G4Isotope("Boron_10",5,10,10*g/mole);
    // G4Isotope* iso_B11 = new G4Isotope("Boron_11",5,11,11*g/mole);
    // G4Element* elB  = new G4Element("Boron","B",2);
    // elB->AddIsotope(iso_B10,99.*perCent);
    // elB->AddIsotope(iso_B11,1.*perCent);
    // float density = 2.46 *g/cm3;
    // act_Mat = new G4Material("act_Mat",density,1);
    // //act_Mat->AddMaterial(Fluorine,50.*perCent);
    // act_Mat->AddElement(elB,100.*perCent);
    // }
    // else if(act_material_choice==1)
    // {
    // G4Isotope* iso_Li6 = new G4Isotope("Litium_6",3,6,6.01512*g/mole);
    // G4Isotope* iso_Li7 = new G4Isotope("Litium_7",3,7,7.01600*g/mole);
    // G4Element* elLi  = new G4Element("Litium","Li",2);
    // elLi->AddIsotope(iso_Li6,95.*perCent);
    // elLi->AddIsotope(iso_Li7,5.*perCent);
    // float density = 2.54 *g/cm3;
    // act_Mat = new G4Material("act_Mat",density,2);
    // act_Mat->AddMaterial(Fluorine,50.*perCent);
    // act_Mat->AddElement(elLi,50.*perCent);
    // }

/***************************/
// Silicio
/***************************/

double xx = 10. * mm *0.5;
double yy = 10. * mm *0.5;
double zz = 25. * um *0.5;

G4Box* solid_kapton = new G4Box("kapton",xx,yy,zz);
G4Material* kapton_material = nist->FindOrBuildMaterial("G4_KAPTON");
G4LogicalVolume* Logic_kapton = new G4LogicalVolume(solid_kapton,kapton_material,"Logic_kapton");
G4VisAttributes* Color_Si = new G4VisAttributes(G4Colour::Red());
Logic_kapton->SetVisAttributes(Color_Si);
new G4PVPlacement(0,G4ThreeVector(0,0,1.*mm),Logic_kapton,"crystal2",logicWorld,false,0,checkOverlaps);

/***************************/
// Detector
/***************************/

xx = 5. * mm *0.5;
yy = 5. * mm *0.5;
zz = 10 * um *0.5;

G4Box* solidEnv = new G4Box("silicon_detector",xx,yy,zz);
G4Material* detector_material = nist->FindOrBuildMaterial("G4_Si");

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
G4VPrimitiveScorer* primitiv = new G4PSEnergyDeposit("edep");
cryst->RegisterPrimitive(primitiv);
SetSensitiveDetector("Logic_detector",cryst);

G4MultiFunctionalDetector* cryst2 = new G4MultiFunctionalDetector("crystal2");
G4SDManager::GetSDMpointer()->AddNewDetector(cryst2);
G4VPrimitiveScorer* primitiv2 = new G4PSEnergyDeposit("edep");
cryst2->RegisterPrimitive(primitiv2);
SetSensitiveDetector("Logic_kapton",cryst2);

}
