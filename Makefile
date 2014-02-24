PREFIX                  = /usr
INCLUDE_DIR             = ${PREFIX}/include
LIBRARY_DIR             = ${PREFIX}/lib

all:
		$(MAKE) -C src all

static:
		$(MAKE) -C src static

shared:
		$(MAKE) -C src shared

clean:
		$(MAKE) -C src clean

install:
		mkdir -p ${INCLUDE_DIR}/reactcpp
		mkdir -p ${LIBRARY_DIR}
		cp -f reactcpp.h ${INCLUDE_DIR}
		cp -fr include/* ${INCLUDE_DIR}/reactcpp
		cp -f src/libreactcpp.so ${LIBRARY_DIR}
		cp -f src/libreactcpp.a ${LIBRARY_DIR}
