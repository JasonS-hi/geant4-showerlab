#ifndef TrackerSD_hh
#define TrackerSD_hh

#include "G4VSensitiveDetector.hh"
#include "globals.hh"

class G4Step;
class G4TouchableHistory;

class TrackerSD : public G4VSensitiveDetector {
public:
    enum class PlaneKind { T1, T2 };

    TrackerSD(const G4String& name, PlaneKind kind);
    ~TrackerSD() override;

    void Initialize(G4HCofThisEvent*) override;
    G4bool ProcessHits(G4Step* step, G4TouchableHistory*) override;

private:
    PlaneKind fKind;
};

#endif
