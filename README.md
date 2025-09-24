# G29_Handbrake

A wireless handbrake controller for Logitech G29 racing wheel simulation using ESP8266 and ESP-NOW protocol. This project enables wireless transmission of handbrake button states for racing simulation applications.

## Features

- **Wireless Communication**: ESP-NOW protocol for low-latency, reliable peer-to-peer communication
- **Hardware Debouncing**: Software debouncing with interrupt-driven detection for reliable button detection
- **Guaranteed Delivery**: Implements retry mechanism until packet delivery is confirmed
- **Interrupt-Driven**: Uses hardware interrupts for precise timing and low CPU overhead
- **Serial Monitoring**: Debug output for transmission status and button states
- **PlatformIO Compatible**: Built for embedded development with PlatformIO

## Hardware Requirements

- **MCU**: ESP8266 (D1 Mini or compatible)
- **Handbrake Button**: Momentary push button or switch
- **Power Supply**: 5V power source for ESP8266
- **Receiver**: Another ESP8266 device running compatible receiver code

## Wiring Configuration

| Component | ESP8266 GPIO | Description |
|-----------|-------------|-------------|
| Handbrake Button | GPIO 14 (D5) | Input from handbrake button/switch |
| Status LED | Built-in LED (varies by board) | Visual feedback (optional) |

### Button Connection
- Connect one side of the handbrake button to GPIO 14 (D5)
- Connect the other side to GND
- Use internal pull-up resistor (no external resistor needed)

## Installation

### Prerequisites
- [PlatformIO](https://platformio.org/) installed
- ESP8266 development board support installed

### Setup Steps
1. Clone this repository
2. Open the project in your preferred IDE (VSCode with PlatformIO recommended)
3. Update the receiver MAC address in `src/main.cpp`:
   ```cpp
   uint8_t broadcastAddress[] = {0x84, 0xF3, 0xEB, 0xE3, 0x3B, 0x43};
   ```
4. Build and upload:
   ```bash
   pio run --target upload
   ```
5. Monitor serial output:
   ```bash
   pio device monitor
   ```

## Usage

### Basic Operation
1. **Power on** both the handbrake transmitter and receiver devices
2. **Press handbrake button** - State will be transmitted wirelessly
3. **Release handbrake button** - State change will be transmitted
4. **Monitor serial output** to verify transmission status

### Communication Protocol
- **Button Pressed**: Transmits `1` via ESP-NOW
- **Button Released**: Transmits `0` via ESP-NOW
- **Retry Logic**: Continues sending until delivery confirmation received
- **Debouncing**: 20ms software debouncing to prevent false triggers

### LED Indicators
- **Serial Messages**: "Button pressed" or "Button released" on state changes
- **Delivery Status**: "Delivery success" or "Delivery fail" for each transmission

## Dependencies

- **Arduino Framework**: Core ESP8266 functionality
- **ESP8266WiFi**: WiFi connectivity for ESP-NOW
- **espnow**: Wireless peer-to-peer communication

## Configuration

### MAC Address Configuration
Replace the broadcast address with your receiver's MAC address:
```cpp
uint8_t broadcastAddress[] = {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX};
```

### Timing Parameters
- **Debounce Delay**: 20ms (minimum time between state changes)
- **Retry Delay**: 10ms (between retry attempts)
- **Polling Rate**: ~100Hz (main loop frequency)

### Pin Configuration
```cpp
const int8_t switch_pin = 14; // D5 - Handbrake button input
```

## Troubleshooting

### Common Issues

**No wireless communication:**
- Verify both devices are on same WiFi channel
- Check receiver MAC address is correct
- Ensure ESP-NOW initialization in serial monitor
- Verify receiver code is running and compatible

**Button not detected:**
- Check button wiring (D5 to button to GND)
- Verify internal pull-up is working
- Monitor serial output for "Button pressed/released" messages
- Test with simple digitalRead() in loop()

**Delivery failures:**
- Check antenna connections and signal strength
- Verify receiver is in range and powered on
- Monitor serial output for "Delivery fail" messages
- Consider adding external antenna for better range

**Intermittent detection:**
- Adjust debounce timing if needed
- Check for electrical noise on button circuit
- Verify stable power supply
- Consider adding hardware debouncing capacitor

### Debug Information
Enable serial monitor at 115200 baud to see:
- Button press/release detection
- ESP-NOW initialization status
- Packet delivery confirmations
- Error messages from ESP-NOW operations

## Development

### Project Structure
```
G29_Handbrake/
├── src/
│   └── main.cpp          # Main application code
├── include/              # Header files (empty)
├── test/                 # Unit tests (placeholder)
└── platformio.ini        # PlatformIO configuration
```

### Adding Features
- Modify `src/main.cpp` for additional button inputs
- Extend `struct_message` for more data fields
- Add analog input support for progressive handbrake
- Implement button combination detection

### Testing
- Use PlatformIO unit testing framework
- Test with mock receiver for wireless testing
- Verify timing accuracy with oscilloscope
- Test range and reliability in actual environment

## Communication Protocol

The handbrake uses a simple protocol:
```cpp
typedef struct struct_message {
    int8_t f;  // Button state: 0 = released, 1 = pressed
} struct_message;
```

### Packet Flow
1. Button state change triggers interrupt
2. Software debouncing validates state change
3. ESP-NOW packet sent to receiver MAC address
4. Delivery confirmation received via callback
5. Retry if delivery fails

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Add tests if applicable
5. Submit a pull request

## License

This project is licensed under the terms specified in the [License](License) file.

## Related Projects

- G29_CustomSteeringWheel - Custom steering wheel with display
- G29_ShifterDisplay - Gear shift indicator display
- G29_ShifterReceiver - Wireless shifter receiver
- G29_ShifterSniffer - CAN bus protocol analyzer

## Support

For issues and questions:
1. Check the troubleshooting section above
2. Review serial monitor output for error messages
3. Verify MAC address configuration
4. Ensure compatible receiver code is running
5. Check ESP-NOW initialization success

## Version History

- v1.0.0 - Initial release with basic functionality
- Wireless handbrake transmission with guaranteed delivery
- Hardware interrupt and software debouncing
- ESP-NOW communication protocol
