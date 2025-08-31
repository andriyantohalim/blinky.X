# FreeRTOS LED Control via UART CLI

A demonstration project for the dsPIC33CK64MC105 microcontroller that implements a command-line interface (CLI) over UART to control an LED using FreeRTOS.

## Hardware Requirements

- **Microcontroller**: dsPIC33CK64MC105
- **LED**: Connected to pin RD10
- **UART**: UART1 configured for serial communication
- **Development Board**: Compatible with dsPIC33CK64MC105

## Software Requirements

- **MPLAB X IDE**: v6.15 or later
- **XC16 Compiler**: v2.10 or later
- **Device Family Pack**: dsPIC33CK-MC_DFP v1.17.244 or later
- **MCC (MPLAB Code Configurator)**: For peripheral configuration

## Features

- **FreeRTOS Integration**: Real-time operating system for task management
- **UART CLI**: Command-line interface over serial communication
- **LED Control**: Turn LED on/off via commands
- **Status Query**: Check current LED state
- **Help System**: Built-in help for available commands

## Project Structure

```
blinky.X/
├── Application/
│   ├── app.c                  # Application code (if used)
│   ├── cli_commands.c         # CLI command implementations
│   ├── cli_commands.h         # CLI command headers
│   ├── uart_cli_task.c        # UART CLI task implementation
│   └── FreeRTOSConfig.h       # FreeRTOS configuration
├── FreeRTOS/                  # FreeRTOS kernel source
├── FreeRTOS-Plus-CLI/         # FreeRTOS CLI component
├── mcc_generated_files/       # MCC generated peripheral drivers
└── main.c                     # Main application entry point
```

## Configuration

### FreeRTOS Settings
- **Heap Size**: 2048 bytes
- **Tick Rate**: 1000 Hz
- **Max Priorities**: 4
- **Static Allocation**: Enabled
- **Stack Overflow Checking**: Disabled (commented out)

### UART Settings
- **Peripheral**: UART1
- **Pins**: Configure via MCC
- **Baud Rate**: As configured in MCC (typically 9600 or 115200)

### Memory Usage
- **Program Memory**: ~34% (23,139 bytes)
- **Data Memory**: ~32% (2,650 bytes)
- **Dynamic Memory**: 5,542 bytes available for stack

## Building the Project

1. **Open in MPLAB X IDE**:
   ```bash
   File → Open Project → Select blinky.X folder
   ```

2. **Configure UART1** (if not already done):
   - Open MCC (Tools → Embedded → MPLAB Code Configurator)
   - Enable UART1 peripheral
   - Configure pins and baud rate
   - Generate code

3. **Build Project**:
   ```bash
   Production → Build Main Project
   # or press F11
   ```

## Usage

### Available CLI Commands

Connect to the device via UART (9600 baud, 8N1) using a serial terminal:

| Command | Description | Example |
|---------|-------------|---------|
| `led on` | Turn LED on | `> led on` |
| `led off` | Turn LED off | `> led off` |
| `status` | Show LED status | `> status` |
| `help` | Show available commands | `> help` |

### Example Session

```
FreeRTOS CLI Ready
Type 'help' for available commands
> help
help:
 Lists all the registered commands

led <on/off>:
 Turn LED on or off

status:
 Show LED status

> led on
LED turned ON

> status  
LED is ON

> led off
LED turned OFF
```

## System Architecture

```
┌─────────────────┐    ┌─────────────────┐    ┌─────────────────┐
│   UART Task     │───▶│  CLI Processor  │───▶│  LED Commands   │
│                 │    │                 │    │                 │
│ - Character I/O │    │ - Parse input   │    │ - led on/off    │
│ - Echo control  │    │ - Route command │    │ - status query  │
│ - Buffer mgmt   │    │ - Format output │    │ - Help system   │
└─────────────────┘    └─────────────────┘    └─────────────────┘
         │                                              │
         ▼                                              ▼
┌─────────────────┐                            ┌─────────────────┐
│   UART1 Driver  │                            │   GPIO Driver   │
│  (MCC Generated)│                            │  (MCC Generated)│
└─────────────────┘                            └─────────────────┘
         │                                              │
         ▼                                              ▼
┌─────────────────┐                            ┌─────────────────┐
│ Hardware UART1  │                            │   LED (RD10)    │
└─────────────────┘                            └─────────────────┘
```

## Task Configuration

- **CLI Task**:
  - Priority: 2
  - Stack Size: 200 words
  - Handles UART I/O and command processing

- **Idle Task**:
  - Static allocation enabled
  - Idle hook implemented (lightweight operations only)

## Troubleshooting

### Build Issues

1. **Missing FreeRTOS files**: Ensure FreeRTOS source is properly included
2. **UART errors**: Check MCC configuration and regenerate code
3. **Memory allocation**: Verify heap size settings in FreeRTOSConfig.h

### Runtime Issues

1. **No UART output**: Check UART pin configuration and baud rate
2. **System hang**: Verify stack sizes and heap allocation
3. **LED not responding**: Check pin configuration for RD10

### Common Solutions

- Clean and rebuild project
- Regenerate MCC code
- Check include paths in project properties
- Verify device selection matches hardware

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make changes and test thoroughly
4. Submit a pull request

## License

This project uses FreeRTOS which is licensed under the MIT License. Microchip-generated code retains Microchip's license terms.

## Additional Resources

- [FreeRTOS Documentation](https://www.freertos.org/Documentation/RTOS_book.html)
- [dsPIC33CK Family Datasheet](https://www.microchip.com/en-us/product/dsPIC33CK64MC105)
- [MPLAB X IDE User Guide](https://microchip.com/mplabx)
- [XC16 Compiler Guide](https://microchip.com/xc16)

## Version History

- **v1.0**: Initial implementation with basic LED control via UART CLI