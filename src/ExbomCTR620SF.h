#ifndef EXBOM_CTR620SF_H
#define EXBOM_CTR620SF_H

#include <Arduino.h>
#include "esp_hidh_api.h"

class ExbomCTR620SF {
public:
    ExbomCTR620SF(const char* name = "CTR-620SF");
    bool begin();
    bool available();

    uint16_t getButtons();       // Bitmask dos botões
    int8_t getAxisX();
    int8_t getAxisY();
    int8_t getAxisZ();
    int8_t getAxisRz();

private:
    static void hidh_callback(esp_hidh_cb_event_t event, esp_hidh_cb_param_t *param);
    static void parseReport(const uint8_t* data, size_t len);

    static volatile bool _hasReport;
    static uint16_t _buttons;
    static int8_t _x, _y, _z, _rz;

    static const char* _targetName;
};

#endif
