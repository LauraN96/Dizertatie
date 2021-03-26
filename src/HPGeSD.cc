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
/// \file HPGeSD.cc
/// \brief Implementation of the HPGeSD class

#include "HPGeSD.hh"
#include "HPGeHit.hh"

#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeSD::HPGeSD(
        const G4String& name,
        const G4String& hitsCollectionName,
        G4int nofCells)
 : G4VSensitiveDetector(name),
   fHitsCollection(nullptr),
   fNofCells(nofCells)
{
  collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeSD::~HPGeSD()
{ 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeSD::Initialize(G4HCofThisEvent* HCE)
{
  // Create hits collection
    fHitsCollection
      = new HPGeHitsCollection(SensitiveDetectorName, collectionName[0]);

    // Add this collection in hce
    auto hcID
      = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
    HCE->AddHitsCollection( hcID, fHitsCollection );

    // Create hits
    //fNofCells for cells + one more for total sums
    for (G4int i=0; i<fNofCells+1; i++ ) {
      fHitsCollection->insert(new HPGeHit());
    }



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool HPGeSD::ProcessHits(G4Step* step,
                                     G4TouchableHistory*)
{  
    // energy deposit
    auto edep = step->GetTotalEnergyDeposit();

 /* // step length
    G4double stepLength = 0.;
    if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
      stepLength = step->GetStepLength();
    }

    if ( edep==0. && stepLength == 0. ) return false;

    auto touchable = (step->GetPreStepPoint()->GetTouchable());
  */
    // Get calorimeter cell id




    G4StepPoint* point1 = step->GetPreStepPoint();
    G4StepPoint* point2 = step->GetPostStepPoint();
            G4int incoming = 0;
    if (point1->GetStepStatus() == fGeomBoundary) {
            incoming = 1;
    };
    G4int outgoing=0;
    if (point2->GetStepStatus() == fGeomBoundary) {
                    outgoing = 1;
            };
    //G4Track* track = aStep->GetTrack();
    //G4double kinEnergy = track->GetKineticEnergy();

    //if(edep==0.) return false;
    G4int trackID = step->GetTrack()->GetTrackID();

    //interaction position (in local coordinates) takes place where the step ends (or continuously along the step...)
    G4ThreeVector pos1 = step->GetPostStepPoint()->GetPosition();
    G4TouchableHandle theTouchable1 =
                                 step->GetPostStepPoint()->GetTouchableHandle();
            G4ThreeVector localpos1 =
                                   theTouchable1->GetHistory()->GetTopTransform().TransformPoint(pos1);
                                 //G4ThreeVector localpos1=pos1;

    //entry position in local coordinates
    G4ThreeVector pos2= step->GetPreStepPoint()->GetPosition();
    G4TouchableHandle theTouchable2 =
                 step->GetPreStepPoint()->GetTouchableHandle();
    G4ThreeVector localpos2 =
                           theTouchable2->GetHistory()->GetTopTransform().TransformPoint(pos2);
    //			     G4ThreeVector localpos2=pos1;
    //energy at start of step:
 G4double eInit=point1->GetKineticEnergy();
    //name of volume taken from prestep, as the poststep can be on the boundary (for transportation). The points on the boundry are put on the 2nd volume. Otherwise the two are the same.
    G4String name = step->GetPreStepPoint()->GetPhysicalVolume()->GetName();
//G4cout<<aStep->GetPostStepPoint()->GetPhysicalVolume()->GetName()<<G4endl;
    G4int copyNr=step->GetPreStepPoint()->GetPhysicalVolume()->GetCopyNo();
    //name of the particle making the step
    G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();

    G4double aTime = step->GetPreStepPoint()->GetGlobalTime();
    //G4cout<<"time is(ns):"<< aTime/ns<<G4endl;




    HPGeHit* newHit = new HPGeHit();
    newHit->SetEdep(edep);
    newHit->SetEinit(eInit);
    newHit->SetTrackID(trackID);
    newHit->SetIntPos(localpos1);
    newHit->SetEntryPos(localpos2);
    newHit->SetName(name);
    newHit->SetIncoming(incoming);
    newHit->SetOutgoing(outgoing);
    newHit->SetParticle(particleName);
    newHit->SetCopyNr(copyNr);
    newHit->SetTime(aTime);
    //newHit->Print();

    fHitsCollection->insert(newHit);




 /*   auto layerNumber = touchable->GetReplicaNumber(1);

    // Get hit accounting data for this cell
    auto hit = (*fHitsCollection)[layerNumber];
    if ( ! hit ) {
      G4ExceptionDescription msg;
      msg << "Cannot access hit " << layerNumber;
      G4Exception("HPGeSD::ProcessHits()",
        "MyCode0004", FatalException, msg);
    } */

    // Get hit for total accounting
   /* auto hitTotal
      = (*fHitsCollection)[fHitsCollection->entries()-1];

    // Add values
    hit->SetEdep(edep);
    hitTotal->SetEdep(edep);*/

    return true;

    /*
    HPGeHit* hit = new HPGeHit();

    G4double edep = step->GetTotalEnergyDeposit();
    hit->SetEdep(edep);

    hitsCollection->insert(hit);
            return true;
     */

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void HPGeSD::EndOfEvent(G4HCofThisEvent*)
{
    if ( verboseLevel>1 ) {
       auto nofHits = fHitsCollection->entries();
       G4cout
         << G4endl
         << "-------->Hits Collection: in this event they are " << nofHits
         << " hits in the tracker chambers: " << G4endl;
       for ( std::size_t i=0; i<nofHits; ++i ) (*fHitsCollection)[i]->Print();
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
