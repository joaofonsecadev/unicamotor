#!/bin/bash

# Get the directory where the script is located
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Default build type
BUILD_TYPE="Debug"

# Function to print usage
print_usage() {
    echo "Usage: $0 [-t <build_type>]"
    echo "  -t <build_type>    Specify build type (Debug, Release, RelWithDebInfo, MinSizeRel)"
    echo "  -h                 Show this help message"
}

# Parse command line arguments
while getopts "t:h" opt; do
    case $opt in
        t)
            BUILD_TYPE="$OPTARG"
            ;;
        h)
            print_usage
            exit 0
            ;;
        \?)
            echo "Invalid option: -$OPTARG" >&2
            print_usage
            exit 1
            ;;
    esac
done

# Validate build type
case $BUILD_TYPE in
    Debug|Release|RelWithDebInfo|MinSizeRel)
        ;;
    *)
        echo "Invalid build type: $BUILD_TYPE" >&2
        print_usage
        exit 1
        ;;
esac

# Configure with CMake
BUILD_DIR="$SCRIPT_DIR/build"
cmake -G Ninja -DCMAKE_BUILD_TYPE="$BUILD_TYPE" -B "$BUILD_DIR" -S "$SCRIPT_DIR"

# Build with Ninja
ninja -C "$BUILD_DIR"

echo "Build completed for $BUILD_TYPE configuration"
