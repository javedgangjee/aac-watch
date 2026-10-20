cmake_minimum_required(VERSION 3.22)

set(EXTRA_COMPONENT_DIRS ../../components/screen_cycle)
set(COMPONENTS main)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(screen_cycle_test)
