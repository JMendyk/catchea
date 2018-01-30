set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

include_directories(${EXECUTABLE_NAME} ${SRC_DIR})

# adds files to targer sources
target_sources(${EXECUTABLE_NAME}
    PRIVATE
        ${SRC_DIR}/main.cpp

        ${SRC_DIR}/app.cpp

        ${SRC_DIR}/gui/menu_widget.cpp
        ${SRC_DIR}/gui/map_widget.cpp
        ${SRC_DIR}/gui/control_widget.cpp
        ${SRC_DIR}/gui/footer_widget.cpp

        ${SRC_DIR}/real_tile/real_tile.cpp
        ${SRC_DIR}/real_tile/hgt_plugin.cpp

        ${SRC_DIR}/dis_interpreters/topographer.cpp
        ${SRC_DIR}/dis_interpreters/catchmenter.cpp

        ${SRC_DIR}/resource_manager.cpp
        ${SRC_DIR}/utils.cpp
)
