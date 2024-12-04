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

#include "G4SubtractionSolid.hh"

#include "G4VSDFilter.hh"
#include <G4SDParticleFilter.hh>

#include "G4SDManager.hh"

#include "G4MultiUnion.hh"
#include "G4RotationMatrix.hh"

#include <cmath>
#include <iostream>
#include <fstream>

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{  
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  G4bool checkOverlaps = true;

// __        __         _     _ 
// \ \      / /__  _ __| | __| |
//  \ \ /\ / / _ \| '__| |/ _` |
//   \ V  V / (_) | |  | | (_| |
//    \_/\_/ \___/|_|  |_|\__,_|
                             
  G4double world_sizeXY = 10.*cm;
  G4double world_sizeZ  = 10.*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Box* solidWorld = new G4Box("World",world_sizeXY,world_sizeXY,world_sizeZ);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld,world_mat,"World");              
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0,G4ThreeVector(),logicWorld,"World",0,false,0,checkOverlaps);
    
//  ____       _            _             
// |  _ \  ___| |_ ___  ___| |_ ___  _ __ 
// | | | |/ _ \ __/ _ \/ __| __/ _ \| '__|
// | |_| |  __/ ||  __/ (__| || (_) | |   
// |____/ \___|\__\___|\___|\__\___/|_|   
                                     
    double radius_hole        = 1e-6    * m;
    double pixel_size         = 10e-3   * m * 0.5;
    double lenght_silicon     = 275e-6  * m  * 0.5;
    double free_space_hole    = 5e-9    * m;
    double distance_from_hole = 5e-6    * m;
    double depth_hole         = 25e-6   * m  * 0.5;

    float spes_strat_sup_b10 = 1e-6 * m *0.5;
    float spes_strat_sup_li6 = 1e-6 * m *0.5;

    std::ifstream myfile;
    myfile.open("dimension_act_mat");
    myfile>>spes_strat_sup_b10;
    myfile>>spes_strat_sup_li6;
    myfile.close();

    spes_strat_sup_b10 = ((float)spes_strat_sup_b10)*1e-6*m*0.5;
    spes_strat_sup_li6 = ((float)spes_strat_sup_li6)*1e-6*m*0.5;

    //matrice silicio
    G4Box* solidEnv = new G4Box("Envelope",pixel_size,pixel_size,lenght_silicon); //its size
    G4Material* Si_mat = nist->FindOrBuildMaterial("G4_Si");
    G4LogicalVolume* Si_detector = new G4LogicalVolume(solidEnv,Si_mat,"Si_detector");
    G4VisAttributes* Color_Si = new G4VisAttributes(G4Colour::Blue());
    Si_detector->SetVisAttributes(Color_Si);
    new G4PVPlacement(0,G4ThreeVector(0,0,0),Si_detector,"crystal",logicWorld,false,0,checkOverlaps);          //overlaps checking

    if(spes_strat_sup_b10>0){
    // layer ghe-sboro-10
    G4Box* strato_sup_box_b10 = new G4Box("box_b10",pixel_size,pixel_size,spes_strat_sup_b10); //its size
    G4Isotope* iso_B10 = new G4Isotope("Boron_10",5,10,10*g/mole);
    G4Isotope* iso_B11 = new G4Isotope("Boron_11",5,11,11*g/mole);
    G4Element* elB  = new G4Element("Boron","B",2);
    elB->AddIsotope(iso_B10,99.*perCent);
    elB->AddIsotope(iso_B11,1.*perCent);
    float density_b10 = 2.46 *g/cm3;
    G4Material* act_Mat_b10 = new G4Material("act_Mat",density_b10,1);
    act_Mat_b10->AddElement(elB,100.*perCent);

    G4LogicalVolume* active_material_sup_b10 = new G4LogicalVolume(strato_sup_box_b10,act_Mat_b10,"active_material_part_detector_superficie_b10");
    G4VisAttributes* Color_b10 = new G4VisAttributes(G4Colour::Red());
    active_material_sup_b10->SetVisAttributes(Color_b10);
    new G4PVPlacement(0,G4ThreeVector(0,0,lenght_silicon+free_space_hole+spes_strat_sup_b10),active_material_sup_b10,"active_material_sup_b10",logicWorld,false,0,checkOverlaps);
    }

    if(spes_strat_sup_li6>0){
    // layer litio-6
    G4Box* strato_sup_box_li6 = new G4Box("box_li6",pixel_size,pixel_size,spes_strat_sup_li6); //its size

    G4Material* Fluorine = nist->FindOrBuildMaterial("G4_F");
    G4Isotope* iso_Li6 = new G4Isotope("Litium_6",3,6,6.01512*g/mole);
    G4Isotope* iso_Li7 = new G4Isotope("Litium_7",3,7,7.01600*g/mole);
    G4Element* elLi  = new G4Element("Litium","Li",2);
    elLi->AddIsotope(iso_Li6,95.*perCent);
    elLi->AddIsotope(iso_Li7,5.*perCent);
    float density_li6 = 2.54 *g/cm3;
    G4Material* act_Mat_li6 = new G4Material("act_Mat",density_li6,2);
    act_Mat_li6->AddMaterial(Fluorine,50.*perCent);
    act_Mat_li6->AddElement(elLi,50.*perCent);
    
    G4LogicalVolume* active_material_sup_li6 = new G4LogicalVolume(strato_sup_box_li6,act_Mat_li6,"active_material_part_detector_superficie_li6");
    
    G4VisAttributes* Color_Li = new G4VisAttributes(G4Colour::Green());
    active_material_sup_li6->SetVisAttributes(Color_Li);
    new G4PVPlacement(0,G4ThreeVector(0,0,lenght_silicon+free_space_hole+spes_strat_sup_li6+spes_strat_sup_b10*2),active_material_sup_li6,"active_material_sup",logicWorld,false,0,checkOverlaps);
    }

    return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void B1DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);
  
  // declare crystal as a MultiFunctionalDetector scorer
  //  
    G4MultiFunctionalDetector* cryst = new G4MultiFunctionalDetector("crystal");
    G4SDManager::GetSDMpointer()->AddNewDetector(cryst);
    G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");

    G4SDParticleFilter* alphaFilter = new G4SDParticleFilter("alphaFilter");

    bool gamma_source = true;
    bool neutron_source = true;

    if (gamma_source){
    alphaFilter->add("gamma");
    alphaFilter->add("e-");   
    }
    if (neutron_source){
    alphaFilter->add("alpha");
    }
        
    // primitiv1->SetFilter(alphaFilter);
    cryst->RegisterPrimitive(primitiv1);

    SetSensitiveDetector("Si_detector",cryst);
}
