#!/bin/bash
set -e
source /opt/ros/foxy/setup.bash
export ROS_DOMAIN_ID=12
# If a CMD is passed, execute it
exec "$@"