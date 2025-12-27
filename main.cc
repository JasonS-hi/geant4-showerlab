// main.cc
#include "G4RunManagerFactory.hh"
#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4EmStandardPhysics_option4.hh"

#ifdef G4MULTITHREADED
  #include "G4MTRunManager.hh"
#endif

#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv) {
    // 1) Create run manager (MT build will return MT run manager by default)
    auto* runManager = G4RunManagerFactory::CreateRunManager();

    if (argc == 0) {            // UI / visualisation
        if (auto* mt = dynamic_cast<G4MTRunManager*>(runManager)) {
            mt->SetNumberOfThreads(1);
        }
    } else {                     // batch
        if (auto* mt = dynamic_cast<G4MTRunManager*>(runManager)) {
            mt->SetNumberOfThreads(6);
        }
    }


    // 3) Mandatory user initializations
    runManager->SetUserInitialization(new DetectorConstruction());

    // Physics list: FTFP_BERT + EM Option4
    G4PhysListFactory plFactory;
    auto* phys = plFactory.GetReferencePhysList("FTFP_BERT");
    phys->ReplacePhysics(new G4EmStandardPhysics_option4());
    runManager->SetUserInitialization(phys);

    runManager->SetUserInitialization(new ActionInitialization());

    // 4) Initialize run manager NOW (don’t defer to macros while debugging)
    runManager->Initialize();

    // 5) Visualization
    auto* visManager = new G4VisExecutive();
    visManager->Initialize();

    auto* uiManager = G4UImanager::GetUIpointer();

    // 6) UI / batch logic
    if (argc == 1) {
        // Interactive mode
        auto* ui = new G4UIExecutive(argc, argv);

        // Run a vis macro that assumes geometry already exists
        uiManager->ApplyCommand("/control/alias E 2");
        // uiManager->ApplyCommand("/control/execute vis.mac");

        ui->SessionStart();
        delete ui;
    } else {
        // Batch mode
        G4String macro = argv[1];
        uiManager->ApplyCommand("/control/execute " + macro);
    }

    // 7) Cleanup
    delete visManager;
    delete runManager;
    return 0;
}
