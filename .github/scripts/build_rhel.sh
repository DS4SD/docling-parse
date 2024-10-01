#!/bin/bash

set -e  # trigger failure on error - do not remove!
set -x  # display command on output

# Build the sdist
poetry build -f sdist

# Compile the wheel from sdist in centos stream

docker build -f - . <<EOF
    FROM quay.io/centos/centos:stream9
    # RUN dnf config-manager --set-enabled crb
    # RUN dnf copr -y enable cheimes/deepsearch-glm rhel-9-x86_64
    RUN dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm \
       && dnf clean all
    RUN dnf install -y --nodocs \
            gcc gcc-c++ git make cmake pkgconfig glibc-devel \
            python3.11 python3.11-pip python3.11-devel \
            libjpeg-turbo-devel libpng-devel zlib-devel \
            loguru-devel  \
        && dnf clean all
    
    RUN mkdir /src
    COPY ./dist/*.tar.gz /src/

    RUN pip3.11 install /src/docling_parse*.tar.gz \
        && python3.11 -c 'from docling_parse.docling_parse import pdf_parser'
    
    COPY ./tests /src/tests
    RUN cd /src \
        && pip3.11 install pytest \
        && pytest
EOF
