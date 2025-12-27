#include "ActionInitialization.hh"

#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

ActionInitialization::ActionInitialization() = default;
ActionInitialization::~ActionInitialization() = default;

void ActionInitialization::BuildForMaster() const {
    SetUserAction(new RunAction());
}

void ActionInitialization::Build() const {
    auto* gen = new PrimaryGeneratorAction();
    SetUserAction(gen);

    SetUserAction(new RunAction());
    SetUserAction(new EventAction(gen));
    SetUserAction(new SteppingAction());
}
