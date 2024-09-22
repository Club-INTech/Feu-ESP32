#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SPIFFS.h>
#include <ArduinoOTA.h>

#include <light.hpp>
#include <behaviours.hpp>

#define RED_PIN 19
#define YELLOW_PIN 18
#define GREEN_PIN 5

AsyncWebServer server(80);
Light lt(RED_PIN, YELLOW_PIN, GREEN_PIN);

// Defines set by secrets.py, also OTA_PASWD and OTA_PORT
const char* ssid = WIFI_SSID;
const char* password = WIFI_PSWD;

void notFound(AsyncWebServerRequest *request) {
	request->send(404, "text/plain", "Not found");
}

void otaTask(void *param) {
	while (true) {
		ArduinoOTA.handle();
	}
}

void initOTA() {
	Serial.println("Init OTA");
	ArduinoOTA.setPassword(OTA_PASWD);
	ArduinoOTA.setPort(OTA_PORT);

	ArduinoOTA
		.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH) {
				type = "sketch";
			} else {  // U_SPIFFS
				type = "filesystem";
			}

			// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
			Serial.println("Start updating " + type);
		})
		.onEnd([]() {
			Serial.println("\nEnd");
		})
		.onProgress([](unsigned int progress, unsigned int total) {
			Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		})
		.onError([](ota_error_t error) {
			Serial.printf("Error[%u]: ", error);
			if (error == OTA_AUTH_ERROR) {
				Serial.println("Auth Failed");
			} else if (error == OTA_BEGIN_ERROR) {
				Serial.println("Begin Failed");
			} else if (error == OTA_CONNECT_ERROR) {
				Serial.println("Connect Failed");
			} else if (error == OTA_RECEIVE_ERROR) {
				Serial.println("Receive Failed");
			} else if (error == OTA_END_ERROR) {
				Serial.println("End Failed");
			}
		});

	ArduinoOTA.begin();

	xTaskCreate(otaTask,"OTA",10000,NULL,1,NULL);
}

String indexProcessor(const String& var){
	if (var == "MODES") {
		String list = String();
		std::map<int, Behaviour*> *map = Behaviour::getBehavMap();
		
		for (std::map<int, Behaviour*>::iterator it = map->begin(); it != map->end(); it++)
			list += "<option value=\"" + String(it->first) + "\">" + it->second->name() + "</option>";

		return list;
	}
	return String();
}

void initWeb() {
	Serial.println("Init Web");

	if (!SPIFFS.begin(true)) {
		Serial.println("An Error has occurred while mounting SPIFFS");
		return;
	}

	server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/index.html", String(), false, indexProcessor);
	});

	server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
		request->send(SPIFFS, "/style.css", "text/css");
	});

	server.on("/set", HTTP_GET, [] (AsyncWebServerRequest *request) {
		if (request->hasParam("mode")) {
			int mode = request->getParam("mode")->value().toInt();
			Behaviour *behav = Behaviour::findBehav(mode);
			if (behav)
				lt.setBehaviour(behav);
		}
		if (request->hasParam("enable")) {
			int en = request->getParam("enable")->value().toInt();
			lt.setEnableBehav(en > 0);
		}
		if (request->hasParam("timescale")) {
			float ts = request->getParam("timescale")->value().toFloat();
			lt.setTimescale(ts);
		}
		if (request->hasParam("light")) {
			int val = request->getParam("light")->value().toInt();
			if (!lt.isBehavEnabled()) {
				lt.setR(val&1);
				lt.setY((val>>1)&1);
				lt.setG((val>>2)&1);
			}
		}

		Behaviour *behav = lt.getBehav();
		String bname = "none";
		if (behav)
			bname = behav->name();

		request->send(200, "text/plain", "Behav: '"+bname+"', en:"+lt.isBehavEnabled());
	});

	server.onNotFound(notFound);

	server.begin();
}

void initWifi() {
	Serial.println("Init Wifi");
	WiFi.mode(WIFI_STA);
	while (true) {
		WiFi.begin(ssid, password);
		if (WiFi.waitForConnectResult() != WL_CONNECTED) {
			Serial.printf("WiFi Failed!\n");
			continue;
		}
		break;
	}

	Serial.print("IP Address: ");
	Serial.println(WiFi.localIP());
}

void lightTask(void *param) {
	while (true) {
		lt.run();
	}
}

void initLight() {
	Serial.println("Init Light");
	lt.init();
	lt.setEnableBehav(true);
	lt.setBehaviour(new OriginalBehaviour());
	xTaskCreatePinnedToCore(lightTask,"Light",10000,NULL,2,NULL,0);
}

void setup() {
	Serial.begin(115200);
	Serial.println("Init");

	initLight();
	initWifi();
	initOTA();
	initWeb();

	Serial.println("Finished");
}

void loop() {
	// Everything is in tasks...
}