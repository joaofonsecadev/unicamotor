# 2022-2023 Copyright joaofonseca.dev, All Rights Reserved.

import os
import datetime
import pathlib

CopyrightDisclaimer = "2021-{} Copyright joaofonseca.dev, All Rights Reserved.\n".format(datetime.date.today().year)

# Make sure we are in the right directory
FileDirectory = os.path.dirname(__file__)
os.chdir(FileDirectory)
os.chdir('..')

SourceFilePaths = list(pathlib.Path(os.getcwd()).rglob("Unica/Source/**/*.*"))
UtilitiesFilePaths = list(pathlib.Path(os.getcwd()).rglob("Utilities/**/*.*"))

AllFilePaths = SourceFilePaths + UtilitiesFilePaths

for FilePath in AllFilePaths:
    if FilePath.suffix == ".py":
        AdaptedCopyrightDisclaimer = "# {}".format(CopyrightDisclaimer)
    else:
        AdaptedCopyrightDisclaimer = "// {}".format(CopyrightDisclaimer)

    with open(FilePath.absolute()) as File:
        FileLines = File.readlines()

    FirstLine = FileLines[0]
    if FirstLine == AdaptedCopyrightDisclaimer:
        print("\033[90m{}\033[0m".format(FilePath.absolute()))
        continue

    if "copyright" in FirstLine.lower():
        FileLines[0] = AdaptedCopyrightDisclaimer
    else:
        FileLines.insert(0, "\n")
        FileLines.insert(0, AdaptedCopyrightDisclaimer)

    with open(FilePath.absolute(), "w") as NewFile:
        NewFile.writelines(FileLines)
        print(FilePath.absolute())
