# Makefile template file for FLAME
#
# FLAME is the FLAME source files
# SOURCE is the user source files
#
# The assumption is that the user only provides C files
# and a model files <file>.xml
#
DEFINES       = -DNONE
#LIBMBOARD_DIR = libmboard
#LIBMBOARD_DIR = /usr/local/
LIBMBOARD_DIR = /home/caos/amoreno/sfw/
LIBMBOARD_INC = $(LIBMBOARD_DIR)/include
LIBMBOARD_LIB = $(LIBMBOARD_DIR)/lib

 


#CC      = mpicc

#export TAU_MAKEFILE = ~/sfw/tau-2.26.2p1/x86_64/lib/Makefile.tau-mpi-pdt
CC      = tau_cc.sh
CFLAGS  = -std=c99 -Wall -I$(LIBMBOARD_INC) ${DEFINES}

CFLAGS += -D_DEBUG_MODE -g
CFLAGS += -tau_makefile=~/sfw/tau-2.26.3/x86_64/lib/Makefile.tau-mpi-pdt -tau_options=-optCompInst
#CFLAGS += -tau_makefile=~/sfw/tau-2.26.3/x86_64/lib/Makefile.tau-mpi-pdt 

LDFLAGS = -L$(LIBMBOARD_LIB)
LIBS    = -lmboard_pd  -lm  -lfftw3 -lpthread -luuid -lfnv

#FLAME source files
FLAME   = main.c memory.c xml.c messageboards.c partitioning.c rules.c timing.c

#FLAME auxilary files
AUX 	= stategraph.dot stategraph_colour.dot process_order_graph.dot Doxyfile latex.tex

# FLAME generated model files
SOURCES =  persons_interact.c 
HEADERS = header.h low_primes.h mboard.h  person_agent_header.h 

DEPS    = Makefile header.h low_primes.h

OBJECTS = $(SOURCES:.c=.o) $(FLAME:.c=.o)
EXECUTABLE = main
RM = rm -f

all: $(SOURCES) $(FLAME) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

$(OBJECTS): $(DEPS)

.c.o:
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	$(RM) $(OBJECTS) $(EXECUTABLE) $(EXECUTABLE).exe
vclean:
	$(RM) main $(EXECUTABLE) $(EXECUTABLE).exe $(OBJECTS)  $(FLAME) $(HEADERS)  $(AUX) Makefile
