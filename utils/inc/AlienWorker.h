#include <string>
#include <vector>

// TODO: check working of multithreading. looks like that it doesn't work
// TODO: change TString to std::string
// TODO: change TError to std::cout
// TODO: interface should have alisync analouge
// TODO: implement all alisync facilities
// TODO: supress output and add progress bar
// TODO: estimation of elapsed time based on download speed
// TODO: check if file exists skip copying
// TODO: check if dest dir exists
// TODO: check to leaks with valgrind
// "/alice/data/2015/LHC15o/000245145/pass2_UD_CCUP/"

/* USAGE
.L AlienWorker.cxx+
AlienWorker aw("/alice/data/2015/LHC15o/000245145/pass2_UD_CCUP/",
"AliESDs.root",
"/mnt/d/GoogleDrive/Job/cern/Alice/analysis/rho/data/2015/4merge", 5, 2 );
aw.Copy();
*/

class AlienWorker {
private:
  std::vector<std::string> filesList;
  std::vector<std::vector<std::string>> splittedFileList;

  std::string dir;
  std::string pattern;
  std::string destination;
  int numOfProcesses;
  int keepNLastDirNames;
  void FillFilesList();
  void SplitVectorToChunks();

public:
  AlienWorker() = delete;
  std::string FormFileName(std::string inputFile);
  void Copy();
  void SetDestination();
  AlienWorker(std::string dir, std::string pattern, std::string destination,
              int numOfProcesses, int keepNLastDirNames);
  ~AlienWorker();
};
