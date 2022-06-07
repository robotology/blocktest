# 1. Terminology

**host**: your Ubuntu laptop  
**Docker image**: the Docker ready to be executed  
**Docker container**: the running/stopped docked  

# 2. Ubuntu prerequisite
Install the missing applications.  
Use the following command in a bash terminal:
```bash
sudo apt update
sudo apt install apt-transport-https ca-certificates curl software-properties-common
curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo apt-key add -
sudo add-apt-repository "deb [arch=amd64] https://download.docker.com/linux/ubuntu focal stable"
sudo apt update
sudo apt install docker-ce
```

# 3. Pull the Blocktest Docker

Execute the following:
```bash
docker pull ghcr.io/robotology/yarp-device-ultrapython:latest
```

# 4. Using the Blocktest Docker
Follow the steps:

- Use the command on your host:
```bash
docker run --rm -it --network host --privileged --env DISPLAY=${DISPLAY} --env XAUTHORITY=/root/.Xauthority --mount type=bind,source=${XAUTHORITY},target=/root/.Xauthority --mount type=bind,source=/tmp/.X11-unix,target=/tmp/.X11-unix --name my_blocktest  blocktest

cd /project/install/bin
./blocktestrunner test.xml test

```

# 6. Create (only if you need it)
Use the following command in the docker folder:
```console
docker build . --build-arg "BLOCKTEST_RELEASE=devel" --build-arg "ROBOTOLOGY_SUPERBUILD_RELEASE=v2022.05.0" --tag blocktest
```

# Note
```
https://hub.docker.com/r/icubteamcode/superbuild
```