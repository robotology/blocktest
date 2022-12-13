# 1. Test summary
This test is designed for testing ROS2 applications.

# 2. Prerequisites
- [Robotology-superbuild](https://github.com/robotology/robotology-superbuild)
- [Blocktest](https://github.com/robotology/blocktest)
- [Blocktest-ros](https://github.com/robotology/blocktest-ros-plugins)
- Install ROS2 foxy
- Install ROS2 turtleSim application
```bash
    sudo apt update
    sudo apt install ros-foxy-turtlesim
   ```
   and follow ROS2 tutorial here:https://docs.ros.org/en/foxy/Tutorials/Turtlesim/Introducing-Turtlesim.html

# 3. Execution
```bash
cd <your-blocktest-clonedir>/blocktest/install/bin
./blocktestrunner test.xml tests/test-ros-turtle
```

# 4. Test description
The test will send messages through the ROS2 topic message system to move the turtle.