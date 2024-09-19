#include <reg51.h>

sbit fan = P1^0;          // Fan connected to P1.0
sbit light = P1^1;        // Light connected to P1.1
sbit clap_sensor = P3^2;  // Clap sensor connected to P3.2
sbit button = P3^3;       // Push button connected to P3.3

bit light_state = 0;      // Variable to store the current state of the light (0 = off, 1 = on)
bit fan_state = 0;        // Variable to store the current state of the fan (0 = off, 1 = on)

// Simulated voice commands
#define VOICE_COMMAND_LIGHT_ON    1
#define VOICE_COMMAND_LIGHT_OFF   2
#define VOICE_COMMAND_FAN_ON      3
#define VOICE_COMMAND_FAN_OFF     4

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 123; j++);  // Approximate delay
    }
}

void toggle_light() {
    if (light_state == 0) {
        light = 1;      // Turn on light
        light_state = 1; // Update state
    } else {
        light = 0;      // Turn off light
        light_state = 0; // Update state
    }
}

void toggle_fan() {
    if (fan_state == 0) {
        fan = 1;      // Turn on fan
        fan_state = 1; // Update state
    } else {
        fan = 0;      // Turn off fan
        fan_state = 0; // Update state
    }
}

void process_voice_command(unsigned char command) {
    switch (command) {
        case VOICE_COMMAND_LIGHT_ON:
            light = 1;      // Turn on light
            light_state = 1; // Update state
            break;
        case VOICE_COMMAND_LIGHT_OFF:
            light = 0;      // Turn off light
            light_state = 0; // Update state
            break;
        case VOICE_COMMAND_FAN_ON:
            fan = 1;      // Turn on fan
            fan_state = 1; // Update state
            break;
        case VOICE_COMMAND_FAN_OFF:
            fan = 0;      // Turn off fan
            fan_state = 0; // Update state
            break;
        default:
            break; // No action for unknown commands
    }
}

void main() {
    unsigned char simulated_command = VOICE_COMMAND_LIGHT_ON; // Simulated voice command

    // Initialize the fan and light states to off
    fan = 0;   
    light = 0;

    while (1) {
        if (clap_sensor == 0) {  // Detecting clap (low signal from the sensor)
            delay_ms(50);        // Debounce delay
            if (clap_sensor == 0) {  // Confirm it was a clap
                toggle_light();    // Toggle light on/off
            }
            while (clap_sensor == 0);  // Wait until clap signal goes high
        }

        if (button == 0) {  // Button pressed (simulates voice command)
            delay_ms(50);   // Debounce delay
            if (button == 0) {  // Confirm button press
                // For demonstration, use button press to cycle through commands
                static unsigned char command_index = 0;
                simulated_command = VOICE_COMMAND_LIGHT_ON + command_index;
                command_index = (command_index + 1) % 4; // Cycle through commands
                process_voice_command(simulated_command);  // Process the simulated command
            }
            while (button == 0);  // Wait until button signal goes high
        }

        delay_ms(100);  // Small delay to debounce and avoid continuous toggling
    }
}















/*#include <reg51.h>

sbit fan = P1^0;    // Fan connected to P1.0
sbit light = P1^1; // Light connected to P1.1
sbit clap_sensor = P3^2; // Clap sensor connected to P3.2

bit light_state = 0; // Variable to store the current state of the light (0 = off, 1 = on)

void UART_Init() {
    TMOD = 0x20;    // Timer1 in Mode2, auto-reload for baud rate generation
    TH1 = 0xFD;    // Baud rate of 9600
    SCON = 0x50;  // UART mode 1, 8-bit data, 1 stop bit, receive enabled
    TR1 = 1;     // Start Timer1
}

void UART_Tx(unsigned char data) {
    SBUF = data;           // Load data into UART buffer
    while (TI == 0);      // Wait for transmission to complete
    TI = 0;              // Clear transmit interrupt flag
}

unsigned char UART_Rx() {
    while (RI == 0);       // Wait for reception to complete
    RI = 0;               // Clear receive interrupt flag
    return SBUF;         // Return received data
}

void delay_ms(unsigned int ms) {
    unsigned int i, j;
    for (i = 0; i < ms; i++) {
        for (j = 0; j < 123; j++);  // Approximate delay
    }
}

void toggle_light() {
    if (light_state == 0) {
        light = 1;        // Turn on light
        light_state = 1; // Update state
        UART_Tx("Light On by Clap\n");  // Send confirmation (optional)
    } else {
        light = 0;        // Turn off light
        light_state = 0; // Update state
        UART_Tx("Light Off by Clap\n"); // Send confirmation (optional)
    }
}

void clap_control() {
    if (clap_sensor == 0) {   // Detecting clap (low signal from the sensor)
        delay_ms(50);        // Debounce delay
        if (clap_sensor == 0) {  // Confirm it was a clap
            toggle_light();     // Toggle light on/off
        }
        while (clap_sensor == 0);  // Wait until clap signal goes high
    }
}

void main() {
    unsigned char command;
    
    UART_Init();          // Initialize UART for communication with voice module

    // Initialize the fan and light states to off
    fan = 0;   
    light = 0;

    while (1) {
        clap_control();     // Check for clap command
        
        // Voice command processing
        command = UART_Rx();  // Receive voice command
        
        // Voice command interpretation
        if (command == '1') {         // "light on" via voice
            light = 1;        // Turn on light
            light_state = 1; // Update state
            UART_Tx("Light On by Voice\n");
        } 
        else if (command == '2') {    // "light off" via voice
            light = 0;        // Turn off light
            light_state = 0; // Update state
            UART_Tx("Light Off by Voice\n");
        } 
        else if (command == '3') {    // "fan on"
            fan = 1;  // Turn on fan
            UART_Tx("Fan On\n");
        } 
        else if (command == '4') {    // "fan off"
            fan = 0;  // Turn off fan
            UART_Tx("Fan Off\n");
        }
        
        delay_ms(100);  // Small delay to debounce and avoid continuous toggling
    }
}*/
