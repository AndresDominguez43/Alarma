#ifndef WebSPIFFS_h
#define WebSPIFFS_h

#include <SPIFFS.h>
#include "WebSocket.h"

void WebArchiveSPIFFS();
void saveValueToSPIFFS(const char* filename, String value);

#endif