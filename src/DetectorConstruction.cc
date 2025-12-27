#include "DetectorConstruction.hh"
#include "Data/EventData.hh"

#include "G4NistManager.hh"
#include "G4Material.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SDManager.hh"
#include "DetectorSD/TrackerSD.hh"
#include "DetectorSD/ScintillatorSD.hh"
#include "DetectorSD/EcalCrystalSD.hh"
#include "DetectorSD/CherenkovSD.hh"

namespace {
  // World
  constexpr G4double kWorldX = 2.0*m;
  constexpr G4double kWorldY = 2.0*m;
  constexpr G4double kWorldZ = 4.0*m;

  // Planes
  constexpr G4double kPlaneXY = 30.0*cm;
  constexpr G4double kScintThick = 5.0*mm;     // S1 & S2
  constexpr G4double kTrackerThick = 0.3*mm;   // T1 & T2
  constexpr G4double kCherLength = 20.0*cm;    // Cher radiator length

  // ECAL
  constexpr G4int    kNx = 11;
  constexpr G4int    kNy = 11;
  constexpr G4double kCrystalXY  = 4.0*cm;
  constexpr G4double kCrystalGap = 1.0*mm;
  constexpr G4double kCrystalZ   = 37.0*cm;

  // Z positions (centers)
  constexpr G4double kZ_S1   = 0.0*cm;
  constexpr G4double kZ_T1   = 5.0*cm;
  constexpr G4double kZ_Cher = 20.0*cm;
  constexpr G4double kZ_T2   = 35.0*cm;

  constexpr G4double kEcalFrontFaceZ = 50.0*cm;
  constexpr G4double kZ_EcalCenter   = kEcalFrontFaceZ + 0.5*kCrystalZ;

  constexpr G4double kZ_S2 = (kEcalFrontFaceZ + kCrystalZ) + 3.0*cm;

  // Lead-glass proxy mass fractions (placeholder recipe)
  constexpr G4double kPbO = 0.60;
  constexpr G4double kSiO2 = 0.40;
}

DetectorConstruction::DetectorConstruction() = default;
DetectorConstruction::~DetectorConstruction() = default;

G4VPhysicalVolume* DetectorConstruction::Construct() {
  DefineMaterials();
  BuildGeometry();
  return fWorldPV;
}

void DetectorConstruction::DefineMaterials() {
  auto* nist = G4NistManager::Instance();
  nist->FindOrBuildMaterial("G4_AIR");
  nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  nist->FindOrBuildMaterial("G4_Si");

  // Components for proxy lead glass
  auto* elPb = nist->FindOrBuildElement("Pb");
  auto* elSi = nist->FindOrBuildElement("Si");
  auto* elO  = nist->FindOrBuildElement("O");

  auto* matPbO = new G4Material("PbO", 9.53*g/cm3, 2);
  matPbO->AddElement(elPb, 1);
  matPbO->AddElement(elO,  1);

  auto* matSiO2 = new G4Material("SiO2", 2.20*g/cm3, 2);
  matSiO2->AddElement(elSi, 1);
  matSiO2->AddElement(elO,  2);

  auto* leadGlass = new G4Material("LeadGlassProxy", 5.20*g/cm3, 2);
  leadGlass->AddMaterial(matPbO,  kPbO);
  leadGlass->AddMaterial(matSiO2, kSiO2);
}

