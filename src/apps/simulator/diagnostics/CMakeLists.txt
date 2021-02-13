cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

project(diagnostics LANGUAGES CXX)


###############################################
# Create stub library interface
###############################################

if (NOT TARGET diagnostics) 
    add_library(diagnostics INTERFACE)
endif()
