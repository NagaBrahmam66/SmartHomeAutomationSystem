// Define pins
#define CLAP_BUTTON_PIN 2       // Pin connected to the push button (clap simulation)
#define VOICE_LIGHT_ON_PIN 3    // Pin connected to Button1 (simulates "light on" voice command)
#define VOICE_LIGHT_OFF_PIN 4   // Pin connected to Button2 (simulates "light off" voice command)
#define VOICE_FAN_ON_PIN 5      // Pin connected to Button3 (simulates "fan on" voice command)
#define VOICE_FAN_OFF_PIN 6     // Pin connected to Button4 (simulates "fan off" voice command)
#define LED_PIN 13              // Pin connected to the LED (light)
#define RELAY_PIN 12            // Pin connected to the relay (fan)

bool lightState = false;          // Current state of the light
bool fanState = false;            // Current state of the fan
bool lastButtonState[5] = {HIGH}; // Previous states of all buttons
bool currentButtonState[5] = {HIGH}; // Current states of all buttons

unsigned long lastDebounceTime[5] = {0};  // Last time button states were checked
unsigned long debounceDelay = 50;    // Debounce time (ms)

void setup() {
  pinMode(CLAP_BUTTON_PIN, INPUT_PULLUP); // Set clap button pin as input with internal pull-up resistor
  pinMode(VOICE_LIGHT_ON_PIN, INPUT_PULLUP); // Set "light on" button pin as input with internal pull-up resistor
  pinMode(VOICE_LIGHT_OFF_PIN, INPUT_PULLUP); // Set "light off" button pin as input with internal pull-up resistor
  pinMode(VOICE_FAN_ON_PIN, INPUT_PULLUP); // Set "fan on" button pin as input with internal pull-up resistor
  pinMode(VOICE_FAN_OFF_PIN, INPUT_PULLUP); // Set "fan off" button pin as input with internal pull-up resistor
  pinMode(LED_PIN, OUTPUT);              // Set LED pin as output
  pinMode(RELAY_PIN, OUTPUT);            // Set relay pin as output

  digitalWrite(LED_PIN, LOW);            // Initialize LED state (OFF)
  digitalWrite(RELAY_PIN, HIGH);         // Initialize relay state (OFF) assuming relay is active-low
  Serial.begin(9600); // Start serial communication for debugging
}

void loop() {
  const int buttonPins[] = {CLAP_BUTTON_PIN, VOICE_LIGHT_ON_PIN, VOICE_LIGHT_OFF_PIN, VOICE_FAN_ON_PIN, VOICE_FAN_OFF_PIN};
  const char* buttonNames[] = {"Clap Button", "Voice Light On", "Voice Light Off", "Voice Fan On", "Voice Fan Off"};

  for (int i = 0; i < 5; i++) {
    int reading = digitalRead(buttonPins[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis(); // Reset debounce timer
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != currentButtonState[i]) {
        currentButtonState[i] = reading;

        if (currentButtonState[i] == LOW) {
          switch (i) {
            case 0: // Clap Button
              lightState = !lightState;
              fanState = !fanState;
              digitalWrite(LED_PIN, lightState ? HIGH : LOW);
              digitalWrite(RELAY_PIN, fanState ? LOW : HIGH);
              Serial.print(buttonNames[i]);
              Serial.print(" Pressed. Light: ");
              Serial.print(lightState);
              Serial.print(", Fan: ");
              Serial.println(fanState);
              break;
            case 1: // Voice Light On Button
              lightState = true;
              digitalWrite(LED_PIN, HIGH);
              Serial.println(buttonNames[i]);
              break;
            case 2: // Voice Light Off Button
              lightState = false;
              digitalWrite(LED_PIN, LOW);
              Serial.println(buttonNames[i]);
              break;
            case 3: // Voice Fan On Button
              fanState = true;
              digitalWrite(RELAY_PIN, LOW);
              Serial.println(buttonNames[i]);
              break;
            case 4: // Voice Fan Off Button
              fanState = false;
              digitalWrite(RELAY_PIN, HIGH);
              Serial.println(buttonNames[i]);
              break;
          }
        }
      }
    }

    lastButtonState[i] = reading;
  }
}
