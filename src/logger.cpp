#include "logger.h"

namespace {

	const char* logFilePath = "/bleskomat.log";
	const unsigned int logFileMaxBytes = 32768;
	const uint8_t maxLogFileNum = 5;
	unsigned long lastLogFileSizeCheckTime = millis();
	const unsigned int logFileSizeCheckDelay = 5000;
	const char* defaultLogLevel = "info";
	uint8_t configuredLogLevelValue = 2;
	unsigned long lastHeapSizePrintTime = 0;
	const unsigned int heapSizePrintDelay = 5000;

	const std::map<const char*, const uint8_t, util::MapCharPointerComparator> logLevelValues = {
		{ "trace", 0 },
		{ "debug", 1 },
		{ "info", 2 },
		{ "warn", 3 },
		{ "error", 4 },
		{ "critical", 5 },
		{ "none", 6 }
	};

	void handleOverSizedLogFile() {
		if (!spiffs::isInitialized()) return;
		File file = LittleFS.open(logFilePath, FILE_READ);
		if (file) {
			const auto logFileSize = file.size();
			file.close();
			if (logFileSize >= logFileMaxBytes) {
				for (int num = maxLogFileNum; num >= 0; num--) {
					const std::string oldFilePath = logger::getLogFilePath(num);
					if (spiffs::fileExists(oldFilePath.c_str())) {
						if (num >= maxLogFileNum) {
							spiffs::deleteFile(oldFilePath.c_str());
						} else {
							const std::string newLogFilePath = logger::getLogFilePath(num + 1);
							spiffs::renameFile(oldFilePath.c_str(), newLogFilePath.c_str());
						}
					}
				}
			}
		}
	}

	uint8_t getLogLevelValue(const char* logLevel) {
		if (logLevelValues.count(logLevel) > 0) {
			return logLevelValues.at(logLevel);
		}
		return 6;
	}

	void writeToSerialMonitor(const std::string &msg) {
		std::cout << msg;
	}

	void writeToLogFile(const std::string &msg) {
		spiffs::appendFile(logFilePath, msg.c_str());
	}
}

namespace logger {

	void init() {
		configuredLogLevelValue = getLogLevelValue(config::getString("logLevel").c_str());
	}

	void loop() {
		if (configuredLogLevelValue <= 1) {
			if (lastHeapSizePrintTime == 0 || millis() - lastHeapSizePrintTime > heapSizePrintDelay) {
				heap_caps_print_heap_info(MALLOC_CAP_DEFAULT);
				lastHeapSizePrintTime = millis();
			}
		}
		if (millis() - lastLogFileSizeCheckTime > logFileSizeCheckDelay) {
			handleOverSizedLogFile();
			lastLogFileSizeCheckTime = millis();
		}
	}

	void write(const std::string &msg, const char* logLevel) {
		logger::write(msg.c_str(), logLevel);
	}

	void write(const std::string &msg) {
		logger::write(msg.c_str(), defaultLogLevel);
	}

	void write(const char* msg) {
		logger::write(msg, defaultLogLevel);
	}

	void write(const char* t_msg, const char* logLevel) {
		if (getLogLevelValue(logLevel) >= configuredLogLevelValue) {
			const std::string msg = "[" + std::string(logLevel) + "] " + std::string(t_msg) + "\n";
			writeToSerialMonitor(msg);
			writeToLogFile(msg);
		}
	}

	std::string getLogFilePath(const uint8_t &num) {
		std::string filePath = std::string(logFilePath);
		if (num > 0) {
			filePath += "." + std::to_string(num);
		}
		return filePath;
	}
}
