#include "AliAnalysisAlien.h"
#include "AliAnalysisDataContainer.h"
#include "AliAnalysisManager.h"
#include "AliAnalysisTaskUpcRhoPrime.h"
#include "AliESDInputHandler.h"
#include "AliVEventHandler.h"
#include "TChain.h"
#include "TError.h"
#include "TInterpreter.h"
#include "TList.h"
#include "TROOT.h"
#include "TString.h"
#include "TTree.h"

AliAnalysisTaskUpcRhoPrime *AddTaskUpcRhoPrime()
{
  //--- get the current analysis manager ---//
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr)
  {
    Error("AddTask_UpcRhoPrime", "No analysis manager found.");
    return 0;
  }

  // Check the analysis type using the event handlers connected to the analysis
  // manager.
  //==============================================================================
  if (!mgr->GetInputEventHandler())
  {
    Error("AddTask_UpcRhoPrime", "This task requires an input event handler");
    return 0;
  }

  TString inputDataType =
      mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"

  // Create tasks
  AliAnalysisTaskUpcRhoPrime *task =
      new AliAnalysisTaskUpcRhoPrime(inputDataType.Data());
  mgr->AddTask(task);

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(
      "RhoPrimeTree", TTree::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:RhoPrimeCentral", AliAnalysisManager::GetCommonFileName()));
  AliAnalysisDataContainer *coutput2 = mgr->CreateContainer(
      "Histograms", TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:RhoPrimeCentral", AliAnalysisManager::GetCommonFileName()));
  AliAnalysisDataContainer *coutput3 = NULL;

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, coutput1);
  // mgr->ConnectOutput(task, 2, coutput2);

  return task;
}
