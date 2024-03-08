
# !/bin/sh

# The autoreconf runs all the autotools configuration tools in the right order
# and will avoid regenerating files.

autoreconf --install --make # install missing files
# automake --add-missing --copy >/dev/null 2>&1 # add install-sh
