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

    
/***************************/    
// Detector 
/***************************/   

    //buco silicio
    int act_material_choice=3;
    double radius_hole_B  =  5e-6* m;
    double radius_hole_Li = 10e-6* m;
    
    double depth_hole =  25e-6* m * 0.5;
    double pixel_size =  200e-6     * m * 0.5;
    double lenght_silicon = 275e-6 * m * 0.5;
    double free_space_hole = 5e-9  * m;
    double distance_from_hole = 5e-6  * m;


    std::ifstream myfile;
    myfile.open("hole_dimension");
    myfile>>distance_from_hole;
    myfile>>radius_hole_B;
    myfile>>radius_hole_Li;
    myfile>>pixel_size;
    myfile.close();

    distance_from_hole = ((float)distance_from_hole)*1e-6*m;
    radius_hole_B = ((float)radius_hole_B )*1e-6*m;
    radius_hole_Li= ((float)radius_hole_Li)*1e-6*m;
    pixel_size = ((float)pixel_size)*1e-6*m*0.5;

    std::cout<<"Rad_hole_B:"<<radius_hole_B<<std::endl;
    std::cout<<"Rad_hole_Li:"<<radius_hole_Li<<std::endl;
    
    double radius_hole = radius_hole_B + radius_hole_Li;

    //matrice silicio
    G4Box* solidEnv = new G4Box("Envelope",pixel_size,pixel_size,lenght_silicon); //its size
    
    float spes_strat_sup = 1e-6 * m *0.5;
    G4Box* strato_1u_sup_box = new G4Box("Envelope",pixel_size,pixel_size,spes_strat_sup); //its size
    
    G4MultiUnion* munion_hole = new G4MultiUnion("UnitedHole");
    G4MultiUnion* munion_B10 = new G4MultiUnion("UnitedHole_B10");
    G4MultiUnion* munion_Li6 = new G4MultiUnion("UnitedHole_Li6");

    float nHole_y = 2*pixel_size/(distance_from_hole+radius_hole*2);
    float nHole_x = 2*pixel_size/(distance_from_hole+radius_hole*2);

    float xx = radius_hole + distance_from_hole - pixel_size;
    float yy = radius_hole + distance_from_hole - pixel_size;
    
    std::cout<<nHole_x<<" "<<nHole_y<<std::endl;
    std::cout<<xx<<" "<<yy<<std::endl;

    for( int i=1 ; i < (nHole_x) ; i++)
    {
      for( int j=1 ; j < (nHole_y) ; j++)
      { 
        G4Tubs* detector_hole =  new G4Tubs("hole",0,radius_hole+free_space_hole,depth_hole+free_space_hole,0*deg,360*deg);
        
        G4RotationMatrix rot = G4RotationMatrix(0, 0, 0);
        G4ThreeVector pos_fori = G4ThreeVector(xx,yy,0);
        G4Transform3D tr = G4Transform3D(rot, pos_fori);

        if(radius_hole_Li>0){
          G4Tubs* hole_Li6 =  new G4Tubs("hole_li6",0,radius_hole_Li-free_space_hole,depth_hole,0*deg,360*deg);
          munion_Li6 -> AddNode(*hole_Li6, tr );
        }
        
        if(radius_hole_B>0){
          G4Tubs* hole_B10 =  new G4Tubs("hole_B10",radius_hole_Li,radius_hole,depth_hole,0*deg,360*deg);
          munion_B10 -> AddNode(*hole_B10, tr );
        }

        munion_hole -> AddNode( *detector_hole, tr );
             

        yy = yy + 2 * radius_hole + distance_from_hole;
      }

      xx = xx + 2 * radius_hole + distance_from_hole;
      yy = radius_hole + distance_from_hole - pixel_size;
    }

    munion_hole -> Voxelize();
    if(radius_hole_Li>0)
      munion_Li6 -> Voxelize();
    if(radius_hole_B>0)
      munion_B10 -> Voxelize();
    float zz = lenght_silicon-free_space_hole-depth_hole;

    //  ____  _ _ _      _       
    // / ___|(_) (_) ___(_) ___  
    // \___ \| | | |/ __| |/ _ \ 
    //  ___) | | | | (__| | (_) |
    // |____/|_|_|_|\___|_|\___/ 
                          
    G4VSolid* subtract = new G4SubtractionSolid("Envelope_hole", solidEnv, munion_hole, 0, G4ThreeVector(0.*mm,0.*mm,zz));
    G4Material* Si_mat = nist->FindOrBuildMaterial("G4_Si");
    G4LogicalVolume* Si_detector = new G4LogicalVolume(subtract,Si_mat,"Si_detector");
    G4VisAttributes* Color_Si = new G4VisAttributes(G4Colour::Blue());
    Si_detector->SetVisAttributes(Color_Si);
    new G4PVPlacement(0,G4ThreeVector(0,0,0),Si_detector,"crystal",logicWorld,false,0,checkOverlaps);
    
    //  ____                        _  ___  
    // | __ )  ___  _ __ ___       / |/ _ \ 
    // |  _ \ / _ \| '__/ _ \ _____| | | | |
    // | |_) | (_) | | | (_) |_____| | |_| |
    // |____/ \___/|_|  \___/      |_|\___/ 
                                        
    G4Material* act_Mat_b10;

    G4Isotope* iso_B10 = new G4Isotope("Boron_10",5,10,10*g/mole);
    G4Isotope* iso_B11 = new G4Isotope("Boron_11",5,11,11*g/mole);
    G4Element* elB  = new G4Element("Boron","B",2);
    elB->AddIsotope(iso_B10,99.*perCent);
    elB->AddIsotope(iso_B11,1.*perCent);
    float density_b10 = 2.46 *g/cm3;
    act_Mat_b10 = new G4Material("act_Mat_li6",density_b10,1);
    act_Mat_b10->AddElement(elB,100.*perCent);

    if(radius_hole_B>0){
      G4LogicalVolume* active_material_b10 = new G4LogicalVolume(munion_B10,act_Mat_b10,"active_material_part_detector_b10");      
      G4VisAttributes* Color_b10 = new G4VisAttributes(G4Colour::Green());
      active_material_b10->SetVisAttributes(Color_b10);
      new G4PVPlacement(0,G4ThreeVector(0,0,zz),active_material_b10,"active_material",logicWorld,false,0,checkOverlaps);
    }

    //  _     _ _   _              __   
    // | |   (_) |_(_) ___        / /_  
    // | |   | | __| |/ _ \ _____| '_ \ 
    // | |___| | |_| | (_) |_____| (_) |
    // |_____|_|\__|_|\___/       \___/ 
                                 
    G4Material* act_Mat_li6;

    G4Material* Fluorine = nist->FindOrBuildMaterial("G4_F");
    G4Isotope* iso_Li6 = new G4Isotope("Litium_6",3,6,6.01512*g/mole);
    G4Isotope* iso_Li7 = new G4Isotope("Litium_7",3,7,7.01600*g/mole);
    G4Element* elLi  = new G4Element("Litium","Li",2);
    elLi->AddIsotope(iso_Li6,95.*perCent);
    elLi->AddIsotope(iso_Li7,5.*perCent);
    float density_li6 = 2.54 *g/cm3;
    act_Mat_li6 = new G4Material("act_Mat",density_li6,2);
    act_Mat_li6->AddMaterial(Fluorine,50.*perCent);
    act_Mat_li6->AddElement(elLi,50.*perCent);

    if(radius_hole_Li>0){
      G4LogicalVolume* active_material_li6 = new G4LogicalVolume(munion_Li6,act_Mat_li6,"active_material_part_detector_li6");      
      G4VisAttributes* Color_Li = new G4VisAttributes(G4Colour::Red());
      active_material_li6->SetVisAttributes(Color_Li);
      new G4PVPlacement(0,G4ThreeVector(0,0,zz),active_material_li6,"active_material",logicWorld,false,0,checkOverlaps);
    }

    //  ____  _             _                          
    // / ___|| |_ _ __ __ _| |_ ___    ___ _   _ _ __  
    // \___ \| __| '__/ _` | __/ _ \  / __| | | | '_ \ 
    //  ___) | |_| | | (_| | || (_) | \__ \ |_| | |_) |
    // |____/ \__|_|  \__,_|\__\___/  |___/\__,_| .__/ 
    //                                          |_|    

    G4LogicalVolume* active_material_sup = new G4LogicalVolume(strato_1u_sup_box,act_Mat_b10,"active_material_part_detector_superficie");
    new G4PVPlacement(0,G4ThreeVector(0,0,lenght_silicon+free_space_hole+spes_strat_sup),active_material_sup,"active_material_sup",logicWorld,false,0,checkOverlaps);

    return physWorld;
}

void B1DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

    G4MultiFunctionalDetector* cryst = new G4MultiFunctionalDetector("crystal");
    G4SDManager::GetSDMpointer()->AddNewDetector(cryst);
    G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");

    G4SDParticleFilter* alphaFilter = new G4SDParticleFilter("alphaFilter");

    bool gamma_source = false;
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
