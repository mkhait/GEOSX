'''
Created on 9/02/2021

@author: macpro
'''

import pygeosx
import sys
import multiprocessing as mp

from mesh import *
from acquisition import *
from shotFileManager import *

basePath = sys.argv[3] #Absolute path where the GEOSX folder is located

wavePropagationPath = os.path.join(basePath, "GEOSX/src/coreComponents/physicsSolvers/wavePropagation/")
pygeosxPath = os.path.join(wavePropagationPath, "pygeosx/")
segyPath = os.path.join(pygeosxPath, "segyAcquisition/")

xmlPath = sys.argv[2]


def multiProcessing(shot_list, nb_proc = 1):
    p = []
    nb_shot_m1 = len(shot_list)
    ind = 0

    #Loop over the process launch
    for i in range(nb_proc):
        nb_shot = int(nb_shot_m1/(nb_proc-i))

        shot_file = exportShotList(i, shot_list[ind:ind + nb_shot])

        p.append( mp.Process(target = mainProcess,
                             args   = (shot_file,) ) )

        ind = ind + nb_shot
        nb_shot_m1 = nb_shot_m1 - nb_shot

        #Start process
        p[i].start()


    for i in range(nb_proc):
        p[i].join()


def mainProcess(shot_file):

    tracePath = os.path.join(rootPath, "outputSismoTrace/")
    if os.path.exists(tracePath):
        pass
    else:
        os.mkdir(tracePath)

    traceProcPath = os.path.join(tracePath, "traceProc" + str(mp.current_process()._identity)[1] + "/")
    if os.path.exists(traceProcPath):
        pass
    else:
        os.mkdir(traceProcPath)

    fsh = open("bash.sh", 'w+')
    fsh.write("#!/bin/bash \n")
    fsh.write("SBATCH --job-name=test_pygeosx \n")
    fsh.write("SBATCH -n 8 \n")
    fsh.write("SBATCH -N 1 \n")
    fsh.write("SBATCH -C bora \n")
    fsh.write("cd /beegfs/jbesset/codes/GEOSX/src/coreComponents/physicsSolvers/wavePropagation/pygeosx/ \n")
    fsh.write("mpirun </dev/null --map-by node python" + pygeosxPath + "/main.py -i " + xmlPath + " -x 4 -y 2 " + shot_file + " " + traceProcPath)
    fsh.close()

    os.system("bash bash.sh")

    #cmd = "mpirun -np 32 python " + pygeosxPath + "/main.py -i " + xmlPath + " -x 8 -y 4 " + shot_file + " " + traceProcPath

    #os.system(cmd)




def remove_tmp_files():
    for root, dir, files in os.walk(os.path.join(rootPath, "shots_lists")):
        for file in files:
            os.remove(os.path.join(root, file))

    os.rmdir(os.path.join(rootPath, "shots_lists"))



def main():

    os.system("mpirun -np 8 python firstInit.py " + str(sys.argv[1]) + " " + str(sys.argv[2]) +" -x 4 -y 2" )

    maxT, dt, boundary_box = readInitVariable()

    frequency = 5.0
    wavelet   = ricker(maxT, dt, frequency)


    shot_list = equispaced_acquisition(boundary_box,
                                       wavelet,
                                       dt,
                                       [101, 1901],
                                       [1001, 1001],
                                       [21, 1981],
                                       [1001, 1001],
                                       4,
                                       1,
                                       20,
                                       1)
    """

    shot_list = cross_acquisition(boundary_box,
                                  wavelet,
                                  dt,
                                  [1001, 11001, 4],
                                  [6751, 6751, 1],
                                  209,
                                  [[21, 13481, 675],[6751, 6751, 1]],
                                  [[6751, 6751, 1],[21, 13481, 675]],
                                  189,
                                  export = 1
                                  )


    shot_list = moving_acquisition(boundary_box,
                                   wavelet,
                                   dt,
                                   nbsourcesx = 30,
                                   nbsourcesy = 1,
                                   nbreceiversx = 50,
                                   nbreceiversy = 1,
                                   lenRx = 50,
                                   lenRy = 0,
                                   export = 1
                                   )
    """

    #shot_list = segy_acquisition(segyPath + acqName, wavelet, dt)

    multiProcessing(shot_list)

    remove_tmp_files()

if __name__ == "__main__":
    main()
