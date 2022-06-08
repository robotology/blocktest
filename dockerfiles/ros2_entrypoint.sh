#!/bin/bash
set -e
source /opt/ros/foxy/setup.bash
# If a CMD is passed, execute it
exec "$@"