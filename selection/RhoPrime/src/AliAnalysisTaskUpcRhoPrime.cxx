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
    VtxContrib(0), SpdVtxContrib(0), VtxChi2(0), VtxNDF(0), Ntracklets(0),
    Phi(0), IsCUP2(0), IsCUP4(0), IsCUP9(0), IsC1ZED(0), fListHist(0),
    fHistTriggersPerRun(0), fITSmodule(0), fFOchip(0), fFOcount(0),
    TPCclustersP(0), TPCclustersN(0), dEdx(0), EtaPhiP(0), EtaPhiN(0),
    fFOcorr(0), fGoodTracks(0), fTrackChi2(0) {}

AliAnalysisTaskUpcRhoPrime::AliAnalysisTaskUpcRhoPrime(const char* name)
    : AliAnalysisTaskSE(name), fPIDResponse(0), fTriggerName(0), fRhoTree(0),
    BunchCrossNumber(0), OrbitNumber(0), PeriodNumber(0), RunNum(0), Mass(0),
    Q(-10), Pt(0), Rapidity(0), V0Adecision(0), V0Cdecision(0),
    ADAdecision(0), ADCdecision(0), UBAfired(0), UBCfired(0), VBAfired(0),
    VBCfired(0), ZNAenergy(0), ZNCenergy(0), ZPAenergy(0), ZPCenergy(0),
    VtxContrib(0), SpdVtxContrib(0), VtxChi2(0), VtxNDF(0), Ntracklets(0),
    Phi(0), IsCUP2(0), IsCUP4(0), IsCUP9(0), IsC1ZED(0), fListHist(0),
    fHistTriggersPerRun(0), fITSmodule(0), fFOchip(0), fFOcount(0),
    TPCclustersP(0), TPCclustersN(0), dEdx(0), EtaPhiP(0), EtaPhiN(0),
    fFOcorr(0), fGoodTracks(0), fTrackChi2(0)
{
    Init();
    DefineOutput(1, TTree::Class());
    DefineOutput(2, TList::Class());
}

AliAnalysisTaskUpcRhoPrime::~AliAnalysisTaskUpcRhoPrime()
{
    if (fRhoTree)            { delete fRhoTree;            fRhoTree            = nullptr; }
    if (fListHist)           { delete fListHist;           fListHist           = nullptr; }
    if (fPIDResponse)        { delete fPIDResponse;        fPIDResponse        = nullptr; }
    if (fListHist)           { delete fListHist;           fListHist           = nullptr; }
    if (fHistTriggersPerRun) { delete fHistTriggersPerRun; fHistTriggersPerRun = nullptr; }
    if (fITSmodule)          { delete fITSmodule;          fITSmodule          = nullptr; }
    if (fFOchip)             { delete fFOchip;             fFOchip             = nullptr; }
    if (fFOcount)            { delete fFOcount;            fFOcount            = nullptr; }
    if (TPCclustersP)        { delete TPCclustersP;        TPCclustersP        = nullptr; }
    if (TPCclustersN)        { delete TPCclustersN;        TPCclustersN        = nullptr; }
    if (dEdx)                { delete dEdx;                dEdx                = nullptr; }
    if (EtaPhiP)             { delete EtaPhiP;             EtaPhiP             = nullptr; }
    if (EtaPhiN)             { delete EtaPhiN;             EtaPhiN             = nullptr; }
    if (fFOcorr)             { delete fFOcorr;             fFOcorr             = nullptr; }
    if (fGoodTracks)         { delete fGoodTracks;         fGoodTracks         = nullptr; }
    if (fTrackChi2)          { delete fTrackChi2;          fTrackChi2          = nullptr; }
}

void AliAnalysisTaskUpcRhoPrime::Init()
{
    for (Int_t i = 0; i < 3; i++)
    {
        Vertex[i]    = -1212;
        SpdVertex[i] = -1212;
    }
    for (Int_t i = 0; i < 4; i++)
    {
        PIDTPCPion[i]               = -1212.;
        PIDTPCElectron[i]           = -1212.;
        TPCsignal[i]                = -1212;
        TrackP[i]                   = -1212.;
        TrackEta[i]                 = -1212.;
        TrackPhi[i]                 = -1212.;
        ZDCAtime[i]                 = -1212.;
        ZDCCtime[i]                 = -1212.;
        TrackPx[i]                  = -1212.;
        TrackPy[i]                  = -1212.;
        TrackPz[i]                  = -1212.;
        TrackQ[i]                   = -1212.;
        TrackITSModuleInner[i]      = -1212;
        TrackITSModuleOuter[i]      = -1212;
        TrackHasPointOnITSLayer0[i] = -1212;
        TrackHasPointOnITSLayer1[i] = -1212;
    }
}

