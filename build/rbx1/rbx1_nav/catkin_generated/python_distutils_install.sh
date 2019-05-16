#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
    DESTDIR_ARG="--root=$DESTDIR"
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/catkin_ws/src/rbx1/rbx1_nav"

# snsure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/catkin_ws/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/catkin_ws/install/lib/python2.7/dist-packages:/home/catkin_ws/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/catkin_ws/build" \
    "/root/anaconda2/bin/python" \
    "/home/catkin_ws/src/rbx1/rbx1_nav/setup.py" \
    build --build-base "/home/catkin_ws/build/rbx1/rbx1_nav" \
    install \
    $DESTDIR_ARG \
    --install-layout=deb --prefix="/home/catkin_ws/install" --install-scripts="/home/catkin_ws/install/bin"
