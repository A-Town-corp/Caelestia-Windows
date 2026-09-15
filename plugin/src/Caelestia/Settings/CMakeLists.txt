qml_module(caelestia-settings
    URI Caelestia.Settings
    INCLUDE_PREFIX settings
    SOURCES
        common.cpp
        schema.cpp
        codecs.cpp
        quarantine.cpp
        node.cpp
        objectnode.cpp
        listnode.cpp
        rootnode.cpp
        settingsfile.cpp
        changebatcher.cpp
    LIBRARIES
        caelestia-util
)

include(GenerateExportHeader)
generate_export_header(caelestia-settings)
target_include_directories(caelestia-settings PUBLIC "${CMAKE_CURRENT_BINARY_DIR}")
