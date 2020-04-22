include_directories(inc)
set(worker inc/AlienWorker.h src/AlienWorker.cxx)

add_library(Worker SHARED ${worker})
