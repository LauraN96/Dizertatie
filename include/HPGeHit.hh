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
//
/// \file HPGeHit.hh
/// \brief Definition of the HPGeHit class

#ifndef HPGeHit_h
#define HPGeHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "G4Threading.hh"

/// HPGe hit class
///
/// It defines data members to store the the energy deposit and track lengths
/// of charged particles in a selected volume:
/// - fEdep, fTrackLength

class HPGeHit : public G4VHit
{
  public:
    HPGeHit();
    HPGeHit(const HPGeHit&);
    virtual ~HPGeHit();

    // operators
    const HPGeHit& operator=(const HPGeHit&);
    G4bool operator==(const HPGeHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw() {}
    virtual void Print();

    // methods to handle data
    void SetEdep (G4double edep);

    // get methods
    G4double GetEdep() const;

  private:
    G4double fEdep; ///< Energy deposit in the sensitive volume
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

using HPGeHitsCollection = G4THitsCollection<HPGeHit>;

extern G4ThreadLocal G4Allocator<HPGeHit>* HPGeHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* HPGeHit::operator new(size_t)
{
  if (!HPGeHitAllocator) {
    HPGeHitAllocator = new G4Allocator<HPGeHit>;
  }
  void *hit;
  hit = (void *) HPGeHitAllocator->MallocSingle();
  return hit;
}

inline void HPGeHit::operator delete(void *hit)
{
  if (!HPGeHitAllocator) {
    HPGeHitAllocator = new G4Allocator<HPGeHit>;
  }
  HPGeHitAllocator->FreeSingle((HPGeHit*) hit);
}

inline void HPGeHit::SetEdep(G4double edep) {
  fEdep += edep;
}

inline G4double HPGeHit::GetEdep() const {
  return fEdep;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
