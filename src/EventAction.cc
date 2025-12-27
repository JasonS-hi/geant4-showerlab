#include "EventAction.hh"
#include "Analysis.hh"
#include "PrimaryGeneratorAction.hh"
#include "Data/EventData.hh"

#include "G4Event.hh"

EventAction::EventAction(const PrimaryGeneratorAction* gen) : fGen(gen) {}
EventAction::~EventAction() = default;

void EventAction::BeginOfEventAction(const G4Event* event) {
    Analysis::BeginEvent(event->GetEventID());

    // Record beam energy at event start
    auto& ev = Analysis::Event();
    if (fGen) {
        ev.ebeam_GeV = fGen->GetBeamEnergyGeV();
    }
}

void EventAction::EndOfEventAction(const G4Event*) {
    Analysis::EndEvent();
}
