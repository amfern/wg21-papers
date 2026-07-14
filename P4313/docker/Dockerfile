# docker build -t clang-p2996-bazel -f docker/Dockerfile docker
# docker run -ti --rm -v (pwd):/work -w /work vsavkov/clang-p2996:amd64 bash
# bazel test
From vsavkov/clang-p2996:amd64

RUN apt update -y && apt install wget -y
RUN wget https://github.com/bazelbuild/bazelisk/releases/download/v1.29.0/bazelisk-amd64.deb && dpkg -i ./bazelisk-amd64.deb
ENV LD_LIBRARY_PATH=/opt/p2996/clang/lib:/opt/p2996/clang/lib/x86_64-unknown-linux-gnu
RUN apt remove gcc-13 -y

