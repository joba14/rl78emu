
# !/bin/sh

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$SCRIPT_DIR/.."
cd $PROJECT_DIR

# --------------------------------------------------------------------------- #

rm -fr "./build"
rm -fr "./aux"
rm -fr "./autom4te.cache"
rm -fr "./aclocal.m4"
rm -fr "./configure"
rm -fr "./configure~"
rm -fr "./Makefile.in"
rm -fr "./makefile.in"
rm -fr "./version.h.in~"
rm -fr "./depcomp"
rm -fr "./ltmain.sh"
rm -fr "./missing"
rm -fr "./install-sh"
rm -fr "./config.sub"
rm -fr "./config.guess"
rm -fr "./config.log"
rm -fr "./compile"
rm -fr "./m4"
rm -fr "./scripts/config.log"
rm -fr "./docs"
