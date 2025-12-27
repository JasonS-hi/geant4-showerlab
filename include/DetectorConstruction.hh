#ifndef DetectorConstruction_hh
#define DetectorConstruction_hh

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4LogicalVolume;

class DetectorConstruction : public G4VUserDetectorConstruction {
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;

private:
    void DefineMaterials();
    void BuildGeometry();

    G4VPhysicalVolume* fWorldPV = nullptr;
    G4LogicalVolume*   fWorldLV = nullptr;

    // Logical volumes we attach SDs to
    G4LogicalVolume* fS1LV = nullptr;
    G4LogicalVolume* fT1LV = nullptr;
    G4LogicalVolume* fCherLV = nullptr;
    G4LogicalVolume* fT2LV = nullptr;
    G4LogicalVolume* fEcalMotherLV = nullptr;
    G4LogicalVolume* fCrystalLV = nullptr;
    G4LogicalVolume* fS2LV = nullptr;
};

#endif
