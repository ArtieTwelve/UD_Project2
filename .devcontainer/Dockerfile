#FROM i386/ubuntu:18.04
FROM ubuntu:18.04

# Base stuff

RUN apt-get update 
# Setup what's needed to get the latest release of cmake (>3.10)
RUN apt-get install -y apt-transport-https ca-certificates gnupg software-properties-common wget

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null


RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main'
RUN apt-get install -y --no-install-recommends cmake

# Project 2 is a minic of htop. Adding to image so we can compair the two.

RUN apt-get install htop

RUN apt-get install -y  gcc gdb

RUN apt-get install -y libncurses5-dev libncursesw5-dev

RUN apt-get install -y --no-install-recommends \
    libssl-dev \
    openssl \
    libtool \
    autoconf \
    automake \
    build-essential \
    uuid-dev 

RUN apt-get install -y --no-install-recommends \
    tofrodos \
    curl \
    tree \
    silversearcher-ag \
    valgrind \
    git 


RUN apt-get install -y --no-install-recommends libboost-dev
 
# because, why not?
RUN apt-get install -y --no-install-recommends sudo vim


RUN apt-get autoremove

RUN apt-get install stress

RUN rm -rf /var/lib/apt/lists/*



