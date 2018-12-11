set(CHEMSPACETRACKER_SRC
  iodata.cpp
  calculations.cpp
  progressdialog.cpp
  importsmidialog.cpp
  selectionstyledialog.cpp
  removelabeldialog.cpp
  selectbylabeldialog.cpp
  setlabeldialog.cpp
  mainwindow.cpp
  similartysearchdialog.cpp
  AboutDialog.cpp
  main.cpp)

set(CHEMSPACETRACKER_H
  iodata.h
  progressdialog.h
  importsmidialog.h
  selectionstyledialog.h
  setlabeldialog.h
  removelabeldialog.h
  selectbylabeldialog.h
  mainwindow.h
  AboutDialog.h
  similartysearchdialog.h
  calculations.h)

set(CHEMSPACETRACKER_UIS
  progressdialog.ui
  selectionstyledialog.ui
  setlabeldialog.ui
  removelabeldialog.ui
  selectbylabeldialog.ui
  importsmidialog.ui
  mainwindow.ui
  AboutDialog.ui
  similartysearchdialog.ui)

add_subdirectory(QPlotly)

set(icon_RCC icons.qrc)

# start building interface from moc
#set(AUTOMOC_MOC_OPTIONS PROPERTIES FOLDER CMakeAutomocTargets)
QT5_WRAP_CPP(CHEMSPACETRACKER_MOC_H ${CHEMSPACETRACKER_H})
QT5_WRAP_UI(CHEMSPACETRACKER_UIS_H ${CHEMSPACETRACKER_UIS})
# generate rules for building source files from the resources
QT5_ADD_RESOURCES(icon_RCC_SRCS ${icon_RCC})

source_group("Resources" FILES
  ${CHEMSPACETRACKER_UIS}
)

source_group("Generated" FILES
  ${CHEMSPACETRACKER_UIS_H}
  ${CHEMSPACETRACKER_MOC_H}
)

if(WIN32)
  set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS_RELEASE QT_NO_DEBUG)
  set(APP_ICON_RC ${CMAKE_CURRENT_SOURCE_DIR}/win32_icon.rc)
  if(APP_ICON_RC)
    set(WIN32_ICON_O ${CMAKE_CURRENT_BINARY_DIR}/_app_icon.o)
    ADD_CUSTOM_COMMAND( OUTPUT ${WIN32_ICON_O} COMMAND windres.exe -I${CMAKE_CURRENT_SOURCE_DIR} -o${WIN32_ICON_O} -i${APP_ICON_RC})
  endif()
endif()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(ChemSpaceTracker ${CHEMSPACETRACKER_SRC} ${CHEMSPACETRACKER_MOC_H} ${CHEMSPACETRACKER_UIS_H} ${icon_RCC_SRCS})
target_link_libraries(ChemSpaceTracker ${LIBSCIENTIFIC_LIBRARY} libqplotly ${QT5_LIBRARIES} ${RDKIT_LIBRARIES})
install(TARGETS ChemSpaceTracker DESTINATION bin COMPONENT ApplicationData)