void AliAnalysisTaskUpcRhoPrime::UserCreateOutputObjects()
{
    AliAnalysisManager* man = AliAnalysisManager::GetAnalysisManager();
    AliInputEventHandler* inputHandler = (AliInputEventHandler*)(man->GetInputEventHandler());
    fPIDResponse = inputHandler->GetPIDResponse();

    fRhoTree = new TTree("events", "Selected RhoPrime events");

    fRhoTree->Branch("RunNum",                   &RunNum,                   "RunNum/I");
    fRhoTree->Branch("PeriodNumber",             &PeriodNumber,             "PeriodNumber/i");
    fRhoTree->Branch("OrbitNumber",              &OrbitNumber,              "OrbitNumber/i");
    fRhoTree->Branch("BunchCrossNumber",         &BunchCrossNumber,         "BunchCrossNumber/s");
    fRhoTree->Branch("Mass",                     &Mass,                     "Mass/F");
    fRhoTree->Branch("Pt",                       &Pt,                       "Pt/F");
    fRhoTree->Branch("Q",                        &Q,                        "Q/S");
    fRhoTree->Branch("Rapidity",                 &Rapidity,                 "Rapidity/F");
    fRhoTree->Branch("Phi",                      &Phi,                      "Phi/F");
    fRhoTree->Branch("ZNAenergy",                &ZNAenergy,                "ZNAenergy/F");
    fRhoTree->Branch("ZNCenergy",                &ZNCenergy,                "ZNCenergy/F");
    fRhoTree->Branch("ZPAenergy",                &ZPAenergy,                "ZPAenergy/F");
    fRhoTree->Branch("ZPCenergy",                &ZPCenergy,                "ZPCenergy/F");
    fRhoTree->Branch("ZDCAtime",                 &ZDCAtime,                 "ZDCAtime[4]/F");
    fRhoTree->Branch("ZDCCtime",                 &ZDCCtime,                 "ZDCCtime[4]/F");
    fRhoTree->Branch("PIDTPCPion",               &PIDTPCPion,               "PIDTPCPion[4]/F");
    fRhoTree->Branch("PIDTPCElectron",           &PIDTPCElectron,           "PIDTPCElectron[4]/F");
    fRhoTree->Branch("TPCsignal",                &TPCsignal,                "TPCsignal[4]/I");
    fRhoTree->Branch("TrackP",                   &TrackP,                   "TrackP[4]/F");
    fRhoTree->Branch("TrackEta",                 &TrackEta,                 "TrackEta[4]/F");
    fRhoTree->Branch("TrackPhi",                 &TrackPhi,                 "TrackPhi[4]/F");
    fRhoTree->Branch("TrackPx",                  &TrackPx,                  "TrackPx[4]/F");
    fRhoTree->Branch("TrackPy",                  &TrackPy,                  "TrackPy[4]/F");
    fRhoTree->Branch("TrackPz",                  &TrackPz,                  "TrackPz[4]/F");
    fRhoTree->Branch("TrackQ",                   &TrackQ,                   "TrackQ[4]/S");
    fRhoTree->Branch("VtxX",                     &Vertex[0],                "VtxX/F");
    fRhoTree->Branch("VtxY",                     &Vertex[1],                "VtxY/F");
    fRhoTree->Branch("VtxZ",                     &Vertex[2],                "VtxZ/F");
    fRhoTree->Branch("VtxContrib",               &VtxContrib,               "VtxContrib/I");
    fRhoTree->Branch("VtxChi2",                  &VtxChi2,                  "VtxChi2/F");
    fRhoTree->Branch("VtxNDF",                   &VtxNDF,                   "VtxNDF/F");
    fRhoTree->Branch("SpdVtxX",                  &SpdVertex[0],             "SpdVtxX/F");
    fRhoTree->Branch("SpdVtxY",                  &SpdVertex[1],             "SpdVtxY/F");
    fRhoTree->Branch("SpdVtxZ",                  &SpdVertex[2],             "SpdVtxZ/F");
    fRhoTree->Branch("SpdVtxContrib",            &SpdVtxContrib,            "SpdVtxContrib/I");
    fRhoTree->Branch("V0Adecision",              &V0Adecision,              "V0Adecision/I");
    fRhoTree->Branch("V0Cdecision",              &V0Cdecision,              "V0Cdecision/I");
    fRhoTree->Branch("ADAdecision",              &ADAdecision,              "ADAdecision/I");
    fRhoTree->Branch("ADCdecision",              &ADCdecision,              "ADCdecision/I");
    fRhoTree->Branch("UBAfired",                 &UBAfired,                 "UBAfired/O");
    fRhoTree->Branch("UBCfired",                 &UBCfired,                 "UBCfired/O");
    fRhoTree->Branch("VBAfired",                 &VBAfired,                 "VBAfired/O");
    fRhoTree->Branch("VBCfired",                 &VBCfired,                 "VBCfired/O");
    fRhoTree->Branch("Ntracklets",               &Ntracklets,               "Ntracklets/I");
    fRhoTree->Branch("IsCUP2",                   &IsCUP2,                   "IsCUP2/O");
    fRhoTree->Branch("IsCUP4",                   &IsCUP4,                   "IsCUP4/O");
    fRhoTree->Branch("IsCUP9",                   &IsCUP9,                   "IsCUP9/O");
    fRhoTree->Branch("IsC1ZED",                  &IsC1ZED,                  "IsC1ZED/O");
    fRhoTree->Branch("TrackHasPointOnITSLayer0", &TrackHasPointOnITSLayer0, "TrackHasPointOnITSLayer0[4]/O");
    fRhoTree->Branch("TrackHasPointOnITSLayer1", &TrackHasPointOnITSLayer1, "TrackHasPointOnITSLayer1[4]/O");
    fRhoTree->Branch("TrackITSModuleInner",      &TrackITSModuleInner,      "TrackITSModuleInner[4]/I");
    fRhoTree->Branch("TrackITSModuleOuter",      &TrackITSModuleOuter,      "TrackITSModuleOuter[4]/I");
    //fRhoTree->Branch("ITSModule",                &ITSModule,                "ITSModule/I");

    fHistTriggersPerRun = new TH1I("fHistTriggersPerRun", "fHistTriggersPerRun",        50000, 240000.5, 290000.5);
    fITSmodule          = new TH1I("fITSmodule",          "fITSmodule",                 240,   0,        240);
    fFOchip             = new TH1I("fFOchip",             "fFOchip",                    1200,  0,        1200);
    fFOcount            = new TH1I("fFOcount",            "fFOcount",                   30,    0,        30);
    fFOcorr             = new TH2F("fFOcorr",             "fFOcorr",                    240,   0,        240, 240, 0, 240);
    fGoodTracks         = new TH1F("fGoodTracks",         "fGoodTracks",                5,     0,        5);
    fTrackChi2          = new TH1F("fTrackChi2",          "fTrackChi2",                 100,   0,        10);
    TPCclustersP        = new TH1F("TPCclustersP",        "TPCclustersP",               181,   0,        180);
    TPCclustersN        = new TH1F("TPCclustersN",        "TPCclustersN",               181,   0,        180);
    dEdx                = new TH2F("dEdx",                "dEdx",                       500,   0.1,      1.5, 100, 0, 200);
    EtaPhiP             = new TH2F("EtaPhiP",             "EtaPhiP",                    100,   -1,       1, 100, 0, TMath::TwoPi());
    EtaPhiN             = new TH2F("EtaPhiN",             "EtaPhiN",                    100,   -1,       1, 100, 0, TMath::TwoPi());

    fListHist = new TList();
    fListHist->SetOwner();
    fListHist->Add(fHistTriggersPerRun);
    fListHist->Add(fITSmodule);
    fListHist->Add(fFOchip);
    fListHist->Add(fFOcount);
    fListHist->Add(fFOcorr);
    fListHist->Add(fGoodTracks);
    fListHist->Add(fTrackChi2);
    fListHist->Add(TPCclustersP);
    fListHist->Add(TPCclustersN);
    fListHist->Add(dEdx);
    fListHist->Add(EtaPhiP);
    fListHist->Add(EtaPhiN);

    PostData(1, fRhoTree);
    PostData(2, fListHist);
}

