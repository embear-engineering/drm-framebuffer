cmake_minimum_required(VERSION 3.20)
project(drm-framebuffer C)
find_package(PkgConfig REQUIRED)

add_executable(drm-framebuffer main.c framebuffer.c)
pkg_check_modules(LIBDRM REQUIRED libdrm)

target_link_libraries(drm-framebuffer PUBLIC ${LIBDRM_LIBRARIES})
target_include_directories(drm-framebuffer PUBLIC ${LIBDRM_INCLUDE_DIRS})

install(TARGETS drm-framebuffer)
