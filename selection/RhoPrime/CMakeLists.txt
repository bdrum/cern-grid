include_directories(inc)
set(RhoPrimeAnalysisTask inc/AliAnalysisTaskUpcRhoPrime.h src/AliAnalysisTaskUpcRhoPrime.cxx macro/AddTaskUpcRhoPrime.C  macro/runAnalysis.C)

add_library(RhoPrimeAnalysisTask SHARED ${RhoPrimeAnalysisTask})
