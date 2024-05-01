#include <Arduino.h>

// Definición de pines del motor paso a paso
const int IN1 = D0;
const int IN2 = D1;
const int IN3 = D2;
const int IN4 = D3;

// Definición de secuencia de pasos
int seq[4][4] = {
  {1,0,0,1},
  {0,1,0,1},
  {0,0,1,1},
  {1,1,0,0}
};

// Definición de la velocidad del motor (pasos por segundo)
const int velocidad = 80;

// Variable para almacenar la posición actual del motor
int step = 0;

void setup() {
  // Configuración de pines como salida
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void loop() {
  // Ciclo para recorrer la secuencia de pasos
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      // Activar la bobina correspondiente
      digitalWrite(IN1, seq[i][0]);
      digitalWrite(IN2, seq[i][1]);
      digitalWrite(IN3, seq[i][2]);
      digitalWrite(IN4, seq[i][3]);

      // Retardo para la velocidad del motor
      delay(1000 / velocidad);

      // Cambio de estado para la siguiente bobina
      step++;
      if (step >= 16) {
        step = 0;
      }
    }
  }
}
