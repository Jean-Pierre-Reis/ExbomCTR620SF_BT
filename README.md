# ExbomCTR620SF_BT
Biblioteca Arduino (compatível com ESP32) para conectar e ler entradas de um gamepad Exbom CTR-620SF via Bluetooth HID Classic.
O código encapsula as chamadas ao esp_hid_host do ESP-IDF, expondo uma API simples em C++ para quem usa o ecossistema Arduino/PlatformIO.

Em linhas gerais, a biblioteca:

  Inicializa o stack Bluetooth Dual-Mode (Classic + BLE) do ESP32 e o módulo HID Host via esp_bt_hid_host_init().
  Registra um callback que detecta, emparelha e conecta automaticamente ao CTR-620SF quando em modo pairing.
  Parseia os relatórios HID recebidos (botões e eixos) e os disponibiliza via métodos como readButtons() e readAxes().
  Inclui exemplos de uso tanto em Arduino IDE quanto em PlatformIO.

ExbomCTR620SF_BT/
├── library.properties
├── src/
│   ├── ExbomCTR620SF.h
│   ├── ExbomCTR620SF.cpp
│   └── HIDReportParser.h
└── examples/
    └── SimpleTest/
        └── SimpleTest.ino

