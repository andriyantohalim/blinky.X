#include "../mcc_generated_files/system/system.h"
#include "../mcc_generated_files/uart/uart1.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "FreeRTOS_CLI.h"
#include <string.h>

#define CLI_TASK_STACK_SIZE         200
#define CLI_TASK_PRIORITY           2
#define CLI_INPUT_BUFFER_SIZE       50
#define CLI_OUTPUT_BUFFER_SIZE      configCOMMAND_INT_MAX_OUTPUT_SIZE

/* Task handle */
static TaskHandle_t xCLITaskHandle = NULL;

/* Input buffer for CLI commands */
static char pcInputBuffer[CLI_INPUT_BUFFER_SIZE];
static char pcOutputBuffer[CLI_OUTPUT_BUFFER_SIZE];
static uint8_t ucInputIndex = 0;

/* Function prototypes */
void vCLITask( void *pvParameters );
extern void vRegisterCLICommands( void );

/* UART functions using MCC-generated drivers */
static void prvUARTSend( const char *pcString )
{
    while( *pcString )
    {
        /* Use MCC-generated UART1 write function */
        while( !UART1_IsTxReady() );
        UART1_Write( *pcString++ );
    }
}

static char cUARTReceive( void )
{
    /* Use MCC-generated UART1 read function */
    while( !UART1_IsRxReady() );
    return UART1_Read();
}

/* CLI Task */
void vCLITask( void *pvParameters )
{
    char cRxedChar;
    BaseType_t xMoreDataToFollow;
    
    (void)pvParameters;
    
    /* Send welcome message */
    prvUARTSend( "\r\nFreeRTOS CLI Ready\r\nType 'help' for available commands\r\n> " );
    
    for( ;; )
    {
        /* Wait for character */
        cRxedChar = cUARTReceive();
        
        /* Echo character back */
        if( cRxedChar == '\r' || cRxedChar == '\n' )
        {
            prvUARTSend( "\r\n" );
            
            /* Process the command if we have input */
            if( ucInputIndex > 0 )
            {
                pcInputBuffer[ucInputIndex] = '\0';
                
                do
                {
                    /* Process the command */
                    xMoreDataToFollow = FreeRTOS_CLIProcessCommand(
                        pcInputBuffer,
                        pcOutputBuffer,
                        CLI_OUTPUT_BUFFER_SIZE
                    );
                    
                    /* Send the output */
                    prvUARTSend( pcOutputBuffer );
                    
                } while( xMoreDataToFollow != pdFALSE );
                
                /* Reset input buffer */
                ucInputIndex = 0;
                memset( pcInputBuffer, 0, CLI_INPUT_BUFFER_SIZE );
            }
            
            /* Send prompt */
            prvUARTSend( "> " );
        }
        else if( cRxedChar == '\b' || cRxedChar == 0x7F ) /* Backspace */
        {
            if( ucInputIndex > 0 )
            {
                ucInputIndex--;
                pcInputBuffer[ucInputIndex] = '\0';
                prvUARTSend( "\b \b" ); /* Backspace, space, backspace */
            }
        }
        else if( cRxedChar >= 0x20 && cRxedChar <= 0x7E ) /* Printable characters */
        {
            if( ucInputIndex < (CLI_INPUT_BUFFER_SIZE - 1) )
            {
                pcInputBuffer[ucInputIndex] = cRxedChar;
                ucInputIndex++;
                
                /* Echo character */
                char temp[2] = {cRxedChar, '\0'};
                prvUARTSend( temp );
            }
        }
    }
}

/* Create CLI Task */
void vCreateCLITask( void )
{
    /* Register CLI commands */
    vRegisterCLICommands();
    
    /* Create the task */
    xTaskCreate(
        vCLITask,
        "CLI",
        CLI_TASK_STACK_SIZE,
        NULL,
        CLI_TASK_PRIORITY,
        &xCLITaskHandle
    );
}