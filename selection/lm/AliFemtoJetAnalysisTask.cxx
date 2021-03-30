///
/// \file AliFemtoJetAnalysis.cxx
///

#include "AliFemtoJetAnalysisTask.h"
#include "AliAnalysisTaskSE.h"
#include "TChain.h"
#include "TList.h"
#include "TString.h"

#include "AliFemtoCutMonitorDphiDeta.h"
#include "AliFemtoCutMonitorEventMult.h"
#include "AliFemtoCutMonitorEventSphericity.h"
#include "AliFemtoCutMonitorParticlePID.h"
#include "AliFemtoCutMonitorParticleYPt.h"
#include "AliFemtoESDTrackCutMinusJets.h"

#include "AliESD.h"
#include "AliESDEvent.h"
#include "AliESDVZERO.h"
#include "AliESDtrack.h"
#include "AliVTrack.h"

#include "Math/Vector4D.h"
#include "TChain.h"
#include "TDatabasePDG.h"
#include "TFile.h"
#include "TMath.h"
#include "TParticlePDG.h"
#include "TVectorDfwd.h"

#include "AliAnalysisManager.h"
#include "AliDataFile.h"
#include "AliExternalTrackParam.h"
#include "AliInputEventHandler.h"
#include "AliMultiplicity.h"

#include "AliAODEvent.h"
#include "AliAODMCHeader.h"
#include "AliAODMCParticle.h"
#include "AliAODPid.h"
#include "AliAODTrack.h"
#include "AliAODVZERO.h"
#include "AliAODVertex.h"
#include "AliAODZDC.h"

#include "AliESDVertex.h"
#include "AliESDZDC.h"
#include "AliPIDResponse.h"

ClassImp(AliFemtoJetAnalysisTask);

AliFemtoJetAnalysisTask::AliFemtoJetAnalysisTask()
    : AliAnalysisTaskSE(), fPIDResponse(0), fcutPassEvMetaphitpc(),
      fcutFailEvMetaphitpc(), fcutPassEvSpher(), fcutFailEvSpher(),
      fcutPassDphiDeta(), fcutFailDphiDeta(), fcutPassYPtetaphitpc(),
      fcutFailYPtetaphitpc(), fcutPassPIDetaphitpc(), fcutFailPIDetaphitpc(),
      fdtcetaphitpc1(), fdtcetaphitpc2(), fHist1(), fHist2(), fHist3(),
      fHist4(), fHist5(), fHist6(), PWG2FEMTO(), MultSelection() {}

AliFemtoJetAnalysisTask::AliFemtoJetAnalysisTask(const char *name, bool isMC)
    : AliAnalysisTaskSE(name), fPIDResponse(0), fcutPassEvMetaphitpc(),
      fcutFailEvMetaphitpc(), fcutPassEvSpher(), fcutFailEvSpher(),
      fcutPassDphiDeta(), fcutFailDphiDeta(), fcutPassYPtetaphitpc(),
      fcutFailYPtetaphitpc(), fcutPassPIDetaphitpc(), fcutFailPIDetaphitpc(),
      fdtcetaphitpc1(), fdtcetaphitpc2(), fHist1(), fHist2(), fHist3(),
      fHist4(), fHist5(), fHist6(), PWG2FEMTO(), MultSelection() {
  DefineInput(0, TChain::Class());
  DefineOutput(1, TList::Class());
  DefineOutput(2, TList::Class());
}

