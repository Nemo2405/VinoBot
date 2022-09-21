/*
 * StateDispatcher.cpp
 *
 *  Created on: Aug 19, 2022
 *      Author: eddyneshton
 */

#include "StateDispatcher.h"

	 const std::string emo_heart 		=  u8"\u2764";
	 const std::string emo_disheart		=  u8"👎";//U+1F44E
	 const std::string emo_fine			=  u8"😄";
	 const std::string emo_message		=  u8"💬";
	 const std::string emo_processing	=  u8"💭";
	 const std::string emo_sleep		=  u8"💤";


BaseBotState::BaseBotState(int stateLink, std::string description) :
	stateLabel(stateLink), stateDescription(description) {}

BotState::BotState(int stateLink, std::string description, std::vector <StateButton> buttons) :
	BaseBotState(stateLink, description), stateButtons(buttons)
{}

BaseBotState::~BaseBotState() {}
BotState::~BotState() {}
FlowBotState::~FlowBotState(){}
LikerBotState::~LikerBotState() {}

const int BaseBotState::getLabel() const {
	return stateLabel;
}

std::string BaseBotState::getDescription() const {
	return stateDescription;
}

TgBot::InlineKeyboardMarkup::Ptr BotState::getColumnInlineKeyboard() {
	using namespace TgBot;

	InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
	for (StateButton nowBtn : stateButtons) {
		std::vector <InlineKeyboardButton::Ptr> row;
		InlineKeyboardButton::Ptr button(new InlineKeyboardButton);
		button->text = nowBtn.buttonText;
		button->callbackData = std::to_string(nowBtn.buttonCalbackData);
		row.push_back(button);
		keyboard->inlineKeyboard.push_back(row);
	}
	return keyboard;
}

TgBot::InlineKeyboardMarkup::Ptr BotState::getRowInlineKeyboard() {
	using namespace TgBot;

	InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
		std::vector <InlineKeyboardButton::Ptr> row;
	for (StateButton nowBtn : stateButtons) {
		InlineKeyboardButton::Ptr button(new InlineKeyboardButton);
		button->text = nowBtn.buttonText;
		button->callbackData = std::to_string(nowBtn.buttonCalbackData);
		row.push_back(button);
	}
	keyboard->inlineKeyboard.push_back(row);
	return keyboard;
}

FlowBotState::FlowBotState(std::string description, int targetId, int stateLink = DbDispatcher::IND_FLOW_MENU) :
		BaseBotState(stateLink, description), targetId(targetId) {}

TgBot::InlineKeyboardMarkup::Ptr FlowBotState::getColumnInlineKeyboard() {
	using namespace TgBot;

	InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
	for (int i = 0; i < 3; i++) {
		std::vector <InlineKeyboardButton::Ptr> row;
		InlineKeyboardButton::Ptr button(new InlineKeyboardButton);

		switch (i) {
			case 0: {
				button->text = emo_heart;
				button->callbackData = ":::" + std::to_string(targetId);
				break;
			}
			case 1: {
				button->text = emo_disheart;
				button->callbackData = std::to_string(DbDispatcher::IND_FLOW_MENU);
				break;
			}
//			case 2: {
//				button->text = emo_message;
//				button->callbackData = "___" + std::to_string(targetId);
//				break;
//			}
			case 2: {
				button->text = emo_sleep;
				button->callbackData = std::to_string(DbDispatcher::IND_MAIN_MENU);
				break;
			}
		}
		row.push_back(button);
		keyboard->inlineKeyboard.push_back(row);
	}
	return keyboard;
}

TgBot::InlineKeyboardMarkup::Ptr FlowBotState::getRowInlineKeyboard() {
	using namespace TgBot;
	InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
	std::vector <InlineKeyboardButton::Ptr> row;
	for (int i = 0; i < 3; i++) {
		InlineKeyboardButton::Ptr button(new InlineKeyboardButton);

		switch (i) {
			case 0: {
				button->text = emo_heart;
				button->callbackData = ":::" + std::to_string(targetId);
				break;
			}
			case 1: {
				button->text = emo_disheart;
				button->callbackData = std::to_string(DbDispatcher::IND_FLOW_MENU);
				break;
			}
//			case 2: {
//				button->text = emo_message;
//				button->callbackData = "___" + std::to_string(targetId);
//				break;
//			}
			case 2: {
				button->text = emo_sleep;
				button->callbackData = std::to_string(DbDispatcher::IND_MAIN_MENU);
				break;
			}
		}
		row.push_back(button);
	}
	keyboard->inlineKeyboard.push_back(row);
	return keyboard;
}

