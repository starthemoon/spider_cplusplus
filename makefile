export CPLUS_INCLUDE_PATH=/home/starthemoon/star/1tools/c++_lib/boost_1_81_0:/home/starthemoon/star/1tools/c++_lib/asio-1.24.0/include:$(shell printenv CPLUS_INCLUDE_PATH)
all:clean dev bin/website_visitor #bin/tcp_client bin/async_timer #bin/async_daytime_server #bin/sync_daytime_server priviledge

priviledge:
	chmod 777 bin/*

bin/website_visitor: obj/website_visitor.o
	g++ -o $@ $< -lcrypto -lssl

obj/website_visitor.o: src/website_visitor.cpp
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