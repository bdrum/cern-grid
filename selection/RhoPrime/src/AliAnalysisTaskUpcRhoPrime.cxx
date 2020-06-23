/*************************************************************************
 * Copyright(c) 1998-2020, ALICE Experiment at CERN, All rights reserved. *
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

#include <iostream>

#include "AliAnalysisTaskUpcRhoPrime.h"

#include "TDatabasePDG.h"
#include "TFile.h"
#include "TParticlePDG.h"
#include "TMath.h"
#include "TVectorDfwd.h"
#include "Math/Vector4D.h"

#include "AliAnalysisManager.h"
#include "AliInputEventHandler.h"
//#include "AliMultiplicity.h"
//#include "AliDataFile.h"
//#include "AliExternalTrackParam.h"

// #include "AliAODEvent.h"
// #include "AliAODVZERO.h"
// #include "AliAODZDC.h"
// #include "AliAODTrack.h"
// #include "AliAODPid.h"
// #include "AliAODVertex.h"
// #include "AliAODMCParticle.h"
// #include "AliAODMCHeader.h"

#include "AliESDEvent.h"
#include "AliESDVZERO.h"
#include "AliESDZDC.h"
#include "AliPIDResponse.h"
#include "AliESDVertex.h"
#include "AliESDtrack.h"

// TODO: create startup item based on runAnalysis.C macro or even better make from runAnalysis.C startup item
// TODO: add split for trigger string via ';'
// TODO: dynamic switch from esd to aod based on input data
// TODO: add test - comparison of results from local running on ESD data

ClassImp(AliAnalysisTaskUpcRhoPrime);

AliAnalysisTaskUpcRhoPrime::AliAnalysisTaskUpcRhoPrime()
    : AliAnalysisTaskSE(), fPIDResponse(0), fTriggerName(0), fRhoTree(0),
    BunchCrossNumber(0), OrbitNumber(0), PeriodNumber(0), RunNum(0), Mass(0),
    Q(-10), Pt(0), Rapidity(0), V0Adecision(0), V0Cdecision(0),
    ADAdecision(0), ADCdecision(0), UBAfired(0), UBCfired(0), VBAfired(0),
    VBCfired(0), ZNAenergy(0), ZNCenergy(0), ZPAenergy(0), ZPCenergy(0),
    VtxContrib(0), SpdVtxContrib(0), VtxChi2(0), VtxNDF(0), Ntracklets(0), nTracks(0),
    Phi(0), IsCUP2(0), IsCUP4(0), IsCUP9(0), IsC1ZED(0), PIDTPCPion(0), PIDTPCElectron(0),
    TPCsignal(0), TrackP(0), TrackEta(0), TrackPhi(0), TrackPx(0), TrackPy(0), 
    TrackPz(0), TrackQ(0), TrackHasPointOnITSLayer0(0), TrackHasPointOnITSLayer1(0), 
    TrackITSModuleInner(0), TrackITSModuleOuter(0) {}

AliAnalysisTaskUpcRhoPrime::AliAnalysisTaskUpcRhoPrime(const char* name)
    : AliAnalysisTaskSE(name), fPIDResponse(0), fTriggerName(0), fRhoTree(0),
    BunchCrossNumber(0), OrbitNumber(0), PeriodNumber(0), RunNum(0), Mass(0),
    Q(-10), Pt(0), Rapidity(0), V0Adecision(0), V0Cdecision(0),
    ADAdecision(0), ADCdecision(0), UBAfired(0), UBCfired(0), VBAfired(0),
    VBCfired(0), ZNAenergy(0), ZNCenergy(0), ZPAenergy(0), ZPCenergy(0),
    VtxContrib(0), SpdVtxContrib(0), VtxChi2(0), VtxNDF(0), Ntracklets(0), nTracks(0),
    Phi(0), IsCUP2(0), IsCUP4(0), IsCUP9(0), IsC1ZED(0), PIDTPCPion(0), PIDTPCElectron(0),
    TPCsignal(0), TrackP(0), TrackEta(0), TrackPhi(0), TrackPx(0), TrackPy(0), TrackPz(0),
    TrackQ(0), TrackHasPointOnITSLayer0(0), TrackHasPointOnITSLayer1(0), TrackITSModuleInner(0),
    TrackITSModuleOuter(0) {
    Init();
    DefineOutput(1, TTree::Class());
    DefineOutput(2, TList::Class());
}

AliAnalysisTaskUpcRhoPrime::~AliAnalysisTaskUpcRhoPrime()
{
    if (fRhoTree)            { delete fRhoTree;            fRhoTree            = nullptr; }
    if (fPIDResponse)        { delete fPIDResponse;        fPIDResponse        = nullptr; }
}

void AliAnalysisTaskUpcRhoPrime::Init()
{
    for (Int_t i = 0; i < 3; i++)
    {
        Vertex[i] = -1717;
        SpdVertex[i] = -1717;
        ZDCAtime[i] = -1717.;
        ZDCCtime[i] = -1717.;
    }

    ZDCAtime[3] = -1717.;
    ZDCCtime[3] = -1717.;

  /*  PIDTPCPion.reserve(200);
    PIDTPCElectron.reserve(200);
    TPCsignal.reserve(200);
    TrackP.reserve(200);
    TrackEta.reserve(200);
    TrackPhi.reserve(200);
    TrackPx.reserve(200);
    TrackPy.reserve(200);
    TrackPz.reserve(200);
    TrackQ.reserve(200);
    TrackITSModuleInner.reserve(200);
    TrackITSModuleOuter.reserve(200);
    TrackHasPointOnITSLayer0.reserve(200);
    TrackHasPointOnITSLayer1.reserve(200);*/
}

