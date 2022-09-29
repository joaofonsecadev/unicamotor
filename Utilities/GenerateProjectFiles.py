# 2021-2022 Copyright joaofonseca.dev, All Rights Reserved.

import os
import sys
import time
import shutil
import platform
import subprocess

startTime = time.time()

helpText = "\n\
Run the script without any commands to generate project files adequate for your system.\n\n\
The following arguments are accepted:\n\
-h: Print this text.\n"

# 'Linux', 'Darwin', 'Java', 'Windows'
systemType = platform.system()

projectType = ""
argList = sys.argv[1:]
buildHelperDir = os.path.dirname(__file__)

# Make sure we are in the right directory
os.chdir(buildHelperDir)
os.chdir('..')


def ensurecmake():
    cmakepath = shutil.which("cmake")
    if cmakepath is None:
        print("No CMake found in PATH, aborting")
        endexecution()
    return cmakepath


def generateproject():
    subprocess.run([ensurecmake(),
                    "-S.",
                    "-B{0}/Intermediate".format(os.getcwd()),
                    projectType
                    ])


def endexecution():
    print("Execution finished in {0} seconds".format(time.time() - startTime))
    sys.exit()


if sys.version_info < (3, 10):
    print('Python 3.10 or greater is required.')
    endexecution()

if systemType == 'Windows':
    projectType = '-GVisual Studio 17 2022'
elif systemType == 'Darwin':
    projectType = '-GXcode'
else:
    print('System "{0}" not supported by default. Defaulting to Make Files'.format(systemType))
    projectType = '-GUnix Makefiles'

if len(argList) < 1:
    generateproject()
    endexecution()

for arg in argList:
    match arg:
        case "-h":
            print(helpText)

        case "-vs2019":
            projectType = '-GVisual Studio 16 2019'

        case "-vs2022":
            projectType = '-GVisual Studio 17 2022'

        case "-xcode":
            projectType = '-GXcode'

        case "-make":
            projectType = '-GUnix Makefiles'

        case "-clean":
            subprocess.run(["git", "clean", "-fxd"])
            endexecution()

        case _:
            print("Invalid command.\nTry -h for some useful information on how to run this script.")

generateproject()
endexecution()
