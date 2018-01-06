set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR})

target_include_directories(${EXECUTABLE_NAME} PRIVATE ${SRC_DIR})

# adds files to targer sources
target_sources(${EXECUTABLE_NAME}
    PRIVATE
        ${SRC_DIR}/main.cpp

        ${SRC_DIR}/geotile.cpp
)
