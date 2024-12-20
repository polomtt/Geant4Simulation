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

#include "eRositaActionInitialization.hh"
#include "eRositaDetectorConstruction.hh"
#include "eRositaPrimaryGeneratorAction.hh"

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

eRositaPrimaryGeneratorAction::eRositaPrimaryGeneratorAction()
{
    constexpr auto NUMBER_OF_PARTICLES{1};
    particleGun = new G4ParticleGun(NUMBER_OF_PARTICLES);

    // default particle
    auto *particleTable = G4ParticleTable::GetParticleTable();
    auto *particle = particleTable->FindParticle("proton");

    constexpr auto PARTICLE_ENERGY{4.7 * MeV};
    particleGun->SetParticleDefinition(particle);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(momentumDirectionX, momentumDirectionY, momentumDirectionZ));
    particleGun->SetParticleEnergy(PARTICLE_ENERGY);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

eRositaPrimaryGeneratorAction::~eRositaPrimaryGeneratorAction()
{
    delete particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void eRositaPrimaryGeneratorAction::GeneratePrimaries(G4Event* event)
{
    constexpr auto INITIAL_POSITION_X{0.0 * cm};
    constexpr auto INITIAL_POSITION_Y{0 * cm};
    constexpr auto INITIAL_POSITION_Z{5 * cm};
    
    positionX = INITIAL_POSITION_X;
    positionY = INITIAL_POSITION_Y;
    positionZ = INITIAL_POSITION_Z;

    particleGun->SetParticlePosition(G4ThreeVector(positionX, positionY, positionZ));
    particleGun->GeneratePrimaryVertex(event);
}
