/* Copyright(c) 1998-2021, ALICE Experiment at CERN, All rights reserved. *
 * See cxx source for full Copyright notice */
/* $Id$ */

#ifndef FourProngsTaskMC_H
#define FourProngsTaskMC_H

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

class FourProngsTaskMC : public AliAnalysisTaskSE {
public:
  FourProngsTaskMC();
  FourProngsTaskMC(const char *name);
  virtual ~FourProngsTaskMC();

  virtual void Init();
  virtual void UserCreateOutputObjects();
  virtual void UserExec(Option_t *option);
  virtual void Terminate(Option_t *){};

  void SetTrigger(TString _fTriggerName) { fTriggerName = _fTriggerName; }

private:
  TString fTriggerName;

  TTree *fRhoTree;
  TTree *fStartedRunsTree;

  // event params

  uint RunNum, BunchCrossNumber, OrbitNumber, PeriodNumber;
  float Mass, Pt, Rapidity, Phi;
  short Q;

  int V0Adecision, V0Cdecision, ADAdecision, ADCdecision;

  int StartedRuns;

  float ZNAenergy, ZNCenergy, ZPAenergy, ZPCenergy;

  float ZDCAtime[4];
  float ZDCCtime[4];

  int VtxContrib;
  float VtxChi2, VtxNDF;
  int SpdVtxContrib;

  int nTracklets, nTracks;

  float Vertex[3], SpdVertex[3];

  // event params

  // track params

  std::vector<float> MCPart_PdgCode, MCPart_Px, MCPart_Py, MCPart_Pz;

  std::vector<float> T_NumberOfSigmaITSPion, T_NumberOfSigmaITSElectron,
      T_NumberOfSigmaTPCPion, T_NumberOfSigmaTPCElectron, T_P, T_Eta, T_Phi,
      T_Px, T_Py, T_Pz, T_Dca0, T_Dca1, T_Lets_Theta, T_Lets_Phi;

  std::vector<bool> T_TPCRefit, T_ITSRefit, T_HasPointOnITSLayer0,
      T_HasPointOnITSLayer1;

  std::vector<int> T_ITSModuleInner, T_ITSModuleOuter, T_TPCsignal, T_TPCNCls,
      T_ITSNCls, T_Q;

  // track params

  AliPIDResponse *fPIDResponse;

  FourProngsTaskMC(const FourProngsTaskMC &);            // not implemented
  FourProngsTaskMC &operator=(const FourProngsTaskMC &); // not implemented

  void ClearTracksVectors();
  void ReserveTracksVectors(int size);
  void ShrinkToFitTracksVectors();

  ClassDef(FourProngsTaskMC, 1);
};

#endif
