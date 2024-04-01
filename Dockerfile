FROM ubuntu:22.04

# Specify terminal color
ENV TERM xterm-256color

# Install nvim
RUN apt-get update && apt-get install -y neovim 

# Install platformio
RUN apt install curl -y
RUN apt-get install python3-venv -y
RUN curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
RUN python3 get-platformio.py

RUN mkdir -p /usr/local/bin
RUN ln -s ~/.platformio/penv/bin/platformio /usr/local/bin/platformio
RUN ln -s ~/.platformio/penv/bin/pio /usr/local/bin/pio
RUN ln -s ~/.platformio/penv/bin/piodebuggdb /usr/local/bin/piodebuggdb