include_directories(inc)
set(4ProngsAnalysisTask "inc/FourProngsTask.h" "src/FourProngsTask.cxx") # "macro/AddTaskUpc4Prongs.C"  macro/runAnalysis.C)

add_library(4ProngsAnalysisTask SHARED ${4ProngsAnalysisTask})
