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
/// \file EDEventAction.cc
/// \brief Implementation of the EDEventAction class

#include "EDEventAction.hh"
#include "HPGeSD.hh"
#include "HPGeHit.hh"
#include "EDAnalysis.hh"


#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4UnitsTable.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDEventAction::EDEventAction()
 : G4UserEventAction(),
   fAbsHCID(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EDEventAction::~EDEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

HPGeHitsCollection*
EDEventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection
    = static_cast<HPGeHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID;
    G4Exception("EDEventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }

  return hitsCollection;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDEventAction::PrintEventStatistics(
                              G4double absoEdep) const
{
  // print event statistics
  G4cout
     << "   Absorber: total energy: "
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDEventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EDEventAction::EndOfEventAction(const G4Event* event)
{
    // Get hits collections IDs (only once)
    if ( fAbsHCID == -1 ) {
      fAbsHCID
        = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection");
    }

    // Get hits collections
    auto absoHC = GetHitsCollection(fAbsHCID, event);

    // Get hit with total values
    auto absoHit = (*absoHC)[absoHC->entries()-1];

    // Print per event (modulo n)
    //
    auto eventID = event->GetEventID();
    auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
    if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
      G4cout << "---> End of event: " << eventID << G4endl;

      PrintEventStatistics(
        absoHit->GetEdep());
    }

    // Fill histograms, ntuple
    //

    // get analysis manager
    auto analysisManager = G4AnalysisManager::Instance();

    // fill histograms
    analysisManager->FillH1(0, absoHit->GetEdep());

    // fill ntuple
    analysisManager->FillNtupleDColumn(0, absoHit->GetEdep());
    analysisManager->AddNtupleRow();



}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