LikerBotState::LikerBotState(std::string description, int targetId, int stateLink = DbDispatcher::IND_LIKERS) :
		BaseBotState(stateLink, description), targetId(targetId) {}

TgBot::InlineKeyboardMarkup::Ptr LikerBotState::getColumnInlineKeyboard() {
	using namespace TgBot;

	InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
	for (int i = 0; i < 3; i++) {
		std::vector <InlineKeyboardButton::Ptr> row;
		InlineKeyboardButton::Ptr button(new InlineKeyboardButton);
		switch (i) {
			case 0: {
				button->text = emo_heart;
				button->callbackData = "!!!" + std::to_string(targetId);
				break;
			}
			case 1: {
				button->text = emo_disheart;
				button->callbackData = "---" + std::to_string(targetId);
				break;
			}
			case 2: {
				button->text = emo_sleep;
				button->callbackData = std::to_string(DbDispatcher::IND_MAIN_MENU);
				break;
			}
		}
		row.push_back(button);
		keyboard->inlineKeyboard.push_back(row);
	}
	return keyboard;
}

TgBot::InlineKeyboardMarkup::Ptr LikerBotState::getRowInlineKeyboard() {
	using namespace TgBot;
	InlineKeyboardMarkup::Ptr keyboard(new InlineKeyboardMarkup);
	std::vector <InlineKeyboardButton::Ptr> row;
	for (int i = 0; i < 3; i++) {
		InlineKeyboardButton::Ptr button(new InlineKeyboardButton);

		switch (i) {
			case 0: {
				button->text = emo_heart;
				button->callbackData = "+++" + std::to_string(targetId);
				break;
			}
			case 1: {
				button->text = emo_disheart;
				button->callbackData = "---" + std::to_string(targetId);
				break;
			}
			case 2: {
				button->text = emo_sleep;
				button->callbackData = std::to_string(DbDispatcher::IND_MAIN_MENU);
				break;
			}
		}
		row.push_back(button);
	}
	keyboard->inlineKeyboard.push_back(row);
	return keyboard;
}

