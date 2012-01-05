all: 
	gcc g15date.c clock.c  -lg15daemon_client -lg15render -lpthread -lm -lrt -o g15date -Wunused -Wall -Wextra

install:
	cp ./g15date /usr/bin/

uninstall:
	rm /usr/bin/g15date

