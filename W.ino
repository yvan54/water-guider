// Pin Definitions
#define TRIG_PIN 3       // Ultrasonic sensor trigger pin
#define ECHO_PIN 2       // Ultrasonic sensor echo pin
#define BUZZER_PIN 9     // Buzzer pin
#define RELAY_PIN 8      // Relay pin
const int LDR_PIN = A0;  // LDR connected to analog pin A0

// Thresholds
const int LDR_THRESHOLD = 300; // Adjust based on ambient light and cup placement
const float MIN_DISTANCE = 5.0; // Minimum distance for cup detection (in cm)
const float MAX_DISTANCE = 15.0; // Maximum distance for cup detection (in cm)
const float WATER_LEVEL_DISTANCE = 6.0; // Distance indicating the cup is full (in cm)

void setup() {
  // Pin modes
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LDR_PIN, INPUT);

  // Initialize relay to OFF (active low relay -> HIGH = off)
  digitalWrite(RELAY_PIN, HIGH);

  // Initialize serial communication
  Serial.begin(9600);
  Serial.println("System Initialized");
}

void loop() {
  // Read the LDR value
  int ldrValue = analogRead(LDR_PIN);
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // Check if a cup is detected based on LDR value
  if (ldrValue > LDR_THRESHOLD) { // Cup detected by LDR
    Serial.println("Cup detected by LDR. Activating ultrasonic sensor...");

    // Ultrasonic sensor work
    long duration;
    float distance;

    // Trigger ultrasonic sensor
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Measure the time it takes for the echo to return
    duration = pulseIn(ECHO_PIN, HIGH);

    // Calculate distance in centimeters
    distance = (duration * 0.0343) / 2;

    // Print distance to the Serial Monitor
    Serial.print("Distance: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Control relay and buzzer based on ultrasonic distance
    if (distance > MIN_DISTANCE && distance <= MAX_DISTANCE) { // Cup detected in range
      Serial.println("Cup in range. Switching relay ON.");
      digitalWrite(RELAY_PIN, LOW); // Turn relay ON (active low)

      if (distance <= WATER_LEVEL_DISTANCE) { // Water level reached
        Serial.println("Water level reached. Activating buzzer.");
        tone(BUZZER_PIN, 1000); // Turn on buzzer
        delay(200);             // Buzzer on for 200 milliseconds
        noTone(BUZZER_PIN);     // Turn off buzzer
        delay(200);             // Small delay
        tone(BUZZER_PIN, 1000); // Second beep
        delay(200);
        noTone(BUZZER_PIN);
      } else {
        Serial.println("Water level not yet reached.");
      }
    } else { // Cup out of ultrasonic range
      Serial.println("Cup not in range. Keeping relay OFF.");
      digitalWrite(RELAY_PIN, HIGH); // Ensure relay is OFF
    }
  } else { // No cup detected by LDR
    Serial.println("No cup detected by LDR. Relay and ultrasonic sensor OFF.");
    digitalWrite(RELAY_PIN, HIGH); // Turn relay OFF
    noTone(BUZZER_PIN);            // Ensure buzzer is off
  }

  // Delay for stability
  delay(500);
}