StateDispatcher::StateDispatcher() {

    db = new DbDispatcher();

	///MAIN MENU
	stateMachine.push_back(BotState(		DbDispatcher::IND_MAIN_MENU,
											"Главное меню. Выбери действие",
		std::vector<BotState::StateButton>({{"К анкетам", 				DbDispatcher::IND_SHOW_LIKER},
											{"Редактировать анкету", 	DbDispatcher::IND_BLANC_EDIT},
											{"Выйти", 					DbDispatcher::IND_DISABLE_ACCOUNT},
											{"Пригласить друзей", 		DbDispatcher::IND_INVITE},})));
//
//	///FLOW MENU
//	stateMachine.push_back(BotState(		DbDispatcher::IND_FLOW_MENU,
//											"Target Action",
//		std::vector<BotState::StateButton>({{emo_heart, 	DbDispatcher::IND_FLOW_MENU},
//											{emo_disheart,	DbDispatcher::IND_FLOW_MENU},
//											{emo_message,	DbDispatcher::IND_MESSAGE_MENU},
//											{emo_sleep, 	DbDispatcher::IND_MAIN_MENU},})));
//
//	///SEND MESSAGE TO USER
	stateMachine.push_back(BotState(		DbDispatcher::IND_MESSAGE_MENU,
											"Leave your message to This User",
		std::vector<BotState::StateButton>({{"Send Message", 		DbDispatcher::IND_FLOW_MENU},
											{"Go back to blancs", 	DbDispatcher::IND_FLOW_MENU},})));
//	///PROFIEL EDIT
	stateMachine.push_back(BotState(		DbDispatcher::IND_BLANC_EDIT,
											"Меню редактирования анкеты. Что ты хочешь изменить?",
		std::vector<BotState::StateButton>({{"Изменить полностью", 			DbDispatcher::IND_E_AGE_EDIT},
											{"Изменить описание", 	DbDispatcher::IND_E_DESCRIPT_EDIT},
											{"Изменить фото", 			DbDispatcher::IND_E_IMG_EDIT},
											{"Вернуться", 			DbDispatcher::IND_MAIN_MENU},})));
//
//	stateMachine.push_back(BotState(		DbDispatcher::IND_E_AGE_EDIT,
//											"What's your age?",
//		std::vector<BotState::StateButton>({{"Leave current: 777", DbDispatcher::IND_E_USER_SEX_EDIT},})));
//
	stateMachine.push_back(BotState(		DbDispatcher::IND_E_USER_SEX_EDIT,
											"Укажи свой пол",
		std::vector<BotState::StateButton>({{"Я парень", 	DbDispatcher::IND_GENDER_MALE},
											{"Я девушка", 	DbDispatcher::IND_GENDER_FEMALE},})));
//
	stateMachine.push_back(BotState(		DbDispatcher::IND_E_TARGET_SEX_EDIT,
											"Кого ты ищешь?",
		std::vector<BotState::StateButton>({{"Парней", 			DbDispatcher::IND_TARGET_GENDER_MALE},
											{"Девушек", 			DbDispatcher::IND_TARGET_GENDER_FEMALE},
											{"Неважно",	DbDispatcher::IND_TARGET_GENDER_NOMATT},})));
//
//	stateMachine.push_back(BotState(		DbDispatcher::IND_E_NAME_EDIT,
//											"What's your name?",
//		std::vector<BotState::StateButton>({{"Leave current: AAA", DbDispatcher::IND_E_LOCATION_EDIT},})));
//
//	stateMachine.push_back(BotState(		DbDispatcher::IND_E_LOCATION_EDIT,
//											"Where are you from?",
//		std::vector<BotState::StateButton>({{"Leave current: New york", DbDispatcher::IND_E_DESCRIPTION_EDIT},})));
//
//	stateMachine.push_back(BotState(		DbDispatcher::IND_E_DESCRIPTION_EDIT,
//											"Say a couple of words about yourself to interest other users",
//		std::vector<BotState::StateButton>({{"Leave current", DbDispatcher::IND_E_IMAGE_EDIT},})));
//
//	stateMachine.push_back(BotState(		DbDispatcher::IND_E_DESCRIPT_EDIT,
//											"Say a couple of words about yourself to interest other users",
//		std::vector<BotState::StateButton>({{"Leave current", DbDispatcher::IND_BLANC_EDIT},})));
//
	stateMachine.push_back(BotState(		DbDispatcher::IND_E_IMAGE_EDIT,
											"Пришли мне фото, чтобы тебя могли увидеть другие",
		std::vector<BotState::StateButton>({{"Оставить как есть", DbDispatcher::IND_E_FINAL_EDIT},})));

	stateMachine.push_back(BotState(		DbDispatcher::IND_E_IMG_EDIT,
											"Пришли мне фото, чтобы тебя могли увидеть другие",
		std::vector<BotState::StateButton>({{"Оставить как есть", DbDispatcher::IND_BLANC_EDIT},})));
//
//	stateMachine.push_back(BotState(		DbDispatcher::IND_E_FINAL_EDIT,
//											"That's result. Do you like it?",
//		std::vector<BotState::StateButton>({{"Yes, it's cool", 			DbDispatcher::IND_MAIN_MENU},
//											{"No, I want to try again", DbDispatcher::IND_BLANC_EDIT},})));

}

StateDispatcher::~StateDispatcher() {
}

void StateDispatcher::sendMessage(TgBot::Bot *bot, int chatId, std::string text, TgBot::InlineKeyboardMarkup::Ptr keyboard) {
	//std::cout << "message: " << text << std::endl;
	try {
	bot->getApi().sendMessage(chatId, text, false, 0, keyboard, "Markdown");
	} catch (TgBot::TgException &e) {
		std::cout <<"Error while sending message: "<< e.what() << std::endl << "To user " << chatId << std::endl;
		if (e.what() ==  "Forbidden: bot was blocked by the user") {
			db->deleteBlockerUser(chatId);
		}
	}
}

void StateDispatcher::sendPhoto(TgBot::Bot *bot, int chatId, std::string image, std::string text) {
	try {
	bot->getApi().sendPhoto(chatId, image, text);
	} catch (TgBot::TgException &e) {
		if (e.what() ==  "Forbidden: bot was blocked by the user") {
			db->deleteBlockerUser(chatId);
		}
	}
}


