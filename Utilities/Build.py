import os
import sys
import time
import platform
import subprocess

startTime = time.time()

helpText = "\n\
Run the script without any commands to generate project files adequate for your system (Windows: Visual Studio 2019; MacOS: XCode 4).\n\n\
The following arguments are accepted:\n\
-h: Print this text.\n\
-gmake: Generate GNU Make project files.\n"

# 'Linux', 'Darwin', 'Java', 'Windows'
systemType = platform.system();

premakeTool = ""
projectType = ""
argList = sys.argv[1:]
buildHelperDir = os.path.dirname(__file__)

# Make sure we are in the right directory
os.chdir(buildHelperDir)
os.chdir('..')

def generateProject():
    subprocess.run([premakeTool, projectType])

def endExecution():
    print("Execution finished in {0} seconds".format(time.time() - startTime))
    sys.exit()

if sys.version_info < (3, 10):
    print('Python 3.10 or greater is required.')
    endExecution()

if systemType == 'Windows':
    premakeTool = os.path.join(buildHelperDir, 'premake5\premake5.exe')
    projectType = 'vs2019'
elif systemType == 'Darwin':
    premakeTool = os.path.join(buildHelperDir, 'premake5/premake5')
    projectType = 'xcode4'
else:
    print('System "{0}" not supported'.format(systemType))
    endExecution()

if (len(argList) < 1):
    generateProject()
    endExecution()

for arg in argList:
    match arg:
        case "-h":
            print(helpText)

        case "-vstudio":
            projectType = 'vs2019'

        case "-xcode":
            projectType = 'xcode4'

        case "-cmake":
            projectType = 'cmake'

        case "-gmake":
            projectType = 'gmake2'

        case _:
            print("Invalid command.\nTry -h for some useful information on how to run this script.")

generateProject()
endExecution()