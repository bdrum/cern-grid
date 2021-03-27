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

#include "FourProngsTask.h"

// TODO: add split for trigger string via ';'
// TODO: dynamic switch from esd to aod based on input data
// TODO: add tests - comparison of results from local running on ESD data

ClassImp(FourProngsTask);

FourProngsTask::FourProngsTask()
    : AliAnalysisTaskSE(), fPIDResponse(0), fTriggerName(0), fRhoTree(0),
      fStartedRunsTree(0), BunchCrossNumber(0), OrbitNumber(0), PeriodNumber(0),
      RunNum(0), Mass(0), Q(-10), Pt(0), Rapidity(0), V0Adecision(0),
      V0Cdecision(0), ADAdecision(0), ADCdecision(0), V0Afired(0), V0Cfired(0),
      ADAfired(0), ADCfired(0), STPfired(0), SMBfired(0), SM2fired(0),
      SH1fired(0), OM2fired(0), OMUfired(0), IsTriggered(0), ZNAenergy(0),
      ZNCenergy(0), ZPAenergy(0), ZPCenergy(0), VtxContrib(0), SpdVtxContrib(0),
      VtxChi2(0), VtxNDF(0), nTracklets(0), nTracks(0), Phi(0),
      T_NumberOfSigmaTPCPion(0), T_NumberOfSigmaTPCElectron(0),
      T_NumberOfSigmaITSPion(0), T_NumberOfSigmaITSElectron(0), T_TPCsignal(0),
      T_P(0), T_Eta(0), T_Phi(0), T_Px(0), T_Py(0), T_Pz(0), T_Q(0),
      T_HasPointOnITSLayer0(0), T_HasPointOnITSLayer1(0), T_ITSModuleInner(0),
      T_ITSModuleOuter(0), T_TPCNCls(0), T_ITSNCls(0), T_Dca0(0), T_Dca1(0),
      T_TPCRefit(0), T_ITSRefit(0), T_Lets_Theta(0), T_Lets_Phi(0), FORChip(0),
      StartedRuns(0) {}

FourProngsTask::FourProngsTask(const char *name)
    : AliAnalysisTaskSE(name), fPIDResponse(0), fTriggerName(0), fRhoTree(0),
      fStartedRunsTree(0), BunchCrossNumber(0), OrbitNumber(0), PeriodNumber(0),
      RunNum(0), Mass(0), Q(-10), Pt(0), Rapidity(0), V0Adecision(0),
      V0Cdecision(0), ADAdecision(0), ADCdecision(0), V0Afired(0), V0Cfired(0),
      ADAfired(0), ADCfired(0), STPfired(0), SMBfired(0), SM2fired(0),
      SH1fired(0), OM2fired(0), OMUfired(0), IsTriggered(0), ZNAenergy(0),
      ZNCenergy(0), ZPAenergy(0), ZPCenergy(0), VtxContrib(0), SpdVtxContrib(0),
      VtxChi2(0), VtxNDF(0), nTracklets(0), nTracks(0), Phi(0),
      T_NumberOfSigmaTPCPion(0), T_NumberOfSigmaTPCElectron(0),
      T_NumberOfSigmaITSPion(0), T_NumberOfSigmaITSElectron(0), T_TPCsignal(0),
      T_P(0), T_Eta(0), T_Phi(0), T_Px(0), T_Py(0), T_Pz(0), T_Q(0),
      T_HasPointOnITSLayer0(0), T_HasPointOnITSLayer1(0), T_ITSModuleInner(0),
      T_ITSModuleOuter(0), T_TPCNCls(0), T_ITSNCls(0), T_Dca0(0), T_Dca1(0),
      T_TPCRefit(0), T_ITSRefit(0), T_Lets_Theta(0), T_Lets_Phi(0), FORChip(0),
      StartedRuns(0) {
  DefineInput(0, TChain::Class());
  DefineOutput(1, TTree::Class());
  DefineOutput(2, TTree::Class());
}

FourProngsTask::~FourProngsTask() {
  if (fRhoTree) {
    delete fRhoTree;
    fRhoTree = nullptr;
  }
  if (fStartedRunsTree) {
    delete fStartedRunsTree;
    fStartedRunsTree = nullptr;
  }
  if (fPIDResponse) {
    delete fPIDResponse;
    fPIDResponse = nullptr;
  }
}

