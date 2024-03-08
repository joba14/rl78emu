
# !/bin/sh

./clean.sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$SCRIPT_DIR/.."
cd $PROJECT_DIR
mkdir "./build"
./autogen.sh
cd $PROJECT_DIR/build
./../configure CFLAGS="$FLAGS" CPPFLAGS="$DEFINES"
cd $SCRIPT_DIR

./build.sh
