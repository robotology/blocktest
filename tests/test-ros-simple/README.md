# 1. Test summary
This test is designed for testing ROS2 applications.

# 2. Prerequisites
- [Robotology-superbuild](https://github.com/robotology/robotology-superbuild)
- [Blocktest](https://github.com/robotology/blocktest)
- [Blocktest-ros](https://github.com/robotology/blocktest-ros-plugins)
- Install ROS2 foxy

# 3. Execution
```bash
cd <your-blocktest-clonedir>/blocktest/install/bin
./blocktestrunner test.xml tests/test-ros-simple
```

# 4. Test description
The test will send and receive messages through the ROS2 topic message system.
