// interface.h - Manipulação da interface HTML e rotas do servidor

#ifndef INTERFACE_H
#define INTERFACE_H

#include <WiFi.h>      // Biblioteca para conexão Wi-Fi
#include "config.h"    // Inclui definições de pinos e variáveis globais
#include "atuadores.h" // Inclui funções para controlar atuadores (servo motor)
#include "sensores.h"  // Inclui funções para lidar com sensores

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
    for (int i = 0; i < numLuzes; i++)
    {
        if (request.indexOf("GET /ligar" + String(i)) >= 0)
        {
            estadosLuzes[i] = true;
            digitalWrite(pinosLuzes[i], HIGH);
        }
        if (request.indexOf("GET /desligar" + String(i)) >= 0)
        {
            estadosLuzes[i] = false;
            digitalWrite(pinosLuzes[i], LOW);
        }
    }

    if (request.indexOf("GET /ligarPIR") >= 0)
        sensorPresencaAtivo = true;
    if (request.indexOf("GET /desligarPIR") >= 0)
        sensorPresencaAtivo = false;
    if (request.indexOf("GET /abrir") >= 0)
        moverServo(90);
    if (request.indexOf("GET /fechar") >= 0)
        moverServo(0);

    if (request.indexOf("GET /status_gas") >= 0)
    {
        client.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n");
        client.println();
        client.println(digitalRead(GasD) == 0 ? "ALERTA! Vazamento de gás detectado" : "OK");
        client.flush();
        delay(1);
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
        }
        client.println("HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nConnection: close\r\n");
        client.println();
        client.println(String(temperatura) + "°C / " + String(umidade) + "%");
        client.flush();
        delay(1);
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
        delay(1);
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
        delay(1);
        client.stop();
        return;
    }

    // Variável para armazenar o conteúdo HTML da página
    String html;
    html += "HTTP/1.1 200 OK\r\nContent-type:text/html\r\nConnection: close\r\n\r\n";
    html += "<!DOCTYPE html><html><head><meta charset='UTF-8'><meta name='viewport' content='width=device-width, initial-scale=1'>";
    html += "<title>Automação</title><style>body{text-align:center;font-family:Arial;}button{padding:10px;margin:5px;width:120px;}.ligado{background:green;color:white;}.desligado{background:red;color:white;}</style></head><body>";
    html += "<h1>CONTROLE RESIDENCIAL</h1><h2>Monitoramento de Ambiente</h2>";
    html += "<p><b>Temperatura:</b> <span id='statusTemp'>Carregando...</span></p>";
    html += "<p><b>Umidade:</b> <span id='statusUmid'>Carregando...</span></p><hr>";
    html += "<h2>Sensor de Gás</h2><p>Status: <b id='statusGas'>Carregando...</b></p><hr>";
    html += "<h2>Portão</h2><button onclick=\"enviarComando('/abrir')\">Abrir</button><button onclick=\"enviarComando('/fechar')\">Fechar</button>";
    html += "<h2>Sensor de Presença</h2><p>Status: <b id='statusPIR'>Carregando...</b></p>";
    html += "<button class='ligado' onclick=\"enviarComando('/ligarPIR')\">Ligar</button>";
    html += "<button class='desligado' onclick=\"enviarComando('/desligarPIR')\">Desligar</button><hr>";
    html += "<h2>Luz da Fachada</h2><p>Status: <b id='statusFachada'>Carregando...</b></p><hr>";
    html += "<h2>Piso Inferior</h2>";
    for (int i = 0; i < 5; i++)
    {
        html += "<h3>Luz da " + String(nomesLuzes[i]) + "</h3>";
        html += "<p>Status: <b id='statusLuz" + String(i) + "'>" + (estadosLuzes[i] ? "Ligada" : "Desligada") + "</b></p>";
        html += "<button class='ligado' onclick=\"enviarComando('/ligar" + String(i) + "'," + String(i) + ",'Ligada')\">Ligar</button>";
        html += "<button class='desligado' onclick=\"enviarComando('/desligar" + String(i) + "'," + String(i) + ",'Desligada')\">Desligar</button><hr>";
    }
    html += "<h2>Piso Superior</h2>";
    for (int i = 5; i < 10; i++)
    {
        html += "<h3>Luz da " + String(nomesLuzes[i]) + "</h3>";
        html += "<p>Status: <b id='statusLuz" + String(i) + "'>" + (estadosLuzes[i] ? "Ligada" : "Desligada") + "</b></p>";
        html += "<button class='ligado' onclick=\"enviarComando('/ligar" + String(i) + "'," + String(i) + ",'Ligada')\">Ligar</button>";
        html += "<button class='desligado' onclick=\"enviarComando('/desligar" + String(i) + "'," + String(i) + ",'Desligada')\">Desligar</button><hr>";
    }
    html += "<script>";
    html += "function atualizarStatusGas(){fetch('/status_gas').then(r=>r.text()).then(t=>{document.getElementById('statusGas').innerHTML=t;});}";
    html += "function atualizarStatusDHT(){fetch('/status_dht').then(r=>r.text()).then(t=>{let v=t.split(' / ');document.getElementById('statusTemp').innerHTML=v[0];document.getElementById('statusUmid').innerHTML=v[1];});}";
    html += "function atualizarStatusPIR(){fetch('/status_pir').then(r=>r.text()).then(t=>{document.getElementById('statusPIR').innerHTML=t;});}";
    html += "function atualizarStatusFachada(){fetch('/status_fachada').then(r=>r.text()).then(t=>{document.getElementById('statusFachada').innerHTML=t;});}";
    html += "function enviarComando(caminho, indice=null, novoStatus=''){fetch(caminho).then(r=>{if(r.ok && indice !== null){document.getElementById('statusLuz'+indice).innerText = novoStatus;}});}";
    html += "atualizarStatusGas();atualizarStatusDHT();atualizarStatusPIR();atualizarStatusFachada();";
    html += "setInterval(atualizarStatusGas,3000);setInterval(atualizarStatusDHT,5000);setInterval(atualizarStatusPIR,2000);setInterval(atualizarStatusFachada,5000);";
    html += "</script></body></html>";

    // Envia a resposta HTML para o navegador e encerra a conexão
    client.print(html);
    client.flush();
    delay(1);
    client.stop();
}

#endif
// Fim do arquivo interface.h
