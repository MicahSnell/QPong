TARGET = qpong
TEMPLATE = app
QT += widgets

SRC_DIR = ../src
MOC_DIR = moc
OBJECTS_DIR = obj

QMAKE_POST_LINK = \ 
  $${QMAKE_COPY} $${SRC_DIR}/stylesheet.qss stylesheet.qss

QMAKE_CLEAN += \
  $${SRC_DIR}/*~

QMAKE_DISTCLEAN += \
  -r $${MOC_DIR} $${OBJECTS_DIR} \
  stylesheet.qss
     
QMAKE_EXTRA_TARGETS += debug
debug.commands = $(MAKE) $(TARGET) DEFINES+="-DDEBUG"

HEADERS += \
  $${SRC_DIR}/QPong.h

SOURCES += \
  $${SRC_DIR}/QPong.cpp \
  $${SRC_DIR}/Main.cpp
