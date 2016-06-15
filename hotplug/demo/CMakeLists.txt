message(STATUS "Processing hotplug demo...")

set(HOTPLUGDEMO_SOURCES "main.cpp")

add_executable(hotplugDemo ${HOTPLUGDEMO_SOURCES})

target_link_libraries (hotplugDemo LINK_PUBLIC hotplug)

message(STATUS "Processing hotplug demo done")

