#!/bin/sh
 
clear
ls -al

cd ./math_library/blas_dense_library;
ls -l
python generate_BLAS_library.py

cd ../..;
cd ./math_library/lapack_library;
ls -l
python generate_LAPACK_library.py