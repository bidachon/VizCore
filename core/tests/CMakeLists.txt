set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (CoreTest_HEADER
    mymodelitem.h
    myviewitem.h
    mycontroller.h
    )


add_executable(CoreTest ${CoreTest_HEADER} core_tests.cxx)
target_link_libraries(CoreTest Core Qt4::QtCore Qt4::QtGui Qt4::QtTest QVTK vtkCommon Logging)

add_test(NAME CoreTestView COMMAND CoreTest testView)
add_test(NAME CoreTestEvents COMMAND CoreTest testEvents)
add_test(NAME CoreTestModelItem COMMAND CoreTest testModelItem)
add_test(NAME CoreTestViewItem COMMAND CoreTest testViewItem)
add_test(NAME CoreTestScene COMMAND CoreTest testScene)
add_test(NAME CoreTestController COMMAND CoreTest testControllerItem)

#add_executable(CoreTest2 main.cpp )
#target_link_libraries(CoreTest2 Core  ${QT_LIBRARIES} Qt4::QtCore Qt4::QtGui Qt4::QtTest QVTK vtkCommon Logging)
