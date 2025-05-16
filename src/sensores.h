// sensores.h - Funções responsáveis pela leitura dos sensores usados no sistema

#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h>
#include "config.h"

// Função para verificar presença de gás e acionar alarme
void verificarGas()
{
    int valorGas = digitalRead(GasD);
    if (valorGas == 0)
    {
        Serial.println("Gás: ALERTA! Vazamento detectado!");
        tone(Buzzer, 600); // Emite som com frequência de 600 Hz
        delay(1000);
        tone(Buzzer, 2112); // Emite som com frequência de 2112 Hz
        delay(1000);
        noTone(Buzzer);
    }
    else
    {
        Serial.println("Gás: OK");
        noTone(Buzzer);
    }
}

// Função para verificar o sensor PIR e emitir alerta sonoro em caso de presença
void verificarPresenca()
{
    if (sensorPresencaAtivo)
    {
        presencaDetectada = digitalRead(PIR_SENSOR);
        if (presencaDetectada)
        {
            Serial.println("PRESENÇA DETECTADA!");
            for (int i = 0; i < 4; i++)
            {
                tone(Buzzer, 1500);
                delay(500);
            }
            noTone(Buzzer);
        }
        else
        {
            Serial.println("Sensor de Presença: Ativo (Sem presença)");
        }
    }
    else
    {
        presencaDetectada = false;
    }
}

// Função para controlar a luz da fachada com base na luminosidade do ambiente
void atualizarLuzFachada()
{
    int valorLDR = analogRead(LDR);
    bool ligarFachada = valorLDR < 300; // valor de corte ajustável
    digitalWrite(luzFachada, ligarFachada ? HIGH : LOW);
}

#endif
// Fim do arquivo sensores.h