#include "DetectorSD/TrackerSD.hh"
#include "Analysis.hh"
#include "Data/EventData.hh"

#include "G4Step.hh"
#include "G4SystemOfUnits.hh"

TrackerSD::TrackerSD(const G4String& name, PlaneKind kind)
  : G4VSensitiveDetector(name), fKind(kind) {}

TrackerSD::~TrackerSD() = default;

void TrackerSD::Initialize(G4HCofThisEvent*) {
    // Nothing to init; EventData reset happens in EventAction.
}

G4bool TrackerSD::ProcessHits(G4Step* step, G4TouchableHistory*) {
    // We record first hit (earliest time) for the event in this plane.
    auto& ev = Analysis::Event();

    const auto pos = step->GetPreStepPoint()->GetPosition();
    const auto t   = step->GetPreStepPoint()->GetGlobalTime();

    const G4double x_mm = pos.x() / mm;
    const G4double y_mm = pos.y() / mm;
    const G4double t_ns = t / ns;

    if (fKind == PlaneKind::T1) {
        if (!(ev.t1t_ns == ev.t1t_ns) || t_ns < ev.t1t_ns) { // NaN check and earliest time
            ev.t1x_mm = x_mm; ev.t1y_mm = y_mm; ev.t1t_ns = t_ns;
        }
    } else {
        if (!(ev.t2t_ns == ev.t2t_ns) || t_ns < ev.t2t_ns) {
            ev.t2x_mm = x_mm; ev.t2y_mm = y_mm; ev.t2t_ns = t_ns;
        }
    }
    return true;
}
