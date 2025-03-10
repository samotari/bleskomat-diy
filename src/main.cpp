#include "main.h"

unsigned int buttonDelay;

void setup() {
	Serial.begin(MONITOR_SPEED);
	spiffs::init();
	sdcard::init();
	config::init();
	logger::init();
	logger::write(firmwareName + ": Firmware version = " + firmwareVersion + ", commit hash = " + firmwareCommitHash);
	logger::write(config::getConfigurationsAsString());
	jsonRpc::init();
	screen::init();
	coinAcceptor::init();
	billAcceptor::init();
	button::init();
	buttonDelay = config::getUnsignedInt("buttonDelay");
}

void disinhibitAcceptors() {
	if (coinAcceptor::isInhibited()) {
		coinAcceptor::disinhibit();
	}
	if (billAcceptor::isInhibited()) {
		billAcceptor::disinhibit();
	}
}

void inhibitAcceptors() {
	if (!coinAcceptor::isInhibited()) {
		coinAcceptor::inhibit();
	}
	if (!billAcceptor::isInhibited()) {
		billAcceptor::inhibit();
	}
}

void resetAccumulatedValues() {
	coinAcceptor::resetAccumulatedValue();
	billAcceptor::resetAccumulatedValue();
}

float amountShown = 0;
unsigned long tradeCompleteTime = 0;
bool buttonReleased = true;

void writeTradeCompleteLog(const float &amount, const std::string &signedUrl) {
	std::string msg = "Trade completed:\n";
	msg += "  Amount  = " + util::floatToStringWithPrecision(amount, config::getUnsignedShort("fiatPrecision")) + " " + config::getString("fiatCurrency") + "\n";
	msg += "  URL     = " + signedUrl;
	logger::write(msg);
}

void runAppLoop() {
	coinAcceptor::loop();
	billAcceptor::loop();
	button::loop();
	const std::string currentScreen = screen::getCurrentScreen();
	float accumulatedValue = 0;
	accumulatedValue += coinAcceptor::getAccumulatedValue();
	accumulatedValue += billAcceptor::getAccumulatedValue();

	if (currentScreen == "") {
		disinhibitAcceptors();
		screen::showWelcomeScreen(0);
	} else if (
		accumulatedValue > 0 &&
		currentScreen != "insertFiat" &&
		currentScreen != "tradeComplete"
	) {
		screen::showInsertFiatScreen(accumulatedValue);
		amountShown = accumulatedValue;
	} else if (currentScreen == "welcome") {
		disinhibitAcceptors();
		if (button::isPressed() && buttonReleased) {
			buttonReleased = false;
			delay(100);
			screen::showWelcomeScreenNext();
		} else if (!button::isPressed()){
			buttonReleased = true;
		}
	} else if (currentScreen == "insertFiat") {
		disinhibitAcceptors();
		if (button::isPressed()) {
			if (accumulatedValue > 0) {
				// Button pushed while insert fiat screen shown and accumulated value greater than 0.
				// Create a withdraw request and render it as a QR code.
				const std::string signedUrl = util::createSignedLnurlWithdraw(accumulatedValue);
				const std::string encoded = util::lnurlEncode(signedUrl);
				std::string qrcodeData = "";
				// Allows upper or lower case URI schema prefix via a configuration option.
				// Some wallet apps might not support uppercase URI prefixes.
				qrcodeData += config::getString("uriSchemaPrefix");
				// QR codes with only uppercase letters are less complex (easier to scan).
				qrcodeData += util::toUpperCase(encoded);
				screen::showTradeCompleteScreen(accumulatedValue, qrcodeData);
				writeTradeCompleteLog(accumulatedValue, signedUrl);
				inhibitAcceptors();
				tradeCompleteTime = millis();
			}
		} else {
			// Button not pressed.
			// Ensure that the amount shown is correct.
			if (amountShown != accumulatedValue) {
				screen::showInsertFiatScreen(accumulatedValue);
				amountShown = accumulatedValue;
			}
		}
	} else if (currentScreen == "tradeComplete") {
		inhibitAcceptors();
		if (button::isPressed() && millis() - tradeCompleteTime > buttonDelay) {
			// Button pushed while showing the trade complete screen.
			// Reset accumulated values.
			resetAccumulatedValues();
			amountShown = 0;
			disinhibitAcceptors();
			screen::showWelcomeScreen(0);
			buttonReleased = false;
			delay(100);
			logger::write("Screen cleared");
		}
	}
}

void loop() {
	logger::loop();
	jsonRpc::loop();
	if (!jsonRpc::hasPinConflict() || !jsonRpc::inUse()) {
		runAppLoop();
	}
}
