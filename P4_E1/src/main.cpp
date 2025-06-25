#include <Arduino.h>

// Declaración previa de la función para evitar errores de compilación
void anotherTask(void *parameter);

void setup() 
{ 
    Serial.begin(115200);  // Corrección de velocidad de baudios a un valor común
    /* Creamos una nueva tarea aquí */ 
    xTaskCreate(
        anotherTask,     // Función de la tarea
        "another Task",  // Nombre de la tarea
        10000,           // Tamaño de la pila de la tarea en bytes
        NULL,            // Parámetro de la tarea (en este caso, ninguno)
        1,               // Prioridad de la tarea
        NULL             // Manejador de la tarea creada (no utilizado aquí)
    );
}

void loop() 
{ 
    Serial.println("this is ESP32 Task");  // Imprime un mensaje por serial
    delay(1000);  // Espera 1 segundo
} 

void anotherTask(void *parameter) 
{ 
    /* Bucle infinito */ 
    for(;;) 
    { 
        Serial.println("this is another Task");  // Imprime un mensaje por serial
        vTaskDelay(pdMS_TO_TICKS(1000));  // Espera 1 segundo de manera eficiente
    } 
    
    /* Nunca se llegará a este punto porque el bucle es infinito */
    vTaskDelete(NULL);  // Elimina la tarea (nunca se ejecuta debido al bucle infinito)
}
