############## SunOS ########################
CC       = gcc -g
CCOPTS   = -c #-Wall
LINK     = gcc -g

DEPEND  = makedepend

LINKOPTS = -lm -lnsl
#############################################

SOURCES = ftlmgr.c devicedriver.c main.c

OBJECTS = ftlmgr.o devicedriver.o main.o

SOURCES_r = ftlmgr.c devicedriver.c main_r.c

OBJECTS_r = ftlmgr.o devicedriver.o main_r.o

SOURCES_u = ftlmgr.c devicedriver.c main_u.c

OBJECTS_u = ftlmgr.o devicedriver.o main_u.o

SOURCES_a = ftlmgr.c devicedriver.c main_a.c

OBJECTS_a = ftlmgr.o devicedriver.o main_a.o

.SUFFIXES : .C .c .o

.c.o:
	$(CC) $(CCOPTS) $<

write: blkmapftl_w clean
read: blkmapftl_r clean
update: blkmapftl_u clean
advance: blkmapftl_a clean

blkmapftl_w: $(OBJECTS)
	$(LINK) -o blkmapftl $(OBJECTS) $(LINKOPTS)

blkmapftl_r: $(OBJECTS_r)
	$(LINK) -o blkmapftl $(OBJECTS_r) $(LINKOPTS)

blkmapftl_u: $(OBJECTS_u)
	$(LINK) -o blkmapftl $(OBJECTS_u) $(LINKOPTS)

blkmapftl_a: $(OBJECTS_a)
	$(LINK) -o blkmapftl $(OBJECTS_a) $(LINKOPTS)

clean:
	rm *.o 

depend : $(SOURCES)
	$(DEPEND) $(DEFINES) -I/usr/local/lib/g++-include $(SOURCES)

# DO NOT DELETE
