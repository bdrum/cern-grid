/* Copyright(c) 1998-2020, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice */
/* $Id$ */

#ifndef ALIANALYSISTASKUPCRHOPRIME_H
#define ALIANALYSISTASKUPCRHOPRIME_H

class TClonesArray;
class TFile;
class TTree;
class TList;
class TH1;
class TList;
class AliPIDResponse;
class AliESDEvent;
class AliESDTrack;
class TBits;

#include "AliAnalysisTaskSE.h"
#include "AliESDtrack.h"
#include <vector>

class AliAnalysisTaskUpcRhoPrime : public AliAnalysisTaskSE
{
public:
    AliAnalysisTaskUpcRhoPrime(); // = delete;
    AliAnalysisTaskUpcRhoPrime(const char* name);
    virtual ~AliAnalysisTaskUpcRhoPrime();

    virtual void Init();
    virtual void UserCreateOutputObjects();
    virtual void UserExec(Option_t* option);
    virtual void Terminate(Option_t*) {};

    void SetTrigger(TString _fTriggerName) { fTriggerName = _fTriggerName; }

private:
    Bool_t Is0STPfired(Int_t*, Int_t*);

    TString fTriggerName;

    // tree
    TTree* fRhoTree;
    // tree variables and branches
    Int_t RunNum;
    UShort_t BunchCrossNumber;
    UInt_t OrbitNumber;
    UInt_t PeriodNumber;
    Float_t Mass;
    Float_t Pt;
    Short_t Q;
    Float_t Rapidity;
    Int_t V0Adecision;
    Int_t V0Cdecision;
    Int_t ADAdecision;
    Int_t ADCdecision;
    Bool_t UBAfired;
    Bool_t UBCfired;
    Bool_t VBAfired;
    Bool_t VBCfired;
    Float_t ZNAenergy;
    Float_t ZNCenergy;
    Float_t ZPAenergy;
    Float_t ZPCenergy;
    Int_t VtxContrib;
    Float_t VtxChi2, VtxNDF;
    Int_t SpdVtxContrib;
    Int_t Ntracklets;
    Int_t nTracks;
    Float_t Phi;
    Float_t Vertex[3];
    Float_t SpdVertex[3];
    Float_t ZDCAtime[4];
    Float_t ZDCCtime[4];

    std::vector<Float_t> PIDTPCPion;
    std::vector<Float_t> PIDTPCElectron;
    std::vector<Int_t>   TPCsignal;
    std::vector<Float_t> TrackP;
    std::vector<Float_t> TrackEta;
    std::vector<Float_t> TrackPhi;
    std::vector<Float_t> TrackPx;
    std::vector<Float_t> TrackPy;
    std::vector<Float_t> TrackPz;
    std::vector<Short_t> TrackQ;
    std::vector<Bool_t>  TrackHasPointOnITSLayer0;
    std::vector<Bool_t>  TrackHasPointOnITSLayer1;
    std::vector<Int_t>   TrackITSModuleInner;
    std::vector<Int_t>   TrackITSModuleOuter;

    // TODO: Possible good idea is keep fTriggerName in such view
    // TString of "CUP9;CUP2"
    // in this case we could add new triggers from lego train interface
    Bool_t IsCUP9;
    Bool_t IsCUP2;
    Bool_t IsCUP4;
    Bool_t IsC1ZED;

    AliPIDResponse* fPIDResponse;

    AliAnalysisTaskUpcRhoPrime(
        const AliAnalysisTaskUpcRhoPrime&); // not implemented
    AliAnalysisTaskUpcRhoPrime&
        operator=(const AliAnalysisTaskUpcRhoPrime&); // not implemented

    bool TrackSelection(AliESDtrack* &trk);

    ClassDef(AliAnalysisTaskUpcRhoPrime, 2);
};

#endif
