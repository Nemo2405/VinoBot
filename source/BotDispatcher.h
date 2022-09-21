/*
 * BotDispatcher.h
 *
 *  Created on: Aug 16, 2022
 *      Author: eddyneshton
 */

#ifndef BOTDISPATCHER_H_
#define BOTDISPATCHER_H_

#include "StateDispatcher.h"
#include <typeinfo>

class BotDispatcher {
public:
	BotDispatcher();
	virtual ~BotDispatcher();
	void startBot();

private:
	TgBot::Bot * bot;


	StateDispatcher *stateDispatcher;
};

#endif /* BOTDISPATCHER_H_ */
