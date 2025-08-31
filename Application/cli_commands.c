#include "../mcc_generated_files/system/system.h"
#include "../mcc_generated_files/system/pins.h"
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_CLI.h"
#include <string.h>
#include <stdio.h>

/* LED control command */
static BaseType_t prvLEDCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

/* CLI command structure for LED control */
static const CLI_Command_Definition_t xLEDCommand =
{
    "led",
    "\r\nled <on/off>:\r\n Turn LED on or off\r\n",
    prvLEDCommand,
    1
};

/* LED status command */
static BaseType_t prvLEDStatusCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString );

static const CLI_Command_Definition_t xLEDStatusCommand =
{
    "status",
    "\r\nstatus:\r\n Show LED status\r\n",
    prvLEDStatusCommand,
    0
};

/* LED control command implementation for ACTIVE-LOW LED */
static BaseType_t prvLEDCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    const char *pcParameter;
    BaseType_t xParameterStringLength;

    /* Get the first parameter (on/off) */
    pcParameter = FreeRTOS_CLIGetParameter( pcCommandString, 1, &xParameterStringLength );

    if( pcParameter != NULL )
    {
        if( strncmp( pcParameter, "on", 2 ) == 0 )
        {
            LED_SetDigitalOutput();
            LED_SetLow();   // For active-low LED, LOW = ON
            snprintf( pcWriteBuffer, xWriteBufferLen, "LED turned ON (Active-Low)\r\n" );
        }
        else if( strncmp( pcParameter, "off", 3 ) == 0 )
        {
            LED_SetDigitalOutput();
            LED_SetHigh();  // For active-low LED, HIGH = OFF
            snprintf( pcWriteBuffer, xWriteBufferLen, "LED turned OFF (Active-Low)\r\n" );
        }
        else
        {
            snprintf( pcWriteBuffer, xWriteBufferLen, "Invalid parameter '%.*s'. Use 'on' or 'off'\r\n", 
                     (int)xParameterStringLength, pcParameter );
        }
    }
    else
    {
        snprintf( pcWriteBuffer, xWriteBufferLen, "Missing parameter. Use: led <on/off>\r\n" );
    }

    return pdFALSE;
}

/* LED status command implementation */
static BaseType_t prvLEDStatusCommand( char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString )
{
    (void)pcCommandString;
    
    if( LED_GetValue() )
    {
        snprintf( pcWriteBuffer, xWriteBufferLen, "LED is OFF (Active Low)\r\n" );
    }
    else
    {
        snprintf( pcWriteBuffer, xWriteBufferLen, "LED is ON (Active Low)\r\n" );
    }

    return pdFALSE;
}

/* Register CLI commands */
void vRegisterCLICommands( void )
{
    FreeRTOS_CLIRegisterCommand( &xLEDCommand );
    FreeRTOS_CLIRegisterCommand( &xLEDStatusCommand );
}