void DetectorConstruction::BuildGeometry() {
  auto* nist = G4NistManager::Instance();

  auto* airMat    = nist->FindOrBuildMaterial("G4_AIR");
  auto* scintMat  = nist->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
  auto* siMat     = nist->FindOrBuildMaterial("G4_Si");
  auto* leadGlass = G4Material::GetMaterial("LeadGlassProxy");
  auto* Pb = G4Material::GetMaterial("G4_Pb");

  // World
  auto* worldS = new G4Box("WorldS", 0.5*kWorldX, 0.5*kWorldY, 0.5*kWorldZ);
  fWorldLV = new G4LogicalVolume(worldS, airMat, "WorldLV");
  fWorldPV = new G4PVPlacement(nullptr, {}, fWorldLV, "WorldPV", nullptr, false, 0, true);
  fWorldLV->SetVisAttributes(G4VisAttributes::GetInvisible());

  // S1
  {
    auto* s = new G4Box("S1S", 0.5*kPlaneXY, 0.5*kPlaneXY, 0.5*kScintThick);
    fS1LV = new G4LogicalVolume(s, scintMat, "S1LV");
    new G4PVPlacement(nullptr, {0,0,kZ_S1}, fS1LV, "S1PV", fWorldLV, false, 0, true);

    auto* va = new G4VisAttributes(G4Colour(0.1, 0.9, 0.1,0.5));
    va->SetForceSolid(true);
    fS1LV->SetVisAttributes(va);
  }

  // T1
  {
    auto* s = new G4Box("T1S", 0.5*kPlaneXY, 0.5*kPlaneXY, 0.5*kTrackerThick);
    fT1LV = new G4LogicalVolume(s, siMat, "T1LV");
    new G4PVPlacement(nullptr, {0,0,kZ_T1}, fT1LV, "T1PV", fWorldLV, false, 0, true);

    auto* va = new G4VisAttributes(G4Colour(0.1, 0.1, 0.9,0.5));
    va->SetForceSolid(true);
    fT1LV->SetVisAttributes(va);
  }

  // Cherenkov radiator (proxy; just a volume)
  {
    auto* s = new G4Box("CherS", 0.5*kPlaneXY, 0.5*kPlaneXY, 0.5*kCherLength);
    fCherLV = new G4LogicalVolume(s, airMat, "CherLV");
    new G4PVPlacement(nullptr, {0,0,kZ_Cher}, fCherLV, "CherPV", fWorldLV, false, 0, true);

    auto* va = new G4VisAttributes(G4Colour(0.9, 0.9, 0.1));
    va->SetForceWireframe(true);
    fCherLV->SetVisAttributes(va);
  }

  // T2
  {
    auto* s = new G4Box("T2S", 0.5*kPlaneXY, 0.5*kPlaneXY, 0.5*kTrackerThick);
    fT2LV = new G4LogicalVolume(s, siMat, "T2LV");
    new G4PVPlacement(nullptr, {0,0,kZ_T2}, fT2LV, "T2PV", fWorldLV, false, 0, true);

    auto* va = new G4VisAttributes(G4Colour(0.1, 0.1, 0.9,0.5));
    va->SetForceSolid(true);
    fT2LV->SetVisAttributes(va);
  }

  // ECAL mother
  const G4double pitch = kCrystalXY + kCrystalGap;
  const G4double ecalX = kNx * pitch;
  const G4double ecalY = kNy * pitch;
  const G4double ecalZ = kCrystalZ;

  {
    auto* s = new G4Box("EcalMotherS", 0.5*ecalX, 0.5*ecalY, 0.5*ecalZ);
    fEcalMotherLV = new G4LogicalVolume(s, airMat, "EcalMotherLV");
    new G4PVPlacement(nullptr, {0,0,kZ_EcalCenter}, fEcalMotherLV, "EcalMotherPV", fWorldLV, false, 0, true);

    auto* va = new G4VisAttributes(G4Colour(0.6, 0.6, 0.6,0.3));
    va->SetForceWireframe(true);
    fEcalMotherLV->SetVisAttributes(va);
  }

  // ECAL crystals (121 copies)
  {
    auto* s = new G4Box("CrystalS", 0.5*kCrystalXY, 0.5*kCrystalXY, 0.5*kCrystalZ);
    fCrystalLV = new G4LogicalVolume(s, leadGlass, "CrystalLV");

    auto* va = new G4VisAttributes(G4Colour(0.9, 0.2, 0.2, 0.3));
    va->SetForceSolid(true);
    fCrystalLV->SetVisAttributes(va);

    for (G4int iy = 0; iy < kNy; ++iy) {
      for (G4int ix = 0; ix < kNx; ++ix) {
        const G4int copyNo = ix + kNx*iy;
        const G4double x = ( (G4double)ix - 0.5*(kNx - 1) ) * pitch;
        const G4double y = ( (G4double)iy - 0.5*(kNy - 1) ) * pitch;

        new G4PVPlacement(nullptr, {x,y,0.0}, fCrystalLV, "CrystalPV",
                          fEcalMotherLV, false, copyNo, true);
      }
    }
  }

  // S2 veto
  {
    auto* s = new G4Box("S2S", 0.5*kPlaneXY, 0.5*kPlaneXY, 0.5*kScintThick);
    fS2LV = new G4LogicalVolume(s, scintMat, "S2LV");
    new G4PVPlacement(nullptr, {0,0,kZ_S2}, fS2LV, "S2PV", fWorldLV, false, 0, true);

    auto* va = new G4VisAttributes(G4Colour(0.1, 0.9, 0.1,0.5));
    va->SetForceSolid(true);
    fS2LV->SetVisAttributes(va);
  }
}

void DetectorConstruction::ConstructSDandField() {
  auto* sdman = G4SDManager::GetSDMpointer();

  // S1 scintillator SD
  auto* s1sd = new ScintillatorSD("S1SD", ScintillatorSD::PlaneKind::S1);
  sdman->AddNewDetector(s1sd);
  fS1LV->SetSensitiveDetector(s1sd);

  // Trackers
  auto* t1sd = new TrackerSD("T1SD", TrackerSD::PlaneKind::T1);
  sdman->AddNewDetector(t1sd);
  fT1LV->SetSensitiveDetector(t1sd);

  auto* t2sd = new TrackerSD("T2SD", TrackerSD::PlaneKind::T2);
  sdman->AddNewDetector(t2sd);
  fT2LV->SetSensitiveDetector(t2sd);

  // Cherenkov proxy SD
  auto* chsd = new CherenkovSD("CherSD");
  sdman->AddNewDetector(chsd);
  fCherLV->SetSensitiveDetector(chsd);

  // ECAL crystals SD
  auto* ecalsd = new EcalCrystalSD("EcalSD");
  sdman->AddNewDetector(ecalsd);
  fCrystalLV->SetSensitiveDetector(ecalsd);

  // S2 scintillator SD
  auto* s2sd = new ScintillatorSD("S2SD", ScintillatorSD::PlaneKind::S2);
  sdman->AddNewDetector(s2sd);
  fS2LV->SetSensitiveDetector(s2sd);
}
