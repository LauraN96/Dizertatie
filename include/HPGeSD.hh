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
/// \file HPGeSD.hh
/// \brief Definition of the HPGeSD class

#ifndef HPGeSD_h
#define HPGeSD_h 1

#include "G4VSensitiveDetector.hh"

#include "HPGeHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;


/// HPGe Sensivite Detector class
class HPGeSD : public G4VSensitiveDetector
{
  public:

    HPGeSD (const G4String& name,
            const G4String& hitsCollectionName,
            G4int nofCells);

    virtual ~HPGeSD();

    //methods from base class
    virtual void   Initialize(G4HCofThisEvent* HCE);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* ROhist);
    virtual void   EndOfEvent(G4HCofThisEvent* HCE);



  private:
    HPGeHitsCollection* fHitsCollection;
    G4int fNofCells;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

