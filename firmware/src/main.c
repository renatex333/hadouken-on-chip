/************************************************************************
 * 5 semestre - Eng. da Computao - Insper
 *
 * Projeto de Computação Embarcada
 * Por: Lucas, Matheus e Renato
 */

#include <asf.h>
#include "conf_board.h"
#include <string.h>
#include "helpers/helpers.h"
#include "ledHelpers/dataTypes.h"
#include "ledHelpers/ledColors.h"

/************************************************************************/
/* RTOS                                                                 */
/************************************************************************/

#define TASK_BLUETOOTH_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_BLUETOOTH_STACK_PRIORITY (tskIDLE_PRIORITY)

// task Volume Handler
#define TASK_VOL_STACK_SIZE (1024 * 10 / sizeof(portSTACK_TYPE))
#define TASK_VOL_STACK_PRIORITY (tskIDLE_PRIORITY)

// task Button Handler
#define TASK_BUTTON_HANDLER_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_BUTTON_HANDLER_STACK_PRIORITY (tskIDLE_PRIORITY)

// task Joystick Handler
#define TASK_JOY_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_JOY_STACK_PRIORITY (tskIDLE_PRIORITY)

// task Handshake
#define TASK_HANDSHAKE_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_HANDSHAKE_STACK_PRIORITY (tskIDLE_PRIORITY)

// task Led
#define TASK_LED_STACK_SIZE (4096*8 / sizeof(portSTACK_TYPE))
#define TASK_LED_STACK_PRIORITY        (tskIDLE_PRIORITY)

/************************************************************************/
/* recursos RTOS                                                        */
/************************************************************************/

TimerHandle_t xTimer;

// Queue do volume
QueueHandle_t xQueueVOL;

// Queue dos botoes
QueueHandle_t xQueueButton;

// Queue do Joystick
QueueHandle_t xQueueJoy;

// Queue da fita de led
QueueHandle_t xQueueLed;

/************************************************************************/
/* prototypes                                                           */
/************************************************************************/

extern void vApplicationStackOverflowHook(xTaskHandle *pxTask,
										  signed char *pcTaskName);
extern void vApplicationIdleHook(void);
extern void vApplicationTickHook(void);
extern void vApplicationMallocFailedHook(void);
extern void xPortSysTickHandler(void);
static void config_AFEC_pot(Afec *afec, uint32_t afec_id, uint32_t afec_channel,
afec_callback_t callback);
void envia_dado(char comando);
void create_tasks(void);


/************************************************************************/
/* variaveis globais                                                    */
/************************************************************************/

volatile char handshake;
volatile char apertado;

/************************************************************************/
/* prototypes local                                                     */
/************************************************************************/

static void LED_init(void);
void LEDS_light_up(char *, int);

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


// handler do joystick

void joy1_callback(void)
{
	if (pio_get(JOY1_PIO, PIO_INPUT, JOY1_IDX_MASK)) {
		// PINO == 1 --> Borda de subida
		apertado = 0;
		} else {
		// PINO == 0 --> Borda de descida
		BaseType_t xHigherPriorityTaskWoken = pdTRUE;
		char direcao = 'd';
		xQueueSendFromISR(xQueueJoy, &direcao, &xHigherPriorityTaskWoken);
		apertado = 1;
	}
	
}

void joy2_callback(void)
{
	if (pio_get(JOY2_PIO, PIO_INPUT, JOY2_IDX_MASK)) {
		// PINO == 1 --> Borda de subida
		apertado = 0;
		} else {
		// PINO == 0 --> Borda de descida
		BaseType_t xHigherPriorityTaskWoken = pdTRUE;
		char direcao = 's';
		xQueueSendFromISR(xQueueJoy, &direcao, &xHigherPriorityTaskWoken);
		apertado = 1;
	}
}

