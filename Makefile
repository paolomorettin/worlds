# Makefile for Irrlicht Examples
# It's usually sufficient to change just the target name and source file list
# and be sure that CXX is set to a valid compiler

# Name of the executable created (.exe will be added automatically if necessary)
Target := Worlds
# Path to Irrlicht directory, should contain include/ and lib/
IrrlichtHome := ./irrlicht-1.8.1
# Path for the executable. Note that Irrlicht.dll should usually also be there for win32 systems
BinPath = ./bin

# general compiler settings (might need to be set when compiling the lib, too)
# preprocessor flags, e.g. defines and include paths
USERCPPFLAGS = 
# compiler flags such as optimization flags
USERCXXFLAGS = -O3 -ffast-math -std=c++11 -g -Wall -Wextra -pedantic -Wno-unused-parameter
#USERCXXFLAGS = -g -Wall
# linker flags such as additional libraries and link paths
USERLDFLAGS =

####
#no changes necessary below this line
####

CPPFLAGS = -I$(IrrlichtHome)/include -I/usr/X11R6/include `pkg-config --cflags bullet` $(USERCPPFLAGS)
CXXFLAGS = $(USERCXXFLAGS)
LDFLAGS = $(USERLDFLAGS) `pkg-config --cflags --libs bullet`

#default target is Linux
all: all_linux

# target specific settings
# all_linux all_win32 static_win32: LDFLAGS += -L$(IrrlichtHome)/lib/$(SYSTEM) -lIrrlicht
# all_linux: LDFLAGS += -L/usr/X11R6/lib$(LIBSELECT) -lGL -lXxf86vm -lXext -lX11 -lXcursor
# all_linux clean_linux: SYSTEM=Linux
# all_win32 clean_win32 static_win32: SYSTEM=Win32-gcc
# all_win32 clean_win32 static_win32: SUF=.exe
# static_win32: CPPFLAGS += -D_IRR_STATIC_LIB_
# all_win32: LDFLAGS += -lopengl32 -lm
# static_win32: LDFLAGS += -lgdi32 -lwinspool -lcomdlg32 -lole32 -loleaut32 -luuid -lodbc32 -lodbccp32 -lopengl32
include makefile.config

# name of the binary - only valid for targets which set SYSTEM
DESTPATH = $(BinPath)/$(Target)$(SUF)
# Should set CPPFLAGS, LDFLAGS, Sources
Objects := $(Sources:.cpp=.o)

%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

all_linux all_win32 static_win32: $(Objects)
	@mkdir -p $(BinPath)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(Objects) -o $(DESTPATH) $(LDFLAGS)

clean:
	$(RM) $(DESTPATH)
	$(RM) $(Objects)

.PHONY: all all_win32 static_win32 clean clean_linux clean_win32

#multilib handling
ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif
#solaris real-time features
ifeq ($(HOSTTYPE), sun4)
LDFLAGS += -lrt
endif

