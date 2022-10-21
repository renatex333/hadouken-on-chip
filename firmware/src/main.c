/************************************************************************
 * 5 semestre - Eng. da Computao - Insper
 *
 * 2021 - Exemplo com HC05 com RTOS
 *
 */

#include <asf.h>
#include "conf_board.h"
#include <string.h>

/************************************************************************/
/* defines                                                              */
/************************************************************************/

// LEDs
#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_IDX 8
#define LED_IDX_MASK (1 << LED_IDX)

// #region

// Botão
#define BUT_PIO PIOA
#define BUT_PIO_ID ID_PIOA
#define BUT_IDX 11
#define BUT_IDX_MASK (1 << BUT_IDX)

// Botão Azul 1 - PC31
#define BUT_PIO_BLUE_1 PIOC
#define BUT_PIO_ID_BLUE_1 ID_PIOC
#define BUT_IDX_BLUE_1 31
#define BUT_IDX_MASK_BLUE_1 (1 << BUT_IDX_BLUE_1)
// Botão Azul 2 - PA19
#define BUT_PIO_BLUE_2 PIOA
#define BUT_PIO_ID_BLUE_2 ID_PIOA
#define BUT_IDX_BLUE_2 19
#define BUT_IDX_MASK_BLUE_2 (1 << BUT_IDX_BLUE_2)
// Botão Azul 3 - PB3
#define BUT_PIO_BLUE_3 PIOB
#define BUT_PIO_ID_BLUE_3 ID_PIOB
#define BUT_IDX_BLUE_3 3
#define BUT_IDX_MASK_BLUE_3 (1 << BUT_IDX_BLUE_3)
// Botão Azul 4 - PB2
#define BUT_PIO_BLUE_4 PIOB
#define BUT_PIO_ID_BLUE_4 ID_PIOB
#define BUT_IDX_BLUE_4 2
#define BUT_IDX_MASK_BLUE_4 (1 << BUT_IDX_BLUE_4)

// Botão Vermelho 5 - PD30
#define BUT_PIO_RED_5 PIOD
#define BUT_PIO_ID_RED_5 ID_PIOD
#define BUT_IDX_RED_5 30
#define BUT_IDX_MASK_RED_5 (1 << BUT_IDX_RED_5)
// Botão Vermelho 6 - PC13
#define BUT_PIO_RED_6 PIOC
#define BUT_PIO_ID_RED_6 ID_PIOC
#define BUT_IDX_RED_6 13
#define BUT_IDX_MASK_RED_6 (1 << BUT_IDX_RED_6)
// Botão Vermelho 7 - PA6
#define BUT_PIO_RED_7 PIOA
#define BUT_PIO_ID_RED_7 ID_PIOA
#define BUT_IDX_RED_7 6
#define BUT_IDX_MASK_RED_7 (1 << BUT_IDX_RED_7)
// Botão Vermelho 8 - PD11
#define BUT_PIO_RED_8 PIOD
#define BUT_PIO_ID_RED_8 ID_PIOD
#define BUT_IDX_RED_8 11
#define BUT_IDX_MASK_RED_8 (1 << BUT_IDX_RED_8)

// #endregion

// usart (bluetooth ou serial)
// Descomente para enviar dados
// pela serial debug

//#define DEBUG_SERIAL

#ifdef DEBUG_SERIAL
#define USART_COM USART1
#define USART_COM_ID ID_USART1
#else
#define USART_COM USART0
#define USART_COM_ID ID_USART0
#endif

/************************************************************************/
/* RTOS                                                                 */
/************************************************************************/

#define TASK_BLUETOOTH_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_BLUETOOTH_STACK_PRIORITY (tskIDLE_PRIORITY)

// task Button Handler
#define TASK_BUTTON_HANDLER_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_BUTTON_HANDLER_STACK_PRIORITY (tskIDLE_PRIORITY)

// Queue dos botoes
QueueHandle_t xQueueButton;
/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
										  signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern void xPortSysTickHandler(void);

/************************************************************************/
/* constants                                                            */
/************************************************************************/

/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

/************************************************************************/
/* RTOS application HOOK                                                */
/************************************************************************/

/* Called if stack overflow during execution */
extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
										  signed char *pcTaskName)
{
	printf("stack overflow %x %s\r\n", pxTask, (portCHAR *)pcTaskName);
	/* If the parameters have been corrupted then inspect pxCurrentTCB to
	 * identify which task has overflowed its stack.
	 */
	for (;;)
	{
	}
}

