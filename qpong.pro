QT += widgets

SRC_DIR = src
MOC_DIR = moc
OBJECTS_DIR = obj

HEADERS += $$files($${SRC_DIR}/*.h)
SOURCES += $$files($${SRC_DIR}/*.cpp)

QMAKE_POST_LINK = \ 
  $${QMAKE_COPY} ../$${SRC_DIR}/stylesheet.qss stylesheet.qss

QMAKE_CLEAN += \
  ../$${SRC_DIR}/*~

QMAKE_DISTCLEAN += \
  stylesheet.qss \
  -r $${MOC_DIR} $${OBJECTS_DIR}
     
debug.commands = $(MAKE) $(TARGET) DEFINES+="-DDEBUG"
QMAKE_EXTRA_TARGETS += debug
