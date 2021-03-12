set(lmTest "AliFemtoCutMonitorDphiDeta.cxx" "AliFemtoCutMonitorDphiDeta.h" "AliFemtoESDTrackCutMinusJets.cxx"  "AliFemtoESDTrackCutMinusJets.h" "AliFemtoESDTrackCutPlusJets.cxx" "AliFemtoESDTrackCutPlusJets.h" "ConfigFemtoAnalysis.C" "ConfigFemtoAnalysis_PlusJets.C" "runBatch_local.C") 

add_library(lmTest SHARED ${lmTest})
