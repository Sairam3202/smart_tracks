#include <Servo.h>
Servo myservo1;
Servo myservo2;
int pos = 0;

// Pin Definitions
const int IR1 = 3;        // IR sensor for leaving train (Second Sensor)
const int IR2 = 2;        // IR sensor for approaching train (First Sensor)
const int buzzerPin = 8;  // Buzzer pin
const int redLED = 6;     // Red LED pin (Gate closed)
const int greenLED = 7;   // Green LED pin (Gate open)

bool isGateOpen = true;   // Keep track of whether the gate is open
bool trainDetected = false; // Track if the train was detected by the first sensor

void setup() {
  // Pin Modes
  pinMode(IR1, INPUT);    // IR sensor input (train leaving)
  pinMode(IR2, INPUT);    // IR sensor input (train approaching)
  pinMode(buzzerPin, OUTPUT);  // Buzzer pin as output
  pinMode(redLED, OUTPUT);     // Red LED pin as output
  pinMode(greenLED, OUTPUT);   // Green LED pin as output

  // Attach servos
  myservo1.attach(9);     // Micro servo 1
  myservo2.attach(10);    // Micro servo 2

  // Initialize gate to open
  openGate();             // Initially open the gate
}

void loop() {
  // If the first sensor (IR2) detects the train (LOW signal)
  if(!isGateOpen){
      digitalWrite(buzzerPin, HIGH);  // Turn the buzzer ON
      delay(20);                    // Wait for 1 second
      digitalWrite(buzzerPin, LOW);   // Turn the buzzer OFF
      delay(200); 
  }
  
  if (digitalRead(IR2) == LOW) {
    if (isGateOpen) {  // If the gate is currently open, close it
      closeGate();
      trainDetected = true;  // Train has been detected by the first sensor
      alertTrainDetected();   // Sound buzzer when train is detected
    }
  }

  // If the second sensor detects the train has passed
  if (trainDetected && digitalRead(IR1) == LOW) {
    if (!isGateOpen) {  // If the gate is closed, open it
      openGate();
      trainDetected = false;  // Reset train detection
      noTone(buzzerPin);      // Turn off the buzzer when the train leaves
    }
  }
}

// Function to close the gate
void closeGate() {
  tone(buzzerPin, 1000);  // 1 kHz tone while closing gate
  digitalWrite(redLED, HIGH);    // Turn on red LED
  digitalWrite(greenLED, LOW);   // Turn off green LED
  
  for (pos = 120; pos >= 0; pos -= 1) {  // Decrement the angle
    myservo1.write(pos);
    myservo2.write(pos);
    delay(2);  // Adjusted for smoother motion
  }
  
  noTone(buzzerPin);  // Stop the buzzer after gate is closed
  isGateOpen = false; // Mark gate as closed
}

// Function to open the gate
void openGate() {
  tone(buzzerPin, 2000);  // 2 kHz tone while opening gate
  digitalWrite(redLED, LOW);     // Turn off red LED
  digitalWrite(greenLED, HIGH);  // Turn on green LED
  
  for (pos = 0; pos <= 120; pos += 1) {  // Increment the angle
    myservo1.write(pos);
    myservo2.write(pos);
    
    // Adjust delay dynamically for smoother movement
    if (pos >= 60) 
      delay(1);  // Faster after halfway
    else 
      delay(5);  // Slower before halfway
  }
  
  noTone(buzzerPin);  // Stop the buzzer after gate is opened
  isGateOpen = true;  // Mark gate as open
}

// Function to alert when a train is detected
void alertTrainDetected() {
  tone(buzzerPin, 500);  // 500 Hz tone for alert when train is detected
  delay(200);            // Keep the buzzer on for 200 ms
  noTone(buzzerPin);     // Turn off the buzzer after alert
}
   