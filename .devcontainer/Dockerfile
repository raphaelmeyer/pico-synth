FROM ubuntu:mantic

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive && \
  apt-get install -qy \
  build-essential \
  clangd \
  cmake \
  gcc-arm-none-eabi \
  git \
  libnewlib-arm-none-eabi \
  libstdc++-arm-none-eabi-newlib \
  ninja-build \
  python3 \
  && apt-get autoremove -y \
  && apt-get clean -y \
  && rm -rf /var/lib/apt/lists/*

RUN mkdir /pico-sdk && chown -R ubuntu:ubuntu /pico-sdk

USER ubuntu

RUN git clone https://github.com/raspberrypi/pico-sdk.git /pico-sdk \
  && cd /pico-sdk \
  && git switch -c raphaelmeyer/1.5.1 1.5.1 \
  && git submodule update --init

ENV PICO_SDK_PATH /pico-sdk

VOLUME [ "/pico-sdk" ]
