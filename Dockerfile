FROM ubuntu:20.04
CMD bash

# Install Ubuntu packages.
# Please add packages in alphabetical order.
ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -y update 
RUN apt-get -y install sudo
COPY . /JuCC
WORKDIR /JuCC

RUN echo y | ./script/installation/packages.sh  all
RUN mkdir build
WORKDIR /JuCC/build
#RUN cmake -GNinja -DCMAKE_BUILD_TYPE=Release ..

WORKDIR /JuCC
