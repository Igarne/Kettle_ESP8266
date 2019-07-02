void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
                // send message to client
                webSocket.sendTXT(num, "Hello");
            }
        case WStype_TEXT:
            socket_data = (char*)payload;
            Serial.println("Get data: " + socket_data);
            if(!socket_data.compareTo("led_on"))
            {
              digitalWrite(LED_BUILTIN, LOW);
              Serial.println("on");
              return;
            }
            if(!socket_data.compareTo("led_off"))
            {
              digitalWrite(LED_BUILTIN, HIGH);
              Serial.println("off");
              return;
            } 
    }
}