/* This function is called by FreeRTOS idle task */
extern void vApplicationIdleHook(void)
{
	pmc_sleep(SAM_PM_SMODE_SLEEP_WFI);
}

/* This function is called by FreeRTOS each tick */
extern void vApplicationTickHook(void) {}

extern void vApplicationMallocFailedHook(void)
{
	/* Called if a call to pvPortMalloc() fails because there is insufficient
	free memory available in the FreeRTOS heap.  pvPortMalloc() is called
	internally by FreeRTOS API functions that create tasks, queues, software
	timers, and semaphores.  The size of the FreeRTOS heap is set by the
	configTOTAL_HEAP_SIZE configuration constant in FreeRTOSConfig.h. */

	/* Force an assert. */
	configASSERT((volatile void *)NULL);
}

/************************************************************************/
/* handlers / callbacks                                                 */
/************************************************************************/

// #region  handlers dos botões
void but_callback_blue_1(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char button = '1';
	xQueueSendFromISR(xQueueButton, &button, &xHigherPriorityTaskWoken);
}
void but_callback_blue_2(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char button = '2';
	xQueueSendFromISR(xQueueButton, &button, &xHigherPriorityTaskWoken);
}
void but_callback_blue_3(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char button = '3';
	xQueueSendFromISR(xQueueButton, &button, &xHigherPriorityTaskWoken);
}
void but_callback_blue_4(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char button = '4';
	xQueueSendFromISR(xQueueButton, &button, &xHigherPriorityTaskWoken);
}
void but_callback_red_5(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char button = '5';
	xQueueSendFromISR(xQueueButton, &button, &xHigherPriorityTaskWoken);
}
void but_callback_red_6(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char button = '6';
	xQueueSendFromISR(xQueueButton, &button, &xHigherPriorityTaskWoken);
}
void but_callback_red_7(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char button = '7';
	xQueueSendFromISR(xQueueButton, &button, &xHigherPriorityTaskWoken);
}
void but_callback_red_8(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char button = '8';
	xQueueSendFromISR(xQueueButton, &button, &xHigherPriorityTaskWoken);
}
// #endregion

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

