QT += widgets

FORMS       = mainwindow.ui
HEADERS     = treeitem.h \
              treemodel.h \
              datawidget.h \
              mainwindow.h \
    treeproxymodel.h
RESOURCES   = QtGUI.qrc
SOURCES     = mainwindow.cpp \
              treeitem.cpp \
              treemodel.cpp \
              datawidget.cpp\
              main.cpp \
    treeproxymodel.cpp

# install
target.path = $$[QT_INSTALL_EXAMPLES]/widgets/itemviews/editabletreemodel
INSTALLS += target

