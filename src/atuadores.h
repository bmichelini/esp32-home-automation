// atuadores.h - Funções relacionadas aos atuadores (servo motor)

#ifndef ATUADORES_H
#define ATUADORES_H

#include <Arduino.h> // Biblioteca padrão do Arduino
#include "config.h"  // Inclui definições de pinos e variáveis globais

// Função para mover o servo motor da garagem para uma posição específica (0 ou 90 graus)
void moverServo(int destino)
{
    motorgaragem.attach(15); // Conecta o servo ao pino 15

    // Define o sentido de movimentação
    int passo = destino > posicaoAtual ? 1 : -1;

    // Gira gradualmente o motor até a nova posição
    for (int pos = posicaoAtual; pos != destino; pos += passo)
    {
        motorgaragem.write(pos);
        delay(10); // Delay para suavizar o movimento
    }

    motorgaragem.write(destino); // Garante que chega na posição final
    posicaoAtual = destino;      // Atualiza posição atual

    delay(200);            // Tempo para o motor estabilizar
    motorgaragem.detach(); // Desconecta para evitar ruído ou consumo excessivo
}

#endif
// Fim do arquivo atuadores.h