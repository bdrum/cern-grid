/*********************************************************************
 *                                                                   *
 * ConfigFemtoAnalysis.C - configuration macro for the femtoscopic   *
 * analysis, meant as a QA process for two-particle effects          *
 *                                                                   *
 * Author: Adam Kisiel (Adam.Kisiel@cern.ch)                         *
 *                                                                   *
 *********************************************************************/

AliFemtoManager *ConfigFemtoAnalysisTest() {

  double PionMass = 0.13956995;
  double KaonMass = 0.493677;

  //-------Single track cuts------------------------------------------------->
  double DCAxy =
      0.3; // 2.4;// cm // our standard is 0.20 cm; super narrow was 0.015cm
  double DCAz = 0.3; // 3.0;// cm // our standard is 0.15 cm;
  //-------Single track cuts-------------------------------------------------<

  auto *Reader = new AliFemtoEventReaderAODChain();

  Reader->SetFilterBit(128);
  Reader->SetUseMultiplicity(AliFemtoEventReaderAOD::kRefComb08);
  Reader->SetCentralityFlattening(kFALSE);
  Reader->SetReadV0(0);
  Reader->SetDCAglobalTrack(kTRUE);

  auto Manager = new AliFemtoManager();
  Manager->SetEventReader(Reader);

  auto anetaphitpc = new AliFemtoJetAnalysis(10, -10.0, 10.0, 8, 1, 18);
  anetaphitpc->SetNumEventsToMix(10);
  anetaphitpc->SetMinSizePartCollection(1);
  // anetaphitpc->SetVerboseMode(1);

  // auto mecetaphitpc = new AliFemtoBasicEventCut();
  // mecetaphitpc->SetEventMult(0, 100000);
  // mecetaphitpc->SetVertZPos(-10.0, 10.0);

  // auto cutPassEvMetaphitpc =
  //     new AliFemtoCutMonitorEventMult(Form("cutPass%stpcM%i", "Kp", 0));
  // auto cutFailEvMetaphitpc =
  //     new AliFemtoCutMonitorEventMult(Form("cutFail%stpcM%i", "Kp", 0));
  // mecetaphitpc->AddCutMonitor(cutPassEvMetaphitpc, cutFailEvMetaphitpc);

  // auto cutPassEvSpher =
  //     new AliFemtoCutMonitorEventSphericity(Form("cutPass%stpcM%i", "Kp",
  //     0));
  // auto cutFailEvSpher =
  //     new AliFemtoCutMonitorEventSphericity(Form("cutFail%stpcM%i", "Kp",
  //     0));
  // mecetaphitpc->AddCutMonitor(cutPassEvSpher, cutFailEvSpher);

  // auto cutPassDphiDeta =
  //     new AliFemtoCutMonitorDphiDeta(Form("cutPassEv%stpcM%i", "Kp", 0));
  // auto cutFailDphiDeta =
  //     new AliFemtoCutMonitorDphiDeta(Form("cutFailEv%stpcM%i", "Kp", 0));
  // mecetaphitpc->AddCutMonitor(cutPassDphiDeta, cutFailDphiDeta);

  // auto dtc1etaphitpc = new AliFemtoESDTrackCutMinusJets();

  // dtc1etaphitpc->SetNsigmaTPCTOF(true);
  // dtc1etaphitpc->AddCutMonitor(cutPassDphiDeta);
  // dtc1etaphitpc->SetNsigma(3.0);
  // dtc1etaphitpc->SetCharge(1.0);
  // dtc1etaphitpc->SetPt(0.15, 5.0);
  // dtc1etaphitpc->SetEta(-0.8, 0.8);
  // dtc1etaphitpc->SetMass(PionMass);
  // dtc1etaphitpc->SetMostProbablePion();
  // dtc1etaphitpc->SetMaxImpactXY(DCAxy);
  // dtc1etaphitpc->SetMaxImpactZ(DCAz);

  // auto cutPass1YPtetaphitpc = new AliFemtoCutMonitorParticleYPt(
  //     Form("cutPass1%stpcM%i", "Kp", 0), 0.139569);
  // auto cutFail1YPtetaphitpc = new AliFemtoCutMonitorParticleYPt(
  //     Form("cutFail1%stpcM%i", "Kp", 0), 0.139569);
  // dtc1etaphitpc->AddCutMonitor(cutPass1YPtetaphitpc, cutFail1YPtetaphitpc);

  // auto cutPass1PIDetaphitpc =
  //     new AliFemtoCutMonitorParticlePID(Form("cutPass1%stpcM%i", "Kp", 0),
  //     0);
  // auto cutFail1PIDetaphitpc =
  //     new AliFemtoCutMonitorParticlePID(Form("cutFail1%stpcM%i", "Kp", 0),
  //     0);
  // dtc1etaphitpc->AddCutMonitor(cutPass1PIDetaphitpc, cutFail1PIDetaphitpc);

  // auto cutPassDphiDetaTracks =
  //     new AliFemtoCutMonitorDphiDeta(Form("cutPassTracks%stpcM%i", "Kp", 0));
  // auto cutFailDphiDetaTracks =
  //     new AliFemtoCutMonitorDphiDeta(Form("cutFailTracks%stpcM%i", "Kp", 0));
  // dtc1etaphitpc->AddCutMonitor(cutPassDphiDetaTracks, cutFailDphiDetaTracks);

  // auto sqpcetaphitpc =
  //     new AliFemtoPairCutRadialDistanceKKdist(); // AliFemto dphi* cut
  // sqpcetaphitpc->SetShareQualityMax(1.0);
  // sqpcetaphitpc->SetShareFractionMax(0.05);
  // sqpcetaphitpc->SetRemoveSameLabel(kFALSE);
  // sqpcetaphitpc->SetAverageSeparation(0.0); // 0.8

  // anetaphitpc->SetEventCut(mecetaphitpc);
  // anetaphitpc->SetFirstParticleCut(dtc1etaphitpc);
  // anetaphitpc->SetSecondParticleCut(dtc1etaphitpc);
  // anetaphitpc->SetPairCut(sqpcetaphitpc);

  Manager->AddAnalysis(anetaphitpc);

  // *** End Kaon-Kaon (positive) analysis

  return Manager;
}
