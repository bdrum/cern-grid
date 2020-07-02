include_directories(inc)
set(4ProngsAnalysisTask "inc/AliAnalysisTaskUpc4Prongs.h" "src/AliAnalysisTaskUpc4Prongs.cxx" "macro/AddTaskUpc4Prongs.C"  macro/runAnalysis.C)

add_library(4ProngsAnalysisTask SHARED ${4ProngsAnalysisTask})
