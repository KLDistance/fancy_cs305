
TARS = 	bin/portable_file_browser	

OBJS = 	obj/tcp_server.o 			\
		obj/message_queue.o			\
		obj/http_interpreter.o		\
		obj/content_provider.o 		

bin/portable_file_browser:obj/tcp_server.o obj/message_queue.o obj/http_interpreter.o obj/content_provider.o
	g++ src/main.cpp -o bin/portable_file_browser obj/tcp_server.o obj/message_queue.o obj/http_interpreter.o obj/content_provider.o -Iinclude -lpthread
	-rm obj/*

obj/tcp_server.o:src/tcp_server.cpp
	g++ -c src/tcp_server.cpp -o obj/tcp_server.o -Iinclude

obj/message_queue.o:src/message_queue.cpp 
	g++ -c src/message_queue.cpp -o obj/message_queue.o -Iinclude 

obj/http_interpreter.o:src/http_interpreter.cpp 
	g++ -c src/http_interpreter.cpp -o obj/http_interpreter.o -Iinclude

obj/content_provider.o:src/content_provider.cpp 
	g++ -c src/content_provider.cpp -o obj/content_provider.o -Iinclude

clean:
	rm obj/*
