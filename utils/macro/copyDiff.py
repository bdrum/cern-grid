import os
import numpy as np

gridFiles    = np.array(os.popen(f'alien.py find 4Prongs2015o AnalysisResults.root').readlines())
localFiles   =  np.array(os.popen('find . -name *.root'))

diff = np.setdiff1d(gridFiles, localFiles)


