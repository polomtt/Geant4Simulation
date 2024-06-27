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
/// \file B1DetectorConstruction.cc
/// \brief Implementation of the B1DetectorConstruction class

#include "B1DetectorConstruction.hh"
//#include "B1SteppingAction.hh"
   // use of stepping action to set the accounting volume

#include "G4RunManager.hh"
#include "G4Transform3D.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
//#include "G4Sphere.hh"
//#include "G4Orb.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
//#include "G4Isotope.hh"
#include "G4Element.hh"
#include "G4Material.hh"
#include "G4UnitsTable.hh"
#include "G4SubtractionSolid.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
//#include "G4PSSphereSurfaceCurrent.hh"
//#include "G4SDParticleWithEnergyFilter.hh"

#include "G4VisAttributes.hh"

#include "G4VSDFilter.hh"
#include <G4SDParticleFilter.hh>

//#include "G4SDParticleFilter.hh"

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

	// materials ====================================================
/*
	G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");
	G4Material* tank_mat = nist->FindOrBuildMaterial("G4_STAINLESS-STEEL");
    	G4Material* tank_water = nist->FindOrBuildMaterial("G4_WATER");
	G4Material* al_mat = nist->FindOrBuildMaterial("G4_Al");
*/
	G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");
        G4Material* tank_mat = nist->FindOrBuildMaterial("G4_Galactic");
        G4Material* tank_water = nist->FindOrBuildMaterial("G4_Galactic");
        G4Material* al_mat = nist->FindOrBuildMaterial("G4_Galactic");


	G4String name, symbol;
	G4int ncomponents, natoms, n;
	G4double a, z;
	G4double abundance; //fractionmass

	a = 1.00794*g/mole;
	G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);

	a = 12.0107*g/mole;
	G4Element* elC  = new G4Element(name="Carbon"  ,symbol="C" , z= 6., a);

	G4double density_stilbene = 1.15*g/cm3;
	
	G4Material* stilbene_mat = new G4Material(name="stilbene_mat",density_stilbene,ncomponents=2);
	stilbene_mat->AddElement(elH, natoms=12);
	stilbene_mat->AddElement(elC, natoms=14);

	//NaIL material

	G4Material* el_Na = nist->FindOrBuildMaterial("G4_Na");
	G4Material* el_Tl = nist->FindOrBuildMaterial("G4_Tl");
	G4Material* el_I = nist->FindOrBuildMaterial("G4_I");

	// define an Element from isotopes, by relative abundance
	G4Isotope* Li6 = new G4Isotope(name="Li6", z=3, n=6, a=6.015*g/mole);
	G4Isotope* Li7 = new G4Isotope(name="Li7", z=3, n=7, a=7.016*g/mole);

	G4Element* el_Li  = new G4Element(name="enriched Lithium", symbol="Li", ncomponents=2);
	el_Li->AddIsotope(Li6, abundance = 95.*perCent);
	el_Li->AddIsotope(Li7, abundance =  5.*perCent);

	G4double density_nail = 3.66*g/cm3;
	
	G4Material* nail_mat = new G4Material(name="nail_mat",density_nail,ncomponents=4);
	nail_mat->AddMaterial(el_Na, 15.2706*perCent);
	nail_mat->AddMaterial(el_I,  84.2944*perCent);
	nail_mat->AddMaterial(el_Tl,  0.3534*perCent);
	nail_mat->AddElement(el_Li,  0.0816*perCent);

	a = 140.116*g/mole;
	G4Element* elCe  = new G4Element(name="Cerium", symbol="Ce" , z= 58., a);

	a = 79.94*g/mole;
	G4Element* elBr  = new G4Element(name="Bromine",symbol="Br" , z= 35., a);

	G4double density_cebr = 5.1*g/cm3;
	
	G4Material* cebr_mat = new G4Material(name="cebr_mat",density_cebr,ncomponents=2);
	cebr_mat->AddElement(elCe, natoms=1);
	cebr_mat->AddElement(elBr, natoms=3);

	// UGV material

	G4Material* ugv_mat = nist->FindOrBuildMaterial("G4_POLYVINYL_CHLORIDE");

	G4Material* ground_mat = nist->FindOrBuildMaterial("G4_CONCRETE");

	// Option to switch on/off checking of volumes overlaps

	G4bool checkOverlaps = true;

	//     
	// World
	//
	G4double world_sizeXY = (25./2.)*m;
	G4double world_sizeZ  = (20./2.)*m;

	G4Box* solidWorld =    
		new G4Box("World",								//its name
		world_sizeXY, world_sizeXY, world_sizeZ);	//its size

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

    // UGV model

    G4double ugv_sizeX = (650./2.)*mm;
    G4double ugv_sizeY = (350./2.)*mm;
    G4double ugv_sizeZ = (1040./2.)*mm;

    G4Box* ugv_solid_a =    
    new G4Box("UGV_solid_a",					//its name
    ugv_sizeX, ugv_sizeY, ugv_sizeZ);	//its size

    ugv_sizeX = (640./2.)*mm;
    ugv_sizeY = (340./2.)*mm;
    ugv_sizeZ = (1030./2.)*mm;

    G4Box* ugv_solid_b =    
    new G4Box("UGV_solid_b",					//its name
    ugv_sizeX, ugv_sizeY, ugv_sizeZ);	//its size

    G4VSolid* ugv_solid
    = new G4SubtractionSolid("UGV_solid_a-UGV_solid_b", ugv_solid_a, ugv_solid_b,
        0, G4ThreeVector(0.,0.,0.));

    G4LogicalVolume* ugv_log =                         
    new G4LogicalVolume(ugv_solid,
    ugv_mat,
    "UGV_log");            //its name
	
	
	G4double pos_x_detectors = -1100*mm;
	G4double pos_y_detectors = 600*mm;
	G4double pos_z_detectors = 500*mm;
	
	std::ifstream fin("pos_detector");
	
	while (fin>>pos_x_detectors){
    fin>>pos_y_detectors;
    fin>>pos_z_detectors;
	};
	
	/*G4double pos_x_detectors = pos_x_ugv+10.0*cm;
	G4double pos_y_detectors = pos_y_ugv+(25.0)*cm;
	G4double pos_z_detectors = pos_z_ugv+(50.0)*cm;
	*/
	
    G4double pos_x_ugv = pos_x_detectors - 10.0*cm ;
    G4double pos_y_ugv = pos_y_detectors - 25.0*cm;
    G4double pos_z_ugv = pos_z_detectors - (50.0)*cm;
    
    new G4PVPlacement(0,                     //no rotation
    G4ThreeVector(pos_x_ugv, pos_y_ugv,pos_z_ugv),       //at (0,0,0)
    ugv_log,            //its logical volume
    "UGV",               //its name
    logicWorld,                     //its mother  volume
    false,                 //no boolean operation
    0,                     //copy number
    checkOverlaps);        //overlaps checking
    
    G4Tubs* smile = new G4Tubs("smile_solid", 0.0*mm,10.*cm,1.*cm,180.0*deg,180.*deg);
    G4LogicalVolume* ugv_smile_log = new G4LogicalVolume(smile,world_mat,"UGV_smile_log");
    new G4PVPlacement(0,G4ThreeVector(-1000.0*mm, pos_y_ugv, 600.0*mm),ugv_smile_log,"UGV",logicWorld,false,0,checkOverlaps);
    
	// Stilbene 2"x2", aluminium cover 2mm

	G4double radio_al_stilb = (54.8/2.0)*mm;
	G4double half_z_al_stilb = (52.8/2.0)*mm;
	G4double min_phi = 0.0*deg;
	G4double max_phi = 360.0*deg;

	G4Tubs* al_stilb_solid =
	  new G4Tubs("Al_stilb_solid", 0.0*mm, radio_al_stilb, half_z_al_stilb, min_phi, max_phi);

	G4LogicalVolume* al_stilb_log =
    new G4LogicalVolume(al_stilb_solid, al_mat, "Al_stilb_log");



    
    std::cout<<pos_x_detectors<<"__"<<pos_y_detectors<<"__"<<pos_z_detectors<<std::endl;
    
	new G4PVPlacement(0,                      // no rotation
		G4ThreeVector(pos_x_detectors+00.*cm,pos_y_detectors,pos_z_detectors),  // translation position
		al_stilb_log,                // its logical volume
		"Al_stilb",                    // its name
		logicWorld,                    // its mother (logical) volume
		false,                         // no boolean operations
		0,                            // its copy number
		checkOverlaps);               //overlaps checking


	// stilbene crystal 2"x2"

	G4double radio_stilb = (50.8/2.0)*mm;
	G4double half_z_stilb = radio_stilb;

	G4Tubs* stilb_solid =
	  new G4Tubs("Stilb_solid", 0.0*mm, radio_stilb, half_z_stilb, min_phi, max_phi);

	G4LogicalVolume* stilb_log =
	  new G4LogicalVolume(stilb_solid, stilbene_mat, "Stilb_log");

	  fScoringVolume = stilb_log;

	new G4PVPlacement(0,                      // no rotation
		G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm),  // translation position
		stilb_log,                // its logical volume
		"Stilb_detector",                    // its name
		al_stilb_log,                    // its mother (logical) volume
		false,                         // no boolean operations
		0,                            // its copy number
		checkOverlaps);               //overlaps checking

    G4VisAttributes* Color_stil = new G4VisAttributes(G4Colour::Blue());  
    al_stilb_log->SetVisAttributes(Color_stil);
 
    // CeBr3 1.5"x1.5" aluminium cover 2mm

	G4double radio_al_CeBr = (42.1/2.0)*mm;
	G4double half_z_al_CeBr = (40.1/2.0)*mm;

	G4Tubs* al_CeBr_solid =
	  new G4Tubs("Al_CeBr_solid", 0.0*mm, radio_al_CeBr, half_z_al_CeBr, min_phi, max_phi);

	G4LogicalVolume* al_CeBr_log =
	  new G4LogicalVolume(al_CeBr_solid, al_mat, "Al_CeBr_log");

	new G4PVPlacement(0,                      // no rotation
		G4ThreeVector(pos_x_detectors+20.0*cm, pos_y_detectors, pos_z_detectors),  // translation position
		al_CeBr_log,                // its logical volume
		"Al_CeBr",                    // its name
		logicWorld,                    // its mother (logical) volume
		false,                         // no boolean operations
		0,                            // its copy number
		checkOverlaps);               //overlaps checking
	std::cout<<"Centro_CeBr "<<pos_x_detectors<<"  "<<pos_y_detectors<<" "<<pos_z_detectors<<std::endl;

 // CeBr3 1.5"x1.5" crystal 

	G4double radio_CeBr = (38.1/2.0)*mm;
	G4double half_z_CeBr = radio_CeBr;

	G4Tubs* cebr_solid =
	  new G4Tubs("CeBr_solid", 0.0*mm, radio_CeBr, half_z_CeBr, min_phi, max_phi);

	G4LogicalVolume* cebr_log =
	  new G4LogicalVolume(cebr_solid, cebr_mat, "CeBr_log");

	new G4PVPlacement(0,                      // no rotation
		G4ThreeVector(0.0*cm, 0.0*cm, 0.0*cm),  // translation position
		cebr_log,                // its logical volume
		"CeBr_detector",                    // its name
		al_CeBr_log,                    // its mother (logical) volume
		false,                         // no boolean operations
		0,                            // its copy number
		checkOverlaps);               //overlaps checking

    
    G4VisAttributes* Color_cebr = new G4VisAttributes(G4Colour::Green());  
    al_CeBr_log->SetVisAttributes(Color_cebr);
    

    
    G4VisAttributes* Color_Poly = new G4VisAttributes(G4Colour::Magenta());  
    ugv_log->SetVisAttributes(Color_Poly);

	// ground

    G4double ground_sizeX = (19./2.)*m;
    G4double ground_sizeY = (0.65/2.)*m;
    G4double ground_sizeZ = ((11.2)/2.)*m;
    
    G4double roof_sizeY = (1.2/2.)*m;
    
    G4double wall_sizeY = (5.5/2.)*m;
    
    G4Box* ground_solid = new G4Box("Ground_solid",ground_sizeX, ground_sizeY, ground_sizeZ);
    G4LogicalVolume* ground_log = new G4LogicalVolume(ground_solid,ground_mat,"Ground_log");
    new G4PVPlacement(0,G4ThreeVector(0.0*mm, -ground_sizeY, 0.0*mm),ground_log,"Ground",logicWorld,false,0,checkOverlaps);
    
    // wall_1
    
    G4Box* wall_1 = new G4Box("wall_1",0.2*m,wall_sizeY,ground_sizeZ);
    G4LogicalVolume* wall_1_log = new G4LogicalVolume(wall_1,ground_mat,"wall_1_log");
     new G4PVPlacement(0,G4ThreeVector(ground_sizeX-0.2*m,wall_sizeY, 0.0*m),wall_1_log,"wall_1",logicWorld,false,0,checkOverlaps);
    
    G4Box* wall_2 = new G4Box("wall_2",0.6*m,wall_sizeY,ground_sizeZ);    
    G4LogicalVolume* wall_2_log = new G4LogicalVolume(wall_2,ground_mat,"wall_2_log");
    new G4PVPlacement(0,G4ThreeVector(0.5*m,wall_sizeY, 0.0*m),wall_2_log,"wall_2",logicWorld,false,0,checkOverlaps);
    
    G4Box* wall_3 = new G4Box("wall_3",0.2*m,wall_sizeY,ground_sizeZ);    
    G4LogicalVolume* wall_3_log = new G4LogicalVolume(wall_3,ground_mat,"wall_3_log");
    new G4PVPlacement(0,G4ThreeVector(-ground_sizeX+0.2*m,wall_sizeY, 0.0*m),wall_3_log,"wall_3",logicWorld,false,0,checkOverlaps);
    
    G4Box* m_wall_1 = new G4Box("m_wall_1",5.0*cm,30.0*cm,ground_sizeZ);    
    G4LogicalVolume* m_wall_1_log = new G4LogicalVolume(m_wall_1,ground_mat,"m_wall_1_log");
    new G4PVPlacement(0,G4ThreeVector(-5.2*m+0.2*m,30.0*cm, 0.0*m),m_wall_1_log,"m_wall_1",logicWorld,false,0,checkOverlaps);
    
    G4Box* m_wall_2 = new G4Box("m_wall_2",5.0*cm,30.0*cm,ground_sizeZ);    
    G4LogicalVolume* m_wall_2_log = new G4LogicalVolume(m_wall_2,ground_mat,"m_wall_2_log");
    new G4PVPlacement(0,G4ThreeVector(4.7*m+0.2*m,30.0*cm, 0.0*m),m_wall_2_log,"m_wall_2",logicWorld,false,0,checkOverlaps);
    
    //roof
    
    G4Box* roof_solid = new G4Box("roof_solid",ground_sizeX,roof_sizeY, ground_sizeZ);
    G4LogicalVolume* roof_log = new G4LogicalVolume(roof_solid,ground_mat,"roof_log");
    new G4PVPlacement(0,G4ThreeVector(0.0*mm,roof_sizeY+wall_sizeY*2.0, 0.0*mm),roof_log,"roof",logicWorld,false,0,checkOverlaps);
    
    
    G4Box* wall_back = new G4Box("wall_back",ground_sizeX,wall_sizeY+roof_sizeY+ground_sizeY,0.2*m);    
    G4LogicalVolume* wall_back_log = new G4LogicalVolume(wall_back,ground_mat,"wall_back_log");
    new G4PVPlacement(0,G4ThreeVector(0.0*m,wall_sizeY+roof_sizeY/2.0,(-1.0)*ground_sizeZ-0.2*m),wall_back_log,"wall_back_l",logicWorld,false,0,checkOverlaps);
    
    G4Box* wall_front = new G4Box("wall_front",ground_sizeX,wall_sizeY+roof_sizeY+ground_sizeY,0.2*m);    
    G4LogicalVolume* wall_front_log = new G4LogicalVolume(wall_front,ground_mat,"wall_front_log");
    new G4PVPlacement(0,G4ThreeVector(0.0*m,wall_sizeY+roof_sizeY/2.0,(1.0)*ground_sizeZ+0.2*m),wall_front_log,"wall_front_l",logicWorld,false,0,checkOverlaps);
            
    //tank_4
    
    
    G4VisAttributes* Color_tank = new G4VisAttributes(G4Colour::Cyan());  
    G4VisAttributes* Color_tank_water = new G4VisAttributes(G4Colour::Blue());
    
    G4double radio_tank_big = (3.2/2.0)*m;
	G4double  radio_z_big = (8.86/2.0)*m;
    
    G4double radio_tank_small = (2.0/2.0)*m;
    G4double  radio_z_small = (5.0/2.0)*m;
	
    G4double  thickness_steel_tank = (0.8)*cm;
    
    //tank 4
    
    G4Tubs* tank_4 = new G4Tubs("tank_4_solid", 0.0*mm, radio_tank_big, radio_z_big, min_phi, max_phi);
    G4LogicalVolume* tank_4_log = new G4LogicalVolume(tank_4, tank_mat, "tank_4_log");     
    new G4PVPlacement(0,G4ThreeVector(-1.0*(ground_sizeX)+0.9*m+radio_tank_big, radio_tank_big+0.2*m, 0.0*m),tank_4_log,"tank_4",logicWorld,false,0,checkOverlaps);
	tank_4_log->SetVisAttributes(Color_tank);
    
    G4Tubs* tank_4_water = new G4Tubs("tank_4_water_solid", 0.0*mm, radio_tank_big-thickness_steel_tank, radio_z_big-thickness_steel_tank, min_phi, max_phi);
    G4LogicalVolume* tank_4_water_log = new G4LogicalVolume(tank_4_water, tank_water, "tank_4w_log");     
    new G4PVPlacement(0,G4ThreeVector(),tank_4_water_log,"tank_4_water",tank_4_log,false,0,checkOverlaps);
	tank_4_water_log->SetVisAttributes(Color_tank_water);
    
    //tank 3
    
    G4Tubs* tank_3 = new G4Tubs("tank_3_solid", 0.0*mm, radio_tank_big, radio_z_big, min_phi, max_phi);
    G4LogicalVolume* tank_3_log = new G4LogicalVolume(tank_3, tank_mat, "tank_3_log");     
    new G4PVPlacement(0,G4ThreeVector(-1.0*(ground_sizeX)+0.9*m+radio_tank_big+4*m, radio_tank_big+0.2*m, 0.0*m),tank_3_log,"tank_3",logicWorld,false,0,checkOverlaps);
	tank_3_log->SetVisAttributes(Color_tank);
    
    
    std::cout<<"Tank_3 "<< -1.0*(ground_sizeX)+0.9*m+radio_tank_big+4*m << "  " << radio_tank_big+0.2*m << "  " << 0.0*m <<std::endl;
    
    G4Tubs* tank_3_water = new G4Tubs("tank_3_water_solid", 0.0*mm, radio_tank_big-thickness_steel_tank, radio_z_big-thickness_steel_tank, min_phi, max_phi);
    G4LogicalVolume* tank_3_water_log = new G4LogicalVolume(tank_3_water, tank_water, "tank_3w_log");     
    new G4PVPlacement(0,G4ThreeVector(),tank_3_water_log,"tank_3_water",tank_3_log,false,0,checkOverlaps);
	tank_3_water_log->SetVisAttributes(Color_tank_water);

    //tank 2
    
    G4Tubs* tank_2 = new G4Tubs("tank_2_solid", 0.0*mm, radio_tank_small, radio_z_small, min_phi, max_phi);
    G4LogicalVolume* tank_2_log = new G4LogicalVolume(tank_2, tank_mat, "tank_2_log");     
    new G4PVPlacement(0,G4ThreeVector(2.0*m+radio_tank_small, radio_tank_small+0.2*m, 2.0*m),tank_2_log,"tank_2",logicWorld,false,0,checkOverlaps);
	tank_2_log->SetVisAttributes(Color_tank);
	
	std::cout<<"Tank_2 "<< 2.0*m+radio_tank_small << "  " << radio_tank_small+0.2*m << "  " << 2.0*m <<std::endl;
    
    G4Tubs* tank_2_water = new G4Tubs("tank_2_water_solid", 0.0*mm, radio_tank_small-thickness_steel_tank, radio_z_small-thickness_steel_tank, min_phi, max_phi);
    G4LogicalVolume* tank_2_water_log = new G4LogicalVolume(tank_2_water, tank_water, "tank_2w_log");     
    new G4PVPlacement(0,G4ThreeVector(),tank_2_water_log,"tank_2_water",tank_2_log,false,0,checkOverlaps);
	tank_2_water_log->SetVisAttributes(Color_tank_water);
    
    //tank 1
    
    G4Tubs* tank_1 = new G4Tubs("tank_1_solid", 0.0*mm, radio_tank_small, radio_z_small, min_phi, max_phi);
    G4LogicalVolume* tank_1_log = new G4LogicalVolume(tank_1, tank_mat, "tank_1_log");     
    new G4PVPlacement(0,G4ThreeVector(6.0*m+radio_tank_small, radio_tank_small+0.2*m, 2.0*m),tank_1_log,"tank_1",logicWorld,false,0,checkOverlaps);
	tank_1_log->SetVisAttributes(Color_tank);
    
    std::cout<<"Tank_1 "<< 6.0*m+radio_tank_small << "  " << radio_tank_small+0.2*m << "  " << 2.0*m <<std::endl;
    
    G4Tubs* tank_1_water = new G4Tubs("tank_1_water_solid", 0.0*mm, radio_tank_small-thickness_steel_tank, radio_z_small-thickness_steel_tank, min_phi, max_phi);
    G4LogicalVolume* tank_1_water_log = new G4LogicalVolume(tank_1_water, tank_water, "tank_1w_log");     
    new G4PVPlacement(0,G4ThreeVector(),tank_1_water_log,"tank_1_water",tank_1_log,false,0,checkOverlaps);
	tank_1_water_log->SetVisAttributes(Color_tank_water);
    

	return physWorld;
	
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OO

void B1DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // declare crystal as a MultiFunctionalDetector scorer
  G4MultiFunctionalDetector* cryst = new G4MultiFunctionalDetector("crystal");
  G4SDManager::GetSDMpointer() -> AddNewDetector(cryst);
  G4VPrimitiveScorer* primitiv = new G4PSEnergyDeposit("edep");
  

  
  cryst -> RegisterPrimitive(primitiv);
  SetSensitiveDetector("CeBr_log",cryst);
}



