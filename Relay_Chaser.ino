#include "BluetoothSerial.h" // Include BluetoothSerial library
#include <Preferences.h>     // Include Preferences library

BluetoothSerial SerialBT;    // Initialize Bluetooth object
Preferences preferences;     // Preferences object for storing state

// Define relay pins
const int relay1 = 16; // GPIO16
const int relay2 = 17; // GPIO17
const int relay3 = 18; // GPIO18
const int relay4 = 19; // GPIO19

volatile bool runPattern = false; // Global flag to control patterns
char activePattern = '\0';        // Current active pattern

void setup() {
  // Initialize Serial for debugging and Bluetooth communication
  Serial.begin(115200);
  SerialBT.begin("C_Project"); // Set Bluetooth name to "C_Project"

  // Configure relay pins as OUTPUT and set them OFF (HIGH for active-low relays)
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);

  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);

  // Open Preferences storage
  preferences.begin("relayPrefs", false);

  // Restore the last active pattern from Preferences
  activePattern = preferences.getChar("pattern", '\0');
  if (activePattern != '\0') {
    runPattern = true;
    Serial.print("Restored Pattern: ");
    Serial.println(activePattern);

    // If the pattern was 'Z' (all relays ON), turn them ON
    if (activePattern == 'Z') {
      turnAllOn();
    }
  } else {
    Serial.println("No pattern to restore.");
  }

  Serial.println("ESP32 Relay Controller Initialized!");
}

void loop() {
  // Check if Bluetooth data is available
  if (SerialBT.available()) {
    char command = SerialBT.read(); // Read the incoming command
    Serial.print("Received Command: ");
    Serial.println(command);        // Debug print the command
    handleCommand(command);         // Process the command
  }

  // Run the active pattern if set
  if (runPattern && activePattern != '\0') {
    executePattern(activePattern);
  }
}

void handleCommand(char cmd) {
  if (cmd >= 'A' && cmd <= 'Z') {
    // Capital letters start a pattern
    activePattern = cmd;
    runPattern = true;

    // Save the pattern to Preferences
    preferences.putChar("pattern", activePattern);

    Serial.print("Starting Pattern: ");
    Serial.println(cmd);

    if (cmd == 'Z') {
      turnAllOn(); // Turn all relays ON and save the state
    }
  } else if (cmd >= 'a' && cmd <= 'z') {
    // Small letters stop the pattern
    activePattern = '\0';
    runPattern = false;

    // Clear the stored pattern in Preferences
    preferences.putChar("pattern", activePattern);

    Serial.print("Stopping Pattern: ");
    Serial.println(cmd);
    relayAllOff(); // Turn off all relays
  }
}

void executePattern(char pattern) {
  switch (pattern) {
    case 'A': relayPattern1(); break;
    case 'B': relayPattern2(); break;
    case 'C': relayPattern3(); break;
    case 'D': relayPattern4(); break;
    case 'E': relayPattern5(); break;
    case 'F': relayPattern6(); break;
    case 'G': relayPattern7(); break;
    case 'H': relayPattern8(); break;
    case 'I': relayPattern9(); break;
    case 'J': relayPattern10(); break;
    case 'K': relayPattern11(); break;
    case 'L': relayPattern12(); break;
    case 'M': relayPattern13(); break;
    case 'N': randomCombinedPattern(); break;
    default: Serial.println("Unknown Pattern"); break;
  }
}

// Pattern implementations
void relayPattern1() {
  Serial.println("Pattern 1: Sequential Relay ON-OFF");
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay1 + i, LOW);
    delay(300);
    digitalWrite(relay1 + i, HIGH);
    delay(300);
  }
}

void relayPattern2() {
  Serial.println("Pattern 2: Alternating Relays");
  digitalWrite(relay1, LOW);
  digitalWrite(relay3, LOW);
  delay(500);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay2, LOW);
  digitalWrite(relay4, LOW);
  delay(500);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay4, HIGH);
}

