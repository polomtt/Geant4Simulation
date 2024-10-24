
#include "B1DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Orb.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4SDManager.hh"
#include "G4Transform3D.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::B1DetectorConstruction()
: G4VUserDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

B1DetectorConstruction::~B1DetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* B1DetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();


  //Option to switch on/off checking of volumes overlaps
  G4bool checkOverlaps = true;

  //AMBIENTE ----------------------------------------------------------------------------------

  //Dimensioni ambiente
  G4double world_sizeX = 20.0*cm; 				//dimensioni ambiente in X
  G4double world_sizeY = 20.0*cm;				//dimensioni ambiente in Y
  G4double world_sizeZ = 20.0*cm;				//dimensioni ambiente in Z
	
  //Materiale ambiente
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR"); 	//ambiente composto d'aria

  //Forma e dimensioni ambiente 
  G4Box* solidWorld = 						//foma a scatola
            new G4Box("World", 				//nome
                      world_sizeX, 					//dimensioni in X
                      world_sizeY, 					//dimensioni in Y
                      world_sizeZ);       				//dimensioni in Z
	
  //Proprietà ambiente   
  G4LogicalVolume* logicWorld =                         
            new G4LogicalVolume(solidWorld,          	//è un solido
                                world_mat,           				//materiale
                                "World");            				//nome

  //Proprietà fisiche ambiente
  G4VPhysicalVolume* physWorld = 
                new G4PVPlacement(0,                     	//no rotazione
                                  G4ThreeVector(),       				//creato in (0,0,0)
                                  logicWorld,            				//è un logical volume
                                  "World",               				//nome
                                  0,                     				//il suo volume madre
                                  false,                 				//no operazioni booleane
                                  0,                     				//numero di copie
                                  checkOverlaps);        				//overlaps checking

	
  //detector ---------------------------------------------------------------------------------------

  //Materials
  G4Material* cap_mat = nist->FindOrBuildMaterial("G4_Al");

  G4double cap_rmax = (55.5/2.)*mm; 	//raggio esterno
  G4double cap_rmin = 0.0*mm;		//raggio interno
  G4double cap_lz = (51.8/2.)*mm; 		//metà lunghezza in Z, 51.8 = 50.8 + 1.0(Al+reflector)
  G4double cap_SPhi = 0.0*deg; 		//angolo iniziale in rad
  G4double cap_Phi = 360.0*deg;		//angolo totale

 	//Scintillator solid
  G4Tubs* solid_cap = 				//forma cilindrica
            new G4Tubs("CapSolid", 			//nome
                      cap_rmin, 					//raggio interno
                      cap_rmax, 					//raggio esterno
                      cap_lz, 					//metà lunghezza in Z
                      cap_SPhi, 					//angolo iniziale in rad
                      cap_Phi);       				//angolo totale


  //Proprietà armatura forata  
  G4LogicalVolume* logic_cap =                         
            new G4LogicalVolume(solid_cap,	    //è un solido
                                cap_mat,           				//materiale
                                "CapLog");            				//nome

	//Proprietà fisiche armatura forata
           	new G4PVPlacement(0,                     			//rotazione
                      	G4ThreeVector(0,0,-cap_lz),       				//creato in (0,0,0)
                        logic_cap,            				//è un logical volume
                        "CapPlace",               				//nome
                        logicWorld,                     		//il suo volume madre
                        true,                 				//si operazioni booleane
                        0,                     				//numero di copie
                        checkOverlaps);        				//overlaps checking


