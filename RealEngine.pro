TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
    src/stb_image.cpp \
    src/ShaderProgram.cpp \
    src/Shader.cpp \
    src/main.cpp \
    src/Debug.cpp \
    src/Time.cpp \
    src/VBO.cpp \
    src/VAO.cpp \
    src/Texture.cpp \
    src/Image.cpp \
    src/FileLoader.cpp \
    src/FrameBuffer.cpp \
    src/FrameDrawer.cpp \
    src/Mesh.cpp \
    src/RenderTarget.cpp

HEADERS += \
    include/stb_image.h \
    include/ShaderProgram.h \
    include/Shader.h \
    include/FileLoader.h \
    include/Debug.h \
    include/RealEngine.h \
    include/Time.h \
    include/VBO.h \
    include/VAO.h \
    include/Texture.h \
    include/Image.h \
    include/FrameBuffer.h \
    include/FrameDrawer.h \
    include/obj_parser.h \
    include/Mesh.h

OTHER_FILES +=

