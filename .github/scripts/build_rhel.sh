#!/bin/bash

set -e  # trigger failure on error - do not remove!
set -x  # display command on output

# Build the Python package with Poetry
poetry build -f sdist

USE_SYSTEM_DEPS="ON"

docker build --progress=plain \
             --build-arg USE_SYSTEM_DEPS="$USE_SYSTEM_DEPS" \
             -f - . <<EOF
    # syntax=docker/dockerfile:1

    FROM quay.io/centos/centos:stream9

    RUN dnf config-manager --set-enabled crb
    
    RUN dnf copr -y enable cheimes/deepsearch-glm rhel-9-x86_64

    RUN dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm \
       && dnf clean all

    RUN dnf install -y --nodocs \
            autoconf automake binutils cmake gcc gcc-c++ git glibc-devel glibc-headers glibc-static kernel-devel libtool libstdc++-devel make ninja-build pkgconfig zlib-devel \
            python3.11 python3.11-pip python3.11-devel \
            libjpeg-turbo-devel libpng-devel qpdf-devel json-devel utf8cpp-devel \
        && dnf clean all

    # # RUN dnf install -y --nodocs loguru-devel

    # TEMPORARY loguru install method
    # https://koji.fedoraproject.org/koji/buildinfo?buildID=2563067
    RUN curl -O https://kojipkgs.fedoraproject.org//packages/loguru/2.2.0%5E20230406git4adaa18/5.el9/x86_64/loguru-2.2.0%5E20230406git4adaa18-5.el9.x86_64.rpm
    RUN dnf install -y loguru-2.2.0%5E20230406git4adaa18-5.el9.x86_64.rpm
    RUN curl -O https://kojipkgs.fedoraproject.org//packages/loguru/2.2.0%5E20230406git4adaa18/5.el9/x86_64/loguru-devel-2.2.0%5E20230406git4adaa18-5.el9.x86_64.rpm
    RUN dnf install -y loguru-devel-2.2.0%5E20230406git4adaa18-5.el9.x86_64.rpm

    RUN mkdir /src

    COPY ./dist/*.tar.gz /src/

    RUN USE_SYSTEM_DEPS=\$USE_SYSTEM_DEPS pip3.11 install /src/docling_parse*.tar.gz \
        && python3.11 -c 'from docling_parse.docling_parse import pdf_parser_v1, pdf_parser_v2'
    
    COPY ./tests /src/tests

    RUN cd /src \
        && pip3.11 install pytest \
        && pytest
EOF
