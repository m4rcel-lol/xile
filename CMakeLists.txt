cmake_minimum_required(VERSION 3.27)

project(Xile VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

option(XILE_BUILD_TESTS "Build Xile unit tests" ON)
set(XILE_PLATFORM "${XILE_PLATFORM}" CACHE STRING "Target platform: win32 or macos")
set(XILE_ARCH "${XILE_ARCH}" CACHE STRING "Target architecture")

if(NOT XILE_PLATFORM)
    if(WIN32)
        set(XILE_PLATFORM "win32")
    elseif(APPLE)
        set(XILE_PLATFORM "macos")
    else()
        message(FATAL_ERROR "Xile supports Windows and macOS targets only")
    endif()
endif()

if(MSVC)
    add_compile_options(/W4 /WX /utf-8)
    add_compile_definitions(UNICODE _UNICODE WIN32_LEAN_AND_MEAN NOMINMAX _CRT_SECURE_NO_WARNINGS)
else()
    add_compile_options(-Wall -Wextra -Werror)
endif()

include(GNUInstallDirs)

set(XILE_CORE_SOURCES
    src/core/xile_logger.c
    src/core/xile_config.c
    src/core/xile_auth.c
    src/core/xile_ipc.c
    src/core/xile_session.c
    src/server/xile_server.c
    src/server/xile_screen.c
    src/server/xile_input.c
    src/server/xile_render.c
    src/server/xile_font.c
    src/server/xile_extension.c
    src/server/xile_cursor.c
    src/wm/icewm_launcher.c
    src/wm/icewm_config_gen.c
    src/wm/icewm_theme/theme_installer.c)

set(XILE_PLATFORM_SOURCES)
if(XILE_PLATFORM STREQUAL "win32")
    list(APPEND XILE_PLATFORM_SOURCES
        src/platform/win32/win_backend.c
        src/platform/win32/win_gdi_render.c
        src/platform/win32/win_d3d_render.c
        src/platform/win32/win_input.c
        src/platform/win32/win_shell_replace.c
        src/platform/win32/win_overlay.c
        src/platform/win32/win_autostart.c
        src/platform/win32/win_tray.c
        src/platform/win32/win_crash_handler.c)
elseif(XILE_PLATFORM STREQUAL "macos")
    enable_language(OBJC)
    set(CMAKE_OBJC_STANDARD 11)
    set(CMAKE_OBJC_STANDARD_REQUIRED ON)
    add_compile_options("$<$<COMPILE_LANGUAGE:OBJC>:-fobjc-arc>")
    list(APPEND XILE_PLATFORM_SOURCES
        src/platform/macos/mac_backend.m
        src/platform/macos/mac_metal_render.m
        src/platform/macos/mac_cgl_render.m
        src/platform/macos/mac_input.m
        src/platform/macos/mac_overlay.m
        src/platform/macos/mac_session_replace.m
        src/platform/macos/mac_autostart.m
        src/platform/macos/mac_menubar.m
        src/platform/macos/mac_crash_handler.m)
else()
    message(FATAL_ERROR "Unsupported XILE_PLATFORM=${XILE_PLATFORM}")
endif()

add_library(xile_core STATIC ${XILE_CORE_SOURCES} ${XILE_PLATFORM_SOURCES})
target_include_directories(xile_core PUBLIC src/core src/server src/wm src/platform/${XILE_PLATFORM})

if(WIN32)
    target_link_libraries(xile_core PUBLIC advapi32 bcrypt shell32 user32 gdi32 ole32 taskschd)
elseif(APPLE)
    target_link_libraries(xile_core PUBLIC "-framework Cocoa" "-framework ApplicationServices" "-framework Metal" "-framework QuartzCore" "-framework OpenGL")
endif()

add_executable(xile-server src/core/xile_main.c)
target_link_libraries(xile-server PRIVATE xile_core)

add_executable(xile-ctl
    src/ctl/xile_ctl_main.c
    src/ctl/xile_ctl_ipc.c
    src/ctl/xile_ctl_ui.c)
target_include_directories(xile-ctl PRIVATE src/core src/ctl src/platform/${XILE_PLATFORM})
target_link_libraries(xile-ctl PRIVATE xile_core)

install(TARGETS xile-server xile-ctl RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
install(DIRECTORY src/fonts/ DESTINATION share/xile/fonts)
install(DIRECTORY src/wm/icewm_theme/Xile DESTINATION share/xile/themes)

if(XILE_BUILD_TESTS)
    enable_testing()
    add_executable(test_config tests/unit/test_config.c)
    target_link_libraries(test_config PRIVATE xile_core)
    add_test(NAME test_config COMMAND test_config)

    add_executable(test_auth tests/unit/test_auth.c)
    target_link_libraries(test_auth PRIVATE xile_core)
    add_test(NAME test_auth COMMAND test_auth)

    add_executable(test_ipc tests/unit/test_ipc.c)
    target_link_libraries(test_ipc PRIVATE xile_core)
    add_test(NAME test_ipc COMMAND test_ipc)
endif()
