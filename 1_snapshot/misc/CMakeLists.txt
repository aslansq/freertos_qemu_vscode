add_subdirectory(CMSIS)
add_subdirectory(Snapshot-Trace)

file(GLOB misc_src "${CMAKE_CURRENT_SOURCE_DIR}/*.c")

set(src ${src} ${misc_src} PARENT_SCOPE)
set(inc ${inc} PARENT_SCOPE)