AliFemtoJetAnalysisTask::~AliFemtoJetAnalysisTask() {
  if (MultSelection) {
    delete MultSelection;
    MultSelection = nullptr;
  }

  if (PWG2FEMTO) {
    delete PWG2FEMTO;
    PWG2FEMTO = nullptr;
  }

  if (fPIDResponse) {
    delete fPIDResponse;
    fPIDResponse = nullptr;
  }

  if (fcutPassEvMetaphitpc) {
    delete fcutPassEvMetaphitpc;
    fcutPassEvMetaphitpc = nullptr;
  }

  if (fcutFailEvMetaphitpc) {
    delete fcutFailEvMetaphitpc;
    fcutFailEvMetaphitpc = nullptr;
  }

  if (fcutPassEvSpher) {
    delete fcutPassEvSpher;
    fcutPassEvSpher = nullptr;
  }

  if (fcutFailEvSpher) {
    delete fcutFailEvSpher;
    fcutFailEvSpher = nullptr;
  }

  if (fcutPassDphiDeta) {
    delete fcutPassDphiDeta;
    fcutPassDphiDeta = nullptr;
  }

  if (fcutFailDphiDeta) {
    delete fcutFailDphiDeta;
    fcutFailDphiDeta = nullptr;
  }

  if (fcutPassYPtetaphitpc) {
    delete fcutPassYPtetaphitpc;
    fcutPassYPtetaphitpc = nullptr;
  }

  if (fcutFailYPtetaphitpc) {
    delete fcutFailYPtetaphitpc;
    fcutFailYPtetaphitpc = nullptr;
  }

  if (fcutPassPIDetaphitpc) {
    delete fcutPassPIDetaphitpc;
    fcutPassPIDetaphitpc = nullptr;
  }

  if (fcutFailPIDetaphitpc) {
    delete fcutFailPIDetaphitpc;
    fcutFailPIDetaphitpc = nullptr;
  }

  if (fdtcetaphitpc1) {
    delete fdtcetaphitpc1;
    fdtcetaphitpc1 = nullptr;
  }

  if (fdtcetaphitpc2) {
    delete fdtcetaphitpc2;
    fdtcetaphitpc2 = nullptr;
  }

  if (fHist1) {
    delete fHist1;
    fHist1 = nullptr;
  }
  if (fHist2) {
    delete fHist2;
    fHist2 = nullptr;
  }
  if (fHist3) {
    delete fHist3;
    fHist3 = nullptr;
  }
  if (fHist4) {
    delete fHist4;
    fHist4 = nullptr;
  }
  if (fHist5) {
    delete fHist5;
    fHist5 = nullptr;
  }
  if (fHist6) {
    delete fHist6;
    fHist6 = nullptr;
  }
}

void AliFemtoJetAnalysisTask::Init() {}

void AliFemtoJetAnalysisTask::UserCreateOutputObjects() {
  AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
  AliInputEventHandler *inputHandler =
      (AliInputEventHandler *)(man->GetInputEventHandler());
  fPIDResponse = inputHandler->GetPIDResponse();

  OpenFile(1);

  MultSelection = new TList();
  MultSelection->SetOwner();

  fHist1 = new TH1D("hist1", "hist1", 100, -1, 1);
  fHist2 = new TH1D("hist2", "hist2", 100, -2, 2);
  fHist3 = new TH1D("hist3", "hist3", 100, 0, 2);

  MultSelection->Add(fHist1);
  MultSelection->Add(fHist2);
  MultSelection->Add(fHist3);

  PostData(1, MultSelection);

  OpenFile(2);

  PWG2FEMTO = new TList();
  PWG2FEMTO->SetOwner();

  fHist4 = new TH1D("hist4", "hist4", 100, -4, 4);
  fHist5 = new TH1D("hist5", "hist5", 200, 0, 200);
  fHist6 = new TH1D("hist6", "hist6", 100, -6, 6);

  PWG2FEMTO->Add(fHist4);
  PWG2FEMTO->Add(fHist5);
  PWG2FEMTO->Add(fHist6);

  PostData(2, PWG2FEMTO);
}

void AliFemtoJetAnalysisTask::UserExec(Option_t *) {
  auto *aod = (AliESDEvent *)InputEvent();
  if (!aod)
    return;

  int nTracklets = aod->GetMultiplicity()->GetNumberOfTracklets();

  fHist6->Fill(aod->GetPrimaryVertex()->GetNContributors());
  fHist5->Fill(nTracklets);
  for (int i = 0; i < nTracklets; ++i) {
    fHist4->Fill(aod->GetMultiplicity()->GetPhi(i));
  }

  PostData(2, PWG2FEMTO);

  double EventPtMax = 0.0, MonitorParam = 0.0;

  if (fcutPassDphiDeta && fcutFailDphiDeta) {
    MonitorParam = fcutPassDphiDeta->fPtmax;
  }

  for (int i = 0; i < aod->GetNumberOfTracks(); ++i) {
    auto *trk = aod->GetTrack(i);
    if (!trk)
      continue;

    auto pt_t = trk->Pt();
    if (pt_t >= EventPtMax)
      EventPtMax = pt_t;
  }

  // Track loop - cuts
  for (int i = 0; i < aod->GetNumberOfTracks(); ++i) {
    auto *trk = aod->GetTrack(i);
    if (!trk)
      continue;

    auto pt_t = trk->Pt();
    if (pt_t >= EventPtMax / 3 && pt_t > MonitorParam) {
      fHist3->Fill(pt_t);
      std::cout << "EventPtMax: " << EventPtMax << " trk n: " << i << " pt "
                << pt_t << " MonitorParam" << MonitorParam << std::endl;
    }
    if (i > 100)
      break;
  }
  PostData(1, MultSelection);
}