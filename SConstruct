#!/usr/bin/python3

import sys
import os
import shutil

SRC_DIR = './src'
LIB_EXPORT_DIR = './build/netsi'

if os.path.isdir(LIB_EXPORT_DIR):
    shutil.rmtree(LIB_EXPORT_DIR)

shutil.copytree(SRC_DIR, LIB_EXPORT_DIR)

SConscript('tests/SConstruct', variant_dir='build/tests')
