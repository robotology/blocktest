# 1. Test summary
This test is designed for testing ROS2 connection with Gazebo.

# 2. Prerequisites
- [Robotology-superbuild](https://github.com/robotology/robotology-superbuild)
- [Blocktest](https://github.com/robotology/blocktest)
- [Blocktest-ros](https://github.com/robotology/blocktest-ros-plugins)
- Install Gazebo
- Install ROS2 foxy
- Install ROS2 foxy Gazebo plugins see http://gazebosim.org/tutorials?tut=ros2_overview

# 3. Execution
```bash
cd <your-blocktest-clonedir>/blocktest/install/bin
./blocktestrunner test.xml tests/test-ros-gazebo
```

# 4. Test description
The test will send messages through the ROS2 topic message system to move the three-wheeled robot.