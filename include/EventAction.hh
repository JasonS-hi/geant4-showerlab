#ifndef EventAction_hh
#define EventAction_hh

#include "G4UserEventAction.hh"
#include "globals.hh"

class G4Event;
class PrimaryGeneratorAction;

class EventAction : public G4UserEventAction {
public:
    explicit EventAction(const PrimaryGeneratorAction* gen);
    ~EventAction() override;

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

private:
    const PrimaryGeneratorAction* fGen = nullptr; // non-owning
};

#endif