void joy3_callback(void)
{
		if (pio_get(JOY3_PIO, PIO_INPUT, JOY3_IDX_MASK)) {
			// PINO == 1 --> Borda de subida
			apertado = 0;
			} else {
			// PINO == 0 --> Borda de descida
			BaseType_t xHigherPriorityTaskWoken = pdTRUE;
			char direcao = 'a';
			xQueueSendFromISR(xQueueJoy, &direcao, &xHigherPriorityTaskWoken);
			apertado = 1;
		}
}

void joy4_callback(void)
{
	if (pio_get(JOY4_PIO, PIO_INPUT, JOY4_IDX_MASK)) {
		// PINO == 1 --> Borda de subida
		apertado = 0;
		} else {
		// PINO == 0 --> Borda de descida
		BaseType_t xHigherPriorityTaskWoken = pdTRUE;
		char direcao = 'w';
		xQueueSendFromISR(xQueueJoy, &direcao, &xHigherPriorityTaskWoken);
		apertado = 1;
	}
}


// handler do volume
static void AFEC_pot_callback(void) {
	uint leitura;
	leitura = afec_channel_get_value(AFEC_POT, AFEC_POT_CHANNEL);
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	xQueueSendFromISR(xQueueVOL, &leitura, &xHigherPriorityTaskWoken);
}

// #endregion

/************************************************************************/
/* funcoes                                                              */
/************************************************************************/

void io_init(void)
{

	// Ativa PIOs

	pmc_enable_periph_clk(BUT_PIO_ID_BLUE_1);
	pmc_enable_periph_clk(BUT_PIO_ID_BLUE_2);
	pmc_enable_periph_clk(BUT_PIO_ID_BLUE_3);
	pmc_enable_periph_clk(BUT_PIO_ID_BLUE_4);
	pmc_enable_periph_clk(BUT_PIO_ID_RED_5);
	pmc_enable_periph_clk(BUT_PIO_ID_RED_6);
	pmc_enable_periph_clk(BUT_PIO_ID_RED_7);
	pmc_enable_periph_clk(BUT_PIO_ID_RED_8);

	
	// Configura Pinos
	pio_set_debounce_filter(BUT_PIO_BLUE_1, BUT_IDX_MASK_BLUE_1, 120);
	pio_set_debounce_filter(BUT_PIO_BLUE_1, BUT_IDX_MASK_BLUE_1, 120);
	pio_set_debounce_filter(BUT_PIO_BLUE_1, BUT_IDX_MASK_BLUE_1, 120);
	pio_set_debounce_filter(BUT_PIO_BLUE_1, BUT_IDX_MASK_BLUE_1, 120);
	pio_set_debounce_filter(BUT_PIO_RED_5, BUT_IDX_MASK_RED_5, 120);
	pio_set_debounce_filter(BUT_PIO_RED_6, BUT_IDX_MASK_RED_6, 120);
	pio_set_debounce_filter(BUT_PIO_RED_7, BUT_IDX_MASK_RED_7, 120);
	pio_set_debounce_filter(BUT_PIO_RED_8, BUT_IDX_MASK_RED_8, 120);

		
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

	NVIC_SetPriority(BUT_PIO_ID_BLUE_1, 4);
	NVIC_SetPriority(BUT_PIO_ID_BLUE_2, 4);
	NVIC_SetPriority(BUT_PIO_ID_BLUE_3, 4);
	NVIC_SetPriority(BUT_PIO_ID_BLUE_4, 4);
	NVIC_SetPriority(BUT_PIO_ID_RED_5, 4);
	NVIC_SetPriority(BUT_PIO_ID_RED_6, 4);
	NVIC_SetPriority(BUT_PIO_ID_RED_7, 4);
	NVIC_SetPriority(BUT_PIO_ID_RED_8, 4);

	
}

