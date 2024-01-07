FROM ubuntu:22.04

# Install required packages
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
  ca-certificates \
  valgrind \
  dialog 

# Set the working directory to /churchill
WORKDIR /churchill

# Copy the entire project into the container
COPY . /churchill

# Build the main application
RUN mkdir src/build
WORKDIR /churchill/src/build
RUN cmake ../..
RUN cmake --build .

# Build the tests
WORKDIR /churchill/test
# Add commands to build your tests here

# Add and set up the entrypoint script
COPY entrypoint.sh /entrypoint.sh
RUN chmod +x /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
