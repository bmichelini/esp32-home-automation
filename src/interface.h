// interface.h - Manipulação da interface HTML e rotas do servidor

#ifndef INTERFACE_H
#define INTERFACE_H

#include <WiFi.h>
#include "config.h"
#include "atuadores.h"
#include "sensores.h"

// Função para ler a requisição HTTP enviada pelo navegador
String lerRequisicao(WiFiClient &client)
{
    String request = "";
    unsigned long timeout = millis() + 2000;

    while (client.connected() && millis() < timeout)
    {
        if (client.available())
        {
            char c = client.read();
            request += c;
            if (c == '\n' && request.endsWith("\r\n\r\n"))
                break;
        }
    }
    return request;
}

// Função para processar e responder às requisições do navegador
void processarRequisicao(WiFiClient &client, String request)
{
    // Ligar ou desligar luzes conforme solicitado
    for (int i = 0; i < numLuzes; i++)
    {
        if (request.indexOf("GET /ligar" + String(i)) >= 0)
        {
            estadosLuzes[i] = true;
            digitalWrite(pinosLuzes[i], HIGH);
            Serial.println("Luz " + String(nomesLuzes[i]) + " LIGADA.");
        }
        if (request.indexOf("GET /desligar" + String(i)) >= 0)
        {
            estadosLuzes[i] = false;
            digitalWrite(pinosLuzes[i], LOW);
            Serial.println("Luz " + String(nomesLuzes[i]) + " DESLIGADA.");
        }
    }

    // Controle do sensor de presença (PIR)
    if (request.indexOf("GET /ligarPIR") >= 0)
    {
        sensorPresencaAtivo = true;
        Serial.println("Sensor de presença ATIVADO");
    }
    if (request.indexOf("GET /desligarPIR") >= 0)
    {
        sensorPresencaAtivo = false;
        Serial.println("Sensor de presença DESATIVADO");
    }

    // Controle do portão da garagem
    if (request.indexOf("GET /abrir") >= 0)
    {
        Serial.println("Abrindo garagem...");
        moverServo(90);
    }
    else if (request.indexOf("GET /fechar") >= 0)
    {
        Serial.println("Fechando garagem...");
        moverServo(0);
    }

    // Respostas dinâmicas para sensores (usadas via JavaScript)
    if (request.indexOf("GET /status_gas") >= 0)
    {
        client.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n");
        client.println();
        client.println(digitalRead(GasD) == 0 ? "ALERTA! Vazamento de gás detectado" : "OK");
        client.flush();
        client.stop();
        return;
    }

    if (request.indexOf("GET /status_dht") >= 0)
    {
        float temperatura = dht.readTemperature();
        float umidade = dht.readHumidity();
        if (isnan(temperatura) || isnan(umidade))
        {
            temperatura = 0;
            umidade = 0;
            Serial.println("Falha ao ler do DHT11.");
        }
        client.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n");
        client.println();
        client.println(String(temperatura) + "°C / " + String(umidade) + "%");
        client.flush();
        client.stop();
        return;
    }

    if (request.indexOf("GET /status_pir") >= 0)
    {
        client.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n");
        client.println();
        if (!sensorPresencaAtivo)
            client.println("Desativado");
        else if (presencaDetectada)
            client.println("ALERTA! Presença detectada!");
        else
            client.println("Ativo");
        client.flush();
        client.stop();
        return;
    }

    if (request.indexOf("GET /status_fachada") >= 0)
    {
        int valorLDR = analogRead(LDR);
        client.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n");
        client.println();
        client.println(valorLDR < 300 ? "Ligada" : "Desligada");
        client.flush();
        client.stop();
        return;
    }

    // Envia a página HTML principal
    client.println("HTTP/1.1 200 OK\r\nContent-type:text/html\r\nConnection: close\r\n");
    client.println();
    client.print("<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'><title>Automação</title><style>");
    client.print("body{text-align:center;font-family:Arial;}button{padding:10px;margin:5px;width:120px;}.ligado{background:green;color:white;}.desligado{background:red;color:white;}</style></head><body>");
    client.print("<h1>CONTROLE RESIDENCIAL</h1><h2>Monitoramento de Ambiente</h2>");
    client.print("<p><b>Temperatura:</b> <span id='statusTemp'>Carregando...</span></p>");
    client.print("<p><b>Umidade:</b> <span id='statusUmid'>Carregando...</span></p><hr>");
    client.print("<h2>Sensor de Gás</h2><p>Status: <b id='statusGas'>Carregando...</b></p><hr>");
    client.print("<h2>Portão</h2><button onclick=\"location.href='/abrir'\">Abrir</button><button onclick=\"location.href='/fechar'\">Fechar</button>");
    client.print("<h2>Sensor de Presença</h2><p>Status: <b id='statusPIR'>Carregando...</b></p>");
    client.print("<a href='/ligarPIR'><button class='ligado'>Ligar</button></a>");
    client.print("<a href='/desligarPIR'><button class='desligado'>Desligar</button></a><hr>");
    client.print("<h2>Luz da Fachada</h2><p>Status: <b id='statusFachada'>Carregando...</b></p><hr>");

    client.print("<h2>Piso Inferior</h2>");
    for (int i = 0; i < 5; i++)
    {
        client.print("<h3>Luz da " + String(nomesLuzes[i]) + "</h3>");
        client.print("<p>Status: <b>" + String(estadosLuzes[i] ? "Ligada" : "Desligada") + "</b></p>");
        client.print("<a href='/ligar" + String(i) + "'><button class='ligado'>Ligar</button></a>");
        client.print("<a href='/desligar" + String(i) + "'><button class='desligado'>Desligar</button></a><hr>");
    }

    client.print("<h2>Piso Superior</h2>");
    for (int i = 5; i < 10; i++)
    {
        client.print("<h3>Luz da " + String(nomesLuzes[i]) + "</h3>");
        client.print("<p>Status: <b>" + String(estadosLuzes[i] ? "Ligada" : "Desligada") + "</b></p>");
        client.print("<a href='/ligar" + String(i) + "'><button class='ligado'>Ligar</button></a>");
        client.print("<a href='/desligar" + String(i) + "'><button class='desligado'>Desligar</button></a><hr>");
    }

    // JavaScript para atualizar dinamicamente os dados na página
    client.print("<script>");
    client.print("function atualizarStatusGas(){fetch('/status_gas').then(r=>r.text()).then(t=>{document.getElementById('statusGas').innerHTML=t;});}");
    client.print("function atualizarStatusDHT(){fetch('/status_dht').then(r=>r.text()).then(t=>{let v=t.split(' / ');document.getElementById('statusTemp').innerHTML=v[0];document.getElementById('statusUmid').innerHTML=v[1];});}");
    client.print("function atualizarStatusPIR(){fetch('/status_pir').then(r=>r.text()).then(t=>{document.getElementById('statusPIR').innerHTML=t;});}");
    client.print("function atualizarStatusFachada(){fetch('/status_fachada').then(r=>r.text()).then(t=>{document.getElementById('statusFachada').innerHTML=t;});}");
    client.print("atualizarStatusGas();atualizarStatusDHT();atualizarStatusPIR();atualizarStatusFachada();");
    client.print("setInterval(atualizarStatusGas,3000);setInterval(atualizarStatusDHT,5000);setInterval(atualizarStatusPIR,2000);setInterval(atualizarStatusFachada,5000);");
    client.print("</script></body></html>");
    client.flush();
    client.stop();
}

#endif
// Fim do arquivo interface.h