void joy_init(void)
{

	// Configura Pinos
	pio_set_debounce_filter(JOY1_PIO, JOY1_IDX_MASK, 120);
	pio_set_debounce_filter(JOY2_PIO, JOY2_IDX_MASK, 120);
	pio_set_debounce_filter(JOY3_PIO, JOY3_IDX_MASK, 120);
	pio_set_debounce_filter(JOY4_PIO, JOY4_IDX_MASK, 120);
	
	pio_configure(JOY1_PIO, PIO_INPUT, JOY1_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(JOY2_PIO, PIO_INPUT, JOY2_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(JOY3_PIO, PIO_INPUT, JOY3_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(JOY4_PIO, PIO_INPUT, JOY4_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);

	pio_handler_set(JOY1_PIO, JOY1_PIO_ID, JOY1_IDX_MASK, PIO_IT_EDGE, joy1_callback);
	pio_handler_set(JOY2_PIO, JOY2_PIO_ID, JOY2_IDX_MASK, PIO_IT_EDGE, joy2_callback);
	pio_handler_set(JOY3_PIO, JOY3_PIO_ID, JOY3_IDX_MASK, PIO_IT_EDGE, joy3_callback);
	pio_handler_set(JOY4_PIO, JOY4_PIO_ID, JOY4_IDX_MASK, PIO_IT_EDGE, joy4_callback);

	// Ativa interrupcao
	pio_enable_interrupt(JOY1_PIO, JOY1_IDX_MASK);
	pio_enable_interrupt(JOY2_PIO, JOY2_IDX_MASK);
	pio_enable_interrupt(JOY3_PIO, JOY3_IDX_MASK);
	pio_enable_interrupt(JOY4_PIO, JOY4_IDX_MASK);

	pio_get_interrupt_status(JOY1_PIO);
	pio_get_interrupt_status(JOY2_PIO);
	pio_get_interrupt_status(JOY3_PIO);
	pio_get_interrupt_status(JOY4_PIO);

	// Configura NVIC

	NVIC_EnableIRQ(JOY1_PIO_ID);
	NVIC_EnableIRQ(JOY2_PIO_ID);
	NVIC_EnableIRQ(JOY3_PIO_ID);
	NVIC_EnableIRQ(JOY4_PIO_ID);

	NVIC_SetPriority(JOY1_PIO_ID, 4);
	NVIC_SetPriority(JOY2_PIO_ID, 4);
	NVIC_SetPriority(JOY3_PIO_ID, 4);
	NVIC_SetPriority(JOY4_PIO_ID, 4);
}

static void config_AFEC_pot(Afec *afec, uint32_t afec_id, uint32_t afec_channel,
                            afec_callback_t callback) {
  /*************************************
   * Ativa e configura AFEC
   *************************************/
  /* Ativa AFEC - 0 */
  afec_enable(afec);

  /* struct de configuracao do AFEC */
  struct afec_config afec_cfg;

  /* Carrega parametros padrao */
  afec_get_config_defaults(&afec_cfg);

  /* Configura AFEC */
  afec_init(afec, &afec_cfg);

  /* Configura trigger por software */
  afec_set_trigger(afec, AFEC_TRIG_SW);

  /*** Configuracao específica do canal AFEC ***/
  struct afec_ch_config afec_ch_cfg;
  afec_ch_get_config_defaults(&afec_ch_cfg);
  afec_ch_cfg.gain = AFEC_GAINVALUE_0;
  afec_ch_set_config(afec, afec_channel, &afec_ch_cfg);

  /*
  * Calibracao:
  * Because the internal ADC offset is 0x200, it should cancel it and shift
  down to 0.
  */
  afec_channel_set_analog_offset(afec, afec_channel, 0x200);

  /***  Configura sensor de temperatura ***/
  struct afec_temp_sensor_config afec_temp_sensor_cfg;

  afec_temp_sensor_get_config_defaults(&afec_temp_sensor_cfg);
  afec_temp_sensor_set_config(afec, &afec_temp_sensor_cfg);

  /* configura IRQ */
  afec_set_callback(afec, afec_channel, callback, 1);
  NVIC_SetPriority(afec_id, 4);
  NVIC_EnableIRQ(afec_id);
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

void LED_init(void) {
	pmc_enable_periph_clk(PIN_DATA_ID);
	pio_configure(PIN_DATA, PIO_OUTPUT_0, PIN_DATA_IDX_MASK, PIO_DEFAULT);
}

void envia_dado(char comando){
	
	char eof = 'X';
	
	// envia comando
	while (!usart_is_tx_ready(USART_COM))
	{
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	usart_write(USART_COM, comando);

	while (!usart_is_tx_ready(USART_COM))
	{
		vTaskDelay(10 / portTICK_PERIOD_MS);
	}
	usart_write(USART_COM, eof);

	// dorme por 500 ms
	vTaskDelay(1 / portTICK_PERIOD_MS);
}

void LEDS_light_up(char *array, int n) {
	/* Acender todos os leds com apenas uma cor*/
	//for(int i = 0; i < n; i++) {
	//colorGREEN();
	//}
	
	/* Se quiser acender só alguns LEDS específicos */
	//for(int i = 0; i < n; i++) array[i] ? colorRED() : colorBLACK();
	
	/* Se quiser acender todos os leds com cada uma das 3 cores */
	for(int i = 0; i < n; i++) {
		colorGREEN();
	}
	delay_ms(500);
	for(int j = 0; j < n; j++) {
		colorBLUE();
	}
	delay_ms(500);
	for(int k = 0; k < n; k++) {
		colorRED();
	}
	
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
	
	// Task não deve retornar.
	while (1){
		}
}

void task_handshake(void)
{
	handshake = '1';
	char rx;
	
	// Task não deve retornar.
	while (1){
		if(handshake == '1'){
			if(usart_is_tx_ready(USART_COM)){
				printf("Enviando handshake \n");
				while (!usart_is_tx_ready(USART_COM))
				{
					vTaskDelay(10 / portTICK_PERIOD_MS);
				}
				usart_write(USART_COM, 'P');

				while (!usart_is_tx_ready(USART_COM))
				{
					vTaskDelay(10 / portTICK_PERIOD_MS);
				}
				usart_write(USART_COM, 'X');
			}
			
			if(!usart_read(USART_COM, &rx)) {
				if(rx == '1') {
					printf("handshake concluido \n");
					/*Criacao tasks dps do handshake */
					create_tasks();
					handshake = '0';
				}
				
			}
			vTaskDelay(10);
		}

	}
}
void task_button_handler(void)
{
	char botao = '0';

	/* tarefas de um RTOS não devem retornar */
	for (;;) {
	/* verifica se chegou algum dado na queue, e espera por 0 ticks */
	if (xQueueReceive(xQueueButton, &botao, (TickType_t) 0)) {
		/* chegou novo valor, atualiza delay ! */
		/* aqui eu poderia verificar se msg faz sentido (se esta no range certo)
		*/
		/* converte ms -> ticks */
		printf("botao: %c \n", botao);
		envia_dado(botao);
	}
	//else{
		//printf("botao: 0 \n",);
		//envia_dado('0');
	//}

	/* suspende por delayMs */
	vTaskDelay(100);
	}
}

static void task_joy(void *pvParameters) {
  joy_init();

  char direcao = 'joy';

  /* tarefas de um RTOS não devem retornar */
  for (;;) {
    /* verifica se chegou algum dado na queue, e espera por 0 ticks */
    if (xQueueReceive(xQueueJoy, &direcao, (TickType_t) 0)) {
      /* chegou novo valor, atualiza delay ! */
      /* aqui eu poderia verificar se msg faz sentido (se esta no range certo)
       */
      /* converte ms -> ticks */
      printf("direcao: %c \n", direcao);
	  while(apertado){
		  envia_dado(direcao);
	  }
    }

    /* suspende por delayMs */
    vTaskDelay(100);
  }
}

void vTimerCallback(TimerHandle_t xTimer) {
	/* Selecina canal e inicializa conversão */
	afec_channel_enable(AFEC_POT, AFEC_POT_CHANNEL);
	afec_start_software_conversion(AFEC_POT);
}

static void task_vol(void *pvParameters) {
	// configura ADC e TC para controlar a leitura
	config_AFEC_pot(AFEC_POT, AFEC_POT_ID, AFEC_POT_CHANNEL, AFEC_pot_callback);
	
  xTimer = xTimerCreate(/* Just a text name, not used by the RTOS
                        kernel. */
                        "Timer",
                        /* The timer period in ticks, must be
                        greater than 0. */
                        1000,
                        /* The timers will auto-reload themselves
                        when they expire. */
                        pdTRUE,
                        /* The ID is used to store a count of the
                        number of times the timer has expired, which
                        is initialised to 0. */
                        (void *)0,
                        /* Timer callback */
                        vTimerCallback);
  xTimerStart(xTimer, 0);

  // variável para recever dados da fila
  uint leitura;

  for(;;) {
    if (xQueueReceive(xQueueVOL, &(leitura), 2000)) {
      //printf("Leit: %d \n", leitura);
    } else {
      printf("Nao chegou um novo dado em 1 segundo \n");
    }
  }
}

static void task_led(void *pvParameters) {
	char leds[LEDS_NUMBER];
	for(;;){
		// Acende LEDs apenas se o botão for pressionado
		if (xQueueReceive(xQueueLed, leds, 100)) {
			taskENTER_CRITICAL();
			LEDS_light_up(leds, LEDS_NUMBER);
			taskEXIT_CRITICAL();
			vTaskDelay(100);
			clearLEDs();
		}
	}
}

void create_tasks(){
	/* Create task joystick */
	if (xTaskCreate(task_joy, "JOY",TASK_JOY_STACK_SIZE, NULL,
	TASK_JOY_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Falhou ao criar Joy task\r\n");
		} else {
		printf("task joy \r\n");
	}

	/* Create task to handle button */
	if (xTaskCreate(task_button_handler, "BUT", TASK_BUTTON_HANDLER_STACK_SIZE, NULL,
	TASK_BUTTON_HANDLER_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Falhou ao criar BUT task\r\n");
		} else {
		printf("task but \r\n");
	}
	
	if (xTaskCreate(task_vol, "VOL", TASK_VOL_STACK_SIZE, NULL,
	TASK_VOL_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Falhou ao criar VOL task\r\n");
	} else {
		printf("task vol \r\n");
	}
	
	if (xTaskCreate(task_led, "LED", TASK_LED_STACK_SIZE, NULL, 
	TASK_LED_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Falhou ao criar a task_led \r\n");
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

	LED_init();
	
	configure_console();

	xQueueButton = xQueueCreate(1, sizeof(char));
	if (xQueueButton == NULL)
	{
		printf("Erro ao criar fila de botões \n");
	}

	xQueueJoy = xQueueCreate(4, sizeof(char));
	if (xQueueJoy == NULL)
	{
		printf("Erro ao criar fila de botões \n");
	}
	
	xQueueVOL = xQueueCreate(100, sizeof(uint));
	if (xQueueVOL == NULL) {
		printf("Erro ao criar fila de volume \n");
	}
	
	xQueueLed = xQueueCreate(20, sizeof(char));
	if (xQueueLed == NULL){
		printf("Erro ao criar fila dos Leds \n");
	}
	

	
	/* Create task bluetooth */
	if (xTaskCreate(task_bluetooth, "BLT",TASK_BLUETOOTH_STACK_SIZE, NULL,
	TASK_BLUETOOTH_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create Bluetooth task\r\n");
		} else {
		printf("task bluetooth \r\n");
	}
	
	/* Create task Handshake */
	if (xTaskCreate(task_handshake, "BLT",TASK_HANDSHAKE_STACK_SIZE, NULL,
	TASK_HANDSHAKE_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Falhou ao criar Handshake task\r\n");
		} else {
		printf("task handshake \r\n");
	}

	/* Start the scheduler. */
	vTaskStartScheduler();

	while (1)
	{
	}

	/* Will only get here if there was insufficient memory to create the idle task. */
	return 0;
}
