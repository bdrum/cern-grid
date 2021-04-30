#include "/mnt/d/Sources/cern/alice/sw/ubuntu1804_x86-64/AliRoot/latest/ANALYSIS/macros/AddTaskPIDResponse.C"
#include "FourProngsTaskMC.h"

// alias runLocal='aliroot    -l -q "runAnalysis.C(true, false, false, true)"'
// alias runGridTest='aliroot -l -q "runAnalysis.C(false, true, false, true)"'
// alias runGridFull='aliroot -l -q "runAnalysis.C(false, false, false, true)"'
// alias mergeGrid='aliroot   -l -q "runAnalysis.C(false, false, true, true)"'
// alias mergeLocal='aliroot  -l -q "runAnalysis.C(false, false, true, false)"'

void runAnalysis(bool local, bool gridTest, bool terminate, bool mergeViaJDL) {

  gInterpreter->AddIncludePath("-I$ROOTSYS/include");
  gInterpreter->AddIncludePath("-I$ALICE_ROOT/include");
  gInterpreter->AddIncludePath("-I$ALICE_PHYSICS/include");

  /* load libraries */
  gInterpreter->Load("libANALYSIS");
  gInterpreter->Load("libANALYSISalice");
  gInterpreter->Load("libOADB");
  // since we will compile a class, tell root where to look for headers
  gInterpreter->ProcessLine(".include $ROOTSYS/include");
  gInterpreter->ProcessLine(".include $ALICE_ROOT/include");

  auto mgr = new AliAnalysisManager("4ProngsAnalysisMC");

  // AOD
  auto aodH = new AliAODInputHandler();
  mgr->SetInputEventHandler(aodH);

  gROOT->LoadMacro("AddTaskPIDResponse.C");

  // MC
  AddTaskPIDResponse(true);

  // compile the class and load the add task macro
  gInterpreter->LoadMacro("FourProngsTaskMC.cxx++g");
  FourProngsTaskMC *task = reinterpret_cast<FourProngsTaskMC *>(
      gInterpreter->ExecuteMacro("AddTaskUpc4Prongs.C"));

  if (!mgr->InitAnalysis())
    return;

  // mgr->SetDebugLevel(1);
  // mgr->PrintStatus();
  mgr->SetUseProgressBar(1, 25); // in case of that debug will not use

  if (local) {
    TChain *chain = new TChain("aodTree");
    chain->Add("/mnt/d/GoogleDrive/Job/cern/Alice/analysis/data/"
               "RhoPrime/2015/AliAODLHC15oMC.root");

    // start the analysis locally, reading the events from the tchain
    // for successful analysis should be limited -> 2, 1
    // without it seg viol
    mgr->StartAnalysis("local", chain);
  } else {
    // if we want to run on grid, we create and configure the plugin
    auto alienHandler = new AliAnalysisAlien();
    // also specify the include (header) paths on grid
    alienHandler->AddIncludePath(
        "-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include "
        "-I$ALICE_PHYSICS/include");
    // make sure your source files get copied to grid
    alienHandler->SetAdditionalLibs("FourProngsTaskMC.cxx FourProngsTaskMC.h");
    alienHandler->SetAnalysisSource("FourProngsTaskMC.cxx");
    // select the aliphysics version. all other packages
    // are LOADED AUTOMATICALLY!
    alienHandler->SetAliPhysicsVersion("vAN-20201209_ROOT6-1");
    // set the Alien API version
    alienHandler->SetAPIVersion("V1.1x");
    // select the input data
    alienHandler->SetGridDataDir("/alice/cern.ch/user/b/brumyant/mc/output/");
    alienHandler->SetDataPattern("*/AliAOD.root");
    alienHandler->AddRunList("245407, 245507, 245554, 245775, 245952, 246087, "
                             "246153, 246272, 246980, 246984, 246989, 246994");

    alienHandler->SetExecutable("FourProngsTaskMC.sh");
    // specify how many seconds your job may take
    // alienHandler->SetTTL(10000);
    alienHandler->SetJDLName("FourProngsTaskMC.jdl");

    alienHandler->SetOutputToRunNo(kTRUE);
    alienHandler->SetKeepLogs(kFALSE);
    // merging: run with kTRUE to merge on grid
    // after re-running the jobs in SetRunMode("terminate")
    // (see below) mode, set SetMergeViaJDL(kFALSE)
    // to collect final results
    alienHandler->SetMaxMergeStages(1);
    alienHandler->SetMergeViaJDL(mergeViaJDL);

    // define the output folders
    // alienHandler->SetGridWorkingDir("4Prongs2015o");
    // alienHandler->SetGridOutputDir("4Prongs2015o");

    // MC
    alienHandler->SetGridWorkingDir("4Prongs2015oMC");
    alienHandler->SetGridOutputDir("4Prongs2015oMC");

    // connect the alien plugin to the manager
    mgr->SetGridHandler(alienHandler);
    if (gridTest) {
      // speficy on how many files you want to run
      alienHandler->SetNtestFiles(500);
      // and launch the analysis
      alienHandler->SetRunMode("test");
      mgr->StartAnalysis("grid");
    } else if (terminate) {
      alienHandler->SetRunMode("terminate");
      mgr->StartAnalysis("grid");
    } else {
      // else launch the full grid analysis
      alienHandler->SetRunMode("full");
      mgr->StartAnalysis("grid");
    }
  }
}
