
void setup() {
    Serial.begin(9600);
}

void loop() {    
    String data = "";
    while (Serial.available()) {
        Serial.println("Reading Serial...");
        data = Serial.readStringUntil('#');      
    }
    if (!data.isEmpty()) {
        Serial.println("Data from Mega: " + data);
    }
    delay(1000);
}
