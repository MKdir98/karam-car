#include <Arduino.h>
#include <Wire.h>
#include <driver/i2s.h>

// Define the pin for the microphone
#define MIC_PIN 34

// Define the pins for the DAC and amplifier
#define DAC_PIN 25

hw_timer_t *My_timer = NULL;

void IRAM_ATTR timer_isr() {
  // Read data from I2S
  int32_t buffer[64];
  size_t bytes_read;
  i2s_read(I2S_NUM_0, &buffer, 64 * sizeof(int32_t), &bytes_read, portMAX_DELAY);

  // Process the data (you can implement your own processing here)

  // Write data to DAC
  for (size_t i = 0; i < 64; i++) {
    dacWrite(DAC_PIN, buffer[i] >> 20);
  }
}
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);

  // Initialize I2S
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    .sample_rate = 44100,
    .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
    .communication_format = I2S_COMM_FORMAT_I2S_MSB,
    .intr_alloc_flags = 0,
    .dma_buf_count = 8,
    .dma_buf_len = 64
  };
  i2s_pin_config_t pin_config = {
    .bck_io_num = 26,
    .ws_io_num = 22,
    .data_out_num = -1,
    .data_in_num = MIC_PIN
  };
  i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
  i2s_set_pin(I2S_NUM_0, &pin_config);

  // Configure DAC
  dacWrite(DAC_PIN, 0);  // Set initial value

  // Set up Timer
  My_timer = timerBegin(0, 80, true); // Timer 0, div 80
  timerAttachInterrupt(My_timer, &timer_isr, true);
  timerAlarmWrite(timerBegin(0, 80, true), 62500, true);
  timerAlarmEnable(timerBegin(0, 80, true));
}

void loop() {
  // Main loop
}


