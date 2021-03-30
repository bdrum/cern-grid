//_________________________________
// Femto analysis
//_________________________________

#include "AliFemtoJetAnalysisTask.h"

void runBatch_local() {

  gSystem->Load("libProofPlayer");
  gSystem->Load("libVMC");
  gSystem->Load("libPhysics");
  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gSystem->Load("libTender");
  gSystem->Load("libTenderSupplies");

  gSystem->Load("libCORRFW");
  gSystem->Load("libOADB");

  gSystem->Load("libTree");
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libPhysics");

  //____________________________________________________//
  //_____________Setting up required packages___________//
  //____________________________________________________//
  gSystem->Load("libSTEERBase");
  gSystem->Load("libESD");
  gSystem->Load("libAOD");
  gSystem->Load("libANALYSIS");
  gSystem->Load("libANALYSISalice");
  gROOT->ProcessLine(".include $ALICE_ROOT/include");

  gSystem->AddIncludePath("-I$ALICE_ROOT/include");
  gSystem->AddIncludePath("-I$ALICE_PHYSICS/include");

  //___________ Analysis  __________________________

  //______ Make the analysis manager
  AliAnalysisManager *mgr = new AliAnalysisManager("TestManager");

  auto *aodH = new AliESDInputHandler();
  mgr->SetInputEventHandler(aodH);
  mgr->SetDebugLevel(1); // 0, 1, 2, 3 ...

  gROOT->LoadMacro("AddTaskPIDResponse.C");
  AddTaskPIDResponse();

  // AddTaskMultSelection(kFALSE);

  //________AddTaskFemto_______________
  gInterpreter->LoadMacro("AliFemtoJetAnalysisTask.cxx++g");
  AliFemtoJetAnalysisTask *task = reinterpret_cast<AliFemtoJetAnalysisTask *>(
      gInterpreter->ExecuteMacro("AddTaskFemtoJets.C"));
  // gROOT->LoadMacro("AliFemtoJetAnalysisTask.cxx+g");
  // gROOT->LoadMacro("AddTaskFemtoJets.C+g");
  // AliFemtoJetAnalysisTask *taskfemto = AddTaskFemtoJets();
  // taskfemto->SelectCollisionCandidates(AliVEvent::kINT7);

  //____________________________________________//
  // Run the analysis
  if (!mgr->InitAnalysis())
    return;
  mgr->PrintStatus();
  TChain *chain = new TChain("esdTree");
  chain->Add("/mnt/d/GoogleDrive/Job/cern/Alice/analysis/data/RhoPrime/2015/"
             "245145.AliESDs.root");

  mgr->StartAnalysis("local", chain);
}
