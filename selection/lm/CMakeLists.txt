set(lmTest "src/AliFemtoCutMonitorDphiDeta.cxx" "src/AliFemtoCutMonitorDphiDeta.h" "src/AliFemtoESDTrackCutMinusJets.cxx"  "src/AliFemtoESDTrackCutMinusJets.h" "src/AliFemtoESDTrackCutPlusJets.cxx" "src/AliFemtoESDTrackCutPlusJets.h") 

add_library(lmTest SHARED ${lmTest})
