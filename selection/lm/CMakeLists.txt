set(lmTest "AliFemtoCutMonitorDphiDeta.cxx" "AliFemtoCutMonitorDphiDeta.h" "AliFemtoESDTrackCutMinusJets.cxx"  "AliFemtoESDTrackCutMinusJets.h" "AliFemtoESDTrackCutPlusJets.cxx" "AliFemtoESDTrackCutPlusJets.h") 

add_library(lmTest SHARED ${lmTest})
