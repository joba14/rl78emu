
# !/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$SCRIPT_DIR/.."
cd $PROJECT_DIR

# --------------------------------------------------------------------------- #

# Build command
cd ./build
./rl78emu_cli/rl78emu_cli $1
