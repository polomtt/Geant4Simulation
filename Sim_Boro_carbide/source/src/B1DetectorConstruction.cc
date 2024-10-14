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
{ 
  numDetectors = 10;
}


B1DetectorConstruction::~B1DetectorConstruction()
{ }

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


  //   ____           _                       _                    
  //  / ___|__ _ _ __(_) ___ ___    _ __ ___ (_)___ _   _ _ __ ___ 
  // | |   / _` | '__| |/ __/ _ \  | '_ ` _ \| / __| | | | '__/ _ \
  // | |__| (_| | |  | | (_| (_) | | | | | | | \__ \ |_| | | |  __/
  //  \____\__,_|_|  |_|\___\___/  |_| |_| |_|_|___/\__,_|_|  \___|
                                                                
  //      _ _                     _ _   _            
  //   __| (_)___ _ __   ___  ___(_) |_(_)_   _____  
  //  / _` | / __| '_ \ / _ \/ __| | __| \ \ / / _ \ 
  // | (_| | \__ \ |_) | (_) \__ \ | |_| |\ V / (_) |
  //  \__,_|_|___/ .__/ \___/|___/_|\__|_| \_/ \___/ 
  //             |_|                                 


  double larghezza_Al     = -1e-6 * m * 0.5;
  double spessore_Al      = 1e-6 * m * 0.5;
  double free_space       = 1e-9 * m;
  double spessore_mat_att  = 1e-6 * m *0.5;

  // echo $spessore_mat_att $larghezza_alluminio $spessore_alluminio > dimensioni_sensore

  std::ifstream myfile;
  myfile.open("dimensioni_sensore");
  myfile>>spessore_mat_att;
  myfile>>larghezza_Al;
  myfile>>spessore_Al;
  myfile.close();

  larghezza_Al = ((float)larghezza_Al)*1e-6*m*0.5;
  spessore_mat_att = ((float)spessore_mat_att)*1e-6*m*0.5;
  spessore_Al = ((float)spessore_Al)*1e-6*m*0.5;

  //  ____  _                             _ _                 _       _       
  // | __ )| | ___   ___ ___ ___     __ _| | |_   _ _ __ ___ (_)_ __ (_) ___  
  // |  _ \| |/ _ \ / __/ __/ _ \   / _` | | | | | | '_ ` _ \| | '_ \| |/ _ \ 
  // | |_) | | (_) | (_| (_| (_) | | (_| | | | |_| | | | | | | | | | | | (_) |
  // |____/|_|\___/ \___\___\___/   \__,_|_|_|\__,_|_| |_| |_|_|_| |_|_|\___/ 
                                                                          

  G4Box* solidEnv = new G4Box("Envelope",larghezza_Al,larghezza_Al,spessore_Al);     
  G4Material* Al_material = nist->FindOrBuildMaterial("G4_Al");

  
  G4LogicalVolume* Al_detector_array[numDetectors];
  float pos_z_cubo_al = spessore_Al*-1;

  for (int i = 0; i < numDetectors; ++i) {
    Al_detector_array[i] = new G4LogicalVolume(solidEnv, Al_material, "Al_detector_" + std::to_string(i));
    new G4PVPlacement(0,G4ThreeVector(0,0,pos_z_cubo_al),Al_detector_array[i],"crystal_" + std::to_string(i),logicWorld,false,0,checkOverlaps);
    pos_z_cubo_al = pos_z_cubo_al - (2*spessore_Al + 2.*mm);
  }

  //  _                            _                     
  // | |    __ _ _   _  ___ _ __  | |__   ___  _ __ ___  
  // | |   / _` | | | |/ _ \ '__| | '_ \ / _ \| '__/ _ \ 
  // | |__| (_| | |_| |  __/ |    | |_) | (_) | | | (_) |
  // |_____\__,_|\__, |\___|_|    |_.__/ \___/|_|  \___/ 
  //             |___/                                  

  float percentuale_arrichimento_boro_10 = 90.;
  G4Box* strato_1u_sup_box = new G4Box("Envelope",larghezza_Al,larghezza_Al,spessore_mat_att); 

  G4Material* act_Mat;

  // Boro
  G4Isotope* iso_B10 = new G4Isotope("Boron_10",5,10,10*g/mole);
  G4Isotope* iso_B11 = new G4Isotope("Boron_11",5,11,11*g/mole);
  G4Element* elB     = new G4Element("Boron","B",2);
  elB->AddIsotope(iso_B10,percentuale_arrichimento_boro_10*perCent);
  elB->AddIsotope(iso_B11,(100.-percentuale_arrichimento_boro_10)*perCent);

  // Carbonio
  G4Material* el_C = nist->FindOrBuildMaterial("G4_C");

  // Carburo di boro
  float density_boro_carbide = 2.52 *g/cm3;
  act_Mat = new G4Material("act_Mat",density_boro_carbide,2);
  act_Mat->AddMaterial(el_C,20.*perCent);
  act_Mat->AddElement(elB,80.*perCent);
  // act_Mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4LogicalVolume* active_material_sup_array[numDetectors];
  float pos_z_layer_B = spessore_mat_att+free_space;
  G4VisAttributes* Color_B4C = new G4VisAttributes(G4Colour::Red());

  for (int i = 0; i < numDetectors; ++i) {
    active_material_sup_array[i] = new G4LogicalVolume(strato_1u_sup_box, act_Mat, "active_material_part_detector_superficie" + std::to_string(i));
    active_material_sup_array[i]->SetVisAttributes(Color_B4C);
    new G4PVPlacement(0,G4ThreeVector(0,0,pos_z_layer_B),active_material_sup_array[i],"active_material_sup" + std::to_string(i),logicWorld,false,0,checkOverlaps);
    pos_z_layer_B = pos_z_layer_B - (2*spessore_Al + 2.*mm);
  }  

  return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


