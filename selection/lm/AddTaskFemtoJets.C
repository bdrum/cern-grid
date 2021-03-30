#include "AliAnalysisAlien.h"
#include "AliAnalysisDataContainer.h"
#include "AliAnalysisManager.h"
#include "AliFemtoCutMonitorDphiDeta.h"
#include "AliFemtoJetAnalysisTask.h"
#include "AliVEventHandler.h"
#include "TChain.h"
#include "TError.h"
#include "TInterpreter.h"
#include "TList.h"
#include "TROOT.h"
#include "TString.h"

AliFemtoJetAnalysisTask *AddTaskFemtoJets() {
  //--- get the current analysis manager ---//
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
  if (!mgr) {
    Error("AddTask_FemtoJets", "No analysis manager found.");
    return 0;
  }

  // Check the analysis type using the event handlers connected to the analysis
  // manager.
  //==============================================================================
  if (!mgr->GetInputEventHandler()) {
    Error("AddTask_FemtoJets", "This task requires an input event handler");
    return 0;
  }

  TString inputDataType =
      mgr->GetInputEventHandler()->GetDataType(); // can be "ESD" or "AOD"

  // Create tasks
  AliFemtoJetAnalysisTask *task =
      new AliFemtoJetAnalysisTask(inputDataType.Data());

  auto cutPassDphiDeta =
      new AliFemtoCutMonitorDphiDeta(Form("cutPassEv%stpcM%i", "Kp", 0));
  cutPassDphiDeta->fPtmax = 0.2;
  auto cutFailDphiDeta =
      new AliFemtoCutMonitorDphiDeta(Form("cutFailEv%stpcM%i", "Kp", 0));
  task->AddFemtoCutMonitorsEventDphiDeta(cutPassDphiDeta, cutFailDphiDeta);

  mgr->AddTask(task);

  // Create containers for input/output
  AliAnalysisDataContainer *cinput = mgr->GetCommonInputContainer();
  AliAnalysisDataContainer *coutput0 = mgr->CreateContainer(
      "MultiSelection", TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:FemtoTask1", AliAnalysisManager::GetCommonFileName()));
  AliAnalysisDataContainer *coutput1 = mgr->CreateContainer(
      "PWG2FEMTO", TList::Class(), AliAnalysisManager::kOutputContainer,
      Form("%s:FemtoTask2", AliAnalysisManager::GetCommonFileName()));

  // Connect input/output
  mgr->ConnectInput(task, 0, cinput);
  mgr->ConnectOutput(task, 1, coutput0);
  mgr->ConnectOutput(task, 2, coutput1);

  return task;
}
