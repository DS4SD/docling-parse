#!/usr/bin/env python

import os
import re

import sys

import json
import glob

import argparse
import subprocess

#from deepsearch_glm.utils.load_pretrained_models import load_pretrained_nlp_models

ROOT_DIR=os.path.abspath("./")
BUILD_DIR=os.path.join(ROOT_DIR, "build")

def run(cmd, cwd="./"):

    print(f"\nlaunch: {cmd}")
    
    parts = cmd.split(" ")
    message = subprocess.run(parts, cwd=cwd)    

    if "returncode=0" in str(message):
        print(f" -> SUCCESS")
        return True

    print(f" -> ERROR with message: '{message}'\n")        
    return False
    
def build_local(multi_threaded=True):
    
    if not os.path.exists(BUILD_DIR):
        print("python executable: ", sys.executable)
        
        cmd = f"cmake -B {BUILD_DIR} -DPYTHON_EXECUTABLE={sys.executable}"
        run(cmd, cwd=ROOT_DIR)
    else:
        print(f"build directory detected: {BUILD_DIR}")

    if multi_threaded:
        cmd = f"cmake --build {BUILD_DIR} --target install -j"    
        run(cmd, cwd=ROOT_DIR)    
    else:
        cmd = f"cmake --build {BUILD_DIR} --target install"
        run(cmd, cwd=ROOT_DIR)    

if "__main__"==__name__:

    build_local(multi_threaded=True)
