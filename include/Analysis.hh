#pragma once

#include "G4String.hh"
#include "G4Types.hh"
#include "G4Threading.hh"
#include "G4AutoLock.hh"

#include <fstream>
#include <memory>

// Forward declarations
class EventData;

class Analysis {
public:
    static EventData& Event();

    // Configuration
    static void SetOutputDirectory(const G4String& dir);

    // Run / event hooks
    static void BeginRun();
    static void EndRun();
    static void BeginEvent(G4int eventID);
    static void EndEvent();

private:
    // Output directory
    static G4String fOutDir;

    // Per-thread event buffer
    static G4ThreadLocal std::unique_ptr<EventData> tEvent;

    // Single shared output stream (one CSV for all threads)
    static std::unique_ptr<std::ofstream> fStream;
    static G4bool fHeaderWritten;
    static G4Mutex fMutex;

    // Helpers
    static EventData& GetEventData();
    static std::ofstream& GetStream();

    static G4String MakeSingleFilename();
    static void WriteHeader(std::ofstream& os);
};
