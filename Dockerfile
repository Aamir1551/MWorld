FROM ubuntu:latest
ENV DEBIAN_FRONTEND=noninteractive 
RUN apt update
RUN apt install build-essential -y
RUN apt install vim -y
RUN apt install cmake -y
RUN apt install ninja-build -y 
RUN apt install gcc -y


COPY . /usr/src/MWorld

CMD ["bash"]