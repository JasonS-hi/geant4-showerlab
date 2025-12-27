#include "DetectorSD/ScintillatorSD.hh"
#include "Analysis.hh"
#include "Data/EventData.hh"

#include "G4Step.hh"
#include "G4SystemOfUnits.hh"

ScintillatorSD::ScintillatorSD(const G4String& name, PlaneKind kind)
  : G4VSensitiveDetector(name), fKind(kind) {}

ScintillatorSD::~ScintillatorSD() = default;

void ScintillatorSD::Initialize(G4HCofThisEvent*) {}

G4bool ScintillatorSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    auto& ev = Analysis::Event();

    const auto edep = step->GetTotalEnergyDeposit(); // energy deposit
    if (edep <= 0) return false;

    const auto t = step->GetPreStepPoint()->GetGlobalTime();

    if (fKind == PlaneKind::S1) {
        ev.s1Edep_MeV += edep / MeV;
        const G4double t_ns = t / ns;
        if (!(ev.s1Time_ns == ev.s1Time_ns) || t_ns < ev.s1Time_ns) {
            ev.s1Time_ns = t_ns;
        }
    } else {
        ev.s2Edep_MeV += edep / MeV;
    }
    return true;
}
