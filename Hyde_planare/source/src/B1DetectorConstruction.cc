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
// #include "B1Scintillator.hh"
// #include "B1ScintillatorBD.cc"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

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
  

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //     
  // World
  //
  G4double world_sizeXY = 10.*cm;
  G4double world_sizeZ  = 10.*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
  
  G4Box* solidWorld = new G4Box("World",world_sizeXY,world_sizeXY,world_sizeZ);
      
  G4LogicalVolume* logicWorld =                         
    new G4LogicalVolume(solidWorld,          //its solid
                        world_mat,           //its material
                        "World");            //its name
                                   
  G4VPhysicalVolume* physWorld = 
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

// /***************************/
// // Detector Al case
// /***************************/
//
//     double radius_case = 1.0;
//     double lenght_case = 2.0;
//     double inch_to_cm = 2.45;
//
//     G4Tubs* detector_case =  new G4Tubs("Envelope",0.*cm,radius_case*inch_to_cm*cm+0.1*cm,0.5*lenght_case*inch_to_cm*cm+0.025*cm,0*deg,360*deg); //its size
// //     G4Material* Al_material = nist->FindOrBuildMaterial("G4_Al");
//      G4Material* Al_material = nist->FindOrBuildMaterial("G4_AIR");

    
/***************************/    
// Detector 
/***************************/   

    //buco silicio

    double radius_hole = 15e-6* m;
    double depth_hole =  25e-6* m * 0.5;
    double pixel_size =  1000e-6     * m * 0.5;
    double lenght_silicon = 275e-6 * m * 0.5;
    double free_space_hole = 5e-9  * m;
    double distance_from_hole = 5e-6  * m;

    // 1-> LiF 2-> B10 3->Vacuum
    int act_material_choice = 3;
    float spes_strat_sup = 1e-6 * m *0.5;
    std::ifstream myfile;
    myfile.open("hole_dimension");
    myfile>>spes_strat_sup;
    myfile>>pixel_size;
    myfile>>act_material_choice;
    myfile.close();

    pixel_size = ((float)pixel_size)*1e-6*m*0.5;
    spes_strat_sup = ((float)spes_strat_sup)*1e-6*m*0.5;

    //matrice silicio
    G4Box* solidEnv = new G4Box("Envelope",pixel_size,pixel_size,lenght_silicon); //its size
    
    
    G4Box* strato_1u_sup_box = new G4Box("Envelope",pixel_size,pixel_size,spes_strat_sup); //its size

    G4Material* Si_mat = nist->FindOrBuildMaterial("G4_Si");

    G4Material* Fluorine = nist->FindOrBuildMaterial("G4_F");

    // Li6 isotope

    G4Material* act_Mat;

    if(act_material_choice == 2)
    {
    G4Isotope* iso_B10 = new G4Isotope("Boron_10",5,10,10*g/mole);
    G4Isotope* iso_B11 = new G4Isotope("Boron_11",5,11,11*g/mole);
    G4Element* elB  = new G4Element("Boron","B",2);
    elB->AddIsotope(iso_B10,99.*perCent);
    elB->AddIsotope(iso_B11,1.*perCent);
    float density = 2.46 *g/cm3;
    act_Mat = new G4Material("act_Mat",density,1);
    // act_Mat->AddMaterial(Fluorine,50.*perCent);
    act_Mat->AddElement(elB,100.*perCent);
    }
    else if(act_material_choice==1)
    {
    G4Isotope* iso_Li6 = new G4Isotope("Litium_6",3,6,6.01512*g/mole);
    G4Isotope* iso_Li7 = new G4Isotope("Litium_7",3,7,7.01600*g/mole);
    G4Element* elLi  = new G4Element("Litium","Li",2);
    elLi->AddIsotope(iso_Li6,95.*perCent);
    elLi->AddIsotope(iso_Li7,5.*perCent);
    float density = 2.54 *g/cm3;
    act_Mat = new G4Material("act_Mat",density,2);
    act_Mat->AddMaterial(Fluorine,50.*perCent);
    act_Mat->AddElement(elLi,50.*perCent);
    }
    else if(act_material_choice==3){
      act_Mat = nist->FindOrBuildMaterial("G4_Galactic");
    }


    G4LogicalVolume* NaI_L_detector = new G4LogicalVolume(solidEnv,Si_mat,"NaI_L_detector");

    G4VisAttributes* Color_Si = new G4VisAttributes(G4Colour::Blue());
    NaI_L_detector->SetVisAttributes(Color_Si);

    new G4PVPlacement(0,G4ThreeVector(0,0,0),NaI_L_detector,"crystal",logicWorld,false,0,checkOverlaps);          //overlaps checking
    
    G4LogicalVolume* active_material_sup = new G4LogicalVolume(strato_1u_sup_box,act_Mat,"active_material_part_detector_superficie");

    G4VisAttributes* Color_Li = new G4VisAttributes(G4Colour::Red());
    active_material_sup->SetVisAttributes(Color_Li);

    new G4PVPlacement(0,G4ThreeVector(0,0,lenght_silicon+free_space_hole+spes_strat_sup),active_material_sup,"active_material_sup",logicWorld,false,0,checkOverlaps);

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

    SetSensitiveDetector("NaI_L_detector",cryst);
}