//LaBr crystal

  G4double crys_rmax = (50.8/2.0)*mm;

  // Crystal
  G4Tubs* solid_crys = 				//forma cilindrica
            new G4Tubs("CrysSolid", 			//nome
                      0.0*mm, 					//raggio interno
                      crys_rmax, 					//raggio esterno
                      crys_rmax, 					//metà lunghezza in Z
                      cap_SPhi, 					//angolo iniziale in rad
                      cap_Phi);       				//angolo totale


  G4String name, symbol;
  G4int ncomponents;
  G4double a, z;
  G4double fractionmass;
    
  a = 79.904*g/mole;
  G4Element* elBr  = new G4Element(name="Bromine", symbol="Br", z= 35., a);

  a = 138.905*g/mole;
  G4Element* elLa  = new G4Element(name="Lanthanum", symbol="La", z= 57., a);

  a = 140.116*g/mole;
  G4Element* elCe  = new G4Element(name="Cerium",symbol="Ce", z= 58., a);
  
  G4double density_labr = 5.08*g/cm3;
  
  G4Material* labr_mat = new G4Material(name="labr_mat",density_labr,ncomponents=3);
  labr_mat->AddElement(elBr, fractionmass=58.73*perCent);
  labr_mat->AddElement(elLa, fractionmass=34.04*perCent);
  labr_mat->AddElement(elCe, fractionmass= 7.23*perCent);

  G4LogicalVolume* logic_crys =                         
            new G4LogicalVolume(solid_crys,	    //è un solido
                                labr_mat,           				//materiale
                                "CrysLog");            				//nome

  new G4PVPlacement(0,                     			//rotazione
                    G4ThreeVector(0,0,-0.5*mm),  	//creato in (0,0,0)
                    logic_crys,            				//è un logical volume
                    "CrysPlace",               				//nome
                    logic_cap,                     		//il suo volume madre
                    true,                 				//si operazioni booleane
                    0,                     				//numero di copie
                    checkOverlaps);        				//overlaps checking

  // Mn-Cu sample

  G4double mn_rmax = (12.7/2.)*mm;   //raggio esterno
  G4double mn_lz = (0.0508/2.)*mm;

  //Scintillator solid
  G4Tubs* MnCu_solid =         //forma cilindrica
            new G4Tubs("MnSolid",      //nome
                      cap_rmin,           //raggio interno
                      mn_rmax,           //raggio esterno
                      mn_lz,           //metà lunghezza in Z
                      cap_SPhi,           //angolo iniziale in rad
                      cap_Phi);               //angolo totale


  G4Element* elMn = nist->FindOrBuildElement("Mn");
  G4Element* elCu = nist->FindOrBuildElement("Cu");
  
  G4double density_sample = 7.9*g/cm3; // densidad experimental
  
  G4Material* sample_mat = new G4Material(name="MnCu_mat",density_sample,ncomponents=2);
  sample_mat->AddElement(elMn, fractionmass=80.0*perCent);
  sample_mat->AddElement(elCu, fractionmass=20.0*perCent);

  //Proprietà armatura forata  
  G4LogicalVolume* logic_MnCu =                         
            new G4LogicalVolume(MnCu_solid,      //è un solido
                                sample_mat,                  //materiale
                                "MnCuLog");                    //nome

  //Proprietà fisiche armatura forata
            new G4PVPlacement(0,                          //rotazione
                        G4ThreeVector(0,0,mn_lz),               //creato in (0,0,0)
                        logic_MnCu,                    //è un logical volume
                        "MnCuPlace",                       //nome
                        logicWorld,                         //il suo volume madre
                        true,                         //si operazioni booleane
                        0,                            //numero di copie
                        checkOverlaps);               //overlaps checking

//   // Indium sample

//   G4double indio_rmax = (10.0/2.)*mm;   //raggio esterno
//   G4double indio_lz = (0.513/2.)*mm;     //0.512625 mm
  
//   //Scintillator solid
//   G4Tubs* indio_solid =         //forma cilindrica
//             new G4Tubs("IndioSolid",      //nome
//                       cap_rmin,           //raggio interno
//                       indio_rmax,           //raggio esterno
//                       indio_lz,           //metà lunghezza in Z
//                       cap_SPhi,           //angolo iniziale in rad
//                       cap_Phi);               //angolo totale


//   a = 114.818*g/mole;
//   G4Element* elIn = new G4Element(name="Indium",symbol="In", z= 49., a);
  
//   G4double density_indio = 6.36*g/cm3; // densidad experimental
  
//   G4Material* indio_mat = new G4Material(name="Indio_mat",density_indio,ncomponents=1);
//   indio_mat->AddElement(elIn, 1);

//   //Proprietà armatura forata  
//   G4LogicalVolume* logic_indio =                         
//             new G4LogicalVolume(indio_solid,      //è un solido
//                                 indio_mat,                  //materiale
//                                 "IndioLog");                    //nome

//   //Proprietà fisiche armatura forata
//             new G4PVPlacement(0,                          //rotazione
//                         G4ThreeVector(0,0,indio_lz),               //creato in (0,0,0)
//                         logic_indio,                    //è un logical volume
//                         "IndioPlace",                       //nome
//                         logicWorld,                         //il suo volume madre
//                         true,                         //si operazioni booleane
//                         0,                            //numero di copie
//                         checkOverlaps);               //overlaps checking


