#include "AliAnalysisAlien.h"
#include "AliAnalysisDataContainer.h"
#include "AliAnalysisManager.h"
#include "AliESDInputHandler.h"
#include "AliVEventHandler.h"
#include "FourProngsTaskMC.h"
#include "TChain.h"
#include "TError.h"
#include "TInterpreter.h"
#include "TList.h"
#include "TROOT.h"
#include "TString.h"
#include "TTree.h"

FourProngsTaskMC *AddTaskUpc4Prongs() {
  //--- get the current analysis manager ---//
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddTask_Upc4Prongs", "No analysis manager found.");
    return 0;
  }

  // Check the analysis type using the event handlers connected to the analysis
  // manager.
  //==============================================================================
  if (!mgr->GetInputEventHandler()) {
    Error("AddTask_Upc4Prongs", "This task requires an input event handler");
    return 0;
  }

  TString inputDataType =
      mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"

  // Create tasks
  FourProngsTaskMC *task = new FourProngsTaskMC(inputDataType.Data());
  mgr->AddTask(task);

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  AliAnalysisDataContainer *coutput0 = mgr->CreateContainer(
      "DataTree", TTree::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:4Prongs", AliAnalysisManager::GetCommonFileName()));
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(
      "StartedRunsTree", TTree::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:StartedRunsTree", AliAnalysisManager::GetCommonFileName()));

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, coutput0);

  return task;
}
