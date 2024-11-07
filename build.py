import os
import shutil

if __name__ == "__main__":
    if shutil.which("cmake") is None:
        raise Exception("CMAKE is not available")

    if shutil.which("ninja") is None:
        raise Exception("NINJA is not available")

    script_directory = os.path.dirname(os.path.abspath(__file__))
    project_root_path_detector = os.path.join(script_directory, "config", "default.ini")
    print(project_root_path_detector)
