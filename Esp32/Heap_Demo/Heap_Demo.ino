#include <Arduino.h>
#include "esp_heap_caps.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portable.h"
#include "freertos/FreeRTOSConfig.h"
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

static const uint8_t buf_len = 255;
static char *msg_ptr = NULL;
static volatile uint8_t msg_flag = 0;

void readSerial(void *parameters)
{
  char c;
  char buf[buf_len];
  uint8_t idx = 0;
  memset(buf, 0, buf_len);

  while (1)
  {
    if (Serial.available() > 0)
    {
      c = Serial.read();
      if (idx < buf_len - 1)
      {
        buf[idx++] = c;
      }

      if ((c == '\n' || c == '\r') && msg_flag == 0)
      {
        buf[idx - 1] = '\0';
        if (idx > 1)
        {
          size_t heap_before = xPortGetFreeHeapSize();// heap before allocation
          Serial.println(String("Initial Heap Present: ")+ heap_before);
          msg_ptr = (char *)pvPortMalloc(idx * sizeof(char));
          configASSERT(msg_ptr);
          memcpy(msg_ptr, buf, idx);
          msg_flag = 1;
          size_t heap_after = xPortGetFreeHeapSize();// heap after allocation

          Serial.print("Heap used by string allocation: ");
          Serial.println(heap_before - heap_after);
          
          Serial.print("Remaining Free Heap: ");
          Serial.println(heap_after);

          memset(buf, 0, buf_len);
          idx = 0;
        }
      }
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void printMessage(void *parameters)
{
  while (1)
  {
    if (msg_flag == 1)
    {
      Serial.println(msg_ptr);
      vPortFree(msg_ptr);
      msg_ptr = NULL;
      msg_flag = 0;
               
    }
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}

void setup()
{
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("\n----FreeRtos Heap Demo----");
  Serial.println("Enter a String");

  xTaskCreatePinnedToCore(readSerial, "Read Serial", 2048, NULL, 1, NULL, app_cpu);
  xTaskCreatePinnedToCore(printMessage, "Print Message", 2048, NULL, 1, NULL, app_cpu);

  vTaskDelete(NULL);
}

void loop() {

}
