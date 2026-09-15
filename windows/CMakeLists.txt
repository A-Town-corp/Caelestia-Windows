if(NOT WIN32 OR NOT CAELESTIA_WINDOWS_PROTOTYPE)
    message(FATAL_ERROR "The first test requires Windows and CAELESTIA_WINDOWS_PROTOTYPE=ON")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick ShaderTools)
qt_standard_project_setup(REQUIRES 6.9)

include(FetchContent)
FetchContent_Declare(m3shapes_source
    GIT_REPOSITORY https://github.com/soramanew/m3shapes.git
    GIT_TAG 32ad9ce328bb77ed349b40a3be10ee9ea610b8ab
    SOURCE_SUBDIR windows-source-only
)
FetchContent_MakeAvailable(m3shapes_source)

# Source-only reuse: upstream's build applies GCC flags unconditionally.
# This adapter uses the same sources and shader resource paths on MSVC.
set(m3src "${m3shapes_source_SOURCE_DIR}/src")
qt_add_qml_module(m3shapes
    URI M3Shapes VERSION 1.0
    OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/qml/M3Shapes"
    SOURCES
        "${m3src}/core/Cubic.cpp" "${m3src}/core/Feature.cpp" "${m3src}/core/RoundedPolygon.cpp"
        "${m3src}/morph/FloatMapping.cpp" "${m3src}/morph/PolygonMeasure.cpp"
        "${m3src}/morph/FeatureMapping.cpp" "${m3src}/morph/Morph.cpp"
        "${m3src}/shapes/Shapes.cpp" "${m3src}/shapes/MaterialShapes.cpp"
        "${m3src}/qml/MaterialShapeItem.cpp" "${m3src}/qml/MaterialShapeItem.hpp"
        "${m3src}/qml/SmoothShapeMaterial.cpp" "${m3src}/qml/SmoothShapeMaterial.hpp"
)
target_include_directories(m3shapes PRIVATE "${m3src}" "${m3src}/qml")
target_link_libraries(m3shapes PRIVATE Qt::Quick Qt::Qml)
# M3Shapes uses QString's ASCII constructors; match its upstream build contract.
foreach(definition QT_NO_CAST_FROM_ASCII QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_BYTEARRAY)
    if(MSVC)
        target_compile_options(m3shapes PRIVATE "/U${definition}")
    else()
        target_compile_options(m3shapes PRIVATE "-U${definition}")
    endif()
endforeach()
set_target_properties(m3shapes PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
qt_add_shaders(m3shapes "m3shapes_shaders" PREFIX "/m3shapes" BASE "${m3src}"
    FILES "${m3src}/shaders/smoothshape.vert" "${m3src}/shaders/smoothshape.frag")
qt_query_qml_module(m3shapes QMLDIR m3_qmldir TYPEINFO m3_typeinfo)
install(TARGETS m3shapes RUNTIME DESTINATION bin)
install(TARGETS m3shapesplugin LIBRARY DESTINATION "${INSTALL_QMLDIR}/M3Shapes" RUNTIME DESTINATION "${INSTALL_QMLDIR}/M3Shapes")
install(FILES "${m3_qmldir}" "${m3_typeinfo}" DESTINATION "${INSTALL_QMLDIR}/M3Shapes")
install(FILES "${m3shapes_source_SOURCE_DIR}/LICENSE" DESTINATION licenses RENAME M3Shapes-LICENSE)

qt_add_executable(caelestia-first-test WIN32 src/main.cpp)
target_link_libraries(caelestia-first-test PRIVATE Qt::Quick Qt::Qml user32)
target_compile_definitions(caelestia-first-test PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
install(TARGETS caelestia-first-test RUNTIME DESTINATION bin)
install(DIRECTORY qml/ DESTINATION qml/FirstTest)
install(FILES "${PROJECT_SOURCE_DIR}/components/Anim.qml" DESTINATION qml/FirstTest)
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION licenses RENAME Caelestia-LICENSE)
