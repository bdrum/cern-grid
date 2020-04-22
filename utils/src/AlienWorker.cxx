#include "AlienWorker.h"
#include "TError.h"
#include "TFile.h"
#include "TGrid.h"
#include "TGridResult.h"
#include "TStopwatch.h"
#include "TSystem.h"
#include <iostream>
#include <regex>
#include <string>
#include <thread>
#include <vector>

AlienWorker::AlienWorker(std::string dir, std::string pattern = "*.root",
                         std::string destination = "", int numOfProcesses = 5,
                         int keepNLastDirNames = 3)
    : dir{dir}, pattern{pattern}, destination{destination},
      numOfProcesses{numOfProcesses}, keepNLastDirNames{keepNLastDirNames},
      filesList(0), splittedFileList(numOfProcesses) {

  ::Info("AlienWorker initialization", "Connecting to Alien...");
  TGrid::Connect("alien://");
  ::Info("AlienWorker initialization", "Successfully connected to Alien");

  if (!gGrid) {
    ::Error("AlienWorker initialization", "Can't connect to the grid");
  }

  if (!gGrid->Cd(dir.data())) {
    ::Error("AlienWorker initialization", "Directory %s doesn't exists in grid",
            dir.data());
  }

  if (destination.empty()) {
    AlienWorker::destination = std::string(gSystem->pwd());
    ::Warning(
        "AlienWorker initialization",
        "Destination directory is empty. Current directory will be use: '%s'",
        AlienWorker::destination.data());
  }
}

void AlienWorker::Copy() {
  ::Info("Initializing of copying proccess", "Copy from '%s' to '%s'",
         dir.data(), destination.data());
  TStopwatch st;
  st.Start();
  auto res = gGrid->Query(dir.data(), pattern.data());

  if (!res) {
    ::Error("Initializing of copying proccess",
            "The result of search query is nullptr.");
    return;
  }

  if (res->GetEntries() == 0) {
    ::Error(
        "Initializing of copying proccess",
        "Nothing has been found with such pattern '%s' in this direcotry '%s'",
        pattern.data(), dir.data());
    return;
  }

  ::Info("Initializing of copying proccess", "%d files has found for copying",
         res->GetEntries());

  auto cp = [&](const std::vector<std::string> &fileList) {
    for (const auto &f : fileList)
      TFile::Cp(f.data(), FormFileName(f).data(), false);
  };

  filesList.reserve(res->GetEntries());

  for (auto i{0}; i < res->GetEntries(); ++i)
    filesList.push_back(res->GetKey(i, "turl"));

  filesList.shrink_to_fit();

  std::vector<std::thread> threads;
  threads.reserve(numOfProcesses);

  SplitVectorToChunks();

  ::Info("Initializing of copying proccess",
         "Input file list was deleted into %d chunks with size %lu",
         numOfProcesses, splittedFileList[0].size());

  for (auto &chunk : splittedFileList) {
    ::Info("Initializing of copying proccess",
           "Chunk [%s]-[%s] was added to queue.", chunk[0].data(),
           chunk.back().data());
    threads.push_back(std::thread(cp, chunk));
  }

  for (auto &t : threads)
    t.join();

  st.Stop();
  ::Info("cpGRID", "Copying has completed");
  ::Info("cpGRID", "Elapsed time is %f sec.", st.RealTime());
}

AlienWorker::~AlienWorker() {}

std::string AlienWorker::FormFileName(std::string inputFile) {
  std::regex re("/");
  std::sregex_token_iterator first{inputFile.begin(), inputFile.end(), re, -1},
      last;
  std::vector<std::string> namesArr{first, last};

  std::string outputFile =
      std::string("file://") + destination + std::string("/");

  for (auto i = namesArr.size() - keepNLastDirNames; i < namesArr.size() - 1;
       ++i)
    outputFile += std::string(namesArr[i]) + std::string(".");

  outputFile += std::string(namesArr.back());

  return outputFile;
}

void AlienWorker::SplitVectorToChunks() {

  int chunkSize = AlienWorker::filesList.size() / numOfProcesses;

  if (filesList.size() % numOfProcesses)
    chunkSize += 1;

  ::Info("SplitVectorToChunks", "Size of chunk %d", chunkSize);

  int current = 0;

  std::for_each(splittedFileList.begin(), splittedFileList.end(),
                [&](std::vector<std::string> &v) {
                  v.reserve(chunkSize);
                  if (v.begin() != splittedFileList.back().begin())
                    v = std::vector<std::string>(filesList.begin() + current,
                                                 filesList.begin() + current +
                                                     chunkSize);
                  else
                    v = std::vector<std::string>(filesList.begin() + current,
                                                 filesList.end());

                  current += chunkSize;
                });
}
