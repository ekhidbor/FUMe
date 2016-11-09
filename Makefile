CC      = ccache-swig cc
CXX     = ccache-swig c++
RM      ?= rm
RAGEL   ?= ragel

CXXFLAGS  = -std=c++11                  \
            -Wall                       \
            -pedantic                   \
            -O3                         \
            -fomit-frame-pointer        \
            -ffast-math                 \
            -funsafe-math-optimizations \
            -ffinite-math-only          \
            -fno-trapping-math          \
            -fPIC                       \
            -fvisibility=hidden         \
            -fvisibility-inlines-hidden

CXX_SO_FLAGS = -fvisibility=hidden                   \
               -fvisibility-inlines-hidden           \
               -shared                               \
               -Wl,-install_name,libfume.so          \
               -Wl,-exported_symbols_list,export.def

INC_FLAGS = -I./include -I./extern/boost_1_62_0 -I./src

LDLIBS  = 

VALIDATOR_RLS = ${wildcard ragel/*_validator.rl}
VALIDATOR_CPPS = ${VALIDATOR_RLS:.rl=.cpp}

LIB_CPPS = ${wildcard src/*.cpp}          \
           ${wildcard src/fume/*.cpp}     \
           ${wildcard src/fume/vrs/*.cpp} \
           $(VALIDATOR_CPPS)

LIB_OBJS = ${LIB_CPPS:.cpp=.o}

TEST_CPPS = 

TEST_OBJS = ${TEST_CPPS:.cpp=.o}

DEMO_CPPS = 

DEMO_OBJS = ${DEMO_CPPS:.cpp=.o}

INCS = ${wildcard include/*.h}      \
       ${wildcard src/*.h}          \
       ${wildcard src/fume/*.h}     \
       ${wildcard src/fume/vrs/*.h}

.SUFFIXES:

all: libfume.so

libfume.a: $(LIB_OBJS)
	$(AR) rcs libimage_io.a $(LIB_OBJS)

libfume.so: $(LIB_OBJS)
	$(CXX) $(CXX_SO_FLAGS) -o libfume.so $(LIB_OBJS)

%.o: %.cpp $(INCS)
	$(CXX) -c $(CXXFLAGS) $(INC_FLAGS) -o $@ $<

%.cpp: %.rl
	$(RAGEL) -C -G2 -o $@ $<

clean:
	$(RM) -f $(LIB_OBJS) $(TEST_OBJS) $(DEMO_OBJS) libfume.a libfume.so