///STATE SWITCHER
void StateDispatcher::setState(TgBot::Bot *bot, TgBot::Chat::Ptr chat, int newState) {
	// AGE EDIT
	if (newState == DbDispatcher::IND_E_AGE_EDIT) {
		int userAge = db->getAge(chat->id);
		if (userAge != -1) {
			BotState reply (DbDispatcher::IND_E_AGE_EDIT,
							"Сколько тебе лет?",
			std::vector<BotState::StateButton>({{"Оставить текущий возраст: " + std::to_string(userAge), DbDispatcher::IND_E_USER_SEX_EDIT},}));
			sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());

		} else {
			sendMessage(bot, chat->id, "Сколько тебе лет?");
		}
		db->setState(chat->id, newState);
		return;
	}



	// GENDER RESPONSE
	// female = 1 male = 0
	if (newState == DbDispatcher::IND_GENDER_MALE || newState == DbDispatcher::IND_GENDER_FEMALE) {
		db->setGender(chat->id, newState == DbDispatcher::IND_GENDER_FEMALE ? 1 : 0);
		setState(bot, chat, DbDispatcher::IND_E_TARGET_SEX_EDIT);
		return;
	}



	// TARGET GENDER RESPONSE
	// female = 1 male = 0 doesn't matter = 2
	if (newState == DbDispatcher::IND_TARGET_GENDER_MALE ||
		newState == DbDispatcher::IND_TARGET_GENDER_FEMALE ||
		newState == DbDispatcher::IND_TARGET_GENDER_NOMATT) {
			db->setTargetGender(chat->id, newState == DbDispatcher::IND_TARGET_GENDER_FEMALE ? 1 :
									newState == DbDispatcher::IND_TARGET_GENDER_MALE ? 0 : 2);
			setState(bot, chat, DbDispatcher::IND_E_AIM);
			return;
	}


	// aim of using - friendship or relationships
	if (newState == DbDispatcher::IND_E_AIM) {
		BotState reply (DbDispatcher::IND_E_AIM,
						"Хочешь найти человека для отношений или просто для общения?",
		std::vector<BotState::StateButton>({{"Для отношений", DbDispatcher::IND_E_AIM_RELAT},
											{"Для общения", DbDispatcher::IND_E_AIM_FRIEN}}));
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		db->setState(chat->id, newState);
		return;
	}
	if (newState == DbDispatcher::IND_E_AIM_FRIEN ||
		newState == DbDispatcher::IND_E_AIM_RELAT) {
		db->setAim(chat->id, newState == DbDispatcher::IND_E_AIM_RELAT ? 1 : 0);
		setState(bot, chat, DbDispatcher::IND_E_NAME_EDIT);
		return;
	}


	// NAME EDIT
	if (newState == DbDispatcher::IND_E_NAME_EDIT) {
		std::string userName = db->getName(chat->id);
		if (userName != "") {
			BotState reply (				DbDispatcher::IND_E_NAME_EDIT,
											"Как тебя зовут?",
					std::vector<BotState::StateButton>({{"Оставить текущее имя: " + userName, DbDispatcher::IND_E_LOCATION_EDIT},}));
				sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		} else {
			sendMessage(bot, chat->id, "Как тебя зовут?");
		}
			db->setState(chat->id, newState);
			return;
	}



	// LOCATION EDIT
	if (newState == DbDispatcher::IND_E_LOCATION_EDIT) {
		std::string userCity = db->getCity(chat->id);
		if (userCity != "") {
			BotState reply (DbDispatcher::IND_E_LOCATION_EDIT,
											"Где ты находишься? Если хочешь изменить местоположение, пришли сообщением свою геолокацию",
					std::vector<BotState::StateButton>({{"Оставить текущее местоположение: " + userCity, DbDispatcher::IND_E_DESCRIPTION_EDIT},}));
					sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		} else {
			sendMessage(bot, chat->id, "Пришли сообщением свою геолокацию");
		}
			db->setState(chat->id, newState);
			return;
	}
	// LOCATION CONFIRMATION
	if (newState == DbDispatcher::IND_E_CONFIRMATION_LOCATION) {
		BotState reply (DbDispatcher::IND_E_CONFIRMATION_LOCATION,
									"Ты находишься в "+ db->getCity(chat->id) + "?",
							std::vector<BotState::StateButton>({{"Да, всё верно", 				DbDispatcher::IND_E_CONFIRM_LOCATION},
																{"Нет, попробовать ещё раз",	DbDispatcher::IND_E_DECLINE_LOCATION}}));
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		db->setState(chat->id, newState);
		return;
	}
	if (newState == DbDispatcher::IND_E_CONFIRM_LOCATION) {
			setState(bot, chat, DbDispatcher::IND_E_DESCRIPTION_EDIT);
			return;
	}
	if (newState == DbDispatcher::IND_E_DECLINE_LOCATION) {
			setState(bot, chat, DbDispatcher::IND_E_LOCATION_EDIT);
			return;
	}



	// DESCRIPTION EDIT
	if (newState == DbDispatcher::IND_E_DESCRIPTION_EDIT) {
		if (db->isDescriptionPresent(chat->id)) {
			BotState reply (				DbDispatcher::IND_E_DESCRIPTION_EDIT,
														"Расскажи пару слов о себе. Твоё описание будет появляться у других пользователей при просмотре твоей анкеты,"
														" так что постарайся их заинтересовать",
								std::vector<BotState::StateButton>({{"Оставить текущее описание", DbDispatcher::IND_E_IMAGE_EDIT},
																	{"Пропустить",		  DbDispatcher::IND_SKIP_DESCRIPTION}}));
				sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		} else {
			BotState reply (				DbDispatcher::IND_E_DESCRIPTION_EDIT,
														"Расскажи пару слов о себе. Твоё описание будет появляться у других пользователей при просмотре твоей анкеты,"
														" так что постарайся их заинтересовать",
								std::vector<BotState::StateButton>({{"Пропустить",		  DbDispatcher::IND_SKIP_DESCRIPTION}}));
				sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		}
			db->setState(chat->id, newState);
			return;
	}

	// DESCRIPTION EDIT SHORT
	if (newState == DbDispatcher::IND_E_DESCRIPT_EDIT) {
		if (db->isDescriptionPresent(chat->id)) {
			BotState reply (				DbDispatcher::IND_E_DESCRIPT_EDIT,
														"Расскажи пару слов о себе. Твоё описание будет появляться у других пользователей при просмотре твоей анкеты,"
														" так что постарайся их заинтересовать",
								std::vector<BotState::StateButton>({{"Оставить текущее описание", DbDispatcher::IND_BLANC_EDIT},
																	{"Пропустить",		  DbDispatcher::IND_SKIP_DESCRIPT}}));
				sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		} else {
			BotState reply (				DbDispatcher::IND_E_DESCRIPT_EDIT,
														"Расскажи пару слов о себе. Твоё описание будет появляться у других пользователей при просмотре твоей анкеты,"
														" так что постарайся их заинтересовать",
								std::vector<BotState::StateButton>({{"Пропустить",		  DbDispatcher::IND_SKIP_DESCRIPT}}));
				sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		}
			db->setState(chat->id, newState);
			return;
	}

	// DESCRIPTION REPLY
	if (newState == DbDispatcher::IND_SKIP_DESCRIPTION) {
		db->setDescription(chat->id, "");
		setState(bot, chat, DbDispatcher::IND_E_IMAGE_EDIT);
		return;
	}
	if (newState == DbDispatcher::IND_SKIP_DESCRIPT) {
			db->setDescription(chat->id, "");
			setState(bot, chat, DbDispatcher::IND_BLANC_EDIT);
			return;
		}

	//FINAL CONFIRMATION
	if (newState == DbDispatcher::IND_E_FINAL_EDIT) {
		std::tuple<int, std::string, std::string> userInfo = db->getUserDescription(chat->id);
		if (std::get<1>(userInfo) == "") {
			sendMessage(bot, chat->id, std::get<2>(userInfo));
		} else {
			sendPhoto(bot, chat->id, std::get<1>(userInfo), std::get<2>(userInfo));
		}
		db->setState(chat->id, newState);
		//setState(bot, chat, newState);

		BotState reply (		DbDispatcher::IND_E_FINAL_EDIT,
											"Такой получилась твоя анкета. Как тебе?",
		std::vector<BotState::StateButton>({{"Круто, оставить", 			DbDispatcher::IND_MAIN_MENU},
											{"Не очень, попробовать ещё раз", DbDispatcher::IND_BLANC_EDIT},}));
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		return;
	}

	// DISABLE PROFILE
	if (newState == DbDispatcher::IND_DISABLE_ACCOUNT) {
		BotState reply (		DbDispatcher::IND_DISABLE_ACCOUNT,
												"Хочешь сделать анкету неактивной?"
												" Ты в любой момент сможешь вернуться",
			std::vector<BotState::StateButton>({{"Да", 								DbDispatcher::IND_LEFT_STATE},
												{"Вернуться к просмотру анкет", 	DbDispatcher::IND_MAIN_MENU},}));
		db->setState(chat->id, newState);
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		return;
	}
	if (newState == DbDispatcher::IND_LEFT_STATE) {
		BotState reply (	DbDispatcher::IND_LEFT_STATE,
													"Новые люди всё ещё ждут знакомства...",
				std::vector<BotState::StateButton>({{"Вернуться к просмотру анкет", 	DbDispatcher::IND_MAIN_MENU},}));
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		db->setState(chat->id, newState);
		return;
	}

	//INVITE FRIENDS
	if (newState == DbDispatcher::IND_INVITE) {
		sendMessage(bot, chat->id, "Расскажи друзьям о боте с помощью ссылки:\n@" + bot->getApi().getMe()->username +
				"\nЧем больше людей узнает о боте, тем интереснее будет искать новых знакомых");
		setState(bot, chat, DbDispatcher::IND_MAIN_MENU);
		return;
	}

	// REMOVE WATCHED PROFILES LIST FOR USER
	if (newState == DbDispatcher::IND_RESET_WATCHED) {
		db->resetWatched(chat->id);
		sendMessage(bot, chat->id, "Хорошо, начнём просмотр анкет сначала)");
		setState(bot, chat, DbDispatcher::IND_FLOW_MENU);
		return;
	}

	///////////////////////////////////////
	///////////////////////////////////////
	/////////////// FLOW //////////////////
	///////////////////////////////////////
	///////////////////////////////////////
	if (newState == DbDispatcher::IND_FLOW_MENU) {
		db->addAdTick(chat->id);
		if (db->isAdTick(chat->id)) {
			BotState reply (		DbDispatcher::IND_FLOW_MENU,
																db->getAd(chat->id),
							std::vector<BotState::StateButton>({{"Вернуться к просмотру анкет", 	DbDispatcher::IND_FLOW_MENU},}));
			sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
			//db->setState(chat->id, newState);
			return;
		}

		int targetUserId = db->matchPartner(chat->id);

		// sex, target_sex or aim is null
		if (targetUserId == -1) {
			sendMessage(bot, chat->id, "Твоя анкета заполнена не полностью! Заверши заполнение, чтобы начать знакомиться");
			setState(bot, chat, DbDispatcher::IND_MAIN_MENU);
			return;
		}

		sendMessage(bot, chat->id, emo_processing + emo_processing + emo_processing);

		if (targetUserId == -2) {
			BotState reply (DbDispatcher::IND_ENDED_BLANCS,
					"Не могу больше никого найти...\n"
					"Попробуй поменять условия поиска или рассказать о боте друзьям)\n"
					"Также, ты можешь начать просмотр анкет сначала:)",
				std::vector<BotState::StateButton>({{"Изменить мою анкету", 				DbDispatcher::IND_E_AGE_EDIT},
													{"Пригласить новых пользователей", 		DbDispatcher::IND_INVITE},
													{"Смотреть анкеты сначала", 			DbDispatcher::IND_RESET_WATCHED}}));
			sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
			db->setState(chat->id, DbDispatcher::IND_ENDED_BLANCS);
			return;
		}

		// send target blanc
		std::tuple<int, std::string, std::string> userInfo = db->getUserDescription(targetUserId);
		if (std::get<1>(userInfo) != "") {
			sendPhoto(bot, chat->id, std::get<1>(userInfo));
		}
		FlowBotState reply(std::get<2>(userInfo), std::get<0>(userInfo));
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());

		db->setState(chat->id, newState);
		return;
	}

	//////////////////////////////////////
	////////////////// LIKER
	//////////////////////////////////////
	if (newState == DbDispatcher::IND_SHOW_LIKER) {
		int likersCnt = db->getLikerCount(chat->id);
		if (likersCnt == 0) {
			setState(bot, chat, DbDispatcher::IND_FLOW_MENU);
			return;
		}
		BotState reply(DbDispatcher::IND_SHOW_LIKER,
						std::to_string(likersCnt) + " человек(а) хотят с тобой познакомиться! Хочешь посмотреть их анкеты?",
						std::vector<BotState::StateButton>({{"Конечно", 	DbDispatcher::IND_TO_LIKERS},
															{"Не сейчас",		DbDispatcher::IND_FLOW_MENU},}));
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		db->setState(chat->id, newState);
		return;
	}

	if (newState == DbDispatcher::IND_TO_LIKERS) {
		std::vector<int> likers = db->getLikers(chat->id);
		if (likers.size() == 0) {
			setState(bot, chat, DbDispatcher::IND_FLOW_MENU);
			return;
		}

		std::tuple<int, std::string, std::string> userInfo = db->getUserDescription(likers[0]);
		if (std::get<1>(userInfo) != "") {
			sendPhoto(bot, chat->id, std::get<1>(userInfo));
		}
		LikerBotState reply(std::get<2>(userInfo), std::get<0>(userInfo), DbDispatcher::IND_LIKERS);
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		db->setState(chat->id, newState);
		return;
	}


