#include "DetectorSD/EcalCrystalSD.hh"

#include "Analysis.hh"
#include "Data/EventData.hh"

#include "G4Step.hh"
#include "G4TouchableHandle.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

EcalCrystalSD::EcalCrystalSD(const G4String& name)
  : G4VSensitiveDetector(name) {}

EcalCrystalSD::~EcalCrystalSD() = default;

void EcalCrystalSD::Initialize(G4HCofThisEvent*) {
    // Nothing required: EventData is reset by EventAction.
}

G4bool EcalCrystalSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    const G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.0) return false;

    // Crystal ID from copy number (set in DetectorConstruction: copyNo = ix + 4*iy)
    const auto touch = step->GetPreStepPoint()->GetTouchableHandle();
    const int crystalID = touch->GetCopyNumber();

    if (crystalID < 0 || crystalID >= EventData::kNcrystals) return false;

    // Convert world position -> local position of the current volume (the crystal)
    const auto worldPos = step->GetPreStepPoint()->GetPosition();
    const auto localPos = touch->GetHistory()->GetTopTransform().TransformPoint(worldPos);
    const G4double zLocal = localPos.z();

    // Map local z to normalized [0, 1]
    const G4double z01 = (zLocal + 0.5 * fCrystalLength) / fCrystalLength;

    // Bin index along z
    int iz = static_cast<int>(std::floor(z01 * EventData::kNz));

    // Clamp to valid range
    if (iz < 0) iz = 0;
    if (iz >= EventData::kNz) iz = EventData::kNz - 1;

    const int voxelIndex = crystalID * EventData::kNz + iz;

    auto& ev = Analysis::Event();
    ev.ecalEdep_MeV[voxelIndex] += edep / MeV;

    return true;
}
