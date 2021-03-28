FROM ubuntu:20.10

RUN sed -i "s@http://.*archive.ubuntu.com@http://repo.huaweicloud.com@g" /etc/apt/sources.list && \
    sed -i "s@http://.*security.ubuntu.com@http://repo.huaweicloud.com@g" /etc/apt/sources.list

RUN apt-get update && \
    apt-get install --no-install-recommends \
        clang-11 clang++-11 clang-tidy cmake gdb rsyslog git nodejs npm make \
        libgrpc++1 libgrpc10 protobuf-compiler protobuf-compiler-grpc libgrpc++-dev libgrpc-dev libprotobuf-dev \
        libsqlite3-dev -y

WORKDIR /opt