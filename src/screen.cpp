#include "screen.h"

namespace {
	std::string currentScreen = "";
	uint8_t currentWelcomeScreen = 0;
}

namespace screen {

	void init() {
		screen_tft::init();
	}

	std::string getCurrentScreen() {
		return currentScreen;
	}

	void showWelcomeScreen(uint8_t number) {
		screen_tft::showWelcomeScreen(number);
		currentScreen = "welcome";
		currentWelcomeScreen = number;
	}

	void showWelcomeScreenNext() {
		showWelcomeScreen((currentWelcomeScreen + 1) % 2);
	}

	void showInsertFiatScreen(const float &amount) {
		screen_tft::showInsertFiatScreen(amount);
		currentScreen = "insertFiat";
	}

	void showTradeCompleteScreen(const float &amount, const std::string &qrcodeData) {
		screen_tft::showTradeCompleteScreen(amount, qrcodeData);
		currentScreen = "tradeComplete";
	}
}
