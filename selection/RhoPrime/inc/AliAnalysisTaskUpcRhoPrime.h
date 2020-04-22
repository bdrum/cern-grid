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
class TBits;

#include "AliAnalysisTaskSE.h"

class AliAnalysisTaskUpcRhoPrime : public AliAnalysisTaskSE
{
public:
  AliAnalysisTaskUpcRhoPrime(); // = delete;
  AliAnalysisTaskUpcRhoPrime(const char *name);
  virtual ~AliAnalysisTaskUpcRhoPrime();

  virtual void Init();
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *){};

  void SetTrigger(TString _fTriggerName) { fTriggerName = _fTriggerName; }

private:
  Bool_t Is0STPfired(Int_t *, Int_t *);

  TString fTriggerName;

  // tree
  TTree *fRhoTree;
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
  Float_t ZDCAtime[4];
  Float_t ZDCCtime[4];
  Float_t PIDTPCPion[4];
  Float_t PIDTPCElectron[4];
  Int_t TPCsignal[4];
  Float_t TrackP[4];
  Float_t Vertex[3];
  Int_t VtxContrib;
  Float_t VtxChi2, VtxNDF;
  Float_t SpdVertex[3];
  Int_t SpdVtxContrib;
  Int_t Ntracklets;
  Float_t Phi;
  Float_t TrackEta[4];
  Float_t TrackPhi[4];
  Float_t TrackPx[4];
  Float_t TrackPy[4];
  Float_t TrackPz[4];
  Short_t TrackQ[4];
  Bool_t TrackHasPointOnITSLayer0[4];
  Bool_t TrackHasPointOnITSLayer1[4];
  Int_t TrackITSModuleInner[4];
  Int_t TrackITSModuleOuter[4];
  // TODO: Possible good idea is keep fTriggerName in such view
  // TString of "CUP9;CUP2"
  // in this case we could add new triggers from lego train interface
  Bool_t IsCUP9;
  Bool_t IsCUP2;
  Bool_t IsCUP4;
  Bool_t IsC1ZED;

  AliPIDResponse *fPIDResponse;

  TList *fListHist;
  TH1I *fHistTriggersPerRun;
  TH1I *fITSmodule;
  TH1I *fFOchip;
  TH1I *fFOcount;
  TH1F *TPCclustersP;
  TH1F *TPCclustersN;
  TH2F *dEdx;
  TH2F *EtaPhiP;
  TH2F *EtaPhiN;

  TH2F *fFOcorr;
  TH1F *fGoodTracks;
  TH1F *fTrackChi2;

  AliAnalysisTaskUpcRhoPrime(
      const AliAnalysisTaskUpcRhoPrime &); // not implemented
  AliAnalysisTaskUpcRhoPrime &
  operator=(const AliAnalysisTaskUpcRhoPrime &); // not implemented

  ClassDef(AliAnalysisTaskUpcRhoPrime, 1);
};

#endif