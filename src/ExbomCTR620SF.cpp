#include "ExbomCTR620SF.h"
#include <esp_bt_main.h>
#include <esp_bt_device.h>
#include <esp_bt.h>
#include <esp_gap_bt_api.h>

volatile bool ExbomCTR620SF::_hasReport = false;
uint16_t ExbomCTR620SF::_buttons = 0;
int8_t ExbomCTR620SF::_x = 0;
int8_t ExbomCTR620SF::_y = 0;
int8_t ExbomCTR620SF::_z = 0;
int8_t ExbomCTR620SF::_rz = 0;
const char* ExbomCTR620SF::_targetName = nullptr;

ExbomCTR620SF::ExbomCTR620SF(const char* name) {
    _targetName = name;
}

bool ExbomCTR620SF::begin() {
    if (esp_bt_controller_get_status() != ESP_BT_CONTROLLER_STATUS_ENABLED) {
        esp_bt_controller_config_t cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
        if (esp_bt_controller_init(&cfg) != ESP_OK) return false;
        if (esp_bt_controller_enable(ESP_BT_MODE_BTDM) != ESP_OK) return false;
    }

    if (esp_bluedroid_get_status() != ESP_BLUEDROID_STATUS_ENABLED) {
        if (esp_bluedroid_init() != ESP_OK) return false;
        if (esp_bluedroid_enable() != ESP_OK) return false;
    }

    if (esp_hidh_init() != ESP_OK) return false;
    if (esp_hidh_register_callbacks(hidh_callback) != ESP_OK) return false;

    Serial.println("[INFO] Bluetooth HID Host iniciado.");
    return true;
}

bool ExbomCTR620SF::available() {
    return _hasReport;
}

uint16_t ExbomCTR620SF::getButtons() { _hasReport = false; return _buttons; }
int8_t ExbomCTR620SF::getAxisX() { return _x; }
int8_t ExbomCTR620SF::getAxisY() { return _y; }
int8_t ExbomCTR620SF::getAxisZ() { return _z; }
int8_t ExbomCTR620SF::getAxisRz() { return _rz; }

void ExbomCTR620SF::hidh_callback(esp_hidh_cb_event_t event, esp_hidh_cb_param_t *param) {
    switch (event) {
        case ESP_HIDH_INIT_EVT:
            Serial.println("[INFO] Escaneando por dispositivos...");
            esp_bt_gap_start_discovery(ESP_BT_INQ_MODE_GENERAL_INQUIRY, 10, 0);
            break;
        case ESP_HIDH_DISC_EVT:
            if (param->disc_res == nullptr) return;

            char name[ESP_BT_GAP_MAX_BDNAME_LEN + 1] = {0};
            if (esp_bt_gap_get_remote_name(param->disc_res->bd_addr, name, sizeof(name)) == ESP_OK) {
                if (_targetName && strstr(name, _targetName)) {
                    Serial.printf("[INFO] Encontrado %s, conectando...\n", name);
                    esp_hidh_dev_connect(param->disc_res->bd_addr);
                }
            }
            break;
        case ESP_HIDH_CONN_EVT:
            Serial.println("[INFO] Dispositivo conectado!");
            break;
        case ESP_HIDH_INPUT_EVT:
            parseReport(param->input.data, param->input.len);
            break;
        default:
            break;
    }
}

void ExbomCTR620SF::parseReport(const uint8_t* data, size_t len) {
    if (len < 7) return;
    // data[0] = Report ID
    _buttons = data[1] | (data[2] << 8);
    _x = (int8_t)data[3];
    _y = (int8_t)data[4];
    _z = (int8_t)data[5];
    _rz = (int8_t)data[6];
    _hasReport = true;
}
