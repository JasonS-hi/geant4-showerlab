#ifndef EcalCrystalSD_hh
#define EcalCrystalSD_hh

#include "G4VSensitiveDetector.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"   //for cm

class G4Step;
class G4TouchableHistory;

// Sensitive detector for ECAL crystals.
// Accumulates energy deposit into 3D voxels:
// (crystalID 0..15, zbin 0..kNz-1).
class EcalCrystalSD : public G4VSensitiveDetector {
public:
    explicit EcalCrystalSD(const G4String& name);
    ~EcalCrystalSD() override;

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

    // Optional setter if you want to configure from DetectorConstruction later
    void SetCrystalLength(G4double L) { fCrystalLength = L; }

private:
    // PARAMETER: must match the crystal length in DetectorConstruction (kCrystalZ)
    G4double fCrystalLength = 37.0*cm;
};

#endif
