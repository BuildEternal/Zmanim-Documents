# Object files
OBJS += src/get_zmanim.o
OBJS += src/main.o
OBJS += src/parse/parse_date.o
OBJS += src/time_util.o
OBJS += src/timespan.o
OBJS += src/zmanim/day_info.o
OBJS += src/zmanim/document_info.o
OBJS += src/zmanim/zman_info.o
# ---

# Include directories
INCS += src
INCS += vendor
INCS += vendor/json/include
# ---

INCS := $(foreach INC, $(INCS), -I$(INC))

zmanim: $(OBJS)
	@echo Linking... & \
	g++ -o zmanim $(OBJS) -lws2_32 -lcrypt32 -lcryptui -lssl -lcrypto

%.o: %.cpp
	@echo Compiling $< to $@... & \
	g++ -c -o $@ $< $(INCS)
