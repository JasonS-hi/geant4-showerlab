#include "SteppingAction.hh"
#include "G4Step.hh"

SteppingAction::SteppingAction() = default;
SteppingAction::~SteppingAction() = default;

void SteppingAction::UserSteppingAction(const G4Step*) {
    // Empty by default.
    // Put debug hooks here if you want (e.g., kill optical photons, print steps).
}
