idf_component_register(
    SRCS main.c ui.c ui_settings.c audio.c settings_store.c
         font_yes_no_160.c font_settings_28.c font_settings_48.c
         font_icons_150.c font_icons_80.c
    INCLUDE_DIRS .)

# The clips are made with the macOS say command on every clean build and are not checked in.
find_program(PYTHON313 python3.13)
if(NOT PYTHON313)
    message(FATAL_ERROR "python3.13 is missing. The build needs it to make the audio clips.")
endif()

set(CLIP_SCRIPT ${PROJECT_DIR}/tools/make_clips.py)
set(CLIPS
    ${CMAKE_CURRENT_BINARY_DIR}/yes_samantha.pcm
    ${CMAKE_CURRENT_BINARY_DIR}/no_samantha.pcm
    ${CMAKE_CURRENT_BINARY_DIR}/yes_reed.pcm
    ${CMAKE_CURRENT_BINARY_DIR}/no_reed.pcm)

add_custom_command(
    OUTPUT ${CLIPS}
    COMMAND ${PYTHON313} ${CLIP_SCRIPT} ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${CLIP_SCRIPT}
    COMMENT "Making the yes and no clips with say"
    VERBATIM)
add_custom_target(clips DEPENDS ${CLIPS})
add_dependencies(${COMPONENT_LIB} clips)

foreach(clip ${CLIPS})
    target_add_binary_data(${COMPONENT_LIB} ${clip} BINARY)
endforeach()
