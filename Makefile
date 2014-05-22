EXEC = timer.exe
OBJS = timer.o timer_dlg.o
SOURCE = timer.c
WARNS = -Wall
CC = /usr/bin/i686-pc-mingw32-g++



CFLAGS = -O3 -D WINVER=0x501 $(WARNS) 
# -D UNICODE -D _UNICODE -D WINVER=0x501 $(WARNS) 
LDFLAGS = -s -Wl,--subsystem,windows -lgdi32 
RC = /usr/bin/i686-pc-mingw32-windres


all: $(EXEC)


$(EXEC): $(OBJS) 
	$(CC) $(CFLAGS) $(OBJS) $(LDFLAGS) -o $@

timer_dlg.o: timer_dlg.rc
	$(RC)  timer_dlg.rc $@

timer.o: $(SOURCE)
	$(CC) $(CFLAGS) -c $< -o $@ 

clean:
	-rm $(EXEC) $(OBJS)


ctags:  
	ctags ./*.[c,h] *.rc
