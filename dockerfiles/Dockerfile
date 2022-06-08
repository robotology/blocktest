FROM icubteamcode/superbuild:master-unstable_sources

LABEL org.opencontainers.image.title="Blocktest Docker Image"
LABEL org.opencontainers.image.description="Infrastructure to deal with Blocktest"
LABEL org.opencontainers.image.source="https://github.com/robotology/blocktest"
LABEL org.opencontainers.image.authors="Luca Tricerri <luca.tricerri@iit.it>"

EXPOSE 22

ENV DEBIAN_FRONTEND=noninteractive

# Select options
ARG ROBOTOLOGY_SUPERBUILD_RELEASE=devel
ARG BLOCKTEST_RELEASE=devel
ARG BLOCKTEST_YARP_PLUGINS_RELEASE=master
ARG BUILD_TYPE=Debug
ARG PROJECTS_DIR=/projects
ARG CMAKE_GENERATOR="Unix Makefiles"
ARG CMAKE_EXTRA_OPTIONS=-j12
ARG INSTALL_DIR=/usr/local

RUN mkdir -p ${PROJECTS_DIR}
RUN mkdir -p ${PROJECTS_DIR}/install

# The bashrc is read only when opening an interactive shell.
# Thus, let other projects find packages contained in the superbuild.
ENV CMAKE_PREFIX_PATH=${INSTALL_DIR}

# Install blocktest
RUN cd ${PROJECTS_DIR} &&\
    git clone https://github.com/robotology/blocktest --branch ${BLOCKTEST_RELEASE}

RUN cd ${PROJECTS_DIR}/blocktest &&\
    mkdir build && cd build &&\
    cmake .. \
    -G "${CMAKE_GENERATOR}" \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DNON_INTERACTIVE_BUILD:BOOL=ON \
    -DCOMPILE_SCRIPTBUILDER=ON \
    -DCMAKE_INSTALL_PREFIX=${PROJECTS_DIR}/install \
    -DYCM_DIR=${INSTALL_DIR}/share/YCM &&\
    cmake --build . -- ${CMAKE_EXTRA_OPTIONS} &&\
    cmake --install . && \
    cd ../../ 
# && rm -Rf blocktest

# Install blocktest-yarp-plugin
RUN cd ${PROJECTS_DIR} &&\
    git clone https://github.com/robotology/blocktest-yarp-plugins --branch ${BLOCKTEST_YARP_PLUGINS_RELEASE}

RUN cd ${PROJECTS_DIR}/blocktest-yarp-plugins &&\
    mkdir build && cd build &&\
    cmake .. \
    -G "${CMAKE_GENERATOR}" \
    -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    -DNON_INTERACTIVE_BUILD:BOOL=ON \
    -DYCM_DIR=${INSTALL_DIR}/share/YCM \
    -DCMAKE_INSTALL_PREFIX=${PROJECTS_DIR}/install \
    -Dblocktestcore_DIR=${PROJECTS_DIR}/blocktest/build && \
    cmake --build . -- ${CMAKE_EXTRA_OPTIONS} && \
    cmake --install . && \
    cd ../../ 
# && rm -Rf blocktest-yarp-plugins

RUN mkdir -p /root/.config/yarp
COPY yarp.conf /root/.config/yarp/yarp.conf
#RUN echo "export PATH=\${PATH}:${PROJECTS_DIR}/yarp-device-ultrapython/clientScript:${PROJECTS_DIR}/yarp-device-ultrapython/build/bin" >> ${ROBOTOLOGY_INITIALIZATION_FILE}

#Other
RUN apt update &&\
    apt install -y \
    netcat \
    iproute2 \
    screen \
    gedit

# Clean up git configuration
#RUN git config --global --unset-all user.name && \
#    git config --global --unset-all user.email

EXPOSE 10000/tcp 10000/udp

# Some QT-Apps don't show controls without this
ENV QT_X11_NO_MITSHM 1
ENV YARP_COLORED_OUTPUT 1

ARG metadata="none"
ENV img_metadata=${metadata}
RUN echo 'echo 'This image has Blocktest docker build v1.1

# Clean up unnecessary installation products
RUN rm -Rf /var/lib/apt/lists/*

CMD ["bash"]



