qml_module(caelestia-blobs
    URI Caelestia.Blobs
    SOURCES
        blobgroup.cpp
        blobshape.cpp
        blobrect.cpp
        blobinvertedrect.cpp
        blobmaterial.cpp
    LIBRARIES
        Qt::Quick
)

set(blob_shader_platform NOHLSL NOMSL)
if(WIN32)
    # Keep SPIR-V for comparison; bake DXBC so D3D failures occur at build time.
    set(blob_shader_platform PRECOMPILE NOMSL)
endif()

qt_add_shaders(caelestia-blobs "blob_shaders"
    BATCHABLE OPTIMIZED ${blob_shader_platform}
    GLSL "300es,330"
    PREFIX "/"
    FILES
        shaders/blob.frag
        shaders/blob.vert
)
