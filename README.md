# Morse Code NodeMCU

This NodeMCU code provides a web interface for converting text to Morse code and flashing it using an LED.

## Prerequisites

- NodeMCU board
- LED
- Resistor (to limit current to the LED)
- Jumper wires
- Internet connection

## Setup

1. **Connect the LED**: Connect the LED's anode (long leg) to the D4 pin (GPIO13) on the NodeMCU and the cathode (short leg) to a resistor, then connect the other end of the resistor to ground.
2. **Upload the code**: Copy and paste the provided code into your Arduino IDE, select the correct board and port for your NodeMCU, and upload it.

## Usage

1. **Open a web browser**: On your computer or mobile device, open a web browser and enter the IP address of your NodeMCU in the address bar. You can find the IP address in the Serial Monitor after uploading the code.
2. **Enter text**: In the text field on the web page, enter the text you want to convert to Morse code.
3. **Set speed (optional)**: Adjust the speed slider to control the speed of the Morse code flashes.
4. **Convert**: Click the "Convert" button to start the conversion and flashing.

## Explanation

- **WiFi connection**: The code connects to your Wi-Fi network and starts a web server.
- **Web interface**: The web server serves a simple HTML form for entering text and adjusting speed.
- **Morse code conversion**: The code converts the entered text to Morse code using a predefined table.
- **LED flashing**: The Morse code is flashed using the LED, with dot and dash lengths determined by the speed factor.

## Demo

![Demo](demo.gif)

Note: Replace `demo.gif` with the actual filename of your GIF if you have one.

## Additional Features

- **Error handling**: The code includes basic error handling for invalid characters and connection issues.
- **Speed adjustment**: The speed of the Morse code can be adjusted using the web interface.
- **Customizable**: You can modify the Morse code table and LED pin to suit your needs.
