#include <Arduino.h>

// Definir el pin del LED
#define LED_PIN 2  // Cambia este número según tu ESP32

// Declarar el semáforo
SemaphoreHandle_t semaforoEncender;
SemaphoreHandle_t semaforoApagar;

void tareaEncenderLED(void *parameter);
void tareaApagarLED(void *parameter);

void setup() {
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);

    // Crear los semáforos binarios
    semaforoEncender = xSemaphoreCreateBinary();
    semaforoApagar = xSemaphoreCreateBinary();

    if (semaforoEncender != NULL && semaforoApagar != NULL) {
        // Iniciar el proceso encendiendo primero
        xSemaphoreGive(semaforoEncender);

        // Crear las dos tareas
        xTaskCreate(tareaEncenderLED, "EncenderLED", 1000, NULL, 1, NULL);
        xTaskCreate(tareaApagarLED, "ApagarLED", 1000, NULL, 1, NULL);
    }
}

void loop() {
    // No se usa en este ejemplo, ya que FreeRTOS maneja las tareas
}

// Tarea para encender el LED
void tareaEncenderLED(void *parameter) {
    while (1) {
        if (xSemaphoreTake(semaforoEncender, portMAX_DELAY) == pdTRUE) {  
            digitalWrite(LED_PIN, HIGH);
            Serial.println("LED ENCENDIDO");
            vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo
            xSemaphoreGive(semaforoApagar);  // Libera el semáforo para la otra tarea
        }
    }
}

// Tarea para apagar el LED
void tareaApagarLED(void *parameter) {
    while (1) {
        if (xSemaphoreTake(semaforoApagar, portMAX_DELAY) == pdTRUE) {  
            digitalWrite(LED_PIN, LOW);
            Serial.println("LED APAGADO");
            vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo
            xSemaphoreGive(semaforoEncender);  // Libera el semáforo para la otra tarea
        }
    }
}
