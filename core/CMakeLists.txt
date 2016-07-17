set(CMAKE_AUTOMOC ON)

set (Core_SOURCES
modelitem.cpp
scene.cpp
viewitem.cpp
view.cpp
pickinginteractorstyle.cpp
panninginteractorstyle.cpp
interactorpropsubscriber.cpp
controlleritem.cpp
)
set (Core_HEADERS
modelitem.h
scene.h
viewitem.h
view.h
pickinginteractorstyle.h
panninginteractorstyle.h
interactorpropsubscriber.h)

#QT4_WRAP_CPP(CORE_MOC ${Core_HEADERS} ${Core_SOURCES})

add_library(Core STATIC ${CORE_MOC} ${Core_SOURCES} )

target_link_libraries(Core ${VTK_LIBS} Qt4::QtCore)

add_subdirectory(tests)
