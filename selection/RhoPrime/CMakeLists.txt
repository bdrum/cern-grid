include_directories(inc)
set(RhoPrimeAnalysisTask "inc/AliAnalysisTaskUpc4Prongs.h" "src/AliAnalysisTaskUpc4Prongs.cxx" "macro/AddTaskUpc4Prongs.C"  macro/runAnalysis.C)

add_library(RhoPrimeAnalysisTask SHARED ${RhoPrimeAnalysisTask})
