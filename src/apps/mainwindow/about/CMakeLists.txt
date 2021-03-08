# Modified from: https://github.com/Kitware/sprokit/blob/master/src/sprokit/CMakeLists.txt
#
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(${CMAKE_CURRENT_SOURCE_DIR}/rrm_macro_configure.cmake)

find_package(Git)

if (GIT_FOUND)
    if (IS_DIRECTORY "${CMAKE_SOURCE_DIR}/.git")
    set(rrm_is_in_git ON)
  endif ()
endif ()

set(RRM_BUILT_FROM_GIT)
set(RRM_VERSION_FROM_GIT_TAG)

if ("$Format:$" STREQUAL "")
  set(rrm_git_hash       "$Format:%H$")
  set(rrm_git_hash_short "$Format:%h$")

elseif (GIT_FOUND)
  set(configure_code "
  if (IS_DIRECTORY \"${CMAKE_SOURCE_DIR}/.git\")
  set(RRM_BUILT_FROM_GIT TRUE)
  execute_process(
    COMMAND           \"${GIT_EXECUTABLE}\"
                      tag
                      --list \"*${rrm_version}*\"
                      --points-at
                      HEAD
    WORKING_DIRECTORY \"${rrm_source_dir}\"
    RESULT_VARIABLE   git_return
    OUTPUT_VARIABLE   rrm_git_tag)
  execute_process(
    COMMAND           \"${GIT_EXECUTABLE}\"
                      rev-parse
                      HEAD
    WORKING_DIRECTORY \"${rrm_source_dir}\"
    RESULT_VARIABLE   git_return
    OUTPUT_VARIABLE   rrm_git_hash)
  execute_process(
    COMMAND           \"${GIT_EXECUTABLE}\"
                      rev-parse
                      --short
                      HEAD
    WORKING_DIRECTORY \"${rrm_source_dir}\"
    RESULT_VARIABLE   git_return
    OUTPUT_VARIABLE   rrm_git_hash_short)
  execute_process(
    COMMAND           \"${GIT_EXECUTABLE}\"
                      diff
                      --no-ext-diff
                      --quiet
                      --exit-code
    WORKING_DIRECTORY \"${rrm_source_dir}\"
    RESULT_VARIABLE   git_return)
  string(STRIP \"\${rrm_git_hash}\" rrm_git_hash)
  string(STRIP \"\${rrm_git_hash_short}\" rrm_git_hash_short)
  if (rrm_git_tag)
    string(STRIP \"\${rrm_git_tag}\" rrm_git_tag)
    set(RRM_VERSION_FROM_GIT_TAG TRUE)
  endif()
  if (git_return)
    set(rrm_git_dirty \"dirty\")
  endif ()
  message(STATUS \"version: \${rrm_version}\")
  message(STATUS \"git hash: \${rrm_git_hash}\")
  message(STATUS \"git short hash: \${rrm_git_hash_short}\")
  message(STATUS \"git dirty: \${rrm_git_dirty}\")
  message(STATUS \"git tag: \${rrm_git_tag}\")
endif ()
")
else ()
  set(rrm_git_hash       "<unknown>")
  set(rrm_git_hash_short "<unknown>")
  set(rrm_git_dirty      "<unknown>")
  set(rrm_git_tag        "<unknown>")
endif ()

rrm_configure_file_always(version.h
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
  rrm_version_major
  rrm_version_minor
  rrm_version_patch
  rrm_version
  RRM_BUILT_FROM_GIT
  rrm_git_hash
  rrm_git_hash_short
  rrm_git_dirty
  rrm_git_tag)

set(configure_code)
