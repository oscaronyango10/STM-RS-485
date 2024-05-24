#include "main.h"
#include "stm32f1xx_hal.h"

// Definitions for the DE/RE control pin
#define RS485_DE_RE_PIN GPIO_PIN_0
#define RS485_DE_RE_GPIO_PORT GPIOA

UART_HandleTypeDef huart1;

// Function prototypes
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void RS485_Transmit(uint8_t *pData, uint16_t size);
void RS485_Receive(uint8_t *pData, uint16_t size);

int main(void)
{
  // Initialize the HAL Library
  HAL_Init();

  // Configure the system clock
  SystemClock_Config();

  // Initialize all configured peripherals
  MX_GPIO_Init();
  MX_USART1_UART_Init();

  // Example data to transmit
  uint8_t txData[] = "Hello RS-485";
  uint8_t rxData[20];

  // Transmit data over RS-485
  RS485_Transmit(txData, sizeof(txData));

  // Receive data over RS-485
  RS485_Receive(rxData, sizeof(rxData));

  while (1)
  {
    // Main loop
  }
}

// RS-485 transmit function
void RS485_Transmit(uint8_t *pData, uint16_t size)
{
  // Set DE/RE pin to high to enable transmission
  HAL_GPIO_WritePin(RS485_DE_RE_GPIO_PORT, RS485_DE_RE_PIN, GPIO_PIN_SET);

  // Transmit data using UART
  HAL_UART_Transmit(&huart1, pData, size, HAL_MAX_DELAY);

  // Set DE/RE pin to low to enable reception
  HAL_GPIO_WritePin(RS485_DE_RE_GPIO_PORT, RS485_DE_RE_PIN, GPIO_PIN_RESET);
}

// RS-485 receive function
void RS485_Receive(uint8_t *pData, uint16_t size)
{
  // Ensure DE/RE pin is low to enable reception
  HAL_GPIO_WritePin(RS485_DE_RE_GPIO_PORT, RS485_DE_RE_PIN, GPIO_PIN_RESET);

  // Receive data using UART
  HAL_UART_Receive(&huart1, pData, size, HAL_MAX_DELAY);
}

// USART1 Initialization Function
static void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
}

// GPIO Initialization Function
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  // Enable GPIO ports clock
  __HAL_RCC_GPIOA_CLK_ENABLE();

  // Configure GPIO pin for RS-485 DE/RE control
  GPIO_InitStruct.Pin = RS485_DE_RE_PIN;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(RS485_DE_RE_GPIO_PORT, &GPIO_InitStruct);
}

// System Clock Configuration
void SystemClock_Config(void)
{
  // Configure the system clock
}

// Error Handler
void Error_Handler(void)
{
  // User can add their own implementation to report the HAL error return state
}
