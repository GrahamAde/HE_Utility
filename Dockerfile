FROM debian:11.7

RUN apt-get update && apt-get install -y libboost-all-dev libgtkmm-3.0-dev g++ libomp-dev cmake build-essential git autoconf pkg-config

RUN useradd --shell /bin/bash --create-home userone && usermod -aG sudo userone

ENV HOME /home/userone

WORKDIR ${HOME}

RUN git clone https://github.com/openfheorg/openfhe-development.git

WORKDIR ${HOME}/openfhe-development/build

RUN cmake .. 

RUN make -j 4

RUN make install

USER userone

WORKDIR ${HOME}/he_utility

COPY src/ ./src/

COPY CMakeLists.txt .

COPY Sample.csv .

WORKDIR ${HOME}/he_utility/build

RUN cmake .. && make

ENTRYPOINT ["./HE_Utility"]