/*  G4double oro_rmax = (12.7/2.)*mm;
  G4double oro_lz = (0.0508/2.)*mm;
  
  //Scintillator solid
  G4Tubs* oro_solid =         //forma cilindrica
            new G4Tubs("OroSolid",      //nome
                      cap_rmin,           //raggio interno
                      oro_rmax,           //raggio esterno
                      oro_lz,           //metà lunghezza in Z
                      cap_SPhi,           //angolo iniziale in rad
                      cap_Phi);               //angolo totale


  G4Material* oro_mat = nist->FindOrBuildMaterial("G4_Au");

  //Proprietà armatura forata  
  G4LogicalVolume* logic_oro =                         
            new G4LogicalVolume(oro_solid,      //è un solido
                                oro_mat,                  //materiale
                                "OroLog");                    //nome

  //Proprietà fisiche armatura forata
            new G4PVPlacement(0,                          //rotazione
                        G4ThreeVector(0,0,oro_lz),               //creato in (0,0,0)
                        logic_oro,                    //è un logical volume
                        "OroPlace",                       //nome
                        logicWorld,                         //il suo volume madre
                        true,                         //si operazioni booleane
                        0,                            //numero di copie
                        checkOverlaps);               //overlaps checking
                        */

// plastic

  // G4double crys_rmax = (50.8/2.0)*mm;
  // G4double cap_SPhi = 0.0*deg;    //angolo iniziale in rad
  // G4double cap_Phi = 360.0*deg;    //angolo totale

  //   // Crystal
  // G4Tubs* solid_crys =        //forma cilindrica
  //           new G4Tubs("CrysSolid",       //nome
  //                     0.0*mm,           //raggio interno
  //                     crys_rmax,          //raggio esterno
  //                     crys_rmax,          //metà lunghezza in Z
  //                     cap_SPhi,           //angolo iniziale in rad
  //                     cap_Phi);               //angolo totale


  // G4String name, symbol;
  // G4int ncomponents;
  // G4double a, z;
  // G4double fractionmass;
    
  // a = 1.00794*g/mole;
  // G4Element* elH  = new G4Element(name="Hydrogen",symbol="H" , z= 1., a);

  // a = 12.0107*g/mole;
  // G4Element* elC  = new G4Element(name="Carbon"  ,symbol="C" , z= 6., a);
  
  // G4double density_ej276_mat = 1.096*g/cm3;
  
  // G4Material* ej276_mat = new G4Material(name="ej276_mat",density_ej276_mat,ncomponents=2);
  // ej276_mat->AddElement(elH, fractionmass=7.21*perCent);
  // ej276_mat->AddElement(elC, fractionmass=92.79*perCent);

  // G4double density_ej309_mat = 0.959*g/cm3;

  // G4Material* ej309_mat = new G4Material(name="ej309_mat",density_ej309_mat,ncomponents=2);
  // ej309_mat->AddElement(elH, fractionmass=9.4*perCent);
  // ej309_mat->AddElement(elC, fractionmass=90.6*perCent);

  // G4LogicalVolume* logic_crys =                         
  //           new G4LogicalVolume(solid_crys,     //è un solido
  //                               ej309_mat,                   //materiale
  //                               "CrysLog");                   //nome

  // new G4PVPlacement(0,                          //rotazione
  //                   G4ThreeVector(0,0,-crys_rmax*mm),   //creato in (0,0,0)
  //                   logic_crys,                   //è un logical volume
  //                   "CrysPlace",                      //nome
  //                   logicWorld,                        //il suo volume madre
  //                   true,                         //si operazioni booleane
  //                   0,                            //numero di copie
  //                   checkOverlaps);               //overlaps checking


    //always return the physical World
    return physWorld;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void B1DetectorConstruction::ConstructSDandField()
{
  G4SDManager::GetSDMpointer()->SetVerboseLevel(1);

  // declare crystal as a MultiFunctionalDetector scorer
  G4MultiFunctionalDetector* cryst = new G4MultiFunctionalDetector("crystal");
  G4SDManager::GetSDMpointer() -> AddNewDetector(cryst);
  G4VPrimitiveScorer* primitiv1 = new G4PSEnergyDeposit("edep");
  cryst -> RegisterPrimitive(primitiv1);
  SetSensitiveDetector("CrysLog",cryst);	
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......