void relayPattern3() {
  Serial.println("Pattern 3: All Relays Blink");
  for (int i = 0; i < 5; i++) {
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    delay(300);
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    delay(300);
  }
}
void relayPattern4() {
  Serial.println("Pattern 4: Sequential Blink and Pass Control (with final all-off)");

  // Step 1: All relays off initially
  relayAllOff();

  // Step 2: Relay 4 blinks once, then passes to Relay 3
  digitalWrite(relay4, LOW);  // Relay 4 ON
  delay(200);
  digitalWrite(relay4, HIGH); // Relay 4 OFF
  delay(200);
  delay(1000);  // Increased delay before passing to Relay 3 (longer pause)
  // Pass to Relay 3
  digitalWrite(relay4, HIGH);  // Turn off Relay 4
  delay(100);  // Small delay before moving to Relay 3

  // Step 3: Relay 3 blinks once, then passes to Relay 2
  digitalWrite(relay3, LOW);  // Relay 3 ON
  delay(200);
  digitalWrite(relay3, HIGH); // Relay 3 OFF
  delay(200);
  delay(500);  // Increased delay before passing to Relay 2
  // Pass to Relay 2
  digitalWrite(relay3, HIGH);  // Turn off Relay 3
  delay(100);  // Small delay before moving to Relay 2

  // Step 4: Relay 2 blinks once, then passes to Relay 1
  digitalWrite(relay2, LOW);  // Relay 2 ON
  delay(200);
  digitalWrite(relay2, HIGH); // Relay 2 OFF
  delay(200);
  delay(500);  // Increased delay before passing to Relay 1
  // Pass to Relay 1
  digitalWrite(relay2, HIGH);  // Turn off Relay 2
  delay(100);  // Small delay before moving to Relay 1

  // Step 5: Relay 1 stays on permanently, and Relay 4 blinks once, then turns off
  digitalWrite(relay1, LOW);   // Relay 1 ON
  delay(200);
  digitalWrite(relay4, LOW);   // Relay 4 ON
  delay(400);  // Increased delay when Relay 4 is ON
  digitalWrite(relay4, HIGH);  // Relay 4 OFF
  delay(1000);  // Increased delay before moving to Relay 3

  // Step 6: Relay 3 blinks once, then passes control to Relay 2
  digitalWrite(relay3, LOW);   // Relay 3 ON
  delay(200);
  digitalWrite(relay3, HIGH);  // Relay 3 OFF
  delay(200);
  delay(500);  // Increased delay before passing to Relay 2
  // Pass to Relay 2
  digitalWrite(relay3, HIGH);  // Turn off Relay 3
  delay(100);  // Small delay before moving to Relay 2

  // Step 7: Relay 2 turns on permanently while Relay 3 turns off
  digitalWrite(relay2, LOW);   // Relay 2 ON
  digitalWrite(relay3, HIGH);  // Turn off Relay 3
  delay(500);  // Increased delay before moving to Relay 1

  // Step 8: Relay 4 blinks once, then turns on and turns off all relays in order
  digitalWrite(relay4, LOW);   // Relay 4 ON
  delay(400);  // Increased delay when Relay 4 is ON
  digitalWrite(relay4, HIGH);  // Relay 4 OFF
  delay(1000);  // Increased delay before turning Relay 3 on

  // Step 9: Relay 3 turns on permanently, Relay 4 stays on permanently
  digitalWrite(relay3, LOW);   // Relay 3 ON
  digitalWrite(relay4, LOW);   // Relay 4 ON
  delay(500);  // Increased delay before proceeding to final step

  // Step 10: Final all lights off
  relayAllOff();
  delay(1000); // Increased delay before repeating the pattern

  // **NEW DELAY** after Relay 3 turns on before Relay 4 starts
  delay(1000);  // Inserted delay (this delay is customizable)
  
  // After this delay, you can continue with Relay 4's sequence
  digitalWrite(relay4, LOW);   // Relay 4 ON
  delay(400);  // Relay 4 ON delay
  digitalWrite(relay4, HIGH);  // Relay 4 OFF
  delay(1000);  // Increased delay before passing to Relay 3 again
}


void relayPattern5() {
  Serial.println("Pattern 5: Opposite Pairs Toggle");
  digitalWrite(relay1, LOW);
  digitalWrite(relay4, LOW);
  delay(400);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  delay(400);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
}

void relayPattern6() {
  Serial.println("Pattern 6: Two at a Time");
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  delay(400);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  delay(400);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
}

