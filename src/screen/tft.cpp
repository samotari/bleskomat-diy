#include "screen/tft.h"

// To generate PNG byte array, use this tool: https://notisrac.github.io/FileToCArray/
#include "PNGdec.h"
#include "screen/data/welcome0.h"
#include "screen/data/welcome1.h"
#include "screen/data/insertFiat.h"
#include "screen/data/tradeComplete.h"

namespace {
	TFT_eSPI display = TFT_eSPI();

	const auto bg_color = TFT_WHITE;
	const auto text_color = TFT_BLACK;
	const uint8_t text_font = 2;
//	const uint8_t text_font = 4; //hobomat add 	-D LOAD_FONT4 to [tft_module] build_flags = 
	const uint8_t text_size = 1;
	const uint8_t margin_x = 4;
	const uint8_t margin_y = 12;
//	const uint8_t margin_y = 9; // hobomat
	std::string current_screen = "";

	PNG png; // PNG decoder instance
	const int8_t MAX_IMAGE_WDITH = 240;
	uint8_t current_welcome = 99;
	
	struct BoundingBox {
		int16_t x = 0;
		int16_t y = 0;
		uint16_t w = 0;
		uint16_t h = 0;
	};

	BoundingBox amount_text_bbox;

	std::string getAmountFiatCurrencyString(const float &amount) {
		return util::floatToStringWithPrecision(amount, config::getUnsignedShort("fiatPrecision")) + " " + config::getString("fiatCurrency");
	}

	void pngDraw(PNGDRAW *pDraw) {
		uint16_t lineBuffer[240];
		png.getLineAsRGB565(pDraw, lineBuffer, PNG_RGB565_BIG_ENDIAN, 0xffffffff);
		display.pushImage(0, 0 + pDraw->y, pDraw->iWidth, 1, lineBuffer);
	}

	BoundingBox renderText(
		const std::string &t_text,
		const int16_t &x,
		const int16_t &y,
		const bool &center = true
	) {
		const char* text = t_text.c_str();
		display.setTextColor(text_color);
		display.setTextFont(text_font);
		display.setTextSize(text_size);
		int16_t tbw = display.textWidth(text);
		int16_t tbh = display.fontHeight(); // no need to multiply by text_size because TFT_eSPI does this automatically after setTextSize() is called
		int16_t box_x = x;
		int16_t box_y = y;
		if (center) {
			box_x -= (tbw / 2);
		}
		int16_t cursor_x = box_x;
		int16_t cursor_y = box_y;
		display.setCursor(cursor_x, cursor_y);
		display.println(text);
		BoundingBox bbox;
		bbox.x = box_x;
		bbox.y = box_y;
		bbox.w = tbw;
		bbox.h = tbh;
		return bbox;
	}

	BoundingBox renderQRCode(
		const std::string &t_data,
		const int16_t &x,
		const int16_t &y,
		const uint16_t &max_w,
		const uint16_t &max_h,
		const bool &center = true
	) {
		BoundingBox bbox;
		try {
			const char* data = t_data.c_str();
			uint8_t version = 1;
			while (version <= 40) {
				const uint16_t bufferSize = qrcode_getBufferSize(version);
				QRCode qrcode;
				uint8_t qrcodeData[bufferSize];
				const int8_t result = qrcode_initText(&qrcode, qrcodeData, version, ECC_LOW, data);
				if (result == 0) {
					// QR encoding successful.
					uint8_t scale = std::min(std::floor(max_w / qrcode.size), std::floor(max_h / qrcode.size));
					uint16_t w = qrcode.size * scale;
					uint16_t h = w;
					int16_t box_x = x;
					int16_t box_y = y;
					if (center) {
						box_x -= (w / 2);
					}
					display.fillRect(box_x, box_y, w, h, bg_color);
					for (uint8_t y = 0; y < qrcode.size; y++) {
						for (uint8_t x = 0; x < qrcode.size; x++) {
							auto color = qrcode_getModule(&qrcode, x, y) ? text_color: bg_color;
							display.fillRect(box_x + scale*x, box_y + scale*y, scale, scale, color);
						}
					}
					bbox.x = box_x;
					bbox.y = box_y;
					bbox.w = w;
					bbox.h = h;
					break;
				} else if (result == -2) {
					// Data was too long for the QR code version.
					version++;
				} else if (result == -1) {
					throw std::runtime_error("Render QR code failure: Unable to detect mode");
				} else {
					throw std::runtime_error("Render QR code failure: Unknown failure case");
				}
			}
		} catch (const std::exception &e) {
			std::cerr << e.what() << std::endl;
		}
		return bbox;
	}

