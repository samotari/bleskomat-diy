#ifndef BLESKOMAT_SCREEN_TFT_H
#define BLESKOMAT_SCREEN_TFT_H

#include "logger.h"
#include "util.h"

#include <Arduino.h>
#include <qrcode.h>
#include <SPI.h>
#include <TFT_eSPI.h>

#include <cmath>
#include <stdexcept>
#include <string>

namespace screen_tft {
	void init();
	void showWelcomeScreen(uint8_t number);
	void showInsertFiatScreen(const float &amount);
	void showTradeCompleteScreen(const float &amount, const std::string &qrcodeData);
}

#endif