//	//DIRECT MESSAGE
//	if (newState == DbDispatcher::IND_DIRECT_MSG) {
//		BotState reply(DbDispatcher::IND_DIRECT_MSG,
//				"Write your message to user, or click \"Back\" to return",
//				std::vector<BotState::StateButton>({{"Back", DbDispatcher::IND_FLOW_MENU}}));
//		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
//		db->setState(chat->id, newState);
//		return;
//	}



	//////////////
	// ADMIN BLOCK
	if (newState == DbDispatcher::ADM_MAIN_MENU) {
		BotState reply (DbDispatcher::ADM_MAIN_MENU,
							"Помни, большая сила - это большая ответственность.\nКакие будут администраторские распоряжения, хозяин?",
				std::vector<BotState::StateButton>({{"Сообщение всем", 	DbDispatcher::ADM_COMMON_MESSAGE},
													{"Обратно к смертным", 	DbDispatcher::IND_MAIN_MENU},}));
		sendMessage(bot, chat->id, reply.getDescription(), reply.getColumnInlineKeyboard());
		db->setState(chat->id, newState);
		return;
	}
	if (newState == DbDispatcher::ADM_COMMON_MESSAGE) {
		sendMessage(bot, chat->id, "Введите сообщение, которое появится у всех, властелин");
		db->setState(chat->id, newState);
		return;
	}



	// OTHERS
	for (BotState nowState : stateMachine) {
		if (nowState.getLabel() == newState) {
			sendMessage(bot, chat->id, nowState.getDescription(), nowState.getColumnInlineKeyboard());
			db->setState(chat->id, newState);
			return;
		}
	}

	BotState defaultState(		DbDispatcher::IND_MAIN_MENU,
										"Главное меню. Выберите действие",
std::vector<BotState::StateButton>({{"К анкетам", 	DbDispatcher::IND_SHOW_LIKER},
									{"Изменить анкету", 	DbDispatcher::IND_BLANC_EDIT},
									{"Выйти", 				DbDispatcher::IND_DISABLE_ACCOUNT},
									{"Пригласить друзей", 	DbDispatcher::IND_INVITE},}));
	sendMessage(bot, chat->id, defaultState.getDescription(), defaultState.getColumnInlineKeyboard());
	db->setState(chat->id, 0);
}



