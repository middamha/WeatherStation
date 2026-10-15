#include "web_manager.h"

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

AsyncWebServer server(80);

static void restartTask(void *parameter)
{
    vTaskDelay(pdMS_TO_TICKS(3000));

    Serial.println("RESTARTING...");

    ESP.restart();
}

void WebManager::begin(ConfigManager *configManager)
{
    config = configManager;

    // DASHBOARD

    server.on(
        "/",
        HTTP_GET,
        [this](AsyncWebServerRequest *request)
        {
            AppConfig &cfg = config->getConfig();

            String html;

            html += "<!DOCTYPE html>";
            html += "<html>";
            html += "<head>";
            html += "<meta charset='utf-8'>";
            html += "<title>ESP32 Weather Station</title>";
            html += "</head>";
            html += "<body>";

            html += "<h1>ESP32 Weather Station</h1>";

            html += "<hr>";

            html += "<p><b>Station:</b> ";
            html += cfg.stationName;
            html += "</p>";

            html += "<p><b>IP:</b> ";
            html += WiFi.localIP().toString();
            html += "</p>";

            html += "<p><b>RSSI:</b> ";
            html += String(WiFi.RSSI());
            html += " dBm</p>";

            html += "<p><b>Uptime:</b> ";
            html += String(millis() / 1000);
            html += " s</p>";

            html += "<p><b>Free Heap:</b> ";
            html += String(ESP.getFreeHeap());
            html += "</p>";

            html += "<p><b>Free PSRAM:</b> ";
            html += String(ESP.getFreePsram());
            html += "</p>";

            html += "<hr>";

            html += "<a href='/setup'>Configuration</a>";
            html += "<br><br>";

            html += "<a href='/reboot'>Restart ESP32</a>";

            html += "</body>";
            html += "</html>";

            request->send(
                200,
                "text/html",
                html);
        });

    // CONFIGURATION

    server.on(
        "/setup",
        HTTP_GET,
        [this](AsyncWebServerRequest *request)
        {
            AppConfig &cfg = config->getConfig();

            String html;

            html += "<!DOCTYPE html>";
            html += "<html>";
            html += "<head>";
            html += "<meta charset='utf-8'>";
            html += "<title>Configuration</title>";
            html += "</head>";
            html += "<body>";

            html += "<h1>Configuration</h1>";

            html += "<form action='/save' method='POST'>";

            html += "WiFi SSID:<br>";
            html += "<input type='text' name='ssid' value='";
            html += cfg.wifiSSID;
            html += "'><br><br>";

            html += "WiFi Password:<br>";
            html += "<input type='password' name='wifipass'><br><br>";

            html += "MQTT Host:<br>";
            html += "<input type='text' name='mqtthost' value='";
            html += cfg.mqttHost;
            html += "'><br><br>";

            html += "MQTT Port:<br>";
            html += "<input type='number' name='mqttport' value='";
            html += String(cfg.mqttPort);
            html += "'><br><br>";

            html += "MQTT User:<br>";
            html += "<input type='text' name='mqttuser' value='";
            html += cfg.mqttUser;
            html += "'><br><br>";

            html += "MQTT Password:<br>";
            html += "<input type='password' name='mqttpass'><br><br>";

            html += "Station Name:<br>";
            html += "<input type='text' name='station' value='";
            html += cfg.stationName;
            html += "'><br><br>";

            html += "<input type='submit' value='SAVE'>";

            html += "</form>";

            html += "<br><br>";
            html += "<a href='/'>Back to Dashboard</a>";

            html += "</body>";
            html += "</html>";

            request->send(
                200,
                "text/html",
                html);
        });

    // SAVE

    server.on(
        "/save",
        HTTP_POST,
        [this](AsyncWebServerRequest *request)
        {
            AppConfig &cfg = config->getConfig();

            cfg.wifiSSID = request->arg("ssid");
            cfg.wifiPassword = request->arg("wifipass");

            cfg.mqttHost = request->arg("mqtthost");
            cfg.mqttPort = request->arg("mqttport").toInt();

            cfg.mqttUser = request->arg("mqttuser");
            cfg.mqttPassword = request->arg("mqttpass");

            cfg.stationName = request->arg("station");

            config->save();

            Serial.println("CONFIG SAVED");

            request->send(
                200,
                "text/html",
                "<!DOCTYPE html>"
                "<html>"
                "<head>"
                "<meta charset='utf-8'>"
                "<meta http-equiv='refresh' content='5;url=/'>"
                "</head>"
                "<body>"
                "<h1>Configuration Saved</h1>"
                "<p>ESP32 will restart in 3 seconds...</p>"
                "<p>Dashboard will open automatically.</p>"
                "</body>"
                "</html>");

            xTaskCreate(
                restartTask,
                "restartTask",
                2048,
                nullptr,
                1,
                nullptr);
        });

    // REBOOT

    server.on(
        "/reboot",
        HTTP_GET,
        [](AsyncWebServerRequest *request)
        {
            request->send(
               200,
               "text/html",
              "<!DOCTYPE html>"
               "<html>"
               "<head>"
               "<meta charset='utf-8'>"
              "<meta http-equiv='refresh' content='5;url=/'>"
              "</head>"
              "<body>"
              "<h1>Restarting ESP32...</h1>"
              "<p>Dashboard will reload automatically.</p>"
              "</body>"
              "</html>");

            xTaskCreate(
                restartTask,
                "restartTask",
                2048,
                nullptr,
                1,
                nullptr);
        });

    server.begin();

    Serial.println("WEB SERVER STARTED");
}