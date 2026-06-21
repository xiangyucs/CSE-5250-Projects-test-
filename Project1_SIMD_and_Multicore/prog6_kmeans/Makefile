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

CXX=g++ $(CSUSB_ARCHFLAGS) -march=native
CXXFLAGS=-I../common -Iobjs/ -O3 -Wall


APP_NAME=kmeans
OBJDIR=objs
COMMONDIR=../common

PPM_CXX=$(COMMONDIR)/ppm.cpp
PPM_OBJ=$(addprefix $(OBJDIR)/, $(subst $(COMMONDIR)/,, $(PPM_CXX:.cpp=.o)))

TASKSYS_CXX=$(COMMONDIR)/tasksys.cpp
TASKSYS_LIB=-lpthread
TASKSYS_OBJ=$(addprefix $(OBJDIR)/, $(subst $(COMMONDIR)/,, $(TASKSYS_CXX:.cpp=.o)))

default: $(APP_NAME)

.PHONY: dirs clean gen

dirs:
		/bin/mkdir -p $(OBJDIR)/

clean:
		/bin/rm -rf $(OBJDIR) *.ppm *.log *.png *~ $(APP_NAME) gen_data

# CSUSB: standalone dataset generator (optional; kmeans also auto-generates).
gen: gen_data
gen_data: gen_data.cpp
		$(CXX) $(CXXFLAGS) -o gen_data gen_data.cpp

OBJS=$(OBJDIR)/main.o $(OBJDIR)/kmeansThread.o $(OBJDIR)/utils.o $(PPM_OBJ) $(TASKSYS_OBJ)

$(APP_NAME): dirs $(OBJS)
		$(CXX) $(CXXFLAGS) -o $@ $(OBJS) -lm $(TASKSYS_LIB)

$(OBJDIR)/%.o: %.cpp
		$(CXX) $< $(CXXFLAGS) -c -o $@

$(OBJDIR)/%.o: $(COMMONDIR)/%.cpp
	$(CXX) $< $(CXXFLAGS) -c -o $@

$(OBJDIR)/main.o: $(COMMONDIR)/CycleTimer.h