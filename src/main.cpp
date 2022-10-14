#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <espnow.h>
 
// REPLACE WITH RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x84, 0xF3, 0xEB, 0xE3, 0x3B, 0x43};
 
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int8_t f;
} struct_message;
// Create a struct_message called myData
struct_message myData;
 
// constants and variable declaration
const int8_t switch_pin = 14; //D5
volatile bool start_timer = false;
volatile int32_t t0=0;
int8_t state = 0; // 0: LOW, 1: HIGH
int8_t rcv_confirm = 0;


// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  Serial.print("Last Packet Send Status: ");
  if (sendStatus == 0){
    Serial.println("Delivery success");
  }
  else{
    Serial.println("Delivery fail");
    
  }
}
  
// software debounce switch_pin and return debounced state
int8_t debounce(int8_t pin) {
  //check if a change in state has occurred an if last change in state is older than 20ms
  if (start_timer && (millis() - t0) > 20) {
    // reset timer flag for a new recording
    start_timer = false;
    state = digitalRead(pin);
    state ? Serial.println("Button pressed") : Serial.println("Button released");
    return state;
  }
  return 2;
}

// Interrupt Service Routine
IRAM_ATTR void ISR() {
  // capture current timestamp on last change
  t0 = millis();
  start_timer = true;
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
 
  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
  esp_now_register_send_cb(OnDataSent);
   
  // Register peer
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);

  // attach D5 as Interrupt
  attachInterrupt(digitalPinToInterrupt(switch_pin), ISR, CHANGE);
}
  
void loop() {
  // Send message via ESP-NOW
  // Serial.print(esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData)));

  // temporarily store state
  int8_t debounced_state = debounce(switch_pin);

  // skip if button was not pressed (debounced_state=2)
  if (debounced_state !=2) {
    if (debounced_state == 1 || debounced_state == 0) {
      myData.f = debounced_state;
      // resend packet until packet is received: 0 -> delivery successful
      do {
        rcv_confirm = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
      } while(rcv_confirm!=0);
    }
    delay(10);
  }
}