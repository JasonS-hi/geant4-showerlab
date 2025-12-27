#ifndef CherenkovSD_hh
#define CherenkovSD_hh

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class G4Step;
class G4TouchableHistory;

// Cherenkov proxy: counts "above-threshold track segments".
// No optical photons. Useful as a purity tag / sanity check.
class CherenkovSD : public G4VSensitiveDetector {
public:
    explicit CherenkovSD(const G4String& name);
    ~CherenkovSD() override;

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

private:
    // Refractive index placeholder (air-like). Change if you change radiator.
    G4double fN = 1.0003;
};

#endif
