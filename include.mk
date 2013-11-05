
INCS = $(addprefix -I,$(INC_PATH))

ifdef DEBUG
CPPFLAGS = -O0 -g3 -Wall -fmessage-length=0 -D_WINDOWS -D_WIN32_WINNT=0x0500 -DWINVER=0x0500 $(INCS)
CFLAGS = -O0 -g3 -Wall -fmessage-length=0 -D_WINDOWS -DWINVER=0x0500 -D_WIN32_WINNT=0x0500 -DNO_MULTIMON -DNO_HTMLHELP  $(INCS)
else
CPPFLAGS = -O3 -g -Wall -fmessage-length=0 -D_WINDOWS -D_WIN32_WINNT=0x0500 -DWINVER=0x0500  $(INCS)
CFLAGS = -O3 -g -Wall -fmessage-length=0 -D_WINDOWS -DWINVER=0x0500 -D_WIN32_WINNT=0x0500 -DNO_MULTIMON -DNO_HTMLHELP $(INCS)
endif

resources/resources.o: resources/resources.rc resources/Resource.h resources/res/*
	windres $< $@

LIBS = $(addprefix -L,$(LIBS_PATH)) $(addprefix -l,$(LIBS_NAME))

C_SRCS := $(wildcard $(addsuffix /*.c,$(C_PATH)))
CPP_SRCS := $(wildcard $(addsuffix /*.cpp,$(CPP_PATH)))
OBJS := $(RESOURCES) $(patsubst %.c,%.o,$(C_SRCS)) $(patsubst %.cpp,%.o,$(CPP_SRCS))

.c.d:
	echo -n $(@D)/ > $@
	$(CC) $(CFLAGS) -MM $< >> $@

.cpp.d:
	echo -n $(@D)/ > $@
	$(CC) $(CFLAGS) -MM $< >> $@

$(TARGET):	$(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LIBS)

all: $(TARGET)

depends : $(C_SRCS:.c=.d) $(CPP_SRCS:.cpp=.d)

clean:
	rm -f $(OBJS) $(TARGET) $(C_SRCS:.c=.d) $(CPP_SRCS:.cpp=.d)

.SUFFIXES: .d

ifeq ($(MAKECMDGOALS),all) 
include $(C_SRCS:.c=.d)
include $(CPP_SRCS:.cpp=.d)
else 
endif 

.PHONY : all clean