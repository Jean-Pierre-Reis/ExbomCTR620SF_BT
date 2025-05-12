#include <ExbomCTR620SF.h>

ExbomCTR620SF gamepad("CTR-620SF");

void setup() {
    Serial.begin(115200);
    if (!gamepad.begin()) {
        Serial.println("Falha ao iniciar Bluetooth HID");
        while (true);
    }
}

void loop() {
    if (gamepad.available()) {
        Serial.print("Botões: ");
        Serial.print(gamepad.getButtons(), HEX);
        Serial.print(" | X: ");
        Serial.print(gamepad.getAxisX());
        Serial.print(" Y: ");
        Serial.print(gamepad.getAxisY());
        Serial.print(" Z: ");
        Serial.print(gamepad.getAxisZ());
        Serial.print(" Rz: ");
        Serial.println(gamepad.getAxisRz());
    }
    delay(30);
}