void io_init(void)
{

	// Ativa PIOs
	pmc_enable_periph_clk(LED_PIO_ID);

	pmc_enable_periph_clk(BUT_PIO_ID_BLUE_1);
	pmc_enable_periph_clk(BUT_PIO_ID_BLUE_2);
	pmc_enable_periph_clk(BUT_PIO_ID_BLUE_3);
	pmc_enable_periph_clk(BUT_PIO_ID_BLUE_4);
	pmc_enable_periph_clk(BUT_PIO_ID_RED_5);
	pmc_enable_periph_clk(BUT_PIO_ID_RED_6);
	pmc_enable_periph_clk(BUT_PIO_ID_RED_7);
	pmc_enable_periph_clk(BUT_PIO_ID_RED_8);

	// Configura Pinos

	pio_configure(BUT_PIO_BLUE_1, PIO_INPUT, BUT_IDX_MASK_BLUE_1, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT_PIO_BLUE_2, PIO_INPUT, BUT_IDX_MASK_BLUE_2, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT_PIO_BLUE_3, PIO_INPUT, BUT_IDX_MASK_BLUE_3, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT_PIO_BLUE_4, PIO_INPUT, BUT_IDX_MASK_BLUE_4, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT_PIO_RED_5, PIO_INPUT, BUT_IDX_MASK_RED_5, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT_PIO_RED_6, PIO_INPUT, BUT_IDX_MASK_RED_6, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT_PIO_RED_7, PIO_INPUT, BUT_IDX_MASK_RED_7, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(BUT_PIO_RED_8, PIO_INPUT, BUT_IDX_MASK_RED_8, PIO_PULLUP | PIO_DEBOUNCE);

	// Configura interrupcao

	pio_handler_set(BUT_PIO_BLUE_1, BUT_PIO_ID_BLUE_1, BUT_IDX_MASK_BLUE_1, PIO_IT_FALL_EDGE, but_callback_blue_1);
	pio_handler_set(BUT_PIO_BLUE_2, BUT_PIO_ID_BLUE_2, BUT_IDX_MASK_BLUE_2, PIO_IT_FALL_EDGE, but_callback_blue_2);
	pio_handler_set(BUT_PIO_BLUE_3, BUT_PIO_ID_BLUE_3, BUT_IDX_MASK_BLUE_3, PIO_IT_FALL_EDGE, but_callback_blue_3);
	pio_handler_set(BUT_PIO_BLUE_4, BUT_PIO_ID_BLUE_4, BUT_IDX_MASK_BLUE_4, PIO_IT_FALL_EDGE, but_callback_blue_4);
	pio_handler_set(BUT_PIO_RED_5, BUT_PIO_ID_RED_5, BUT_IDX_MASK_RED_5, PIO_IT_FALL_EDGE, but_callback_red_5);
	pio_handler_set(BUT_PIO_RED_6, BUT_PIO_ID_RED_6, BUT_IDX_MASK_RED_6, PIO_IT_FALL_EDGE, but_callback_red_6);
	pio_handler_set(BUT_PIO_RED_7, BUT_PIO_ID_RED_7, BUT_IDX_MASK_RED_7, PIO_IT_FALL_EDGE, but_callback_red_7);
	pio_handler_set(BUT_PIO_RED_8, BUT_PIO_ID_RED_8, BUT_IDX_MASK_RED_8, PIO_IT_FALL_EDGE, but_callback_red_8);

	// Ativa interrupcao

	pio_enable_interrupt(BUT_PIO_BLUE_1, BUT_IDX_MASK_BLUE_1);
	pio_enable_interrupt(BUT_PIO_BLUE_2, BUT_IDX_MASK_BLUE_2);
	pio_enable_interrupt(BUT_PIO_BLUE_3, BUT_IDX_MASK_BLUE_3);
	pio_enable_interrupt(BUT_PIO_BLUE_4, BUT_IDX_MASK_BLUE_4);
	pio_enable_interrupt(BUT_PIO_RED_5, BUT_IDX_MASK_RED_5);
	pio_enable_interrupt(BUT_PIO_RED_6, BUT_IDX_MASK_RED_6);
	pio_enable_interrupt(BUT_PIO_RED_7, BUT_IDX_MASK_RED_7);
	pio_enable_interrupt(BUT_PIO_RED_8, BUT_IDX_MASK_RED_8);

	pio_get_interrupt_status(BUT_PIO_BLUE_1);
	pio_get_interrupt_status(BUT_PIO_BLUE_2);
	pio_get_interrupt_status(BUT_PIO_BLUE_3);
	pio_get_interrupt_status(BUT_PIO_BLUE_4);
	pio_get_interrupt_status(BUT_PIO_RED_5);
	pio_get_interrupt_status(BUT_PIO_RED_6);
	pio_get_interrupt_status(BUT_PIO_RED_7);
	pio_get_interrupt_status(BUT_PIO_RED_8);

	// Configura NVIC

	NVIC_EnableIRQ(BUT_PIO_ID_BLUE_1);
	NVIC_EnableIRQ(BUT_PIO_ID_BLUE_2);
	NVIC_EnableIRQ(BUT_PIO_ID_BLUE_3);
	NVIC_EnableIRQ(BUT_PIO_ID_BLUE_4);
	NVIC_EnableIRQ(BUT_PIO_ID_RED_5);
	NVIC_EnableIRQ(BUT_PIO_ID_RED_6);
	NVIC_EnableIRQ(BUT_PIO_ID_RED_7);
	NVIC_EnableIRQ(BUT_PIO_ID_RED_8);
	NVIC_SetPriority(BUT_PIO_ID, 4);
	NVIC_SetPriority(BUT_PIO_ID_BLUE_1, 4);
	NVIC_SetPriority(BUT_PIO_ID_BLUE_2, 4);
	NVIC_SetPriority(BUT_PIO_ID_BLUE_3, 4);
	NVIC_SetPriority(BUT_PIO_ID_BLUE_4, 4);
	NVIC_SetPriority(BUT_PIO_ID_RED_5, 4);
	NVIC_SetPriority(BUT_PIO_ID_RED_6, 4);
	NVIC_SetPriority(BUT_PIO_ID_RED_7, 4);
	NVIC_SetPriority(BUT_PIO_ID_RED_8, 4);
}

static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#if (defined CONF_UART_CHAR_LENGTH)
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#if (defined CONF_UART_STOP_BITS)
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	stdio_serial_init(CONF_UART, &uart_serial_options);

/* Specify that stdout should not be buffered. */
#if defined(__GNUC__)
	setbuf(stdout, NULL);
#else
/* Already the case in IAR's Normal DLIB default configuration: printf()
 * emits one character at a time.
 */
#endif
}

uint32_t usart_puts(uint8_t *pstring)
{
	uint32_t i;

	while (*(pstring + i))
		if (uart_is_tx_empty(USART_COM))
			usart_serial_putchar(USART_COM, *(pstring + i++));
}

