############## SunOS ########################
CC       = gcc -g
CCOPTS   = -c #-Wall
LINK     = gcc -g

DEPEND  = makedepend

LINKOPTS = -lm -lnsl
#############################################

SOURCES = ftlmgr.c devicedriver.c main.c

OBJECTS = ftlmgr.o devicedriver.o main.o

.SUFFIXES : .C .c .o


############## Grading ######################
GSOURCES = ftlmgr.c devicedriver.c
WMAIN = main_w.c
RMAIN = main_r.c
UMAIN = main_u.c
AMAIN = main_a.c

write    :
	$(LINK) -o write   $(GSOURCES) $(WMAIN) $(LINKOPTS) -DGRADE

read     :
	$(LINK) -o read    $(GSOURCES) $(RMAIN) $(LINKOPTS) -DGRADE
	
update   :
	$(LINK) -o update  $(GSOURCES) $(UMAIN) $(LINKOPTS) -DGRADE

advance :
	$(LINK) -o advance $(GSOURCES) $(AMAIN) $(LINKOPTS) -DGRADE

#myclean:
#	find . ! -name ftlmgr.c -delete
###############################################

.c.o:
	$(CC) $(CCOPTS) $<

all: blkmapftl

blkmapftl: $(OBJECTS)
	$(LINK) -o blkmapftl $(OBJECTS) $(LINKOPTS)

clean:
	rm *.o blkmapftl core

depend : $(SOURCES)
	$(DEPEND) $(DEFINES) -I/usr/local/lib/g++-include $(SOURCES)

# DO NOT DELETE
