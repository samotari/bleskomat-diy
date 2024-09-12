#ifndef BLESKOMAT_JSON_RPC_H
#define BLESKOMAT_JSON_RPC_H

#include "config.h"
#include "logger.h"
#include "main.h"
#include "spiffsb.h"

#include <ArduinoJson.h>
#include <string>

namespace jsonRpc {
	void init();
	void loop();
	bool inUse();
	bool hasPinConflict();
}

#endif
