#!/bin/bash

# This small script creates the documentation
# in the same directory.

# It *MUST* be runned from the top directory of the
# sources.
# It will warn if this is not the case

_DOC=final_doc

# NOTE if any user may end up symlinking the script, this
# will not work. However, who would do that?
_sd="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"

# Get the calling directory
_cwd=`pwd`

if [[ $(dirname $_sd) != $_cwd ]]; then
    echo "$0 must be called from this directory:"
    echo "  $(dirname $_sd)"
    echo "Currently it is called from:"
    echo "  $_cwd"
    exit 1
fi

# Start on the actual creation of the documentation
# We create a temporary directory
rm -rf $_DOC
mkdir -p $_DOC
pushd $_DOC

for f in ../src/*.F90
do
    PREFIX=" " ../tools/pp.sh $f
done
    
# Now we can create the documentation
{
    cat ../doc/Doxyfile
    echo "INPUT = ../README.md ./ ../doc/DEV.md"
} | doxygen -

popd