void AliAnalysisTaskUpcRhoPrime::UserCreateOutputObjects()
{
    AliAnalysisManager* man = AliAnalysisManager::GetAnalysisManager();
    AliInputEventHandler* inputHandler = (AliInputEventHandler*)(man->GetInputEventHandler());
    fPIDResponse = inputHandler->GetPIDResponse();

    fRhoTree = new TTree("events", "Selected events for 4proungs analysis");

    fRhoTree->Branch("RunNum",                     &RunNum,                   "RunNum/I");
    fRhoTree->Branch("PeriodNumber",               &PeriodNumber,             "PeriodNumber/i");
    fRhoTree->Branch("OrbitNumber",                &OrbitNumber,              "OrbitNumber/i");
    fRhoTree->Branch("BunchCrossNumber",           &BunchCrossNumber,         "BunchCrossNumber/s");
    fRhoTree->Branch("Mass",                       &Mass,                     "Mass/F");
    fRhoTree->Branch("Pt",                         &Pt,                       "Pt/F");
    fRhoTree->Branch("Q",                          &Q,                        "Q/S");
    fRhoTree->Branch("Rapidity",                   &Rapidity,                 "Rapidity/F");
    fRhoTree->Branch("Phi",                        &Phi,                      "Phi/F");
    fRhoTree->Branch("ZNAenergy",                  &ZNAenergy,                "ZNAenergy/F");
    fRhoTree->Branch("ZNCenergy",                  &ZNCenergy,                "ZNCenergy/F");
    fRhoTree->Branch("ZPAenergy",                  &ZPAenergy,                "ZPAenergy/F");
    fRhoTree->Branch("ZPCenergy",                  &ZPCenergy,                "ZPCenergy/F");
    fRhoTree->Branch("VtxX",                       &Vertex[0],                "VtxX/F");
    fRhoTree->Branch("VtxY",                       &Vertex[1],                "VtxY/F");
    fRhoTree->Branch("VtxZ",                       &Vertex[2],                "VtxZ/F");
    fRhoTree->Branch("VtxContrib",                 &VtxContrib,               "VtxContrib/I");
    fRhoTree->Branch("VtxChi2",                    &VtxChi2,                  "VtxChi2/F");
    fRhoTree->Branch("VtxNDF",                     &VtxNDF,                   "VtxNDF/F");
    fRhoTree->Branch("SpdVtxX",                    &SpdVertex[0],             "SpdVtxX/F");
    fRhoTree->Branch("SpdVtxY",                    &SpdVertex[1],             "SpdVtxY/F");
    fRhoTree->Branch("SpdVtxZ",                    &SpdVertex[2],             "SpdVtxZ/F");
    fRhoTree->Branch("SpdVtxContrib",              &SpdVtxContrib,            "SpdVtxContrib/I");
    fRhoTree->Branch("V0Adecision",                &V0Adecision,              "V0Adecision/I");
    fRhoTree->Branch("V0Cdecision",                &V0Cdecision,              "V0Cdecision/I");
    fRhoTree->Branch("ADAdecision",                &ADAdecision,              "ADAdecision/I");
    fRhoTree->Branch("ADCdecision",                &ADCdecision,              "ADCdecision/I");
    fRhoTree->Branch("UBAfired",                   &UBAfired,                 "UBAfired/O");
    fRhoTree->Branch("UBCfired",                   &UBCfired,                 "UBCfired/O");
    fRhoTree->Branch("VBAfired",                   &VBAfired,                 "VBAfired/O");
    fRhoTree->Branch("VBCfired",                   &VBCfired,                 "VBCfired/O");
    fRhoTree->Branch("Ntracklets",                 &Ntracklets,               "Ntracklets/I");
    fRhoTree->Branch("nTracks",                    &nTracks,                  "nTracks/I");
    fRhoTree->Branch("IsCUP2",                     &IsCUP2,                   "IsCUP2/O");
    fRhoTree->Branch("IsCUP4",                     &IsCUP4,                   "IsCUP4/O");
    fRhoTree->Branch("IsCUP9",                     &IsCUP9,                   "IsCUP9/O");
    fRhoTree->Branch("IsC1ZED",                    &IsC1ZED,                  "IsC1ZED/O");
    //fRhoTree->Branch("ZDCAtime",                   &ZDCAtime,                 "ZDCAtime[4]/F");
    //fRhoTree->Branch("ZDCCtime",                   &ZDCCtime,                 "ZDCCtime[4]/F");
    fRhoTree->Branch("PIDTPCPion",                 &PIDTPCPion); //,               "PIDTPCPion[4]/F");
    fRhoTree->Branch("PIDTPCElectron",             &PIDTPCElectron); //,           "PIDTPCElectron[4]/F");
    fRhoTree->Branch("TPCsignal",                  &TPCsignal); //,                "TPCsignal[4]/I");
    fRhoTree->Branch("TrackP",                     &TrackP); //,                   "TrackP[4]/F");
    fRhoTree->Branch("TrackEta",                   &TrackEta); //,                 "TrackEta[4]/F");
    fRhoTree->Branch("TrackPhi",                   &TrackPhi); //,                 "TrackPhi[4]/F");
    fRhoTree->Branch("TrackPx",                    &TrackPx); //,                  "TrackPx[4]/F");
    fRhoTree->Branch("TrackPy",                    &TrackPy); //,                  "TrackPy[4]/F");
    fRhoTree->Branch("TrackPz",                    &TrackPz); //,                  "TrackPz[4]/F");
    fRhoTree->Branch("TrackQ",                     &TrackQ); //,                   "TrackQ[4]/S");
    fRhoTree->Branch("TrackHasPointOnITSLayer0",   &TrackHasPointOnITSLayer0); //, "TrackHasPointOnITSLayer0[4]/O");
    fRhoTree->Branch("TrackHasPointOnITSLayer1",   &TrackHasPointOnITSLayer1); //, "TrackHasPointOnITSLayer1[4]/O");
    fRhoTree->Branch("TrackITSModuleInner",        &TrackITSModuleInner); //,      "TrackITSModuleInner[4]/I");
    fRhoTree->Branch("TrackITSModuleOuter",        &TrackITSModuleOuter); //,      "TrackITSModuleOuter[4]/I");
    //fRhoTree->Branch("ITSModule",                &ITSModule,                "ITSModule/I");

    PostData(1, fRhoTree);
}

