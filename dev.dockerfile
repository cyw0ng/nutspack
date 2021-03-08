FROM ubuntu:20.10

RUN sed -i "s@http://.*archive.ubuntu.com@http://repo.huaweicloud.com@g" /etc/apt/sources.list && \
    sed -i "s@http://.*security.ubuntu.com@http://repo.huaweicloud.com@g" /etc/apt/sources.list

RUN apt-get update && \
    apt-get install clang-11 clang++-11 clang-tidy cmake gdb rsyslog git -y

WORKDIR /opt