void StateDispatcher::addToWatchers (int subjectChatId, int targetChatId) {
	db->addToWatched(subjectChatId, targetChatId);
}

void StateDispatcher::deleteLiker (int subjectChatId, int targetChatId) {
	db->removeLiker(subjectChatId, targetChatId);
	//std::cout << "removing..." << subjectChatId << " " << targetChatId << std::endl;
}

bool StateDispatcher::checkUserRegisrty(TgBot::Chat::Ptr chat) {
	if (db->getState(chat->id) == DbDispatcher::IND_NO_AUTH) {
		return false;
	}
	return true;
}

void StateDispatcher::createNewUser(TgBot::Bot *bot, TgBot::Chat::Ptr chat) {
	db->addUser(chat->id, chat->username);
	setState(bot, chat, DbDispatcher::IND_E_AGE_EDIT);
}

void StateDispatcher::restoreSession(TgBot::Bot *bot, TgBot::Chat::Ptr chat) {
	setState(bot, chat, db->getState(chat->id));
}

void StateDispatcher::processText(TgBot::Bot *bot, TgBot::Message::Ptr message) {
	int currentState = db->getState(message->chat->id);

	if (currentState == DbDispatcher::IND_E_AGE_EDIT) {
		try {
			int age = std::stoi(message->text);
			if (age < 12 || age > 99) {
				//bot->getApi().sendMessage(message->chat->id, "Enter your real age!");
				sendMessage(bot, message->chat->id, "Введи свой настоящий возраст!");
				setState(bot, message->chat, currentState);
				return;
			}
			db->setAge(message->chat->id, age);
			setState(bot, message->chat, DbDispatcher::IND_E_USER_SEX_EDIT);
			return;
		} catch (std::exception& e) {
			//bot->getApi().sendMessage(message->chat->id, "Only digits!");
			sendMessage(bot, message->chat->id, "Только цифры!");
			setState(bot, message->chat, currentState);
			return;
		}
	}

	if (currentState == DbDispatcher::IND_E_NAME_EDIT) {
		db->setName(message->chat->id, message->text);
		setState(bot, message->chat, DbDispatcher::IND_E_LOCATION_EDIT);
		return;
	}

	if (currentState == DbDispatcher::IND_E_DESCRIPTION_EDIT) {
		db->setDescription(message->chat->id, message->text);
		setState(bot, message->chat, DbDispatcher::IND_E_IMAGE_EDIT);
		return;
	}

	if (currentState == DbDispatcher::IND_E_DESCRIPT_EDIT) {
		db->setDescription(message->chat->id, message->text);
		setState(bot, message->chat, DbDispatcher::IND_BLANC_EDIT);
		return;
	}

	if (currentState == DbDispatcher::ADM_LOGIN) {
		if (db->validateAdminPwd(message->text)) {
			setState(bot, message->chat, DbDispatcher::ADM_MAIN_MENU);
		} else {
			//bot->getApi().sendMessage(message->chat->id, "Wrong password");
			sendMessage(bot, message->chat->id, "Неправильный пароль");
			setState(bot, message->chat, DbDispatcher::IND_MAIN_MENU);
		}
		return;
	}
	if (currentState == DbDispatcher::ADM_COMMON_MESSAGE) {
		sendCommonMessage(bot, message->text);
		setState(bot, message->chat, DbDispatcher::ADM_MAIN_MENU);
		return;
	}

	//bot->getApi().sendMessage(message->chat->id, "There is no such answer");
	sendMessage(bot, message->chat->id, "Нет такого варианта ответа");
	setState(bot, message->chat, currentState);
}

