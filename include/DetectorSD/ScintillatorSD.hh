#ifndef ScintillatorSD_hh
#define ScintillatorSD_hh

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class G4Step;
class G4TouchableHistory;

class ScintillatorSD : public G4VSensitiveDetector {
public:
    enum class PlaneKind { S1, S2 };

    ScintillatorSD(const G4String& name, PlaneKind kind);
    ~ScintillatorSD() override;

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

private:
    PlaneKind fKind;
};

#endif
