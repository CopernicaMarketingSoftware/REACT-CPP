PREFIX                  = /usr
INCLUDE_DIR             = ${PREFIX}/include
LIBRARY_DIR             = ${PREFIX}/lib
export LIBRARY_NAME		= reactcpp
export SONAME			= 1.0
export VERSION			= 1.0.3

all:
		$(MAKE) -C src all

release:
		$(MAKE) -C src release

static:
		$(MAKE) -C src static

shared:
		$(MAKE) -C src shared

static_fpic:
		$(MAKE) -C src static_fpic

clean:
		$(MAKE) -C src clean

install:
		mkdir -p ${INCLUDE_DIR}/$(LIBRARY_NAME)
		mkdir -p ${LIBRARY_DIR}
		mkdir -p ${INCLUDE_DIR}/$(LIBRARY_NAME)/dns
		mkdir -p ${INCLUDE_DIR}/$(LIBRARY_NAME)/net
		mkdir -p ${INCLUDE_DIR}/$(LIBRARY_NAME)/tcp
		mkdir -p ${INCLUDE_DIR}/$(LIBRARY_NAME)/watchers
		cp -f $(LIBRARY_NAME).h ${INCLUDE_DIR}
		cp -f include/*.h ${INCLUDE_DIR}/$(LIBRARY_NAME)
		cp -f include/dns/*.h ${INCLUDE_DIR}/$(LIBRARY_NAME)/dns
		cp -f include/net/*.h ${INCLUDE_DIR}/$(LIBRARY_NAME)/net
		cp -f include/tcp/*.h ${INCLUDE_DIR}/$(LIBRARY_NAME)/tcp
		cp -f include/watchers/*.h ${INCLUDE_DIR}/$(LIBRARY_NAME)/watchers
		-cp -f src/lib$(LIBRARY_NAME).so.$(VERSION) ${LIBRARY_DIR}
		-cp -f src/lib$(LIBRARY_NAME).a.$(VERSION) ${LIBRARY_DIR}
		-cp -f src/lib$(LIBRARY_NAME)_fpic.a ${LIBRARY_DIR}
		ln -s -f lib$(LIBRARY_NAME).so.$(VERSION) $(LIBRARY_DIR)/lib$(LIBRARY_NAME).so.$(SONAME)
		ln -s -f lib$(LIBRARY_NAME).so.$(VERSION) $(LIBRARY_DIR)/lib$(LIBRARY_NAME).so
		ln -s -f lib$(LIBRARY_NAME).a.$(VERSION) $(LIBRARY_DIR)/lib$(LIBRARY_NAME).a