void StateDispatcher::processLocation(TgBot::Bot *bot, TgBot::Message::Ptr message) {
	if (db->getState(message->chat->id) == DbDispatcher::IND_E_LOCATION_EDIT) {
		//std::cout << message->location->latitude << message->location->longitude << std::endl;
		db->setLocation(message->chat->id, message->location->latitude, message->location->longitude);
		setState(bot, message->chat, DbDispatcher::IND_E_CONFIRMATION_LOCATION);
		return;
	}
	//bot->getApi().sendMessage(message->chat->id, "There is no such answer");
	sendMessage(bot, message->chat->id, "Нет такого варианта ответа");
	setState(bot, message->chat, db->getState(message->chat->id));
}

void StateDispatcher::processImage(TgBot::Bot *bot, TgBot::Message::Ptr message) {

	int currentState = db->getState(message->chat->id);
	if (currentState == DbDispatcher::IND_E_IMAGE_EDIT) {
		db->setImage(message->chat->id, message->photo[0]->fileId);
		setState(bot, message->chat, DbDispatcher::IND_E_FINAL_EDIT);
		return;
	}

	//bot->getApi().sendMessage(message->chat->id, "There is no such answer");
	sendMessage(bot, message->chat->id, "Нет такого варианта ответа");
	setState(bot, message->chat, currentState);
}

void StateDispatcher::gotoAdminLogin (TgBot::Bot *bot, TgBot::Message::Ptr message) {
	//bot->getApi().sendMessage(message->chat->id, "enter administrator password");
	sendMessage(bot, message->chat->id, "Введите пароль администратора:");
	db->setState(message->chat->id, DbDispatcher::ADM_LOGIN);
	return;
}

void StateDispatcher::sendCommonMessage(TgBot::Bot *bot, std::string message) {
	std::vector<int> chatsIdList = db->getUsersChatIds();
	for (int currentId : chatsIdList) {
		try {
			//bot->getApi().sendMessage(currentId, message);
			sendMessage(bot, currentId, message);
		}
		catch (TgBot::TgException &e) {
			//throw;

		}
	}
	return;
}

void StateDispatcher::setLike(int subjectChatId, int targetChatId) {
	db->addLike(subjectChatId, targetChatId);
}

std::string StateDispatcher::getUname (int chatId) {
	return db->getUname(chatId);
}
