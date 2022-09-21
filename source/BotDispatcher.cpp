#include "BotDispatcher.h"

BotDispatcher::BotDispatcher() {

    stateDispatcher = new StateDispatcher();

	bot = new TgBot::Bot("5599412294:AAHV3ZyAY9pSPCZ29R3LA4sIy4X-7yHW66k");


    bot->getEvents().onCommand("start", [this](TgBot::Message::Ptr message) {
    	if (stateDispatcher->checkUserRegisrty(message->chat)) {
    		//if user is already registered
    		stateDispatcher->restoreSession(bot, message->chat);
    	} else {
    		// new user
//            bot->getApi().sendMessage(message->chat->id, "Hi! This is the bot for finding new people by intresting\n"
//            		"Let's fill your blanc");
            stateDispatcher->sendMessage(bot, message->chat->id, "Привет! Я бот, созданный для поиска новых знакомых\n"
            		"Давай познакомимся:)");
            stateDispatcher->createNewUser(bot, message->chat);
    	}
    	return;
    });
    bot->getEvents().onCommand("control", [this](TgBot::Message::Ptr message) {
    	stateDispatcher->gotoAdminLogin(bot, message);
    	return;
    });


    bot->getEvents().onAnyMessage([this](TgBot::Message::Ptr message) {
    	if (StringTools::startsWith(message->text, "/start")) {
    	            return;
    	        }
    	if (StringTools::startsWith(message->text, "/control")) {
    	    	            return;
    	    	        }

    	if (static_cast<TgBot::Location::Ptr>(message->location)) {
    		//remove markup
    		stateDispatcher->processLocation(bot, message);
    		return;
    	}

    	if (message->photo.size()) {
    		//remove markup
    		stateDispatcher->processImage(bot, message);
    		return;
    	}

    	stateDispatcher->processText(bot, message);
    });


    bot->getEvents().onCallbackQuery([this](TgBot::CallbackQuery::Ptr query) {
    	//std::cout << "data " << query->data << std::endl;
    	//remove previous keyboard
    	try {
    		bot->getApi().editMessageReplyMarkup(query->message->chat->id, query->message->messageId, "", nullptr);
    	} catch (TgBot::TgException &e) {}
    	// set new state

    	// if like button
    	if (query->data.substr(0, 3) == ":::") {
    		//std::cout<< "like to " << query->data.substr(3, query->data.size()) << std::endl;
    		stateDispatcher->setLike(query->message->chat->id, std::stoi(query->data.substr(3, query->data.size())));
    		stateDispatcher->addToWatchers(query->message->chat->id, std::stoi(query->data.substr(3, query->data.size())));
    		stateDispatcher->setState(bot, query->message->chat, DbDispatcher::IND_FLOW_MENU);
    		return;
    	}

    	// if message button
//    	if (query->data.substr(0, 3) == "___") {
//    		std::cout<< "message to " << query->data.substr(3, query->data.size()) << std::endl;
//    		//stateDispatcher->addToWatchers(query->message->chat->id, std::stoi(query->data.substr(3, query->data.size())));
//    		stateDispatcher->setState(bot, query->message->chat, DbDispatcher::IND_DIRECT_MSG);
//    		return;
//    	}

    	// if mutual like
    	if (query->data.substr(0, 3) == "!!!") {
//    		std::cout<< "mutual like to " << query->data.substr(3, query->data.size()) << std::endl;
    		int targetChatId = std::stoi(query->data.substr(3, query->data.size()));
//    		std::string rep = "You can start conversation with this user. Here is his personal link:\n@";
//    		rep.append(stateDispatcher->getUname(targetChatId));
    		try {
    			bot->getApi().sendMessage(query->message->chat->id,
    					"Ты можешь начать общаться с этим пользователем. Вот ссылка для личной переписки:\n@" +
    					    		stateDispatcher->getUname(targetChatId));
    		} catch (TgBot::TgException &e) {
    			std::cout << e.what() << std::endl;
    		}

    		stateDispatcher->deleteLiker(targetChatId, query->message->chat->id);
    		stateDispatcher->addToWatchers(query->message->chat->id, targetChatId);
    		stateDispatcher->setState(bot, query->message->chat, DbDispatcher::IND_TO_LIKERS);
    	    return;
    	}
    	// if mutual dislike
    	if (query->data.substr(0, 3) == "---") {
    		//std::cout<< "mutual dislike to " << query->data.substr(3, query->data.size()) << std::endl;
    		int targetChatId = std::stoi(query->data.substr(3, query->data.size()));
    		stateDispatcher->deleteLiker(targetChatId, query->message->chat->id);
    		stateDispatcher->addToWatchers(query->message->chat->id, targetChatId);
    		stateDispatcher->setState(bot, query->message->chat, DbDispatcher::IND_TO_LIKERS);
    	    return;
    	}

    	//default
    	try {
    		stateDispatcher->setState(bot, query->message->chat, std::stoi(query->data));
    	} catch (std::exception &e) {
    		// if can't parse state
    		std::cout << "cannot convert " << query->data << " to int " << std::endl;
    		stateDispatcher->setState(bot, query->message->chat, DbDispatcher::IND_MAIN_MENU);
    	}
    });
}

BotDispatcher::~BotDispatcher() {

}

void BotDispatcher::startBot() {
	while (true) {
    try {
        TgBot::TgLongPoll longPoll(*bot);
        while (true) {
            //printf("Long poll started\n");
            longPoll.start();
        }
    }
	catch (TgBot::TgException& e) {
        printf("error11: %s\n", e.what());
        startBot();
        //throw;
    }
    catch(std::exception &e) {
    	std::cout << "ab" <<std::endl;
    }
    startBot();
	}
}



