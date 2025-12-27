#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"

RunAction::RunAction() = default;
RunAction::~RunAction() = default;

void RunAction::BeginOfRunAction(const G4Run*) {
    // Output directory (change if you want)
    Analysis::SetOutputDirectory("data/output");
    Analysis::BeginRun();
}

void RunAction::EndOfRunAction(const G4Run*) {
    Analysis::EndRun();
}
