#ifndef BLESKOMAT_SDCARD_H
#define BLESKOMAT_SDCARD_H

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <iostream>
#include <string>

namespace sdcard {
	void init();
	bool isMounted();
}

#endif
