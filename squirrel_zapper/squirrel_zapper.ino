#include <SPI.h>
#include "nRF24L01."
#include "RF24.h"

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

const int max_payload_size = 32;
char receive_payload[max_payload_size + 1];

RF24 radio(7, 8);

void setup() {
  Serial.begin(115200);

  radio.begin();
  radio.enableDynamicPayloads();
  radio.setRetries(15, 15);

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();

  radio.printDetails();
}

void loop() {
  if (radio.available()) {
    while (radio.available()) {
      uint8_t len = radio.getDynamicPayloadSize();

      if (!len) {
        Serial.println(F("No data in received payload."));
        continue;
      }

      radio.read(receive_payload, len);
      receive_payload[len] = 0;

      Serial.print(F("Payload size: "));
      Serial.println(len);
      Serial.print(F("Payload: "));
      Serial.println(receive_payload);

      radio.stopListening();

      radio.write(receive_payload, len);
      Serial.println(F("Acknowledgement sent."));

      radio.startListening();
    }
  }
}
