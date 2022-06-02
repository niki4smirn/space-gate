# syntax=docker/dockerfile:1
FROM ubuntu:20.04
ENV TZ=Europe/Minsk
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone
RUN apt -y update
RUN apt -y upgrade
RUN apt-get -y install clang
RUN apt -y install git
RUN apt-get -y update
RUN apt-get -y upgrade
RUN apt -y update
RUN apt -y upgrade
RUN apt -y update
RUN apt -y install python3-venv python3-pip
RUN pip install cmake --upgrade
RUN apt-get -y install software-properties-common
RUN add-apt-repository ppa:beineri/opt-qt-5.15.2-focal -y
RUN apt install -y protobuf-compiler

RUN mkdir -p /root/SpaceGate/
RUN mkdir 1
WORKDIR /root/SpaceGate

ARG SSH_PRIVATE_KEY
RUN mkdir /root/.ssh/
RUN echo "${SSH_PRIVATE_KEY}" > /root/.ssh/id_rsa
RUN chmod 400 /root/.ssh/id_rsa

RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan github.com >> /root/.ssh/known_hosts

RUN git init
RUN git remote add origin git@github.com:niki4smirn/space-gate.git
RUN git pull origin deployment
RUN mkdir cmake-build-release
RUN cmake -S/root/SpaceGate -B /root/SpaceGate/cmake-build-release
RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -G "CodeBlocks - Unix Makefiles" /root/SpaceGate/cmake-build-release
RUN cmake --build /root/SpaceGate/cmake-build-release --target server -j 8
WORKDIR /root/SpaceGate/cmake-build-release
CMD ./server
