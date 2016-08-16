# sem-timer makefile
#
# 
# Use with mingw-gcc under linux
#

######################################################
#                                                    #
#                                                    #
#        To compile for 32 or 64 bit windows         #
#       set ARCH = 32 or 64 in the line bellow       #
#                                                    #
#      Alternatively set the ARCH variable from      #
#    the command line. For example 'make ARCH=32'    #
#                                                    #
#                                                    #
######################################################

ARCH = 64

#-----------------------------------------------------

ifeq ($(ARCH),64) 
	CC = /usr/bin/x86_64-w64-mingw32-g++
  RC = /usr/bin/x86_64-w64-mingw32-windres
else 
	CC = /usr/bin/i686-pc-mingw32-g++
	RC = /usr/bin/i686-pc-mingw32-windres
endif




EXEC = timer.exe
OBJS = timer.o timer_dlg.o
SOURCE = timer.c
WARNS = -Wall



CFLAGS = -O3 -D WINVER=0x501 $(WARNS) 
LDFLAGS = -s -Wl,--subsystem,windows -lgdi32 


all: $(EXEC)


$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

timer_dlg.o: timer_dlg.rc
	$(RC)  timer_dlg.rc $@

timer.o: $(SOURCE)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	$(RM) $(EXEC) $(OBJS)


ctags:  
	ctags ./*.[c,h] *.rc
