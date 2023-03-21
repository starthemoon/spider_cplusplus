export CPLUS_INCLUDE_PATH=/home/starthemoon/star/1tools/c++_lib/boost_1_81_0:/home/starthemoon/star/1tools/c++_lib/asio-1.24.0/include:$(shell printenv CPLUS_INCLUDE_PATH)
all:clean dev bin/main #bin/tcp_client bin/async_timer #bin/async_daytime_server #bin/sync_daytime_server priviledge

priviledge:
	chmod 777 bin/*

bin/main: obj/main.o obj/ResponseParser.o obj/ImgNameGenerator.o obj/ImgWriter.o obj/Utils.o obj/WebsiteVisitor.o
	g++ -o $@ $^ -lssl -lcrypto

obj/Utils.o: src/Utils.cpp
	g++ -c -std=c++20 -g -o $@ $<

obj/ResponseParser.o: src/ResponseParser.cpp
	g++ -c -std=c++20 -g -o $@ $<

obj/WebsiteVisitor.o: src/WebsiteVisitor.cpp
	g++ -c -std=c++20 -g -o $@ $<

obj/ImgNameGenerator.o: src/ImgNameGenerator.cpp
	g++ -c -std=c++20 -g -o $@ $<

obj/ImgWriter.o: src/ImgWriter.cpp
	g++ -c -std=c++20 -g -o $@ $<

obj/main.o: src/main.cpp
	g++ -c -std=c++20 -g -o $@ $<

bin/async_timer: obj/async_timer.o
	g++ -o $@ $<

obj/async_timer.o: src/async_timer.cpp
	g++ -c -std=c++20 -g -o $@ $<

bin/async_daytime_server: obj/async_daytime_server.o
	g++ -o $@ $<

obj/async_daytime_server.o: src/async_daytime_server.cpp
	g++ -c -std=c++20 -g -o $@ $<

bin/sync_daytime_server: obj/sync_daytime_server.o
	g++ -o $@ $<

obj/sync_daytime_server.o: src/sync_daytime_server.cpp
	g++ -c -std=c++20 -g -o $@ $<

bin/tcp_client: obj/tcp_client.o
	g++ -o $@ $<

obj/tcp_client.o: src/tcp_client.cpp
	g++ -c -std=c++20 -g -o $@ $<

clean:
	@rm -f bin/*
	@rm -f obj/*

dev:
	@echo $(CPLUS_INCLUDE_PATH)

prod:export CPLUS_INCLUDE_PATH=/home/starthemoon/1tools/c++_lib/boost_1_81_0

.PHONY: clean