/* Copyright(c) 1998-2020, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice */
/* $Id$ */

#ifndef FourProngsTask_H
#define FourProngsTask_H

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
#include <vector>

class FourProngsTask : public AliAnalysisTaskSE
{
public:
    FourProngsTask(); // = delete;
    FourProngsTask(const char* name);
    virtual ~FourProngsTask();

    virtual void Init();
    virtual void UserCreateOutputObjects();
    virtual void UserExec(Option_t* option);
    virtual void Terminate(Option_t*) {};

    void SetTrigger(TString _fTriggerName) { fTriggerName = _fTriggerName; }

private:

    TString fTriggerName;

    TTree* fRhoTree;
 
    // event params
 
    
    UInt_t  RunNum, BunchCrossNumber, OrbitNumber, PeriodNumber;
    Float_t Mass, Pt, Rapidity, Phi;
    Short_t Q;
 
    Int_t  V0Adecision, V0Cdecision, ADAdecision, ADCdecision;

  	Bool_t V0Afired, V0Cfired, ADAfired, ADCfired, STPfired,\
           SMBfired, SM2fired, SH1fired, OM2fired, OMUfired,\
           IsTriggered;

    std::vector<Int_t>   FORChip;
	
    Float_t ZNAenergy, ZNCenergy, ZPAenergy, ZPCenergy;

    Int_t   VtxContrib;
    Float_t VtxChi2, VtxNDF;
    Int_t   SpdVtxContrib;

    Int_t nTracklets, nTracks;

    Float_t Vertex[3], SpdVertex[3], ZDCAtime[4], ZDCCtime[4];

    // track params

    std::vector<Float_t> T_NumberOfSigmaITSPion,T_NumberOfSigmaITSElectron,\
                         T_NumberOfSigmaTPCPion, T_NumberOfSigmaTPCElectron,\
                         T_P, T_Eta, T_Phi, T_Px, T_Py, T_Pz,\
                         T_Dca0, T_Dca1, T_Lets_Theta, T_Lets_Phi; 

    std::vector<Bool_t>  T_TPCRefit, T_ITSRefit,  T_HasPointOnITSLayer0,  T_HasPointOnITSLayer1;
    
    std::vector<Int_t>   T_ITSModuleInner, T_ITSModuleOuter, T_TPCsignal, T_TPCNCls, T_ITSNCls, T_Q;
   
    AliPIDResponse* fPIDResponse;

    FourProngsTask(
        const FourProngsTask&); // not implemented
    FourProngsTask&
        operator=(const FourProngsTask&); // not implemented

    Bool_t Is0STPfired(Int_t *, Int_t *);
    Bool_t CheckEventTrigger(AliESDEvent *);

    ClassDef(FourProngsTask, 4);
};

#endif
