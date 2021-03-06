CPP		= g++
RM		= rm -f
CPPFLAGS	= -Wall -MMD -c -I. -std=c++11
LD		= g++
LD_FLAGS	= -Wall -shared
SHARED_LIB	= lib$(LIBRARY_NAME).so.$(VERSION)
STATIC_LIB	= lib$(LIBRARY_NAME).a.$(VERSION)
STATIC_FPIC_LIB = lib$(LIBRARY_NAME)_fpic.a
SOURCES		= $(wildcard *.cpp */*.cpp)
DEPENDENCIES    = $(SOURCES:%.cpp=%.d)
SHARED_OBJECTS	= $(SOURCES:%.cpp=%.o)
STATIC_OBJECTS	= $(SOURCES:%.cpp=%.s.o)

all:	CPPFLAGS += -g
all:	LD_FLAGS += -g
all:	shared static

release:	CPPFLAGS += -O2
release:	LD_FLAGS += -O2
release:	shared static

-include ${DEPENDENCIES}

shared:	${SHARED_OBJECTS} ${SHARED_LIB}

static_fpic: ${SHARED_OBJECTS} ${STATIC_FPIC_LIB}

static:	${STATIC_OBJECTS} ${STATIC_LIB}

${SHARED_LIB}: ${SHARED_OBJECTS}
	${LD} ${LD_FLAGS} -Wl,-soname,lib$(LIBRARY_NAME).so.$(SONAME) -o $@ ${SHARED_OBJECTS} -lev -lcares

${STATIC_LIB}: ${STATIC_OBJECTS}
	ar rcs ${STATIC_LIB} ${STATIC_OBJECTS}

${STATIC_FPIC_LIB}: ${SHARED_OBJECTS}
	ar rcs ${STATIC_FPIC_LIB} ${SHARED_OBJECTS}

clean:
	${RM} *.obj *~* ${SHARED_OBJECTS} ${STATIC_OBJECTS} ${SHARED_LIB} ${STATIC_LIB}

${SHARED_OBJECTS}:
	${CPP} ${CPPFLAGS} -fpic -o $@ ${@:%.o=%.cpp}

${STATIC_OBJECTS}:
	${CPP} ${CPPFLAGS} -o $@ ${@:%.s.o=%.cpp}

