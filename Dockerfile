FROM ubuntu:22.04

RUN apt-get update \
  && apt-get install -yq --no-install-recommends \
  pkg-config \
  libjansson-dev \
  libcurl4-openssl-dev \
  nlohmann-json3-dev \
  make \
  gcc \
  g++ \
  cmake \
  ca-certificates

WORKDIR /churchill

COPY . /churchill

RUN mkdir /churchill/build

WORKDIR /churchill/build

RUN cmake ..

RUN cmake --build .

CMD ["./churchill"]