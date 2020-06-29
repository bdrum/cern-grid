#!/usr/bin/python3

import os
import sys
import time
from subprocess import Popen, PIPE
from multiprocessing import Process


def listdir_shell(path, pattern):
    p = Popen(('alien_find', path, pattern),
              shell=False,
              stdout=PIPE,
              close_fds=True)
    return [
        str(path, "utf-8").rstrip('\n').strip()
        for path in p.stdout.readlines()
    ]


def copy(runList, runFiles):
    print('Id of process', os.getpid(), 'and output:')
    for run in runList:
        for file in runFiles[run]:
            tmpArr = file.split('/')
            newName = tmpArr[-3] + '.' + tmpArr[-2] + '.' + tmpArr[-1]

            if os.path.isfile(newName):
                continue

            newName = 'file:' + newName
            os.system('alien_cp -o alien:' + file + ' ' + newName)


# get out last two strings: empty and number of files
runFiles = {}

for file in listdir_shell(sys.argv[1], sys.argv[2])[:-2]:
    if file.split('/')[-3] not in runFiles.keys():
        runFiles[file.split('/')[-3]] = []
    else:
        runFiles[file.split('/')[-3]].append(file)

processNum = 5
if len(sys.argv) > 3:
    processNum = sys.argv[3]

start = 0
partLen = len(runFiles.keys()) // processNum + 1
finish = partLen
runs = list(runFiles.keys())
print(len(runs))

for i in range(processNum):
    print('Start copy files from runs: ', runs[start], ' to ', runs[finish])
    p = Process(target=copy, args=(runs[start:finish], runFiles))
    p.start()
    # p.join()
    start = finish + 1
    finish += partLen
    if finish > len(runs):
        finish = len(runs) - 1
