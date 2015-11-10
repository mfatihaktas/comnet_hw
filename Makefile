BOOST_INC = -I$(BOOST_DIR)/include
BOOST_LIB = -L$(BOOST_DIR)/lib -lboost_system -lboost_iostreams -lboost_filesystem
# 
INC = $(BOOST_INC)
LIB = $(BOOST_LIB)

ODIR = obj

.PHONY: all clean

APPS := hw6

all: ${APPS}

hw6: $(ODIR)/hw6.o
	$(CPP) -g -o $@ $^ $(INC) $(LIB)

$(ODIR)/%.o: %.cpp
	$(CPP) -g -c -o $@ $< $(INC) $(LIB)

clean:
	rm -f $(ODIR)/*.o ${APPS}