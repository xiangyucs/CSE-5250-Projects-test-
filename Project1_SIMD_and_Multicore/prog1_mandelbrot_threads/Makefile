# CSUSB (CSE5250): cross-platform arch detection.
# x86_64 (Windows-WSL2 / Intel Mac / Linux) keeps the original AVX2 path.
# arm64/aarch64 (Apple Silicon) drops -m64 and uses ISPC NEON targets.
UNAME_M := $(shell uname -m)
ifeq ($(UNAME_M),x86_64)
  CSUSB_ARCHFLAGS := -m64
  CSUSB_ISPC_ARCH := --arch=x86-64 --target=avx2-i32x8
else
  CSUSB_ARCHFLAGS :=
  CSUSB_ISPC_ARCH := --arch=aarch64 --target=neon-i32x8
endif


CXX=g++ $(CSUSB_ARCHFLAGS)
CXXFLAGS=-I../common -Iobjs/ -O3 -std=c++11 -Wall -fPIC

APP_NAME=mandelbrot
OBJDIR=objs
COMMONDIR=../common

PPM_CXX=$(COMMONDIR)/ppm.cpp
PPM_OBJ=$(addprefix $(OBJDIR)/, $(subst $(COMMONDIR)/,, $(PPM_CXX:.cpp=.o)))


default: $(APP_NAME)

.PHONY: dirs clean

dirs:
		/bin/mkdir -p $(OBJDIR)/

clean:
		/bin/rm -rf $(OBJDIR) *.ppm *~ $(APP_NAME)

OBJS=$(OBJDIR)/main.o $(OBJDIR)/mandelbrotSerial.o $(OBJDIR)/mandelbrotThread.o $(PPM_OBJ)

$(APP_NAME): dirs $(OBJS)
		$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -lm -lpthread

$(OBJDIR)/%.o: %.cpp
		$(CXX) $< $(CXXFLAGS) -c -o $@

$(OBJDIR)/%.o: $(COMMONDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(OBJDIR)/main.o: $(COMMONDIR)/CycleTimer.h