bool AliAnalysisTaskUpcRhoPrime::TrackSelection(AliESDtrack* &trk)
{
    if (!trk)
        return false;
    // TODO: investigate this code: what is it, why I need it, so on...
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

    TString trigger = esd->GetFiredTriggerClasses();

    // triggered in data for lumi scalling
    // if (trigger.Contains(fTriggerName.Data())) {
    //   fHistTriggersPerRun->Fill(esd->GetRunNumber());
    //   // cout<<trigger<<endl;
    // }

    // CCUP9-B - *0VBA *0VBC *0UBA *0UBC 0STP
    // if (!trigger.Contains(fTriggerName.Data()))
    //   return;

    AliESDVertex* fESDVertex = (AliESDVertex*)esd->GetPrimaryVertex();

    Int_t nGoodTracks = 0;
    Int_t TrackIndex[4] = { -1, -1, -1, -1 };

    // Track loop - cuts
    for (Int_t itr = 0; itr < esd->GetNumberOfTracks(); itr++)
    {
        AliESDtrack* trk = esd->GetTrack(itr);
        if (!TrackSelection(trk)) continue;

        AliExternalTrackParam cParam;
        if (!trk->RelateToVertex(fESDVertex, esd->GetMagneticField(), 300.,
            &cParam))
            continue;

        // store good track index
        TrackIndex[nGoodTracks] = itr;

        nGoodTracks++;
        if (nGoodTracks > 5)
            break;
    }

    fGoodTracks->Fill(nGoodTracks);

    if (nGoodTracks != 4)
    {
        PostData(1, fRhoTree);
        PostData(2, fListHist);
        return;
    }

    TDatabasePDG* pdgdat = TDatabasePDG::Instance();
    TParticlePDG* partPion = pdgdat->GetParticle(211);
    Double_t pionMass = partPion->Mass();

    TLorentzVector lv[4];
    TLorentzVector lvSum; // pair-4vector

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
    RunNum           = esd->GetRunNumber();
    OrbitNumber      = esd->GetOrbitNumber();
    PeriodNumber     = esd->GetPeriodNumber();
    BunchCrossNumber = esd->GetBunchCrossNumber();

    // VZERO, ZDC, AD
    AliESDVZERO* fV0data  = esd->GetVZEROData();
    AliESDZDC*   fZDCdata = esd->GetESDZDC();
    AliESDAD*    fADdata  = esd->GetADData();

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

    for (Int_t i = 0; i < 4; i++)
    {
        ZDCAtime[i] = fZDCdata->GetZDCTDCCorrected(detChZNA, i);
        ZDCCtime[i] = fZDCdata->GetZDCTDCCorrected(detChZNC, i);
    }

    // primary vertex
    VtxContrib = fESDVertex->GetNContributors();
    Vertex[0]  = fESDVertex->GetX();
    Vertex[1]  = fESDVertex->GetY();
    Vertex[2]  = fESDVertex->GetZ();
    VtxChi2    = fESDVertex->GetChi2();
    VtxNDF     = fESDVertex->GetNDF();

    // SPD primary vertex
    AliESDVertex*    fSPDVertex = (AliESDVertex*)esd->GetPrimaryVertexSPD();
    SpdVtxContrib  = fSPDVertex->GetNContributors();
    SpdVertex[0]   = fSPDVertex->GetX();
    SpdVertex[1]   = fSPDVertex->GetY();
    SpdVertex[2]   = fSPDVertex->GetZ();

    // Tracklets
    Ntracklets = esd->GetMultiplicity()->GetNumberOfTracklets();

    // loop over 4 good tracks
    for (Int_t i = 0; i < 4; ++i)
    {
        AliESDtrack* trk = esd->GetTrack(TrackIndex[i]);
        // ::Info("", "%d TrackIndex", TrackIndex[i]);

        TrackITSModuleInner[i] = trk->GetITSModuleIndex(0) / 1000000;
        TrackITSModuleOuter[i] = trk->GetITSModuleIndex(1) / 1000000;

        TrackHasPointOnITSLayer0[i] = trk->HasPointOnITSLayer(0);
        TrackHasPointOnITSLayer1[i] = trk->HasPointOnITSLayer(1);

        if (fPIDResponse)
        {
            // TPC PID n-sigma
            PIDTPCElectron[i] = fPIDResponse->NumberOfSigmasTPC(trk, AliPID::kElectron);
            PIDTPCPion[i]     = fPIDResponse->NumberOfSigmasTPC(trk, AliPID::kPion);
        }

        TrackQ[i]    = trk->Charge();
        TPCsignal[i] = trk->GetTPCsignal();
        TrackP[i]    = trk->P();
        TrackPhi[i]  = trk->Phi();
        TrackEta[i]  = trk->Eta();
        TrackPx[i]   = trk->Px();
        TrackPy[i]   = trk->Py();
        TrackPz[i]   = trk->Pz();

        fTrackChi2->Fill((Float_t)trk->GetTPCchi2() / trk->GetTPCNcls());
        fITSmodule->Fill(TrackITSModuleInner[i]);
        fITSmodule->Fill(TrackITSModuleOuter[i]);

        for (Int_t i = 0; i < 240; i++)
        {
            if (fFOsensores[i] > 0)
            {
                fFOcorr->Fill(TrackITSModuleInner[i], i);
                fFOcorr->Fill(TrackITSModuleOuter[i], i);
            }
        }

        dEdx->Fill(trk->Pt(), trk->GetTPCsignal());
        if (trk->Charge() > 0)
        {
            TPCclustersP->Fill(trk->GetTPCNcls());
            EtaPhiP->Fill(trk->Eta(), trk->Phi());
        }
        else
        {
            TPCclustersN->Fill(trk->GetTPCNcls());
            EtaPhiN->Fill(trk->Eta(), trk->Phi());
        }

        lv[i].SetPtEtaPhiM(trk->Pt(), trk->Eta(), trk->Phi(), pionMass);
    } // end loop over 4 good tracks

    lvSum = lv[0] + lv[1] + lv[2] + lv[3];
    Q     = TrackQ[0] + TrackQ[1] + TrackQ[2] + TrackQ[3];

    Mass     = lvSum.M();
    Pt       = lvSum.Pt();
    Rapidity = lvSum.Rapidity();
    Phi      = lvSum.Phi();

    // virtual cut on FO chip matching
    Int_t SPDInner[20];
    for (Int_t i = 0; i < 20; ++i)
        SPDInner[i] = 0;
    Int_t SPDOuter[40];
    for (Int_t i = 0; i < 40; ++i)
        SPDOuter[i] = 0;

    for (auto i = 0; i < 4; ++i)
    {
        SPDInner[TrackITSModuleInner[i] / 4]++;
        SPDOuter[(TrackITSModuleOuter[i] - 80) / 4]++;
    }

    IsCUP2  = 0;
    IsCUP4  = 0;
    IsCUP9  = 0;
    IsC1ZED = 0;

    Bool_t fFOsensoresFired = 0;
    for (Int_t j = 0; j < 4; ++j)
        fFOsensoresFired += fFOsensores[TrackITSModuleInner[j]];

    if (fFOsensoresFired || !Is0STPfired(SPDInner, SPDOuter))
    {
        if (fTriggerName == "CCUP9-B")
            IsCUP9 = 1;
    }

    if (fFOsensoresFired)
    {
        if (fTriggerName == "CCUP2-B")
            IsCUP2 = 1;
        if (fTriggerName == "CCUP4-B")
            IsCUP4 = 1;
        if (fTriggerName == "C1ZED")
            IsC1ZED = 1;
    }

    Int_t fFOcounter = 0;
    for (Int_t chipkey = 0; chipkey < 1200; chipkey++)
    {
        if (esd->GetMultiplicity()->TestFastOrFiredChips(chipkey))
        {
            fFOchip->Fill(chipkey);
            fFOcounter++;
        }
    }

    fFOcount->Fill(fFOcounter);
    fRhoTree->Fill();

    PostData(1, fRhoTree);
    PostData(2, fListHist);

} // UserExec

// fuction that get two arrays and return if 0STP trigger was fired
Bool_t AliAnalysisTaskUpcRhoPrime::Is0STPfired(Int_t* vPhiInner,
    Int_t* vPhiOuter) // array 20, 40
{
    Int_t fired(0);
    for (Int_t i(0); i < 10; ++i)
    {
        for (Int_t j(0); j < 2; ++j)
        {
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

