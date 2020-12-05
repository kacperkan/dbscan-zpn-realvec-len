FROM ubuntu:20.04


VOLUME /usr/src/dami/experiments

ENV DEBIAN_FRONTEND noninteractive

RUN apt-get update \
    && apt-get upgrade -y \
    && apt-get install -y  \
        libboost-dev \
        libboost-iostreams-dev \
        g++ \
        gcc \
        make \
        cmake \
        python3 \
        python3-dev \
        python3-pip \
        wget \
        curl \
        texlive-fonts-recommended \
        dvipng \
        cm-super \
        texlive-latex-extra \
    && rm -rf /var/lib/apt/lists/* \
    && ln -sf python3 /usr/bin/python 

WORKDIR /usr/src/dami
COPY . .

RUN mkdir build \
    && cd build \
    && cmake .. \
    && make -j8

# data download
RUN pip3 install -r python/requirements.txt
RUN ./download_data.sh

CMD [ "./run_experiments.sh" ]