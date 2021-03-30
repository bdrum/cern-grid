///
/// \file AliFemtoJetAnalysisTask.h
///

#ifndef ALIFEMTOJETANALYSIS_H
#define ALIFEMTOJETANALYSIS_H

#include "AliAnalysisTaskSE.h"

class AliFemtoCutMonitorEventMult;
class AliFemtoCutMonitorEventSphericity;
class AliFemtoCutMonitorDphiDeta;
class AliFemtoCutMonitorParticleYPt;
class AliFemtoCutMonitorParticlePID;
class AliFemtoESDTrackCutMinusJets;
class AliPIDResponse;
class TH1D;

class AliFemtoJetAnalysisTask : public AliAnalysisTaskSE {
public:
  AliFemtoJetAnalysisTask();
  AliFemtoJetAnalysisTask(const char *name = "AliFemtoAnalysisJets",
                          bool isMC = false);
  virtual ~AliFemtoJetAnalysisTask();
  virtual void Init();
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *){};

  void AddFemtoCutMonitorsEventMult(
      AliFemtoCutMonitorEventMult *cutPassEvMetaphitpc,
      AliFemtoCutMonitorEventMult *cutFailEvMetaphitpc) {
    fcutPassEvMetaphitpc = cutPassEvMetaphitpc;
    fcutFailEvMetaphitpc = cutFailEvMetaphitpc;
  }
  void AddFemtoCutMonitorsEventSphericity(
      AliFemtoCutMonitorEventSphericity *cutPassEvSpher,
      AliFemtoCutMonitorEventSphericity *cutFailEvSpher) {
    fcutPassEvSpher = cutPassEvSpher;
    fcutFailEvSpher = cutFailEvSpher;
  }
  void AddFemtoCutMonitorsEventDphiDeta(
      AliFemtoCutMonitorDphiDeta *cutPassDphiDeta,
      AliFemtoCutMonitorDphiDeta *cutFailDphiDeta) {
    fcutPassDphiDeta = cutPassDphiDeta;
    fcutFailDphiDeta = cutFailDphiDeta;
  }
  void AddFemtoCutMonitorsParticleYPt(
      AliFemtoCutMonitorParticleYPt *cutPassYPtetaphitpc,
      AliFemtoCutMonitorParticleYPt *cutFailYPtetaphitpc) {
    fcutPassYPtetaphitpc = cutPassYPtetaphitpc;
    fcutFailYPtetaphitpc = cutFailYPtetaphitpc;
  }
  void AddFemtoCutMonitorsParticlePID(
      AliFemtoCutMonitorParticlePID *cutPassPIDetaphitpc,
      AliFemtoCutMonitorParticlePID *cutFailPIDetaphitpc) {
    fcutPassPIDetaphitpc = cutPassPIDetaphitpc;
    fcutFailPIDetaphitpc = cutFailPIDetaphitpc;
  }
  void
  AddFemtoESDTrackCutMinusJets(AliFemtoESDTrackCutMinusJets *dtcetaphitpc1,
                               AliFemtoESDTrackCutMinusJets *dtcetaphitpc2) {
    fdtcetaphitpc1 = dtcetaphitpc1;
    fdtcetaphitpc2 = dtcetaphitpc2;
  }

private:
  TList *MultSelection;
  TList *PWG2FEMTO;
  AliPIDResponse *fPIDResponse;

  AliFemtoCutMonitorEventMult *fcutPassEvMetaphitpc;
  AliFemtoCutMonitorEventMult *fcutFailEvMetaphitpc;

  AliFemtoCutMonitorEventSphericity *fcutPassEvSpher;
  AliFemtoCutMonitorEventSphericity *fcutFailEvSpher;

  AliFemtoCutMonitorDphiDeta *fcutPassDphiDeta;
  AliFemtoCutMonitorDphiDeta *fcutFailDphiDeta;

  AliFemtoCutMonitorParticleYPt *fcutPassYPtetaphitpc;
  AliFemtoCutMonitorParticleYPt *fcutFailYPtetaphitpc;

  AliFemtoCutMonitorParticlePID *fcutPassPIDetaphitpc;
  AliFemtoCutMonitorParticlePID *fcutFailPIDetaphitpc;

  AliFemtoESDTrackCutMinusJets *fdtcetaphitpc1;
  AliFemtoESDTrackCutMinusJets *fdtcetaphitpc2;

  TH1D *fHist1;
  TH1D *fHist2;
  TH1D *fHist3;
  TH1D *fHist4;
  TH1D *fHist5;
  TH1D *fHist6;

  ClassDef(AliFemtoJetAnalysisTask, 1)
};

#endif
