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


  /*  void Draw();
    void Print();*/

    // methods to handle data
   // void SetEdep (G4double edep);

    // get methods
   // G4double GetEdep() const;

  public:
   // G4double fEdep; ///< Energy deposit in the sensitive volume




    void SetTrackID  (G4int track)      { trackID = track; };
    void SetEdep     (G4double de)      { edep = de; };
    void SetEinit     (G4double de)      { eInit = de; };
    void SetIntPos      (G4ThreeVector xyz){ intPos = xyz; };
    void SetEntryPos      (G4ThreeVector xyz){ entryPos = xyz; };
    void SetName     (G4String nume)    {name=nume;};
    void SetIncoming (G4int isit) {incoming =isit;};
    void SetOutgoing (G4int isit) {outgoing =isit;};
    void SetCopyNr (G4int isit) {copyNr =isit;};
    void SetParticle (G4String nume){particle=nume;};
    void SetTime (G4double time){hour=time;};

    G4int GetTrackID()    { return trackID; };
    G4double GetEdep()    { return edep; };
    G4double GetEinit()    { return eInit; };
    G4ThreeVector GetIntPos(){ return intPos; };
    G4ThreeVector GetEntryPos(){ return entryPos; };
    G4String GetName()    {return name;};
    G4int GetIncoming(){return incoming;};
    G4int GetOutgoing(){return outgoing;};
    G4String GetParticle(){return particle;};
    G4int GetCopyNo(){return copyNr;};
    G4double GetTime(){return hour;};

    G4int	detIDHit;

private:
    G4int         trackID;
    G4double 	eInit; //the initial energy of the particle;
    G4double      edep;  //deposited energy during step
    G4ThreeVector intPos; //interaction position (postStep) in local coordinates
    G4ThreeVector entryPos;//entry position in local coordinates
    G4String      name;  //name of the prestep (the detector where the hit takes place)
    G4int incoming; //set to 1 if the prestep point is on the border.  0 otherwise
    G4int outgoing; //set to 1 if the poststep point is on the border.  0 otherwise
    G4String particle; //the name of the partile creating the hit
    G4int copyNr; //copy number of the physical where the hit takes place
    G4double hour ; //the time of the hit (time variable used by the system...)





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

/*inline void HPGeHit::SetEdep(G4double edep) {
  fEdep += edep;
}

inline G4double HPGeHit::GetEdep() const {
  return fEdep;
}
*/
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
