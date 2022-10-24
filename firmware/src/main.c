/************************************************************************
 * 5 semestre - Eng. da Computao - Insper
 *
 * 2021 - Exemplo com HC05 com RTOS
 *
 */

#include <asf.h>
#include "conf_board.h"
#include <string.h>
#include "Botoes.h"

/************************************************************************/
/* defines                                                              */
/************************************************************************/

// LEDs
#define LED_PIO PIOC
#define LED_PIO_ID ID_PIOC
#define LED_IDX 8
#define LED_IDX_MASK (1 << LED_IDX)

// Botão
#define BUT_PIO PIOA
#define BUT_PIO_ID ID_PIOA
#define BUT_IDX 11
#define BUT_IDX_MASK (1 << BUT_IDX)

// Entrada da leitura do potenciometro que vai alterar o volume
#define AFEC_POT AFEC0
#define AFEC_POT_ID ID_AFEC0
#define AFEC_POT_CHANNEL 0 // Canal do pino PD30


// JOYSTICK
// Primeira entrada
#define JOY1_PIO PIOD
#define JOY1_PIO_ID ID_PIOD
#define JOY1_IDX 22
#define JOY1_IDX_MASK (1 << JOY1_IDX)

#define JOY2_PIO PIOB
#define JOY2_PIO_ID ID_PIOB
#define JOY2_IDX 3
#define JOY2_IDX_MASK (1 << JOY2_IDX)

#define JOY3_PIO PIOA
#define JOY3_PIO_ID ID_PIOA
#define JOY3_IDX 5
#define JOY3_IDX_MASK (1 << JOY3_IDX)

#define JOY4_PIO PIOD
#define JOY4_PIO_ID ID_PIOD
#define JOY4_IDX 12
#define JOY4_IDX_MASK (1 << JOY4_IDX)

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

// task Volume Handler
#define TASK_VOL_STACK_SIZE (1024 * 10 / sizeof(portSTACK_TYPE))
#define TASK_VOL_STACK_PRIORITY (tskIDLE_PRIORITY)

// task Button Handler
#define TASK_BUTTON_HANDLER_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_BUTTON_HANDLER_STACK_PRIORITY (tskIDLE_PRIORITY)

// task Joystick Handler
#define TASK_JOY_STACK_SIZE (4096 / sizeof(portSTACK_TYPE))
#define TASK_JOY_STACK_PRIORITY (tskIDLE_PRIORITY)

/************************************************************************/
/* recursos RTOS                                                        */
/************************************************************************/

TimerHandle_t xTimer;
/** Queue for msg log send data */
QueueHandle_t xQueueVOL;
// Queue dos botoes
QueueHandle_t xQueueButton;
// Queue do Joystick
QueueHandle_t xQueueJoy;

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

// handler do joystick

void joy1_callback(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char direcao = 'joy1';
	xQueueSendFromISR(xQueueJoy, &direcao, &xHigherPriorityTaskWoken);
}

void joy2_callback(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char direcao = 'joy2';
	xQueueSendFromISR(xQueueJoy, &direcao, &xHigherPriorityTaskWoken);
}

void joy3_callback(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char direcao = 'joy3';
	xQueueSendFromISR(xQueueJoy, &direcao, &xHigherPriorityTaskWoken);
}

void joy4_callback(void)
{
	BaseType_t xHigherPriorityTaskWoken = pdTRUE;
	char direcao = 'joy4';
	xQueueSendFromISR(xQueueJoy, &direcao, &xHigherPriorityTaskWoken);
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

void joy_init(void)
{

	// Configura Pinos

	pio_configure(JOY1_PIO, PIO_INPUT, JOY1_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(JOY2_PIO, PIO_INPUT, JOY2_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(JOY3_PIO, PIO_INPUT, JOY3_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);
	pio_configure(JOY4_PIO, PIO_INPUT, JOY4_IDX_MASK, PIO_PULLUP | PIO_DEBOUNCE);

	pio_handler_set(JOY1_PIO, JOY1_PIO_ID, JOY1_IDX_MASK, PIO_IT_FALL_EDGE, joy1_callback);
	pio_handler_set(JOY2_PIO, JOY2_PIO_ID, JOY2_IDX_MASK, PIO_IT_FALL_EDGE, joy2_callback);
	pio_handler_set(JOY3_PIO, JOY3_PIO_ID, JOY3_IDX_MASK, PIO_IT_FALL_EDGE, joy3_callback);
	pio_handler_set(JOY4_PIO, JOY4_PIO_ID, JOY4_IDX_MASK, PIO_IT_FALL_EDGE, joy4_callback);

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
		char direcao = 'joy';
		xQueueReceive(xQueueButton, &button, portMAX_DELAY);
		xQueueReceive(xQueueJoy, &direcao, portMAX_DELAY);
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

void task_joy(void)
{
	printf("Task Joystick started \n");
	joy_init();

	// Task não deve retornar.
	while (1)
	{
		// le a fila e printa o valor

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
      printf("Leit: %d \n", leitura);
    } else {
      printf("Nao chegou um novo dado em 1 segundo \n");
    }
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

	xQueueJoy = xQueueCreate(1, sizeof(char));
	if (xQueueJoy == NULL)
	{
		printf("Erro ao criar fila de botões");
	}
	
	xQueueVOL = xQueueCreate(100, sizeof(uint));
	if (xQueueVOL == NULL) {
		printf("falha em criar a queue xQueueVOL \n");
	}

	/* Create task to make led blink */
	xTaskCreate(task_bluetooth, "BLT", TASK_BLUETOOTH_STACK_SIZE, NULL, TASK_BLUETOOTH_STACK_PRIORITY, NULL);

	/* Create task joystick */
	xTaskCreate(task_joy, "JOY", TASK_JOY_STACK_SIZE, NULL, TASK_JOY_STACK_PRIORITY, NULL);

	/* Create task to handle button */
	xTaskCreate(task_button_handler, "BTN", TASK_BUTTON_HANDLER_STACK_SIZE, NULL, TASK_BUTTON_HANDLER_STACK_PRIORITY, NULL);
	
	if (xTaskCreate(task_vol, "VOL", TASK_VOL_STACK_SIZE, NULL,
	TASK_VOL_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to create test VOL task\r\n");
	}

	/* Start the scheduler. */
	vTaskStartScheduler();

	while (1)
	{
	}

	/* Will only get here if there was insufficient memory to create the idle task. */
	return 0;
}
