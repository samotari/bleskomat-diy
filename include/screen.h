#ifndef BLESKOMAT_SCREEN_H
#define BLESKOMAT_SCREEN_H

#include "config.h"
#include "logger.h"
#include "screen/tft.h"

namespace screen {
	void init();
	std::string getCurrentScreen();
	void showWelcomeScreen(uint8_t number);
	void showWelcomeScreenNext();
	void showInsertFiatScreen(const float &amount);
	void showTradeCompleteScreen(const float &amount, const std::string &qrcodeData);
}

#endif
