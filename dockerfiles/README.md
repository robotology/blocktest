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

# 3. Using the Blocktest Docker

Use these commands on your host:

```bash
docker run --rm -it --network host --privileged --env DISPLAY=${DISPLAY} --env XAUTHORITY=/root/.Xauthority --mount type=bind,source=${XAUTHORITY},target=/root/.Xauthority --mount type=bind,source=/tmp/.X11-unix,target=/tmp/.X11-unix --name my_blocktest  icubteamcode/blocktest-robometry:latest

cd /projects/install/bin
./blocktestrunner test.xml test

```

If you need to open a new bash:
```bash
docker exec -it my_blocktest bash
```

# 4. Create 
Use the following command in the docker folder:
```console
docker build -f ./DockerfileRos2 --tag icubteamcode/blocktest-robometry:latest
```
For push:
```bash
docker login
docker push icubteamcode/blocktest-robometry:latest
```

# 5. Pull the Blocktest Docker -- obsolete

Execute the following:
```bash
docker pull ghcr.io/robotology/yarp-device-ultrapython:latest
```

