CC=gcc
CFLAGS=-Wall -O1

.PHONY: default
default:
	$(error No target specified.)



.PHONY: billboard
billboard: billboard/server

billboard/server: server.c billboard/billboard.so
	$(CC) -no-pie -DPORT=12345 -DLIB=\"./billboard.so\" -o ./billboard/server server.c $(CFLAGS) -ldl
	cp server.c billboard/server.c
	
billboard/billboard.so: billboard.c
	$(CC) -o ./billboard/billboard.so -shared -fPIC billboard.c $(CFLAGS)



.PHONY: giftwrapper
giftwrapper: giftwrapper/server

giftwrapper/server: server.c giftwrapper/giftwrapper.so
	$(CC) -no-pie -fno-stack-protector -D_FORTIFY_SOURCE=0 -DPORT=12345 -DLIB=\"./giftwrapper.so\" -o ./giftwrapper/server server.c $(CFLAGS) -ldl
	cp server.c giftwrapper/server.c
	
giftwrapper/giftwrapper.so: giftwrapper.c
	$(CC) -fno-stack-protector -D_FORTIFY_SOURCE=0 -o ./giftwrapper/giftwrapper.so -shared -fPIC giftwrapper.c $(CFLAGS)



.PHONY: giftwrapper2
giftwrapper2: giftwrapper2/server

giftwrapper2/server: server.c giftwrapper2/giftwrapper2.so
	$(CC) -no-pie -fno-stack-protector -D_FORTIFY_SOURCE=0 -DPORT=12345 -DLIB=\"./giftwrapper2.so\" -o ./giftwrapper2/server server.c $(CFLAGS) -ldl
	cp server.c giftwrapper2/server.c
	
giftwrapper2/giftwrapper2.so: giftwrapper2.c
	$(CC) -fno-stack-protector -D_FORTIFY_SOURCE=0 -o ./giftwrapper2/giftwrapper2.so -shared -fPIC giftwrapper2.c $(CFLAGS)



.PHONY: mate
mate: mate/server

mate/server: server.c mate/mate.so
	$(CC) -no-pie -DPORT=12345 -DLIB=\"./mate.so\" -o ./mate/server server.c $(CFLAGS) -ldl
	cp server.c mate/server.c
	
mate/mate.so: mate.c
	$(CC) -o ./mate/mate.so -shared -fPIC mate.c $(CFLAGS)



.PHONY: mate2
mate2: mate2/server

mate2/server: server.c mate2/mate2.so
	$(CC) -no-pie -DPORT=12345 -DLIB=\"./mate2.so\" -o ./mate2/server server.c $(CFLAGS) -ldl
	cp server.c mate2/server.c
	
mate2/mate2.so: mate2.c
	$(CC) -o ./mate2/mate2.so -shared -fPIC mate2.c $(CFLAGS)



.PHONY: shredder
shredder: shredder/server

shredder/server: server.c shredder/shredder.so
	$(CC) -no-pie -DPORT=12345 -DLIB=\"./shredder.so\" -o ./shredder/server server.c $(CFLAGS) -ldl
	cp server.c shredder/server.c
	
shredder/shredder.so: shredder.c
	$(CC) -o ./shredder/shredder.so -shared -fPIC shredder.c $(CFLAGS)
