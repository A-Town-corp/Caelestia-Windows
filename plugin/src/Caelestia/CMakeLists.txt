set(core_platform_sources)
set(core_platform_libraries)
if(NOT CAELESTIA_WINDOWS_PROTOTYPE)
    list(APPEND core_platform_sources qalculator.cpp)
    list(APPEND core_platform_libraries PkgConfig::Qalculate)
endif()

qml_module(caelestia-core
    URI Caelestia
    INCLUDE_PREFIX core
    SOURCES
        cutils.cpp
        ${core_platform_sources}
        requests.cpp
        toaster.cpp
        circularbuffer.cpp
    LIBRARIES
        Qt::Gui
        Qt::Quick
        Qt::Concurrent
        ${core_platform_libraries}
        caelestia-util
)

# Bake the shell version in so CUtils can expose it without spawning a process
target_compile_definitions(caelestia-core PRIVATE CAELESTIA_VERSION="${VERSION}")

# Node macros expand to Q_OBJECT, automoc can't see it otherwise
list(APPEND CMAKE_AUTOMOC_MACRO_NAMES
    SETTINGS_NODE SETTINGS_NODE_NO_CTOR SETTINGS_LIST_TYPE
    CONFIG_NODE CONFIG_NODE_NO_CTOR CONFIG_LIST_TYPE
)

add_subdirectory(Settings)
add_subdirectory(Config)
add_subdirectory(Blobs)
if(NOT CAELESTIA_WINDOWS_PROTOTYPE)
    add_subdirectory(Components)
    add_subdirectory(Models)
    add_subdirectory(Services)
    add_subdirectory(Images)
    add_subdirectory(I18n)
endif()