void usart_put_string(Usart *usart, char str[])
{
	usart_serial_write_packet(usart, str, strlen(str));
}

int usart_get_string(Usart *usart, char buffer[], int bufferlen, uint timeout_ms)
{
	uint timecounter = timeout_ms;
	uint32_t rx;
	uint32_t counter = 0;

	while ((timecounter > 0) && (counter < bufferlen - 1))
	{
		if (usart_read(usart, &rx) == 0)
		{
			buffer[counter++] = rx;
		}
		else
		{
			timecounter--;
			vTaskDelay(1);
		}
	}
	buffer[counter] = 0x00;
	return counter;
}

void usart_send_command(Usart *usart, char buffer_rx[], int bufferlen,
						char buffer_tx[], int timeout)
{
	usart_put_string(usart, buffer_tx);
	usart_get_string(usart, buffer_rx, bufferlen, timeout);
}

void config_usart0(void)
{
	sysclk_enable_peripheral_clock(ID_USART0);
	usart_serial_options_t config;
	config.baudrate = 9600;
	config.charlength = US_MR_CHRL_8_BIT;
	config.paritytype = US_MR_PAR_NO;
	config.stopbits = false;
	usart_serial_init(USART0, &config);
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);

	// RX - PB0  TX - PB1
	pio_configure(PIOB, PIO_PERIPH_C, (1 << 0), PIO_DEFAULT);
	pio_configure(PIOB, PIO_PERIPH_C, (1 << 1), PIO_DEFAULT);
}

int hc05_init(void)
{
	char buffer_rx[128];
	usart_send_command(USART_COM, buffer_rx, 1000, "AT", 100);
	vTaskDelay(500 / portTICK_PERIOD_MS);
	usart_send_command(USART_COM, buffer_rx, 1000, "AT", 100);
	vTaskDelay(500 / portTICK_PERIOD_MS);
	usart_send_command(USART_COM, buffer_rx, 1000, "AT+NAMECbrutius", 100);
	vTaskDelay(500 / portTICK_PERIOD_MS);
	usart_send_command(USART_COM, buffer_rx, 1000, "AT", 100);
	vTaskDelay(500 / portTICK_PERIOD_MS);
	usart_send_command(USART_COM, buffer_rx, 1000, "AT+PIN1234", 100);
}

/************************************************************************/
/* TASKS                                                                */
/************************************************************************/

void task_bluetooth(void)
{
	printf("Task Bluetooth started \n");

	printf("Inicializando HC05 \n");
	config_usart0();
	hc05_init();

	// configura LEDs e Botões
	io_init();

	char eof = 'X';

	// Task não deve retornar.
	while (1)
	{
		char button = '0';
		xQueueReceive(xQueueButton, &button, portMAX_DELAY);
		// envia status botão
		while (!usart_is_tx_ready(USART_COM))
		{
			vTaskDelay(10 / portTICK_PERIOD_MS);
		}
		usart_write(USART_COM, button);

		while (!usart_is_tx_ready(USART_COM))
		{
			vTaskDelay(10 / portTICK_PERIOD_MS);
		}
		usart_write(USART_COM, eof);

		// dorme por 500 ms
		vTaskDelay(1 / portTICK_PERIOD_MS);
	}
}
void task_button_handler(void)
{
	printf("Task Button Handler started \n");

	// Task não deve retornar.
	while (1)
	{
		// le a fila e printa o valor

	}
}
	/************************************************************************/
	/* main                                                                 */
	/************************************************************************/

	int main(void)
	{
		/* Initialize the SAM system */
		sysclk_init();
		board_init();

		configure_console();

		xQueueButton = xQueueCreate(1, sizeof(char));
		if (xQueueButton == NULL)
		{
			printf("Erro ao criar fila de botões");
		}

		/* Create task to make led blink */
		xTaskCreate(task_bluetooth, "BLT", TASK_BLUETOOTH_STACK_SIZE, NULL, TASK_BLUETOOTH_STACK_PRIORITY, NULL);

		/* Create task to handle button */
		xTaskCreate(task_button_handler, "BTN", TASK_BUTTON_HANDLER_STACK_SIZE, NULL, TASK_BUTTON_HANDLER_STACK_PRIORITY, NULL);

		/* Start the scheduler. */
		vTaskStartScheduler();

		while (1)
		{
		}

		/* Will only get here if there was insufficient memory to create the idle task. */
		return 0;
	}
