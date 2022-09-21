# VinoBot
c++ bot for looking for peoples

Compile command (in source folder):
g++ main.cpp BotDispatcher.h BotDispatcher.cpp DbDispatcher.h DbDispatcher.cpp StateDispatcher.h StateDispatcher.cpp GeoDecoder.h GeoDecoder.cpp -o ../bin/telegram_bot --std=c++14 -I/usr/local/include -lTgBot -lboost_system -lssl -lcrypto -lpthread -lpqxx -lpq -lcpprest
