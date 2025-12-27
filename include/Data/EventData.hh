#ifndef EventData_hh
#define EventData_hh

#include "globals.hh"
#include <array>
#include <limits>

// Per-event data container. Reset at start of each event.
// Sensitive detectors accumulate into this.
struct EventData {
  // Event identification
  G4int eventID = -1;

  // Beam info (filled in EventAction from PrimaryGeneratorAction)
  G4double ebeam_GeV = std::numeric_limits<double>::quiet_NaN();

  // S1 scintillator (trigger)
  G4double s1Time_ns  = std::numeric_limits<double>::quiet_NaN(); // earliest time seen
  G4double s1Edep_MeV = 0.0;

  // Trackers T1 / T2
  G4double t1x_mm = std::numeric_limits<double>::quiet_NaN();
  G4double t1y_mm = std::numeric_limits<double>::quiet_NaN();
  G4double t1t_ns = std::numeric_limits<double>::quiet_NaN();

  G4double t2x_mm = std::numeric_limits<double>::quiet_NaN();
  G4double t2y_mm = std::numeric_limits<double>::quiet_NaN();
  G4double t2t_ns = std::numeric_limits<double>::quiet_NaN();

  // Cherenkov proxy
  G4int cherProxyCount = 0;

  // - Nz longitudinal bins per crystal
  static constexpr int kNx = 11;
  static constexpr int kNy = 11;

  static constexpr int kNcrystals = kNx * kNy;

  //  PARAMETER: longitudinal bins per crystal
  static constexpr int kNz = 20;

  // Total voxels = 16 * Nz
  static constexpr int kNvox = kNcrystals * kNz;

  // Flattened storage:
  // voxelIndex = crystalID * kNz + iz
  std::array<G4double, kNvox> ecalEdep_MeV{};

  // ---------------------------
  // S2 scintillator (veto/leakage)
  // ---------------------------
  G4double s2Edep_MeV = 0.0;

  // Reset all fields for a new event
  void Reset() {
    // Re-initialize everything to defaults
    *this = EventData{};
    // Ensure arrays are zeroed (assignment above should do it, but be explicit)
    ecalEdep_MeV.fill(0.0);
  }
};

#endif
