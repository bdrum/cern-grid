/*************************************************************************
 * Copyright(c) 1998-2021, ALICE Experiment at CERN, All rights reserved. *
 *                                                                        *
 * Author: The ALICE Off-line Project.                                    *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

#include "AliAODEvent.h"
#include "AliAODTrack.h"
#include "AliAODVZERO.h"
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

#include "AliAODVertex.h"
#include "AliAODZDC.h"
#include "AliPIDResponse.h"

#include "FourProngsTaskMC.h"

// TODO: add split for trigger string via ';'
// TODO: dynamic switch from aod to aod based on input data
// TODO: add tests - comparison of results from local running on aod data

ClassImp(FourProngsTaskMC);

FourProngsTaskMC::FourProngsTaskMC()
    : AliAnalysisTaskSE(), fPIDResponse(0), fTriggerName(0), fRhoTree(0),
      fStartedRunsTree(0), BunchCrossNumber(0), OrbitNumber(0), PeriodNumber(0),
      RunNum(0), Mass(0), Q(-10), Pt(0), Rapidity(0), V0Adecision(0),
      V0Cdecision(0), ADAdecision(0), ADCdecision(0), ZNAenergy(0),
      ZNCenergy(0), ZPAenergy(0), ZPCenergy(0), VtxContrib(0), SpdVtxContrib(0),
      VtxChi2(0), VtxNDF(0), nTracklets(0), nTracks(0), Phi(0),
      T_NumberOfSigmaTPCPion(0), T_NumberOfSigmaTPCElectron(0),
      T_NumberOfSigmaITSPion(0), T_NumberOfSigmaITSElectron(0), T_TPCsignal(0),
      T_P(0), T_Eta(0), T_Phi(0), T_Px(0), T_Py(0), T_Pz(0), T_Q(0),
      T_HasPointOnITSLayer0(0), T_HasPointOnITSLayer1(0), T_ITSModuleInner(0),
      T_ITSModuleOuter(0), T_TPCNCls(0), T_ITSNCls(0), T_Dca0(0), T_Dca1(0),
      T_TPCRefit(0), T_ITSRefit(0), T_Lets_Theta(0), T_Lets_Phi(0),
      StartedRuns(0), MCPart_PdgCode(0), MCPart_Px(0), MCPart_Py(0),
      MCPart_Pz(0) {}

FourProngsTaskMC::FourProngsTaskMC(const char *name)
    : AliAnalysisTaskSE(name), fPIDResponse(0), fTriggerName(0), fRhoTree(0),
      fStartedRunsTree(0), BunchCrossNumber(0), OrbitNumber(0), PeriodNumber(0),
      RunNum(0), Mass(0), Q(-10), Pt(0), Rapidity(0), V0Adecision(0),
      V0Cdecision(0), ADAdecision(0), ADCdecision(0), ZNAenergy(0),
      ZNCenergy(0), ZPAenergy(0), ZPCenergy(0), VtxContrib(0), SpdVtxContrib(0),
      VtxChi2(0), VtxNDF(0), nTracklets(0), nTracks(0), Phi(0),
      T_NumberOfSigmaTPCPion(0), T_NumberOfSigmaTPCElectron(0),
      T_NumberOfSigmaITSPion(0), T_NumberOfSigmaITSElectron(0), T_TPCsignal(0),
      T_P(0), T_Eta(0), T_Phi(0), T_Px(0), T_Py(0), T_Pz(0), T_Q(0),
      T_HasPointOnITSLayer0(0), T_HasPointOnITSLayer1(0), T_ITSModuleInner(0),
      T_ITSModuleOuter(0), T_TPCNCls(0), T_ITSNCls(0), T_Dca0(0), T_Dca1(0),
      T_TPCRefit(0), T_ITSRefit(0), T_Lets_Theta(0), T_Lets_Phi(0),
      StartedRuns(0), MCPart_PdgCode(0), MCPart_Px(0), MCPart_Py(0),
      MCPart_Pz(0) {

  DefineInput(0, TChain::Class());
  DefineOutput(1, TTree::Class());
}

FourProngsTaskMC::~FourProngsTaskMC() {
  if (fRhoTree) {
    delete fRhoTree;
    fRhoTree = nullptr;
  }
  if (fPIDResponse) {
    delete fPIDResponse;
    fPIDResponse = nullptr;
  }
}

void FourProngsTaskMC::Init() {
  for (int i = 0; i < 3; i++) {
    Vertex[i] = -1717;
    SpdVertex[i] = -1717;
    ZDCAtime[i] = -1717.;
    ZDCCtime[i] = -1717.;
  }
  ZDCAtime[3] = -1717.;
  ZDCCtime[3] = -1717.;

  ClearTracksVectors();
  // 235 - is the average number of tracks in event
  // 11000 - is the max number of tracks in event
  ReserveTracksVectors(11000);
}

void FourProngsTaskMC::UserCreateOutputObjects() {
  auto *man = AliAnalysisManager::GetAnalysisManager();
  auto *inputHandler = (AliInputEventHandler *)(man->GetInputEventHandler());
  fPIDResponse = inputHandler->GetPIDResponse();
  OpenFile(1);
  fRhoTree = new TTree("events", "Selected events for 4proungs analysis");

  fRhoTree->Branch("RunNum", &RunNum, "RunNum/I");
  fRhoTree->Branch("PeriodNumber", &PeriodNumber, "PeriodNumber/i");
  fRhoTree->Branch("OrbitNumber", &OrbitNumber, "OrbitNumber/i");
  fRhoTree->Branch("BunchCrossNumber", &BunchCrossNumber, "BunchCrossNumber/i");
  fRhoTree->Branch("Mass", &Mass, "Mass/F");
  fRhoTree->Branch("Pt", &Pt, "Pt/F");
  fRhoTree->Branch("Q", &Q, "Q/I");
  fRhoTree->Branch("Rapidity", &Rapidity, "Rapidity/F");
  fRhoTree->Branch("Phi", &Phi, "Phi/F");
  fRhoTree->Branch("ZNAenergy", &ZNAenergy, "ZNAenergy/F");
  fRhoTree->Branch("ZNCenergy", &ZNCenergy, "ZNCenergy/F");
  fRhoTree->Branch("ZPAenergy", &ZPAenergy, "ZPAenergy/F");
  fRhoTree->Branch("ZPCenergy", &ZPCenergy, "ZPCenergy/F");
  fRhoTree->Branch("VtxX", &Vertex[0], "VtxX/F");
  fRhoTree->Branch("VtxY", &Vertex[1], "VtxY/F");
  fRhoTree->Branch("VtxZ", &Vertex[2], "VtxZ/F");
  fRhoTree->Branch("VtxContrib", &VtxContrib, "VtxContrib/I");
  fRhoTree->Branch("VtxChi2", &VtxChi2, "VtxChi2/F");
  fRhoTree->Branch("VtxNDF", &VtxNDF, "VtxNDF/F");
  fRhoTree->Branch("SpdVtxX", &SpdVertex[0], "SpdVtxX/F");
  fRhoTree->Branch("SpdVtxY", &SpdVertex[1], "SpdVtxY/F");
  fRhoTree->Branch("SpdVtxZ", &SpdVertex[2], "SpdVtxZ/F");
  fRhoTree->Branch("SpdVtxContrib", &SpdVtxContrib, "SpdVtxContrib/I");
  fRhoTree->Branch("V0Adecision", &V0Adecision, "V0Adecision/I");
  fRhoTree->Branch("V0Cdecision", &V0Cdecision, "V0Cdecision/I");
  fRhoTree->Branch("ADAdecision", &ADAdecision, "ADAdecision/I");
  fRhoTree->Branch("ADCdecision", &ADCdecision, "ADCdecision/I");

  fRhoTree->Branch("nTracklets", &nTracklets, "nTracklets/I");
  fRhoTree->Branch("nTracks", &nTracks, "nTracks/I");

  fRhoTree->Branch("ZDCAtime_0", &ZDCAtime[0], "ZDCAtime_0/F");
  fRhoTree->Branch("ZDCAtime_1", &ZDCAtime[1], "ZDCAtime_1/F");
  fRhoTree->Branch("ZDCAtime_2", &ZDCAtime[2], "ZDCAtime_2/F");
  fRhoTree->Branch("ZDCAtime_3", &ZDCAtime[3], "ZDCAtime_3/F");

  fRhoTree->Branch("ZDCCtime_0", &ZDCCtime[0], "ZDCCtime_0/F");
  fRhoTree->Branch("ZDCCtime_1", &ZDCCtime[1], "ZDCCtime_1/F");
  fRhoTree->Branch("ZDCCtime_2", &ZDCCtime[2], "ZDCCtime_2/F");
  fRhoTree->Branch("ZDCCtime_3", &ZDCCtime[3], "ZDCCtime_3/F");

  fRhoTree->Branch("MCPart_PdgCode", &MCPart_PdgCode);
  fRhoTree->Branch("MCPart_Px", &MCPart_Px);
  fRhoTree->Branch("MCPart_Py", &MCPart_Py);
  fRhoTree->Branch("MCPart_Pz", &MCPart_Pz);

  fRhoTree->Branch("T_NumberOfSigmaITSPion", &T_NumberOfSigmaITSPion);
  fRhoTree->Branch("T_NumberOfSigmaITSElectron", &T_NumberOfSigmaITSElectron);
  fRhoTree->Branch("T_NumberOfSigmaTPCPion", &T_NumberOfSigmaTPCPion);
  fRhoTree->Branch("T_NumberOfSigmaTPCElectron", &T_NumberOfSigmaTPCElectron);
  fRhoTree->Branch("TPCsignal", &T_TPCsignal);
  fRhoTree->Branch("T_P", &T_P);
  fRhoTree->Branch("T_Eta", &T_Eta);
  fRhoTree->Branch("T_Phi", &T_Phi);
  fRhoTree->Branch("T_Px", &T_Px);
  fRhoTree->Branch("T_Py", &T_Py);
  fRhoTree->Branch("T_Pz", &T_Pz);
  fRhoTree->Branch("T_Q", &T_Q);
  fRhoTree->Branch("T_HasPointOnITSLayer0", &T_HasPointOnITSLayer0);
  fRhoTree->Branch("T_HasPointOnITSLayer1", &T_HasPointOnITSLayer1);
  fRhoTree->Branch("T_ITSModuleInner", &T_ITSModuleInner);
  fRhoTree->Branch("T_ITSModuleOuter", &T_ITSModuleOuter);
  fRhoTree->Branch("T_TPCNCls", &T_TPCNCls);
  fRhoTree->Branch("T_ITSNCls", &T_ITSNCls);
  fRhoTree->Branch("T_Dca0", &T_Dca0);
  fRhoTree->Branch("T_Dca1", &T_Dca1);
  fRhoTree->Branch("T_TPCRefit", &T_TPCRefit);
  fRhoTree->Branch("T_ITSRefit", &T_ITSRefit);
  fRhoTree->Branch("TLets_Theta", &T_Lets_Theta);
  fRhoTree->Branch("TLets_Phi", &T_Lets_Phi);

  PostData(1, fRhoTree);
}

void FourProngsTaskMC::UserExec(Option_t *) {
  auto *aod = (AliAODEvent *)InputEvent();
  if (!aod)
    return;

  TString trigger = aod->GetFiredTriggerClasses();

  nTracks = 0;
  Q = 0;

  auto *fAODVertex = (AliAODVertex *)aod->GetPrimaryVertex();
  double dca[2] = {0.0, 0.0}, cov[3] = {0., 0., 0.};

  auto *pdgdat = TDatabasePDG::Instance();
  auto *partPion = pdgdat->GetParticle(211);
  auto pionMass = partPion->Mass();

  Init();

  // event info
  RunNum = aod->GetRunNumber();
  OrbitNumber = aod->GetOrbitNumber();
  PeriodNumber = aod->GetPeriodNumber();
  BunchCrossNumber = aod->GetBunchCrossNumber();

  // VZERO, ZDC, AD
  auto *fV0data = aod->GetVZEROData();
  // auto *fZDCdata = aod->GetAODZDC();
  auto *fADdata = aod->GetADData();

  V0Adecision = fV0data->GetV0ADecision();
  V0Cdecision = fV0data->GetV0CDecision();

  if (fADdata) {
    ADAdecision = fADdata->GetADADecision();
    ADCdecision = fADdata->GetADCDecision();
  }

  // ZN energy
  // ZNAenergy = fZDCdata->GetZNATowerEnergy()[0];
  // ZNCenergy = fZDCdata->GetZNCTowerEnergy()[0];
  // ZPAenergy = fZDCdata->GetZPATowerEnergy()[0];
  // ZPCenergy = fZDCdata->GetZPCTowerEnergy()[0];

  // // neutron ZDC time
  // int detChZNA = fZDCdata->GetZNATDCChannel();
  // int detChZNC = fZDCdata->GetZNCTDCChannel();

  // for (auto i = 0; i < 4; ++i) {
  //   ZDCAtime[i] = fZDCdata->GetZDCTDCCorrected(detChZNA, i);
  //   ZDCCtime[i] = fZDCdata->GetZDCTDCCorrected(detChZNC, i);
  // }

  // primary vertex
  VtxContrib = fAODVertex->GetNContributors();
  Vertex[0] = fAODVertex->GetX();
  Vertex[1] = fAODVertex->GetY();
  Vertex[2] = fAODVertex->GetZ();
  VtxChi2 = fAODVertex->GetChi2();
  VtxNDF = fAODVertex->GetNDF();

  // SPD primary vertex
  auto *fSPDVertex = (AliAODVertex *)aod->GetPrimaryVertexSPD();
  SpdVtxContrib = fSPDVertex->GetNContributors();
  SpdVertex[0] = fSPDVertex->GetX();
  SpdVertex[1] = fSPDVertex->GetY();
  SpdVertex[2] = fSPDVertex->GetZ();

  // Tracklets
  nTracklets = aod->GetMultiplicity()->GetNumberOfTracklets();

  for (auto i = 0; i < nTracklets; ++i) {
    T_Lets_Theta.push_back(aod->GetMultiplicity()->GetTheta(i));
    T_Lets_Phi.push_back(aod->GetMultiplicity()->GetPhi(i));
  }

  auto *arrayMC = (TClonesArray *)aod->GetList()->FindObject(
      AliAODMCParticle::StdBranchName());

  int ngen = 0;
  float pxsum = 0., pysum = 0., pzsum = 0., esum = 0.;
  for (Int_t imc = 0; imc < arrayMC->GetEntriesFast(); imc++) {
    AliAODMCParticle *aodmc =
        dynamic_cast<AliAODMCParticle *>(arrayMC->At(imc));
    if (!aodmc)
      continue;

    if (aodmc->GetMother() > 0 || fabs(aodmc->GetPdgCode()) != 211)
      continue;

    MCPart_PdgCode.push_back(aodmc->GetPdgCode());
    MCPart_Px.push_back(aodmc->Px());
    MCPart_Py.push_back(aodmc->Py());
    MCPart_Pz.push_back(aodmc->Pz());
  }

  ShrinkToFitTracksVectors();

  fRhoTree->Fill();

  PostData(1, fRhoTree);

  return;

  std::vector<ROOT::Math::PxPyPzMVector> EventVectors;

  // Track loop - cuts
  for (int i = 0; i < aod->GetNumberOfTracks(); ++i) {
    AliAODTrack *trk = dynamic_cast<AliAODTrack *>(aod->GetTrack(i));
    if (!trk)
      continue;

    if (nTracks > 2)
      break;

    if (!(trk->GetFilterMap() & 1 << 0) && !(trk->GetFilterMap() & 1 << 1) &&
        trk->AliAODTrack::GetITSNcls() * trk->AliAODTrack::GetTPCNcls() == 0)
      continue;

    nTracks++;
    Q += trk->Charge();
    trk->PropagateToDCA(fAODVertex, aod->GetMagneticField(), 300., dca, cov);
    // T_ITSModuleInner.push_back(trk->GetITSModuleIndex(0));
    // T_ITSModuleOuter.push_back(trk->GetITSModuleIndex(1));

    T_HasPointOnITSLayer0.push_back(trk->HasPointOnITSLayer(0));
    T_HasPointOnITSLayer1.push_back(trk->HasPointOnITSLayer(1));

    // T_ITSNCls.push_back(trk->GetNumberOfITSClusters());
    // T_TPCNCls.push_back(trk->GetNumberOfTPCClusters());
    T_TPCRefit.push_back(trk->IsOn(AliAODTrack::kTPCrefit));
    T_ITSRefit.push_back(trk->IsOn(AliAODTrack::kITSrefit));

    // TPC&ITS PID n-sigma
    T_NumberOfSigmaTPCElectron.push_back(
        fPIDResponse->NumberOfSigmasTPC(trk, AliPID::kElectron));
    T_NumberOfSigmaTPCPion.push_back(
        fPIDResponse->NumberOfSigmasTPC(trk, AliPID::kPion));
    T_NumberOfSigmaITSPion.push_back(
        fPIDResponse->NumberOfSigmasITS(trk, AliPID::kPion));
    T_NumberOfSigmaITSElectron.push_back(
        fPIDResponse->NumberOfSigmasITS(trk, AliPID::kElectron));

    T_Q.push_back(trk->Charge());
    T_TPCsignal.push_back(trk->GetTPCsignal());
    T_P.push_back(trk->P());
    T_Phi.push_back(trk->Phi());
    T_Eta.push_back(trk->Eta());
    T_Px.push_back(trk->Px());
    T_Py.push_back(trk->Py());
    T_Pz.push_back(trk->Pz());

    T_Dca0.push_back(dca[0]);
    T_Dca1.push_back(dca[1]);

    ROOT::Math::PxPyPzMVector trackV(trk->Px(), trk->Py(), trk->Pz(), pionMass);
    EventVectors.push_back(trackV);

  } // end loop over tracks

  // if (nTracks < 4)
  // return;

  ROOT::Math::PxPyPzMVector sumVector;
  for (const auto &tv : EventVectors)
    sumVector += tv;

  Mass = sumVector.M();
  Pt = sumVector.Pt();
  Rapidity = sumVector.Rapidity();
  Phi = sumVector.Phi();

  ShrinkToFitTracksVectors();

  fRhoTree->Fill();

  PostData(1, fRhoTree);

} // UserExec

void FourProngsTaskMC::ClearTracksVectors() {
  T_NumberOfSigmaITSPion.clear();
  T_NumberOfSigmaITSElectron.clear();
  T_NumberOfSigmaTPCPion.clear();
  T_NumberOfSigmaTPCElectron.clear();
  T_TPCsignal.clear();
  T_TPCNCls.clear();
  T_ITSNCls.clear();
  T_P.clear();
  T_Eta.clear();
  T_Phi.clear();
  T_Px.clear();
  T_Py.clear();
  T_Pz.clear();
  T_Dca0.clear();
  T_Dca1.clear();
  T_Q.clear();
  T_TPCRefit.clear();
  T_ITSRefit.clear();
  T_HasPointOnITSLayer0.clear();
  T_HasPointOnITSLayer1.clear();
  T_ITSModuleInner.clear();
  T_ITSModuleOuter.clear();
  T_Lets_Theta.clear();
  T_Lets_Phi.clear();
}

void FourProngsTaskMC::ReserveTracksVectors(int size) {
  T_NumberOfSigmaITSPion.reserve(size);
  T_NumberOfSigmaITSElectron.reserve(size);
  T_NumberOfSigmaTPCPion.reserve(size);
  T_NumberOfSigmaTPCElectron.reserve(size);
  T_P.reserve(size);
  T_Eta.reserve(size);
  T_Phi.reserve(size);
  T_Px.reserve(size);
  T_Py.reserve(size);
  T_Pz.reserve(size);
  T_Dca0.reserve(size);
  T_Dca1.reserve(size);
  T_Lets_Theta.reserve(size);
  T_Lets_Phi.reserve(size);
  T_TPCRefit.reserve(size);
  T_ITSRefit.reserve(size);
  T_HasPointOnITSLayer0.reserve(size);
  T_HasPointOnITSLayer1.reserve(size);
  T_ITSModuleInner.reserve(size);
  T_ITSModuleOuter.reserve(size);
  T_TPCsignal.reserve(size);
  T_TPCNCls.reserve(size);
  T_ITSNCls.reserve(size);
  T_Q.reserve(size);
  MCPart_PdgCode.reserve(size);
  MCPart_Px.reserve(size);
  MCPart_Py.reserve(size);
  MCPart_Pz.reserve(size);
}

void FourProngsTaskMC::ShrinkToFitTracksVectors() {
  T_NumberOfSigmaITSPion.shrink_to_fit();
  T_NumberOfSigmaITSElectron.shrink_to_fit();
  T_NumberOfSigmaTPCPion.shrink_to_fit();
  T_NumberOfSigmaTPCElectron.shrink_to_fit();
  T_TPCsignal.shrink_to_fit();
  T_TPCNCls.shrink_to_fit();
  T_ITSNCls.shrink_to_fit();
  T_P.shrink_to_fit();
  T_Eta.shrink_to_fit();
  T_Phi.shrink_to_fit();
  T_Px.shrink_to_fit();
  T_Py.shrink_to_fit();
  T_Pz.shrink_to_fit();
  T_Dca0.shrink_to_fit();
  T_Dca1.shrink_to_fit();
  T_Q.shrink_to_fit();
  T_TPCRefit.shrink_to_fit();
  T_ITSRefit.shrink_to_fit();
  T_HasPointOnITSLayer0.shrink_to_fit();
  T_HasPointOnITSLayer1.shrink_to_fit();
  T_ITSModuleInner.shrink_to_fit();
  T_ITSModuleOuter.shrink_to_fit();
  T_Lets_Theta.shrink_to_fit();
  T_Lets_Phi.shrink_to_fit();
  MCPart_PdgCode.shrink_to_fit();
  MCPart_Px.shrink_to_fit();
  MCPart_Py.shrink_to_fit();
  MCPart_Pz.shrink_to_fit();
}