void B1DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  //        _       _           
  //   __ _| |_ __ | |__   __ _ 
  //  / _` | | '_ \| '_ \ / _` |
  // | (_| | | |_) | | | | (_| |
  //  \__,_|_| .__/|_| |_|\__,_|
  //         |_|                

  G4MultiFunctionalDetector* alfa_detector[numDetectors];
  G4VPrimitiveScorer* primitiv_alfa[numDetectors]; 
  G4SDParticleFilter* alphaFilter[numDetectors]; 

  for (int i = 0; i < numDetectors; ++i) {
      primitiv_alfa[i]= new G4PSEnergyDeposit("edep");
      alfa_detector[i] = new G4MultiFunctionalDetector("alfa_detector_" + std::to_string(i));
      alphaFilter[i] = new G4SDParticleFilter("alphaFilter");
      alphaFilter[i]->add("alpha");
      primitiv_alfa[i]->SetFilter(alphaFilter[i]);
      G4SDManager::GetSDMpointer()->AddNewDetector(alfa_detector[i]);
      alfa_detector[i]->RegisterPrimitive(primitiv_alfa[i]);
      SetSensitiveDetector("Al_detector_"+ std::to_string(i), alfa_detector[i]);
  }

  //    _ _ _   _         _____ 
  // | (_) |_(_) ___   |___  |
  // | | | __| |/ _ \     / / 
  // | | | |_| | (_) |   / /  
  // |_|_|\__|_|\___/___/_/   
  //               |_____|    

  G4MultiFunctionalDetector* ion_detector[numDetectors];
  G4VPrimitiveScorer* primitiv_ion[numDetectors]; 
  G4SDParticleFilter* ionFilter[numDetectors]; 

  for (int i = 0; i < numDetectors; ++i) {
      primitiv_ion[i]= new G4PSEnergyDeposit("edep");
      ion_detector[i] = new G4MultiFunctionalDetector("ion_detector_" + std::to_string(i));
      ionFilter[i] = new G4SDParticleFilter("ionFilter");
      ionFilter[i] ->addIon(3,7);
      primitiv_ion[i]->SetFilter(ionFilter[i]);
      G4SDManager::GetSDMpointer()->AddNewDetector(ion_detector[i]);
      ion_detector[i]->RegisterPrimitive(primitiv_ion[i]);
      SetSensitiveDetector("Al_detector_"+ std::to_string(i), ion_detector[i]);
  }

}
