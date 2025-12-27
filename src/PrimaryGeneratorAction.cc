#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() {
    // One primary particle per event
    fGun = new G4ParticleGun(1);

    // -------------------- DEFAULTS (macros can override) --------------------
    // If you forget to set anything in macros, these produce a sensible run.
    auto* electron = G4ParticleTable::GetParticleTable()->FindParticle("e-");
    fGun->SetParticleDefinition(electron);
    fGun->SetParticleEnergy(4.0*GeV);
    fGun->SetParticlePosition({0.0, 0.0, -50.0*cm});
    fGun->SetParticleMomentumDirection({0.0, 0.0, 1.0});
    // ----------------------------------------------------------------------
}

PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete fGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    fGun->GeneratePrimaryVertex(event);
}

G4double PrimaryGeneratorAction::GetBeamEnergyGeV() const {
    return fGun->GetParticleEnergy() / GeV;
}
