#include "/mnt/d/Sources/cern/alice/sw/ubuntu1804_x86-64/AliRoot/latest/ANALYSIS/macros/AddTaskPIDResponse.C"
#include "FourProngsTask.h"

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

  AliAnalysisManager *mgr = new AliAnalysisManager("4ProngsAnalysis");

  // AOD
  // AliAODInputHandler *aodH = new AliAODInputHandler();
  // mgr->SetInputEventHandler(aodH);

  // ESD
  AliESDInputHandler *esdH = new AliESDInputHandler();
  mgr->SetInputEventHandler(esdH);

  gROOT->LoadMacro("AddTaskPIDResponse.C");

  AddTaskPIDResponse();

  // compile the class and load the add task macro
  gInterpreter->LoadMacro("FourProngsTask.cxx++g");
  FourProngsTask *task = reinterpret_cast<FourProngsTask *>(
      gInterpreter->ExecuteMacro("AddTaskUpc4Prongs.C"));

  if (!mgr->InitAnalysis())
    return;

  // mgr->SetDebugLevel(1);
  // mgr->PrintStatus();
  mgr->SetUseProgressBar(1, 25); // in case of that debug will not use

  if (local) {
    TChain *chain = new TChain("esdTree");
    chain->Add("/mnt/d/GoogleDrive/Job/cern/Alice/analysis/data/RhoPrime/2015/"
               "245145.AliESDs.root");

    // start the analysis locally, reading the events from the tchain
    // for successful analysis should be limited -> 2, 1
    // without it seg viol
    mgr->StartAnalysis("local", chain);
  } else {
    // if we want to run on grid, we create and configure the plugin
    AliAnalysisAlien *alienHandler = new AliAnalysisAlien();
    // also specify the include (header) paths on grid
    alienHandler->AddIncludePath(
        "-I. -I$ROOTSYS/include -I$ALICE_ROOT -I$ALICE_ROOT/include "
        "-I$ALICE_PHYSICS/include");
    // make sure your source files get copied to grid
    alienHandler->SetAdditionalLibs("FourProngsTask.cxx FourProngsTask.h");
    alienHandler->SetAnalysisSource("FourProngsTask.cxx");
    // select the aliphysics version. all other packages
    // are LOADED AUTOMATICALLY!
    alienHandler->SetAliPhysicsVersion("vAN-20201209_ROOT6-1");
    // set the Alien API version
    alienHandler->SetAPIVersion("V1.1x");
    // select the input data
    // alienHandler->SetGridDataDir("/alice/data/2018/LHC15q");
    // alienHandler->SetDataPattern("/pass1/*/AliESDs.root");
    alienHandler->SetGridDataDir("/alice/data/2015/LHC15o");
    alienHandler->SetDataPattern("/pass2_UD_CCUP/*/AliESDs.root");

    // MC has no prefix, data has prefix 000
    alienHandler->SetRunPrefix("000");
    // 2018q
    // alienHandler->AddRunList(
    //     "295585, 295586, 295588, 295589, 295610, 295611, 295612, 295615, "
    //     "295666, 295667, 295668, 295673, 295675, 295676, 295712, 295714, "
    //     "295717, 295718, 295719, 295721,295723, 295725, 295754, 295755, "
    //     "295758, 295759, 295762, 295763, 295786, 295788, 295791, 295816, "
    //     "295818, 295819, 295822, 295825, 295826, 295829, 295831, 295853, "
    //     "295854, 295855, 295856, 295859, 295860, 295861, 295909, 295910, "
    //     "295913, 295936, 295937, 295941, 295942, 296016, 296060, 296062, "
    //     "296063, 296065, 296066, 296123, 296132, 296133, 296134, 296135, "
    //     "296142, 296143, 296191, 296192, 296194, 296195, 296196, 296197, "
    //     "296198, 296240, 296241, 296242, 296243, 296244, 296246, 296247, "
    //     "296269, 296270, 296273, 296279, 296280, 296303, 296304, 296309, "
    //     "296312, 296377, 296378, 296379, 296380, 296381, 296383, 296414, "
    //     "296415, 296419, 296420, 296423, 296424, 296433, 296472, 296509, "
    //     "296510, 296511, 296512, 296516, 296547, 296548, 296549, 296550, "
    //     "296551, 296552, 296553, 296594, 296615, 296616, 296618, 296619, "
    //     "296621, 296622, 296623 ");

    // alienHandler->AddRunList("245407, 245507, 245554, 245775, 245952, 246087,
    // "
    //  "246153, 246272, 246980, 246984, 246989, 246994");

    // 2015o //125 combined from pvn runs and legotrains list. these runs
    // don't contain pass2_ccup... 246540     246858        246859
    alienHandler->AddRunList(
        "245145, 246148, 246217, 245963, 246424, 245683, 246036, 
        246487, 246808, 246804, 246271, 245146, 245151, 245152,
        245232, 245259, 245345, 245346, 245347, 245349, 245353,
        245396, 245397, 245401, 245407, 245409, 245410, 245411,
        245441, 245446, 245450, 245453, 245454, 245496, 245497,
        245501, 245504, 245505, 245507, 245540, 245542, 245543,
        245544, 245545, 245554, 245692, 245700, 245702, 245705,
        245775, 245793, 245829, 245831, 245833, 245923, 245949,
        245952, 245954, 246001, 246003, 246012, 246037, 246042,
        246048, 246049, 246052, 246053, 246087, 246089, 246113,
        246151, 246152, 246153, 246178, 246180, 246181, 246182,
        246185, 246222, 246225, 246272, 246275, 246276, 246428,
        246431, 246434, 246488, 246493, 246495, 246675, 246676,
        246750, 246751, 246757, 246758, 246759, 246760, 246763,
        246765, 246766, 246805, 246807, 246809, 246810, 246844,
        246845, 246846, 246847, 246851, 246864, 246865, 246867,
        246870, 246871, 246928, 246945, 246948, 246980, 246982,
        246984, 246989, 246991, 246994, 245231, 246115");

    // number of files per subjob
    // here I tried to specified 40, 77(pvn),100,200,1000
    // I didn't make detailed analysis, but seems 100(default) gives best
    // results
    // alienHandler->SetSplitMaxInputFileNumber(200);
    alienHandler->SetExecutable("FourProngsTask.sh");
    // specify how many seconds your job may take
    // alienHandler->SetTTL(10000);
    alienHandler->SetJDLName("FourProngsTask.jdl");

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
    alienHandler->SetGridWorkingDir("4Prongs2015o");
    alienHandler->SetGridOutputDir("4Prongs2015o");

    // connect the alien plugin to the manager
    mgr->SetGridHandler(alienHandler);
    if (gridTest) {
      // speficy on how many files you want to run
      alienHandler->SetNtestFiles(50);
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
