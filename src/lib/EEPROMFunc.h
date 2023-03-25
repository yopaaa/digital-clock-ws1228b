#ifndef EEPROM_FUNC
#define EEPROM_FUNC

void saveWifiCredentials(const char *ssid, const char *password);
void defaultState();
void readWifiCredentials();

#endif