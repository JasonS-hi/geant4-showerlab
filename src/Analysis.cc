#include "Analysis.hh"
#include "Data/EventData.hh"

#include "G4Threading.hh"
#include "G4AutoLock.hh"
#include "G4SystemOfUnits.hh"

#include <iomanip>
#include <sstream>
#include <filesystem>
#include <fstream>
#include <memory>



// Output directory (can be changed)
G4String Analysis::fOutDir = "data/output";

// Event data stays thread-local (correct)
G4ThreadLocal std::unique_ptr<EventData> Analysis::tEvent;

// NEW:
std::unique_ptr<std::ofstream> Analysis::fStream = nullptr;
G4bool Analysis::fHeaderWritten = false;
G4Mutex Analysis::fMutex = G4MUTEX_INITIALIZER;


void Analysis::SetOutputDirectory(const G4String& dir) {
  G4AutoLock lock(&fMutex);
  fOutDir = dir;
}

EventData& Analysis::GetEventData() {
  if (!tEvent) tEvent = std::make_unique<EventData>();
  return *tEvent;
}

EventData& Analysis::Event() {
  return GetEventData();
}

G4String Analysis::MakeSingleFilename() {
  std::ostringstream oss;
  oss << fOutDir << "/showerlab.csv";
  return oss.str();
}

std::ofstream& Analysis::GetStream() {
  G4AutoLock lock(&fMutex);

  if (!fStream) {
    // Ensure directory exists (best effort)
    try { std::filesystem::create_directories(std::string(fOutDir)); } catch (...) {}

    fStream = std::make_unique<std::ofstream>(MakeSingleFilename());
    fHeaderWritten = false;
  }

  if (!fHeaderWritten) {
    WriteHeader(*fStream);
    fHeaderWritten = true;
  }

  return *fStream;
}

void Analysis::WriteHeader(std::ofstream& os) {
  os << "eventID,ebeam_GeV,"
     << "s1Time_ns,s1Edep_MeV,"
     << "t1x_mm,t1y_mm,t1t_ns,"
     << "t2x_mm,t2y_mm,t2t_ns,"
     << "cherProxyCount,";

  for (int c = 0; c < EventData::kNcrystals; ++c) {
    for (int z = 0; z < EventData::kNz; ++z) {
      os << "ecal_c" << c << "_z" << z << "_MeV,";
    }
  }

  os << "s2Edep_MeV\n";
}

void Analysis::BeginRun() {
  // Open file early (any thread may call; protected by lock inside GetStream)
  (void)GetStream();
}

void Analysis::EndRun() {
  // Only the master thread should close the shared file
  if (!G4Threading::IsMasterThread()) return;

  G4AutoLock lock(&fMutex);
  if (fStream && fStream->is_open()) {
    fStream->flush();
    fStream->close();
  }
  fStream.reset();
  fHeaderWritten = false;
}

void Analysis::BeginEvent(G4int eventID) {
  auto& ev = GetEventData();
  ev.Reset();
  ev.eventID = eventID;
}

void Analysis::EndEvent() {
  auto& ev = GetEventData();

  G4AutoLock lock(&fMutex);

  // Ensure stream + header are ready WITHOUT re-locking
  if (!fStream) {
    try { std::filesystem::create_directories(std::string(fOutDir)); } catch (...) {}
    fStream = std::make_unique<std::ofstream>(MakeSingleFilename());
    fHeaderWritten = false;
  }
  if (!fHeaderWritten) {
    WriteHeader(*fStream);
    fHeaderWritten = true;
  }

  auto& os = *fStream;

  os << ev.eventID << ","
     << std::fixed << std::setprecision(6) << ev.ebeam_GeV << ","
     << ev.s1Time_ns << "," << ev.s1Edep_MeV << ","
     << ev.t1x_mm << "," << ev.t1y_mm << "," << ev.t1t_ns << ","
     << ev.t2x_mm << "," << ev.t2y_mm << "," << ev.t2t_ns << ","
     << ev.cherProxyCount << ",";

  for (int c = 0; c < EventData::kNcrystals; ++c) {
    for (int z = 0; z < EventData::kNz; ++z) {
      const int idx = c * EventData::kNz + z;
      os << ev.ecalEdep_MeV[idx] << ",";
    }
  }

  os << ev.s2Edep_MeV << "\n";
}
