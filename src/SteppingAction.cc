#include "SteppingAction.hh"
#include "G4Step.hh"

SteppingAction::SteppingAction() = default;
SteppingAction::~SteppingAction() = default;

void SteppingAction::UserSteppingAction(const G4Step*) {
}
