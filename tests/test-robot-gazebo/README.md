# 1. Test summary
This test is designed for testing YARP and Gazebo applications.

# 2. Prerequisites
- [Robotology-superbuild](https://github.com/robotology/robotology-superbuild)
- [Blocktest](https://github.com/robotology/blocktest)
- [Blocktest-yarp](https://github.com/robotology/blocktest-yarp-plugins)

# 3. Execution
```bash
cd <your-blocktest-clonedir>/blocktest/install/bin
./blocktestrunner test.xml tests/test-robot-gazebo
```

# 4. Test description
The test will move some part of the iCub robot on Gazebo.
