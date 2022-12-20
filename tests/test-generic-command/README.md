# 1. Test summary
This test is designed for executing a simple echo command and check for a ERROR in log.

# 2. Prerequisites
- [Blocktest](https://github.com/robotology/blocktest)

# 3. Execution

```bash
cd <your-blocktest-clonedir>/blocktest/install/bin
./blocktestrunner test.xml test-generic-command/
```

# 4. Test description

The test generate in the log folder a echo.log file and it checks inside the file for ERROR string.
It will also be generated a .bck file for the echo.log file.


