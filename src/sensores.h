// sensores.h - Funções responsáveis pela leitura dos sensores usados no sistema

#ifndef SENSORES_H
#define SENSORES_H

#include <Arduino.h> // Biblioteca padrão do Arduino
#include "config.h"  // Inclui definições de pinos e variáveis globais

bool buzzerAtivo = false;         // Variável para controlar o estado do buzzer
unsigned long tempoUltimoBip = 0; // Armazena o tempo do último bip
int contadorBips = 0;             // Contador de bips emitidos
const int numBips = 4;            // Número total de bips a serem emitidos
const int duracaoBip = 500;       // Duração de cada bip em milissegundos
bool gasAlarmeAtivo = false;      // Variável para controlar o estado do alarme de gás
unsigned long tempoUltimoTom = 0; // Armazena o tempo do último tom do alarme
int etapaTom = 0;                 // Variável para controlar a etapa do tom do alarme

// Função para verificar presença de gás e acionar alarme
void verificarGas()
{
    int valorGas = digitalRead(GasD); // Lê o valor do sensor de gás

    if (valorGas == 0 && !gasAlarmeAtivo)
    {
        //Serial.println("Gás: ALERTA! Vazamento detectado!");
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
        bool novaPresenca = digitalRead(PIR_SENSOR); // Lê o valor do sensor PIR
        if (novaPresenca && !presencaDetectada)
        {
            //Serial.println("PRESENÇA DETECTADA!");
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
                noTone(Buzzer); // Desliga

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
    int valorLDR = analogRead(LDR);     // Lê o valor do LDR
    bool ligarFachada = valorLDR < 300; // valor de corte ajustável
    digitalWrite(luzFachada, ligarFachada ? HIGH : LOW);
}

#endif
// Fim do arquivo sensores.h