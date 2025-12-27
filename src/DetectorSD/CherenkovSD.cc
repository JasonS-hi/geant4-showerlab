#include "DetectorSD/CherenkovSD.hh"
#include "Analysis.hh"
#include "Data/EventData.hh"
#include <cmath>

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4SystemOfUnits.hh"

CherenkovSD::CherenkovSD(const G4String& name)
  : G4VSensitiveDetector(name) {}

CherenkovSD::~CherenkovSD() = default;

void CherenkovSD::Initialize(G4HCofThisEvent*) {}

G4bool CherenkovSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto* track = step->GetTrack();

    // Only charged particles can produce Cherenkov
    if (track->GetDefinition()->GetPDGCharge() == 0.0) return false;

    // Compute beta from momentum and mass: beta = p / sqrt(p^2 + m^2)
    const G4double p = track->GetMomentum().mag();  // in energy units (MeV/c with Geant4 conventions)
    const G4double m = track->GetDefinition()->GetPDGMass();
    if (p <= 0.0) return false;

    const G4double beta = p / std::sqrt(p*p + m*m);

    // Cherenkov threshold condition
    if (beta <= 1.0 / fN) return false;

    // Proxy yield: +1 per mm of above-threshold step length
    const G4double Lmm = step->GetStepLength() / mm;
    if (Lmm <= 0) return false;

    auto& ev = Analysis::Event();
    ev.cherProxyCount += static_cast<int>(Lmm);
    return true;
}