	void clearScreen() {
		display.fillScreen(bg_color);
	}
}

namespace screen_tft {

	void init() {
		logger::write("Initializing TFT display...");
		display.begin();
		display.setRotation(config::getUnsignedShort("tftRotation"));
		clearScreen();
	}

	void showWelcomeScreen(uint8_t number) {
		Serial.print("Show welcome screen "); Serial.println(number);
		if (current_screen == "welcome" && current_welcome == number)
		{
			Serial.print("Skipping print");
			return;
		}

		int16_t rc;
		if (number == 0)
		{
			rc = png.openFLASH((uint8_t *)welcome0, sizeof(welcome0), pngDraw);
		}
		if (number == 1)
		{
			rc = png.openFLASH((uint8_t *)welcome1, sizeof(welcome1), pngDraw);
		}
		if (rc == PNG_SUCCESS) {
			display.startWrite();
			uint32_t dt = millis();
			rc = png.decode(NULL, 0);
			Serial.print(millis() - dt); Serial.println("ms");
			display.endWrite();
			png.close(); // not needed for memory->memory decode
		}

		current_screen = "welcome";
		current_welcome = number;
	}

	void showInsertFiatScreen(const float &amount) {
		if (current_screen == "insertFiat") {
			// Clear previous text by drawing a rectangle over it.
			display.fillRect(
				amount_text_bbox.x,
				amount_text_bbox.y,
				amount_text_bbox.w,
				amount_text_bbox.h,
				bg_color
			);
		} else {
			// Clear the whole screen.
			clearScreen();
			int16_t rc = png.openFLASH((uint8_t *)insertFiat, sizeof(insertFiat), pngDraw);
			if (rc == PNG_SUCCESS) {
				display.startWrite();
				uint32_t dt = millis();
				rc = png.decode(NULL, 0);
				display.endWrite();
				png.close(); // not needed for memory->memory decode
			}			
		}

		const std::string text = getAmountFiatCurrencyString(amount);
		const int16_t center_x = display.width() / 2;
		const int16_t text_x = center_x;
		const int16_t text_y = margin_y;
		amount_text_bbox = renderText(text, text_x, text_y, true/* center */);
		current_screen = "insertFiat";
	}

	void showTradeCompleteScreen(const float &amount, const std::string &qrcodeData) {
		int16_t rc = png.openFLASH((uint8_t *)tradeComplete, sizeof(tradeComplete), pngDraw);
		if (rc == PNG_SUCCESS)
		{
			display.startWrite();
			uint32_t dt = millis();
			rc = png.decode(NULL, 0);
			display.endWrite();
			png.close(); // not needed for memory->memory decode
		}
		const std::string text = getAmountFiatCurrencyString(amount);
		const int16_t center_x = display.width() / 2;
		const int16_t text_x = center_x;
		const int16_t text_y = margin_y;
		amount_text_bbox = renderText(text, text_x, text_y, true/* center */);
		const int16_t qr_x = center_x;
		const int16_t qr_y = amount_text_bbox.y + amount_text_bbox.h + margin_y;
//		const int16_t qr_y = amount_text_bbox.y + amount_text_bbox.h + margin_y / 2; // hobomat
		const int16_t qr_max_w = display.width();
		const int16_t qr_max_h = display.height() - (qr_y + margin_y);
		renderQRCode(qrcodeData, qr_x, qr_y, qr_max_w, qr_max_h, true/* center */);
		current_screen = "tradeComplete";
	}
}