void FourProngsTask::Init() {
  for (Int_t i = 0; i < 3; i++) {
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
  // 1084 is the max number of fired FORs for event
  FORChip.reserve(1084);
}

void FourProngsTask::UserCreateOutputObjects() {
  AliAnalysisManager *man = AliAnalysisManager::GetAnalysisManager();
  AliInputEventHandler *inputHandler =
      (AliInputEventHandler *)(man->GetInputEventHandler());
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

  fRhoTree->Branch("V0Afired", &V0Afired, "V0Afired/O");
  fRhoTree->Branch("V0Cfired", &V0Cfired, "V0Cfired/O");
  fRhoTree->Branch("ADAfired", &ADAfired, "ADAfired/O");
  fRhoTree->Branch("ADCfired", &ADCfired, "ADCfired/O");
  fRhoTree->Branch("STPfired", &STPfired, "STPfired/O");
  fRhoTree->Branch("SMBfired", &SMBfired, "SMBfired/O");
  fRhoTree->Branch("SM2fired", &SM2fired, "SM2fired/O");
  fRhoTree->Branch("SH1fired", &SH1fired, "SH1fired/O");
  fRhoTree->Branch("OM2fired", &OM2fired, "OM2fired/O");
  fRhoTree->Branch("OMUfired", &OMUfired, "OMUfired/O");
  fRhoTree->Branch("IsTriggered", &IsTriggered, "IsTriggered/O");

  fRhoTree->Branch("nTracklets", &nTracklets, "nTracklets/I");
  fRhoTree->Branch("nTracks", &nTracks, "nTracks/I");
  // fRhoTree->Branch("ZDCAtime",                               &ZDCAtime,
  // "ZDCAtime[4]/F"); fRhoTree->Branch("ZDCCtime", &ZDCCtime, "ZDCCtime[4]/F");
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
  fRhoTree->Branch("FORChip", &FORChip);

  PostData(1, fRhoTree);

  OpenFile(2);

  fStartedRunsTree =
      new TTree("StartedRuns",
                "Run numbers that has been started and not necessary passed");

  fStartedRunsTree->Branch("StartedRuns", &StartedRuns, "StartedRuns/I");

  PostData(2, fStartedRunsTree);
}

void FourProngsTask::UserExec(Option_t *) {
  AliESDEvent *esd = (AliESDEvent *)InputEvent();
  if (!esd)
    return;

  TString trigger = esd->GetFiredTriggerClasses();
  if (!trigger.Contains("CCUP9-B"))
    return;
  StartedRuns = esd->GetRunNumber();
  fStartedRunsTree->Fill();
  PostData(2, fStartedRunsTree);

  IsTriggered = kFALSE;
  nTracks = 0;
  Q = 0;

  AliESDVertex *fESDVertex = (AliESDVertex *)esd->GetPrimaryVertex();
  TDatabasePDG *pdgdat = TDatabasePDG::Instance();
  TParticlePDG *partPion = pdgdat->GetParticle(211);
  Double_t pionMass = partPion->Mass();

  Init();

  // event info
  RunNum = esd->GetRunNumber();
  OrbitNumber = esd->GetOrbitNumber();
  PeriodNumber = esd->GetPeriodNumber();
  BunchCrossNumber = esd->GetBunchCrossNumber();

  // VZERO, ZDC, AD
  AliESDVZERO *fV0data = esd->GetVZEROData();
  AliESDZDC *fZDCdata = esd->GetESDZDC();
  AliESDAD *fADdata = esd->GetADData();

  V0Adecision = fV0data->GetV0ADecision();
  V0Cdecision = fV0data->GetV0CDecision();

  if (fADdata) {
    ADAdecision = fADdata->GetADADecision();
    ADCdecision = fADdata->GetADCDecision();
  }

  // ZN energy
  ZNAenergy = fZDCdata->GetZNATowerEnergy()[0];
  ZNCenergy = fZDCdata->GetZNCTowerEnergy()[0];
  ZPAenergy = fZDCdata->GetZPATowerEnergy()[0];
  ZPCenergy = fZDCdata->GetZPCTowerEnergy()[0];

  // neutron ZDC time
  Int_t detChZNA = fZDCdata->GetZNATDCChannel();
  Int_t detChZNC = fZDCdata->GetZNCTDCChannel();

  /* for (Int_t i = 0; i < 4; i++)
   {
       ZDCAtime[i] = fZDCdata->GetZDCTDCCorrected(detChZNA, i);
       ZDCCtime[i] = fZDCdata->GetZDCTDCCorrected(detChZNC, i);
   }*/

  // primary vertex
  VtxContrib = fESDVertex->GetNContributors();
  Vertex[0] = fESDVertex->GetX();
  Vertex[1] = fESDVertex->GetY();
  Vertex[2] = fESDVertex->GetZ();
  VtxChi2 = fESDVertex->GetChi2();
  VtxNDF = fESDVertex->GetNDF();

  // SPD primary vertex
  AliESDVertex *fSPDVertex = (AliESDVertex *)esd->GetPrimaryVertexSPD();
  SpdVtxContrib = fSPDVertex->GetNContributors();
  SpdVertex[0] = fSPDVertex->GetX();
  SpdVertex[1] = fSPDVertex->GetY();
  SpdVertex[2] = fSPDVertex->GetZ();

  // Tracklets
  nTracklets = esd->GetMultiplicity()->GetNumberOfTracklets();

  for (auto i = 0; i < nTracklets; ++i) {
    T_Lets_Theta.push_back(esd->GetMultiplicity()->GetTheta(i));
    T_Lets_Phi.push_back(esd->GetMultiplicity()->GetPhi(i));
  }

  std::vector<ROOT::Math::PxPyPzMVector> EventVectors;

  // std::cout << RunNum << " | " << PeriodNumber << " | " << OrbitNumber << " |
  // " << BunchCrossNumber << " | " << esd->GetNumberOfTracks() << std::endl;

  // Track loop - cuts
  for (Int_t i = 0; i < esd->GetNumberOfTracks(); ++i) {
    AliESDtrack *trk = esd->GetTrack(i);
    if (!trk)
      continue;
    if (!trk->HasPointOnITSLayer(0) && !trk->HasPointOnITSLayer(1))
      continue;
    Float_t dca[2] = {0.0, 0.0};
    trk->GetImpactParameters(dca[0], dca[1]);
    if (TMath::Abs(dca[1]) > 3)
      continue;
    if (TMath::Abs(dca[0]) > 3)
      continue;
    if (trk->IsPureITSStandalone())
      continue;
    if (trk->GetNumberOfITSClusters() < 3)
      continue;

    // if (nTracks >= 200) break;

    // std::cout << i << " | " << dca[0] << " | " << dca[1] << " | " <<
    // trk->GetNumberOfITSClusters() << " | " << trk->HasPointOnITSLayer(0) << "
    // | " << trk->HasPointOnITSLayer(1) << std::endl;

    nTracks++;
    Q += trk->Charge();

    T_ITSModuleInner.push_back(trk->GetITSModuleIndex(0));
    T_ITSModuleOuter.push_back(trk->GetITSModuleIndex(1));

    T_HasPointOnITSLayer0.push_back(trk->HasPointOnITSLayer(0));
    T_HasPointOnITSLayer1.push_back(trk->HasPointOnITSLayer(1));

    T_ITSNCls.push_back(trk->GetNumberOfITSClusters());
    T_TPCNCls.push_back(trk->GetNumberOfTPCClusters());
    T_TPCRefit.push_back(trk->IsOn(AliESDtrack::kTPCrefit));
    T_ITSRefit.push_back(trk->IsOn(AliESDtrack::kITSrefit));

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

  if (nTracks < 4)
    return;

  IsTriggered = CheckEventTrigger(esd);

  for (Int_t chipkey = 0; chipkey < 1200; chipkey++) {
    if (esd->GetMultiplicity()->TestFastOrFiredChips(chipkey))
      FORChip.push_back(chipkey);
  }

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

Bool_t FourProngsTask::Is0STPfired(Int_t *vPhiInner,
                                   Int_t *vPhiOuter) // array 20, 40
{
  Int_t fired(0);
  for (Int_t i(0); i < 10; ++i) {
    for (Int_t j(0); j < 2; ++j) {
      const Int_t k(2 * i + j);
      fired += ((vPhiOuter[k] || vPhiOuter[k + 1] || vPhiOuter[k + 2]) &&
                (vPhiOuter[k + 20] || vPhiOuter[(k + 21) % 40] ||
                 vPhiOuter[(k + 22) % 40]) &&
                (vPhiInner[i] || vPhiInner[i + 1]) &&
                (vPhiInner[i + 10] || vPhiInner[(i + 11) % 20]));
    }
  }
  if (fired != 0)
    return kTRUE;
  else
    return kFALSE;
}

// return kTRUE if CCUP9 triggered was fired
Bool_t FourProngsTask::CheckEventTrigger(AliESDEvent *esd) {
  V0Afired = kFALSE;
  V0Cfired = kFALSE;
  ADAfired = kFALSE;
  ADCfired = kFALSE;
  STPfired = kFALSE;
  SMBfired = kFALSE;
  SM2fired = kFALSE;
  SH1fired = kFALSE;
  OM2fired = kFALSE;
  OMUfired = kFALSE;

  // SPD inputs
  // Int_t bcMod4 = 0;
  // if (isUsingEffi) bcMod4 = TMath::Nint(hBCmod4->GetRandom());

  AliMultiplicity *mult = esd->GetMultiplicity();
  Int_t vPhiInner[20];
  for (Int_t i = 0; i < 20; ++i)
    vPhiInner[i] = 0;
  Int_t vPhiOuter[40];
  for (Int_t i = 0; i < 40; ++i)
    vPhiOuter[i] = 0;

  Int_t nInner(0), nOuter(0);

  for (Int_t i(0); i < 1200; ++i) {
    // Double_t eff = 1;
    // if (isUsingEffi) eff = hSPDeff->GetBinContent(1+i, 1+bcMod4);

    Bool_t isFired =
        (mult->TestFastOrFiredChips(i)); //&& (gRandom->Uniform(0,1) < eff);
    if (i < 400) {
      vPhiInner[i / 20] += isFired;
      nInner += isFired;
    } else {
      vPhiOuter[(i - 400) / 20] += isFired;
      nOuter += isFired;
    }
  }

  // 0STP
  STPfired = Is0STPfired(vPhiInner, vPhiOuter);
  // 0SMB - At least one hit in SPD
  if (nOuter > 0 || nInner > 0)
    SMBfired = kTRUE;
  // 0SM2 - Two hits on outer layer
  if (nOuter > 1)
    SM2fired = kTRUE;
  // 0SH1 - More then 6 hits on outer layer
  // if (nOuter >= 7) SH1 = kTRUE;
  // 0SH1 2017 - Two hits on inner and outer layer
  if (nInner >= 2 && nOuter >= 2)
    SH1fired = kTRUE;

  // V0
  V0Afired = esd->GetHeader()->IsTriggerInputFired("0VBA");
  V0Cfired = esd->GetHeader()->IsTriggerInputFired("0VBC");
  // AD
  ADAfired = esd->GetHeader()->IsTriggerInputFired("0UBA");
  ADCfired = esd->GetHeader()->IsTriggerInputFired("0UBC");
  // TOF
  OMUfired = esd->GetHeader()->IsTriggerInputFired("0OMU");

  // OM2
  // if (isUsingTOFeff) {
  // const AliTOFHeader *tofH = esd->GetTOFHeader();
  // fTOFmask = tofH->GetTriggerMask();

  // Bool_t firedMaxiPhi[36] = {0};
  // Int_t NfiredMaxiPads = 0;

  // for(Int_t ltm=0;ltm<72;ltm++){
  // Int_t ip = ltm%36;
  // for(Int_t cttm=0;cttm<23;cttm++){
  // if(fTOFmask->IsON(ltm,cttm) &&
  // gRandom->Rndm(1.0)<hTOFeff->GetBinContent(ltm+1,cttm+1)){ firedMaxiPhi[ip]
  // = kTRUE; NfiredMaxiPads++;
  // }
  // }
  // }
  // if(NfiredMaxiPads >= 2) {
  // OM2 = kTRUE; //0OM2 TOF two hits
  // }

  // }
  // else OM2 = esd->GetHeader()->IsTriggerInputFired("0OM2");
  OM2fired = esd->GetHeader()->IsTriggerInputFired("0OM2");

  // save spd and tof trigger decisions to tree
  // TriggerSPD_T = SH1;
  // TriggerTOF_T = OM2;

  if ((fTriggerName == "CCUP9-B") &&
      (!V0Afired && !V0Cfired && !ADAfired && !ADCfired && STPfired))
    return kTRUE; // CCUP9 is fired
  else
    return kFALSE;

  // if (fOption.Contains("17n")) {
  // 	if ((fTriggerName.Contains("CCUP2")) && (!V0Afired && !V0Cfired)) return
  // kTRUE; // CCUP2 in 17n
  // 	}
  // else {
  // 	if ((fTriggerName.Contains("CCUP2")) && (!V0Afired && !V0Cfired &&
  // SM2fired && OM2fired)) return kTRUE; // CCUP2 is fired works only in 2015
  // }
  // if ((fTriggerName == "CCUP4-B") && (!V0A && !V0C && SM2 && OMU)) return
  // kTRUE; // CCUP4 is fired works only in 2015

  // else return kFALSE;
} // end of MC trigger

void FourProngsTask::ClearTracksVectors() {
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
  FORChip.clear();
}

void FourProngsTask::ReserveTracksVectors(int size) {
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
}
void FourProngsTask::ShrinkToFitTracksVectors() {
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
  FORChip.shrink_to_fit();
}
