#ifndef YARR_OTA_H
#define YARR_OTA_H

#include "utility.h"
#include "websocket.h"
#include <esp32FOTA.hpp>

extern esp32FOTA FOTA;
extern bool doOTAUpdate;
extern int ota_current_partition;
extern unsigned long ota_last_message;

void ota_setup();
void ota_loop();

void sendOTAProgressUpdate(float progress, int partition);
void sendOTAProgressFinished();

#endif /* !YARR_OTA_H */