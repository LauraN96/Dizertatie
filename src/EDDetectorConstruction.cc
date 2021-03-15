//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// $Id$
//
/// \file EDDetectorConstruction.cc
/// \brief Implementation of the EDDetectorConstruction class

#include "EDDetectorConstruction.hh"
#include "HPGeSD.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSensitiveDetector.hh"
#include "G4SDManager.hh"
#include "G4VisAttributes.hh"
#include "G4Material.hh"
#include "G4Polycone.hh"
#include "G4Polyhedra.hh"
#include "G4Sphere.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDDetectorConstruction::EDDetectorConstruction()
: G4VUserDetectorConstruction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDDetectorConstruction::~EDDetectorConstruction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* EDDetectorConstruction::Construct()
{
  // Get nist material manager
  G4NistManager* nistManager = G4NistManager::Instance();

  //Elements
  G4double A, Z;
  G4Element* elI  = new G4Element("Iodine",  "I",  Z=53., A= 126.90447*g/mole);
  G4Element* elCs = new G4Element("Cesium",  "Cs", Z=55., A= 132.90543*g/mole);
  G4Element* elC = new G4Element("Carbon",  "C",  Z=6.,  A= 12.011 *g/mole);
  G4Element* elH  = new G4Element("Hydrogen","H",  Z=1.,  A=1.00794*g/mole);
  G4Element* elCl = new G4Element("Chlorine","Cl", Z = 17., A=35.453*g/mole);

  // Build materials
  G4Material* air = nistManager->FindOrBuildMaterial("G4_AIR");
  G4Material* Al = nistManager->FindOrBuildMaterial("G4_Al");
  G4Material* Pb = nistManager->FindOrBuildMaterial("G4_Pb");

  //G4Material* Vacuum = new G4Material("interGalactic", 1., 1.008*g/mole, 1.e-25*g/cm3, kStateGas, 2.73*kelvin, 3.e-18*pascal);

  G4int natoms, nel;
  G4Material* CsI = new G4Material("CsI", 4.51 *g/cm3, nel= 2);
  CsI-> AddElement(elCs, natoms=1);
  CsI-> AddElement(elI,  natoms=1);

  G4Material* PVC = new G4Material("PVC", 1.42 *g/cm3, nel= 3);
  PVC-> AddElement(elC, natoms=2);
  PVC-> AddElement(elH,  natoms=3);
  PVC-> AddElement(elCl,  natoms=1);



  // Option to switch on/off checking of volumes overlaps
   G4bool checkOverlaps = true;

  //
  // CAMERA
  //

  G4double hx = 400./2*mm;
  G4double hy = 150./2*mm;
  G4double hz = 600./2*mm;

  // world volume
  G4Box* cameraExp = new G4Box("Camera", hx, hy, hz);

  G4LogicalVolume* cameraLV
    = new G4LogicalVolume(cameraExp, air, "Camera");

  G4VPhysicalVolume* cameraPV
    = new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      cameraLV,               //its logical volume
                      "Camera",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  G4Colour blue(0.,0.,1.);
  G4VisAttributes atrib(blue);
  cameraLV->SetVisAttributes(atrib);

  //cameraLV-> SetVisAttributes (G4VisAttributes::GetInvisible());

  // Parameters

  G4double d0 = 6.9/2* mm ;
  G4double l0 = 221/2* mm ;
  G4double h0 = 4/2* mm ;

  G4VSolid* activePart = new G4Box("ActivePart", l0, d0, h0);

  G4LogicalVolume* activePartLV
    = new G4LogicalVolume(activePart, CsI, "ActivePart");

  new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(0,0,h0),       //at (0,0,0)
                      activePartLV,               //its logical volume
                      "ActivePart",               //its name
                      cameraLV,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking


  G4Colour red(1.,0.,0.);
  G4VisAttributes atribb(red);
  activePartLV->SetVisAttributes(atribb);

  //Element 1

  G4double d1 = 10/2* mm ;
  G4double l1 = 221/2* mm ;
  G4double h1 = 6/2* mm ;


  G4Box* element1 = new G4Box("Element1", l1, d1, h1);

  G4LogicalVolume* element1LV
    = new G4LogicalVolume(element1, PVC, "Element3");

  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, 2*h0+h1),       //at (0,0,0)
                    element1LV,                //its logical volume
                    "Element1",                //its name
                    cameraLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking


  G4Colour yellow(0.,1.,1.);
  G4VisAttributes attribss(yellow);
  element1LV->SetVisAttributes(attribss);

  //Element 2

  G4double d2 = 12/2* mm ;
  G4double l2 = 221/2* mm ;
  G4double h2 = 8/2* mm ;


  G4Box* element2 = new G4Box("Element2", l2, d2, h2);

  G4LogicalVolume* element2LV
    = new G4LogicalVolume(element2, PVC, "Element2");

  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, 2*h0+2*h1+h2),       //at (0,0,0)
                    element2LV,                //its logical volume
                    "Element2",                //its name
                    cameraLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking



  G4VisAttributes atb_y(yellow);
  element2LV->SetVisAttributes(atb_y);

  //Element 3

  G4double d3 = 18/2* mm ;
  G4double l3 = 221/2* mm ;
  G4double h3 = 4/2* mm ;


  G4Box* element3 = new G4Box("Element3", l3, d3, h3);

  G4LogicalVolume* element3LV
    = new G4LogicalVolume(element3, Al, "Element3");

  new G4PVPlacement(0,
                    G4ThreeVector(0, -(d3-d2), 2*h0+2*h1+2*h2+h3),       //at (0,0,0)
                    element3LV,                //its logical volume
                    "Element3",                //its name
                    cameraLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking


  G4Colour purple(1.,0.,1.);
  G4VisAttributes atb_p(purple);
  element3LV->SetVisAttributes(atb_p);

  auto  SensitDet
    = new HPGeSD("TDI", "AbsorberHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(SensitDet);
  SetSensitiveDetector("ActivePart",SensitDet);

  // Filtru AL

  G4double d4 = 100/2* mm ;
  G4double l4 = 230/2* mm ;
  G4double h4 = 6/2* mm ;


  G4Box* element4 = new G4Box("Element3", l4, d4, h4);

  G4LogicalVolume* element4LV
    = new G4LogicalVolume(element4, Al, "Element4");

  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, - 140* mm),       //at (0,0,0)
                    element4LV,                //its logical volume
                    "Element4",                //its name
                    cameraLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking


  //G4Colour purple(1.,0.,1.);
  G4VisAttributes atb_pp(purple);
  element4LV->SetVisAttributes(atb_pp);


  // Colimator 1

  G4double d5 = 50/2* mm ;
  G4double l5 = 115/2* mm ;
  G4double h5 = 6/2* mm ;


  G4Box* element5 = new G4Box("Element5", l5, d5, h5);

  G4LogicalVolume* element5LV
    = new G4LogicalVolume(element5, Al, "Element5");

  new G4PVPlacement(0,
                    G4ThreeVector(0, 30* mm, - 80* mm),       //at (0,0,0)
                    element5LV,                //its logical volume
                    "Element5",                //its name
                    cameraLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking


  //G4Colour purple(1.,0.,1.);
  G4VisAttributes atb_ppp(purple);
  element4LV->SetVisAttributes(atb_ppp);

  // Colimator 2

  G4double d6 = 50/2* mm ;
  G4double l6 = 115/2* mm ;
  G4double h6 = 6/2* mm ;


  G4Box* element6 = new G4Box("Element6", l6, d6, h6);

  G4LogicalVolume* element6LV
    = new G4LogicalVolume(element6, Al, "Element6");

  new G4PVPlacement(0,
                    G4ThreeVector(0, -30* mm, - 80* mm),       //at (0,0,0)
                    element6LV,                //its logical volume
                    "Element6",                //its name
                    cameraLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking


  //G4Colour purple(1.,0.,1.);
  G4VisAttributes atb_pppp(purple);
  element4LV->SetVisAttributes(atb_pppp);

  //Sfera iradiata

  G4VSolid* element7 = new G4Sphere("Element7", 0* mm, 7* mm, 0.0* deg, 360* deg, 0.0* deg, 360.0* deg);
  G4LogicalVolume* element7LV
    = new G4LogicalVolume(element7, Al, "Element7");

  new G4PVPlacement(0,
                    G4ThreeVector(0, 0, - 22* mm),       //at (0,0,0)
                    element7LV,                //its logical volume
                    "Element7",                //its name
                    cameraLV,               //its mother  volume
                    false,                 //no boolean operation
                    0,                     //copy number
                    checkOverlaps);        //overlaps checking


 // G4Colour yellow(0.,1.,1.);
  G4VisAttributes atb_ppppp(yellow);
  element4LV->SetVisAttributes(atb_ppppp);

  return cameraPV;


}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
