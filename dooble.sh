#!/bin/sh

export AA_ENABLEHIGHDPISCALING=1
export AA_USEHIGHDPIPIXMAPS=1

# Maximum of 50.

export DOOBLE_ADDRESS_WIDGET_HEIGHT_OFFSET=5

# Maximum of 50.

export DOOBLE_TAB_HEIGHT_OFFSET=5

# Automatic scaling.

export QT_AUTO_SCREEN_SCALE_FACTOR=1

# Disable https://en.wikipedia.org/wiki/MIT-SHM.

export QT_X11_NO_MITSHM=1

if [ -r ./Dooble ] && [ -x ./Dooble ]
then
    exec ./Dooble "$@"
    exit $?
elif [ -r /opt/dooble/Dooble ] && [ -x /opt/dooble/Dooble ]
then
    export LD_LIBRARY_PATH=/opt/dooble/Lib
    export QT_PLUGIN_PATH=/opt/dooble/plugins
    cd /opt/dooble && exec ./Dooble "$@"
    exit $?
elif [ -r /usr/local/dooble/Dooble ] && [ -x /usr/local/dooble/Dooble ]
then
    cd /usr/local/dooble && exec ./Dooble "$@"
    exit $?
else
    exit 1
fi
