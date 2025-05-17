// sensores.h - Funções responsáveis pela leitura dos sensores usados no sistema

#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h>
#include "config.h"

bool buzzerAtivo = false;
unsigned long tempoUltimoBip = 0;
int contadorBips = 0;
const int numBips = 4;
const int duracaoBip = 500;
bool gasAlarmeAtivo = false;
unsigned long tempoUltimoTom = 0;
int etapaTom = 0;


// Função para verificar presença de gás e acionar alarme
void verificarGas()
{
    int valorGas = digitalRead(GasD);

    if (valorGas == 0 && !gasAlarmeAtivo)
    {
        Serial.println("Gás: ALERTA! Vazamento detectado!");
        gasAlarmeAtivo = true;
        etapaTom = 0;
        tempoUltimoTom = millis();
        tone(Buzzer, 600); // Primeiro tom
    }
    else if (valorGas != 0)
    {
        // Sem vazamento: cancela alarme se estiver ativo
        if (gasAlarmeAtivo)
        {
            noTone(Buzzer);
            gasAlarmeAtivo = false;
        }
    }

    // Alterna tons do alarme de gás
    if (gasAlarmeAtivo)
    {
        unsigned long agora = millis();
        if (agora - tempoUltimoTom >= 1000)
        {
            tempoUltimoTom = agora;
            etapaTom = (etapaTom + 1) % 2;
            if (etapaTom == 0)
                tone(Buzzer, 600);
            else
                tone(Buzzer, 2112);
        }
    }
}


// Função para verificar o sensor PIR e emitir alerta sonoro em caso de presença
void verificarPresenca()
{
    if (sensorPresencaAtivo)
    {
        bool novaPresenca = digitalRead(PIR_SENSOR);
        if (novaPresenca && !presencaDetectada)
        {
            Serial.println("PRESENÇA DETECTADA!");
            // Inicia a sequência de bips
            buzzerAtivo = true;
            tempoUltimoBip = millis();
            contadorBips = 0;
            tone(Buzzer, 1500); // Inicia primeiro bip
        }
        presencaDetectada = novaPresenca;
    }
    else
    {
        presencaDetectada = false;
    }

    // Controle do buzzer baseado em millis
    if (buzzerAtivo)
    {
        unsigned long agora = millis();
        if (agora - tempoUltimoBip >= duracaoBip)
        {
            tempoUltimoBip = agora;
            contadorBips++;

            if (contadorBips % 2 == 0)
                tone(Buzzer, 1500); // Liga
            else
                noTone(Buzzer);     // Desliga

            if (contadorBips >= numBips * 2)
            {
                buzzerAtivo = false;
                noTone(Buzzer);
            }
        }
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