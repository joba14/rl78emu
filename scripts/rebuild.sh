
# !/bin/sh

# Set the build configuration (default to debug if not provided)
BUILD_CONFIGURATION=${1:-"debug"}

# Choose compiler flags and defines based on build configuration
case $BUILD_CONFIGURATION in
	"debug")
		FLAGS="-g -O0"
		DEFINES=""
		;;
	"hybrid")
		FLAGS="-g -O2"
		DEFINES=""
		;;
	"release")
		FLAGS="-O3"
		DEFINES="-DNDEBUG"
		;;
	*)
		echo "[error]: invalid build configuration. Please use 'debug', 'hybrid', or 'release'."
		exit 1
		;;
esac

./clean.sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$SCRIPT_DIR/.."
cd $PROJECT_DIR

./autogen.sh
mkdir "./build"

cd $PROJECT_DIR/build
./../configure CFLAGS="$FLAGS" CPPFLAGS="$DEFINES"
cd $SCRIPT_DIR

./build.sh
