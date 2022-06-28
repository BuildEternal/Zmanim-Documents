OBJS += src/main.o
OBJS += src/time_util.o
OBJS += src/parse/parse_date.o
OBJS += src/invalid_date_exception.o

zmanim: $(OBJS)
	g++ -o zmanim $(OBJS)