void relayPattern7() {
  Serial.println("Pattern 7: Blink Relay 1 and 4, then 2 and 3, and repeat");

  // Step 1: Relay 1 and Relay 4 blink once, then turn off
  digitalWrite(relay1, LOW);  // Relay 1 ON
  digitalWrite(relay4, LOW);  // Relay 4 ON
  delay(200);                 // Blink delay for Relay 1 and 4
  digitalWrite(relay1, HIGH); // Relay 1 OFF
  digitalWrite(relay4, HIGH); // Relay 4 OFF
  delay(200);                 // Delay before moving to next part

  // Step 2: Relay 2 and Relay 3 blink twice, then turn off
  for (int i = 0; i < 2; i++) {
    digitalWrite(relay2, LOW);  // Relay 2 ON
    digitalWrite(relay3, LOW);  // Relay 3 ON
    delay(200);                 // Blink delay for Relay 2 and 3
    digitalWrite(relay2, HIGH); // Relay 2 OFF
    digitalWrite(relay3, HIGH); // Relay 3 OFF
    delay(200);                 // Delay between blinks
  }
  delay(500); // Small delay before repeating the process

  // Repeat the process by calling relayPattern7() again or adding a loop
  // Uncomment below if you'd like to repeat the pattern manually.
  // relayPattern7();
}

void relayPattern8() {
  Serial.println("Pattern 8: Sequential Turn-On and Reverse Blinking Twice (With Off During Blink)");
  
  // Sequentially turn on relays 1 to 4
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay1 + i, LOW);  // Turn ON relay
    delay(300);                      // Delay for 300ms
  }

  // Blink relays 4 to 1 in reverse order, twice, with OFF state during blink
  for (int j = 0; j < 2; j++) {   // Blink twice
    for (int i = 3; i >= 0; i--) {  // Reverse order: 4, 3, 2, 1
      digitalWrite(relay1 + i, HIGH);  // Turn OFF relay
      delay(200);                      // Wait for 200ms (OFF)
      digitalWrite(relay1 + i, LOW);   // Turn ON relay
      delay(200);                      // Wait for 200ms (ON)
    }
  }

  // Sequentially turn on relays 1 to 4 again
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay1 + i, LOW);  // Turn ON relay
    delay(300);                      // Delay for 300ms
  }

  // Blink relays 4 to 1 in reverse order again, twice, with OFF state during blink
  for (int j = 0; j < 2; j++) {   // Blink twice
    for (int i = 3; i >= 0; i--) {  // Reverse order: 4, 3, 2, 1
      digitalWrite(relay1 + i, HIGH);  // Turn OFF relay
      delay(200);                      // Wait for 200ms (OFF)
      digitalWrite(relay1 + i, LOW);   // Turn ON relay
      delay(200);                      // Wait for 200ms (ON)
    }
  }
}



void relayPattern9() {
  Serial.println("Pattern 9: Random ON-OFF");
  for (int i = 0; i < 10; i++) {
    int randomRelay = random(0, 4);
    digitalWrite(relay1 + randomRelay, LOW);
    delay(150);
    digitalWrite(relay1 + randomRelay, HIGH);
    delay(150);
  }
}

void relayPattern10() {
  Serial.println("Pattern 10: Skipping Ripple");
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay1 + i, LOW);
    delay(300);
    digitalWrite(relay1 + i, HIGH);
    delay(300);
    i++; // Skip the next relay for the ripple effect
  }
}

void relayPattern11() {
  Serial.println("Pattern 11: One Relay ON at a Time");
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay1 + i, LOW);
    delay(400);
    digitalWrite(relay1 + i, HIGH);
  }
}

void relayPattern12() {
  Serial.println("Pattern 12: Expanding and Contracting");
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay1 + i, LOW);
    delay(300);
  }
  for (int i = 3; i >= 0; i--) {
    digitalWrite(relay1 + i, HIGH);
    delay(300);
  }
}

void relayPattern13() {
  Serial.println("Pattern 13: Shimmering Wave");
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay1 + i, LOW);
    delay(200);
  }
  for (int i = 0; i < 4; i++) {
    digitalWrite(relay1 + i, HIGH);
    delay(200);
  }
}

void randomCombinedPattern() {
  Serial.println("Pattern N: Random Combined Pattern");
  char randomPattern = 'A' + random(0, 12);
  executePattern(randomPattern);
}

void relayAllOff() {
  // Function to turn off all relays
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
}


void turnAllOn() {
  Serial.println("Turning all relays ON");
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);

  // Save state
  preferences.putChar("pattern", 'Z');
}
