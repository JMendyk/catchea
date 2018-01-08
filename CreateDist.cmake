# This procedure will add installation functionality to CMake build
# If you open build folder and call `make install`,
# in project's root, folder "dist" will be created with subdirectory
# for your OS. Inside, you will find redistributable package.

set(DIST_DIR ${CMAKE_SOURCE_DIR}/dist)

if(UNIX)
    set(CMAKE_INSTALL_PREFIX ${DIST_DIR}/unix)
elseif(WIN32)
    set(CMAKE_INSTALL_PREFIX ${DIST_DIR}/win32)
elseif(APPLE)
    set(CMAKE_INSTALL_PREFIX ${DIST_DIR}/apple)
elseif(MINGW)
    set(CMAKE_INSTALL_PREFIX ${DIST_DIR}/mingw)
elseif(MSYS)
    set(CMAKE_INSTALL_PREFIX ${DIST_DIR}/msys)
elseif(CYGWIN)
    set(CMAKE_INSTALL_PREFIX ${DIST_DIR}/cygwin)
else()
    MESSAGE(SEND_ERROR "Unknown platform, can't configure install")
endif()

install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX})

install(DIRECTORY ${RES_DIR} DESTINATION ${CMAKE_INSTALL_PREFIX})
