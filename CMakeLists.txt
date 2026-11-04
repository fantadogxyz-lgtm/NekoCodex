cmake_minimum_required(VERSION 3.16)
project(NekoCodex LANGUAGES CXX)

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_INCLUDE_DIR "C:/Program Files/OpenSSL-Win64/include")
set(OPENSSL_CRYPTO_LIBRARY "C:/Program Files/OpenSSL-Win64/lib/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "C:/Program Files/OpenSSL-Win64/lib/libssl.lib")

include(qt.cmake)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS "*.cpp")
file(GLOB HEADERS_FILES CONFIGURE_DEPENDS "*.h")

file(GLOB UI_FILES CONFIGURE_DEPENDS "*.ui")

file(GLOB RESOURCES_FILES CONFIGURE_DEPENDS "*.qrc")

file(GLOB WIN_RC_FILES CONFIGURE_DEPENDS "*.rc")


find_package(OpenSSL REQUIRED)

if(OPENSSL_FOUND)
    message(STATUS "OpenSSL vers: ${OPENSSL_VERSION}")
    message(STATUS "Include dir: ${OPENSSL_INCLUDE_DIR}")
    message(STATUS "Libs idk: ${OPENSSL_LIBRARIES}")
else()
    message(FATAL_ERROR "OpenSSL was not found")
endif()


find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR}
    REQUIRED COMPONENTS
        Core
        Gui
        Widgets
        Sql
        Network
)

qt_standard_project_setup()

qt_add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${HEADERS_FILES} ${RESOURCES_FILES} ${UI_FILES} ${WIN_RC_FILES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        WIN32_EXECUTABLE TRUE
        MACOSX_BUNDLE OFF
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Sql
        OpenSSL::SSL
        OpenSSL::Crypto
        Qt::Network
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8 /wd4828)
endif()

if(WIN32)
    target_link_libraries(${PROJECT_NAME}
        PUBLIC
            ws2_32
            crypt32
    )
endif()


