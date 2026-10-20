idf_component_register(
    SRCS main.c ui.c audio.c font_yes_no_160.c
    INCLUDE_DIRS .)

# The clips are made with the macOS say command on every clean build and are not checked in.
find_program(PYTHON313 python3.13)
if(NOT PYTHON313)
    message(FATAL_ERROR "python3.13 is missing. The build needs it to make the audio clips.")
endif()

set(CLIP_SCRIPT ${PROJECT_DIR}/tools/make_clips.py)
set(CLIP_YES ${CMAKE_CURRENT_BINARY_DIR}/yes.pcm)
set(CLIP_NO ${CMAKE_CURRENT_BINARY_DIR}/no.pcm)

add_custom_command(
    OUTPUT ${CLIP_YES} ${CLIP_NO}
    COMMAND ${PYTHON313} ${CLIP_SCRIPT} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CLIP_SCRIPT}
    COMMENT "Making the yes and no clips with say"
    VERBATIM)
add_custom_target(clips DEPENDS ${CLIP_YES} ${CLIP_NO})
add_dependencies(${COMPONENT_LIB} clips)

target_add_binary_data(${COMPONENT_LIB} ${CLIP_YES} BINARY)
target_add_binary_data(${COMPONENT_LIB} ${CLIP_NO} BINARY)
