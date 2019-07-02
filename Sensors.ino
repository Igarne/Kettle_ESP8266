void BPM_init() {
  if (bmp.begin()) { // включим задачу если датчик есть
    ts.add(0, 2000, [&](void*) { // Запустим задачу 0 с интервалом test
      
      Serial.print("Temperature = ");
      Serial.print(bmp.readTemperature());
      Serial.println(" *C");
      
      Serial.print("Pressure = ");
      Serial.print(bmp.readPressure());
      Serial.println(" Pa");

      Serial.print("Altitude = ");
      Serial.print(bmp.readAltitude());
      Serial.println(" meters");
      Serial.print("\n");
      
    }, nullptr, true);
  }
  else {
    Serial.println("BPM error");
    }
}

void DHT_init() {
  dht.setup(dhtPin); //Запускаем датчик
  delay(1000); // Нужно ждать иначе датчик не определится правильно
  static uint16_t test = dht.getMinimumSamplingPeriod(); // Получим минимальное время между запросами данных с датчика
  dht.getTemperature();   // обязательно делаем пустое чтение первый раз иначе чтение статуса не сработает
  String statusDHT = dht.getStatusString(); // Определим стстус датчика
  Serial.print("DHT = ");
  Serial.println(statusDHT); //  и сообщим в Serial

  if (statusDHT == "OK") { // включим задачу если датчик есть
    ts.add(1, test, [&](void*) { // Запустим задачу 0 с интервалом test
      String temperature = (String)dht.getTemperature();
      String humidity = (String)dht.getHumidity();
      
      Serial.print("Temperature = ");
      Serial.print(temperature);
      Serial.println(" *C");

      Serial.print("Temperature = ");
      Serial.println(humidity);
      
    }, nullptr, true);
  }
}
