/*
 * StateDispatcher.h
 *
 *  Created on: Aug 19, 2022
 *      Author: eddyneshton
 */

#ifndef STATEDISPATCHER_H_
#define STATEDISPATCHER_H_
#include <string>
#include <vector>
#include <tgbot/tgbot.h>
#include "DbDispatcher.h"

class BaseBotState {
public:

	BaseBotState(int stateLink, std::string description);
	virtual ~BaseBotState();

	const int getLabel() const;
	std::string getDescription() const;

	virtual TgBot::InlineKeyboardMarkup::Ptr getColumnInlineKeyboard() = 0;
	virtual TgBot::InlineKeyboardMarkup::Ptr getRowInlineKeyboard() = 0;

protected:
	const int stateLabel;
	std::string stateDescription;
};

class BotState : public BaseBotState {
public:
	struct StateButton {
		std::string buttonText;
		int buttonCalbackData;
	};

	BotState(int stateLink, std::string description, std::vector <StateButton> stateButtons);
	virtual ~BotState();
	virtual TgBot::InlineKeyboardMarkup::Ptr getColumnInlineKeyboard();
	virtual TgBot::InlineKeyboardMarkup::Ptr getRowInlineKeyboard();
private:
	std::vector <StateButton> stateButtons;
};

class FlowBotState : public BaseBotState {
public:
	FlowBotState(std::string description, int targetId, int stateLink);
	virtual ~FlowBotState();
	virtual TgBot::InlineKeyboardMarkup::Ptr getColumnInlineKeyboard();
	virtual TgBot::InlineKeyboardMarkup::Ptr getRowInlineKeyboard();
private:
	int targetId;
};

class LikerBotState : public BaseBotState {
public:
	LikerBotState(std::string description, int targetId, int stateLink);
	virtual ~LikerBotState();
	virtual TgBot::InlineKeyboardMarkup::Ptr getColumnInlineKeyboard();
	virtual TgBot::InlineKeyboardMarkup::Ptr getRowInlineKeyboard();
private:
	int targetId;
};

class StateDispatcher {
public:
	StateDispatcher();
	virtual ~StateDispatcher();

	void sendMessage(TgBot::Bot *bot, int chatId, std::string text, TgBot::InlineKeyboardMarkup::Ptr keyboard = nullptr);

	void sendPhoto(TgBot::Bot *bot, int chatId, std::string image, std::string text ="");

	void setState(TgBot::Bot *bot, TgBot::Chat::Ptr chat, int newState);

	bool checkUserRegisrty(TgBot::Chat::Ptr chat);

	void createNewUser(TgBot::Bot *bot, TgBot::Chat::Ptr chat);

	void restoreSession(TgBot::Bot *bot, TgBot::Chat::Ptr chat);

	void processText(TgBot::Bot *bot, TgBot::Message::Ptr message);

	void processLocation(TgBot::Bot *bot, TgBot::Message::Ptr message);

	void processImage(TgBot::Bot *bot, TgBot::Message::Ptr message);

	void gotoAdminLogin (TgBot::Bot *bot, TgBot::Message::Ptr message);

	void setLike(int subjectChatId, int targetChatId);

	void addToWatchers (int subjectChatId, int targetChatId);

	void deleteLiker (int subjectChatId, int targetChatId);

	std::string getUname (int chatId);

private:
	std::vector <BotState> stateMachine;

	DbDispatcher *db;

	void sendCommonMessage(TgBot::Bot *bot, std::string message);

};

#endif /* STATEDISPATCHER_H_ */
