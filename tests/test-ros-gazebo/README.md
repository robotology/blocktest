# 1. Test summary
This test is designed for testing ROS2 connection with Gazebo.

# 2. Prerequisites
- [Robotology-superbuild](https://github.com/robotology/robotology-superbuild)
- [Blocktest](https://github.com/robotology/blocktest)
- [Blocktest-ros](https://github.com/robotology/blocktest-ros-plugins)
- Install Gazebo
- Install ROS2 humble
- Install ROS2 humble Gazebo plugins `sudo apt install ros-humble-gazebo-ros-pkgs` see [here]([https://](http://classic.gazebosim.org/tutorials?tut=ros2_installing&))    cat=connect_ros

# 3. Execution
```bash
cd <your-blocktest-clonedir>/blocktest/install/bin
./blocktestrunner test.xml tests/test-ros-gazebo
```

# 4. Test description
The test will send messages through the ROS2 topic message system to move the three-wheeled robot.