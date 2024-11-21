#!/bin/bash

set -e  # trigger failure on error - do not remove!
set -x  # display command on output

# Build the Python package with Poetry
poetry build -f sdist

sudo -E XDG_RUNTIME_DIR= podman build --progress=plain \
             --build-arg USE_SYSTEM_DEPS="$USE_SYSTEM_DEPS" \
             -f - . <<EOF
    # syntax=docker/dockerfile:1

    FROM quay.io/centos/centos:stream9

    RUN dnf update -y \
        && dnf install -y 'dnf-command(config-manager)' 'dnf-command(copr)' \
        && dnf install -y https://dl.fedoraproject.org/pub/epel/epel-release-latest-9.noarch.rpm \
        && dnf copr -y enable cheimes/deepsearch-glm rhel-9-x86_64 \
        && crb enable \
        && dnf clean all

    RUN dnf install -y --nodocs \
            autoconf automake binutils cmake gcc gcc-c++ git glibc-devel glibc-headers glibc-static kernel-devel libtool libstdc++-devel make ninja-build pkgconfig zlib-devel \
            python3.11 python3.11-pip python3.11-devel \
            libjpeg-turbo-devel libpng-devel qpdf-devel json-devel utf8cpp-devel loguru-devel cxxopts-devel \
        && dnf clean all

    WORKDIR /src

    COPY ./dist/*.tar.gz .

    ENV USE_SYSTEM_DEPS=on

    # pre-install build requirements + wheel for "--no-build-isolation"
    # build docling-parse wheel in an isolated network namespace (unshare -rn)
    # install the wheel and its dependencies
    RUN pip3.11 install poetry-core pybind11 wheel \
        && unshare -rn pip3.11 wheel \
            --no-deps --no-build-isolation -w /dist/ \
            /src/docling_parse*.tar.gz \
        && pip3.11 install /dist/docling_parse*.whl \
        && python3.11 -c 'from docling_parse.docling_parse import pdf_parser, pdf_parser_v2'
    
    COPY ./tests /src/tests

    RUN pip3.11 install pytest \
        && pytest
EOF