bool AliAnalysisTaskUpcRhoPrime::TrackSelection(AliESDtrack* &trk)
{
    if (!trk)
        return false;
    if (trk->IsOn(AliESDtrack::kITSpureSA))
        return false;
    if (!(trk->GetStatus() & AliESDtrack::kTPCrefit))
        return false;
    if (!(trk->GetStatus() & AliESDtrack::kITSrefit))
        return false;
    if (trk->GetTPCNcls() < 50)
        return false;
    if (!((trk->HasPointOnITSLayer(0)) || (trk->HasPointOnITSLayer(1))))
        return false;

    // if(trk->GetTPCchi2()/trk->GetTPCNcls() > 4)continue;

    Float_t dca[2] = { 0.0, 0.0 };
   
    trk->GetImpactParameters(dca[0], dca[1]);
    if (TMath::Abs(dca[1]) > 2)
        return false;

    Double_t cut_DCAxy = (0.0182 + 0.0350 / TMath::Power(trk->Pt(), 1.01));
    if (TMath::Abs(dca[0]) > cut_DCAxy)
        return false;

    return true;
}

void AliAnalysisTaskUpcRhoPrime::UserExec(Option_t*)
{
    AliESDEvent* esd = (AliESDEvent*)InputEvent();
    if (!esd) return;

    PIDTPCPion.clear();
    PIDTPCElectron.clear();
    TPCsignal.clear();
    TrackP.clear();
    TrackEta.clear();
    TrackPhi.clear();
    TrackPx.clear();
    TrackPy.clear();
    TrackPz.clear();
    TrackQ.clear();
    TrackITSModuleInner.clear();
    TrackITSModuleOuter.clear();
    TrackHasPointOnITSLayer0.clear();
    TrackHasPointOnITSLayer1.clear();
    nTracks = 0;


    // triggered in data for lumi scalling
    // if (trigger.Contains(fTriggerName.Data())) {
    //   fHistTriggersPerRun->Fill(esd->GetRunNumber());
    //   // cout<<trigger<<endl;
    // }

    // CCUP9-B - *0VBA *0VBC *0UBA *0UBC 0STP
    // if (!trigger.Contains(fTriggerName.Data()))
    //   return;

    AliESDVertex* fESDVertex = (AliESDVertex*)esd->GetPrimaryVertex();
    TDatabasePDG* pdgdat = TDatabasePDG::Instance();
    TParticlePDG* partPion = pdgdat->GetParticle(211);
    Double_t pionMass = partPion->Mass();


    Int_t fFOsensores[240];
    for (Int_t i = 0; i < 240; i++)
        fFOsensores[i] = 0;

    for (Int_t chipkey = 0; chipkey < 1200; ++chipkey)
    {
        if (esd->GetMultiplicity()->TestFastOrFiredChips(chipkey))
        {
            fFOsensores[(chipkey / 5)]++;
        }
    }

    // event info
    RunNum = esd->GetRunNumber();
    OrbitNumber = esd->GetOrbitNumber();
    PeriodNumber = esd->GetPeriodNumber();
    BunchCrossNumber = esd->GetBunchCrossNumber();

    // VZERO, ZDC, AD
    AliESDVZERO* fV0data = esd->GetVZEROData();
    AliESDZDC* fZDCdata = esd->GetESDZDC();
    AliESDAD* fADdata = esd->GetADData();

    V0Adecision = fV0data->GetV0ADecision();
    V0Cdecision = fV0data->GetV0CDecision();

    if (fADdata)
    {
        ADAdecision = fADdata->GetADADecision();
        ADCdecision = fADdata->GetADCDecision();
    }

    UBAfired = esd->GetHeader()->IsTriggerInputFired("0UBA");
    UBCfired = esd->GetHeader()->IsTriggerInputFired("0UBC");
    VBAfired = esd->GetHeader()->IsTriggerInputFired("0VBA");
    VBCfired = esd->GetHeader()->IsTriggerInputFired("0VBC");

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
    AliESDVertex* fSPDVertex = (AliESDVertex*)esd->GetPrimaryVertexSPD();
    SpdVtxContrib = fSPDVertex->GetNContributors();
    SpdVertex[0] = fSPDVertex->GetX();
    SpdVertex[1] = fSPDVertex->GetY();
    SpdVertex[2] = fSPDVertex->GetZ();

    // Tracklets
    Ntracklets = esd->GetMultiplicity()->GetNumberOfTracklets();

    //std::vector<ROOT::Math::PtEtaPhiM4D<float>> TrackLV;
    //TrackLV.reserve(200);

    // Track loop - cuts
    for (Int_t i = 0; i < esd->GetNumberOfTracks(); ++i)
    {
        AliESDtrack* trk = esd->GetTrack(i);
        //if (!TrackSelection(trk)) continue;

      /*  AliExternalTrackParam cParam;
        if (!trk->RelateToVertex(fESDVertex, esd->GetMagneticField(), 300.,
            &cParam))
            continue;*/


        if (i >= 200) break;

        nTracks++;

        TrackITSModuleInner.push_back(trk->GetITSModuleIndex(0) / 1000000);
        TrackITSModuleOuter.push_back(trk->GetITSModuleIndex(1) / 1000000);

        TrackHasPointOnITSLayer0.push_back(trk->HasPointOnITSLayer(0));
        TrackHasPointOnITSLayer1.push_back(trk->HasPointOnITSLayer(1));

        if (fPIDResponse)
        {
            // TPC PID n-sigma
            PIDTPCElectron.push_back(fPIDResponse->NumberOfSigmasTPC(trk, AliPID::kElectron));
            PIDTPCPion.push_back(fPIDResponse->NumberOfSigmasTPC(trk, AliPID::kPion));
        }

        TrackQ.push_back(trk->Charge());
        TPCsignal.push_back(trk->GetTPCsignal());
        TrackP.push_back(trk->P());
        TrackPhi.push_back(trk->Phi());
        TrackEta.push_back(trk->Eta());
        TrackPx.push_back(trk->Px());
        TrackPy.push_back(trk->Py());
        TrackPz.push_back(trk->Pz());

        //ROOT::Math::PtEtaPhiM4D<float> tracVec(trk->Pt(), trk->Eta(), trk->Phi(), pionMass);
        //TrackLV.push_back(tracVec);

    } // end loop over tracks

      /*  std::cout << "======================" << std::endl;
        std::cout << esd->GetNumberOfTracks() << std::endl;
        std::cout << nTracks << std::endl;
        std::cout << TrackPx.size() << std::endl;
        std::cout << "======================" << std::endl;*/

  /*  PIDTPCPion.shrink_to_fit();
    PIDTPCElectron.shrink_to_fit();
    TPCsignal.shrink_to_fit();
    TrackP.shrink_to_fit();
    TrackEta.shrink_to_fit();
    TrackPhi.shrink_to_fit();
    TrackPx.shrink_to_fit();
    TrackPy.shrink_to_fit();
    TrackPz.shrink_to_fit();
    TrackQ.shrink_to_fit();
    TrackITSModuleInner.shrink_to_fit();
    TrackITSModuleOuter.shrink_to_fit();
    TrackHasPointOnITSLayer0.shrink_to_fit();
    TrackHasPointOnITSLayer1.shrink_to_fit();*/


    if (nTracks < 4) return;

    /* Q     = TrackQ[0] + TrackQ[1] + TrackQ[2] + TrackQ[3];
     Mass     = lvSum.M();
     Pt       = lvSum.Pt();
     Rapidity = lvSum.Rapidity();
     Phi      = lvSum.Phi();*/

     // virtual cut on FO chip matching
  /*   Int_t SPDInner[20];
     for (Int_t i = 0; i < 20; ++i)
         SPDInner[i] = 0;
     Int_t SPDOuter[40];
     for (Int_t i = 0; i < 40; ++i)
         SPDOuter[i] = 0;

     for (auto i = 0; i < 4; ++i)
     {
         SPDInner[TrackITSModuleInner[i] / 4]++;
         SPDOuter[(TrackITSModuleOuter[i] - 80) / 4]++;
     }*/

    IsCUP2 = 0;
    IsCUP4 = 0;
    IsCUP9 = 0;
    IsC1ZED = 0;


    /*  Bool_t fFOsensoresFired = 0;
      for (Int_t j = 0; j < 4; ++j)
          fFOsensoresFired += fFOsensores[TrackITSModuleInner[j]];

      if (fFOsensoresFired || !Is0STPfired(SPDInner, SPDOuter))
      {*/
    TString trigger = esd->GetFiredTriggerClasses();
    if (trigger.Contains("CCUP9"))
        IsCUP9 = 1;
    if (trigger.Contains("CCUP2"))
        IsCUP2 = 1;
    if (trigger.Contains("CCUP4"))
        IsCUP4 = 1;
    if (trigger.Contains("C1ZED"))
        IsC1ZED = 1;

    fRhoTree->Fill();

    PostData(1, fRhoTree);

} // UserExec

