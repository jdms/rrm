#!/usr/bin/env python

"""
setup.py file for SWIG example
"""

from distutils.core import setup, Extension


colorwrap_module = Extension('_colorwrap',
                           sources=['colorwrap_wrap.cxx', '../src/colorwrap.cpp'],
                           extra_compile_args=['-std=c++11'],
                           include_dirs = ['../include', '../src', '/usr/local/include/eigen3']
                           )

setup (name = 'colorwrap',
       version = '0.1',
       author      = "SWIG Docs",
       description = """Simple swig example from docs""",
       ext_modules = [colorwrap_module],
       py_modules = ["colorwrap"],
       )
