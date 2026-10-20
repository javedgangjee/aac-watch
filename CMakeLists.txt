# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.22)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)

project(aac_watch)

idf_build_get_property(build_components BUILD_COMPONENTS)
if("lvgl__lvgl" IN_LIST build_components)
    idf_component_get_property(LVGL_LIB lvgl__lvgl COMPONENT_LIB)
    if(TARGET ${LVGL_LIB})
        target_compile_options(${LVGL_LIB} PRIVATE -DLV_LVGL_H_INCLUDE_SIMPLE -Wno-error=attributes -Wno-error=cpp)
    endif()
endif()
