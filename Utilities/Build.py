#!/usr/bin/python

import os
import sys
import time
import subprocess

startTime = time.time()
bPrintTime = True
argList = sys.argv[1:]

currentDir = os.getcwd()
riderDir = os.path.dirname("C:\Program Files\JetBrains\Rider for Unreal Engine 2021.2.1\\bin\\")

riderFile = os.path.join(riderDir, "rider64.exe")
solutionFile = os.path.join(currentDir, "Unica.sln")
premakeFile = os.path.join(os.path.dirname(__file__), "premake5.exe")

def disableExecutionTimer():
    global bPrintTime
    bPrintTime = False

def printHelpText():
    print("Help Text")

if (len(argList) < 1):
    print("No command provided.\nTry -h for a list of possible commands.")
    disableExecutionTimer()

for arg in argList:
    match arg:
        case "-h":
            printHelpText()
            disableExecutionTimer()

        case "-generate":
            subprocess.run([premakeFile, "vs2022"])

        case "-rider":
            subprocess.Popen([riderFile, solutionFile])

        case _:
            print("Invalid command.\nTry -h for a list of possible commands.")
            disableExecutionTimer()

if (bPrintTime):
    print("Execution finished in {0} seconds".format(time.time() - startTime))

