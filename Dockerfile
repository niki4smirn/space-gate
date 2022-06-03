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
RUN apt -y install python3-venv python3-pip
RUN pip install cmake --upgrade
RUN apt-get -y install software-properties-common
RUN add-apt-repository ppa:beineri/opt-qt-5.15.2-focal -y
RUN apt install -y libglu1-mesa-dev 
RUN apt install -y libgl1-mesa-dev 
RUN apt install -y qt515base 
RUN apt install -y qt515multimedia 
RUN apt install -y qt515websockets 
RUN apt install -y qt515svg 

RUN apt-get install -y autoconf automake libtool curl make g++ unzip wget
RUN wget https://github.com/protocolbuffers/protobuf/releases/download/v3.15.5/protobuf-cpp-3.15.5.tar.gz
RUN tar -xf protobuf-cpp-3.15.5.tar.gz
WORKDIR protobuf-3.15.5
RUN ./configure
RUN make 
RUN make install 
RUN ldconfig 

ARG SSH_PRIVATE_KEY
RUN mkdir /root/.ssh/
RUN echo "${SSH_PRIVATE_KEY}" > /root/.ssh/id_rsa
RUN chmod 400 /root/.ssh/id_rsa

RUN touch /root/.ssh/known_hosts
RUN ssh-keyscan github.com >> /root/.ssh/known_hosts

WORKDIR /root
RUN git clone git@github.com:niki4smirn/space-gate.git
WORKDIR space-gate
RUN git checkout origin/deployment
RUN mkdir cmake-build-release

RUN cmake -S /root/space-gate -B /root/space-gate/cmake-build-release
RUN cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ -G "CodeBlocks - Unix Makefiles" /root/space-gate/cmake-build-release
RUN cmake --build /root/space-gate/cmake-build-release --target server -j 8
WORKDIR /root/space-gate/cmake-build-release
CMD ./server
