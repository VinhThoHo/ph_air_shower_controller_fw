/*
* @Author: nhantt
* @Date:   2019-12-13
* @Last Modified by:   nhantt
* @Last Modified time: 2019-12-13
*/

#include "usart.h"
#include "ringbuf.h"

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

ringbuf ustRbRx;
uint8_t _ustRead[1024];
uint32_t ustTick;
uint8_t ustFlag;
uint8_t rxdata;

/* USART1 init function */
void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
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
  __HAL_UART_ENABLE_IT(&huart1, UART_IT_RXNE);
  ringbuf_init(&ustRbRx, _ustRead, sizeof(_ustRead));
}

/* USART2 init function */
void MX_USART2_UART_Init(void)
{

  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  __HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE);
  //ringbuf_init(&ustRbRx, _ustRead, sizeof(_ustRead));
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_10;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  }
  else if(uartHandle->Instance==USART2)
  {
    /* USART2 clock enable */
    __HAL_RCC_USART2_CLK_ENABLE();
  
    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* USART2 interrupt Init */
    HAL_NVIC_SetPriority(USART2_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART2_IRQn);
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PA9     ------> USART1_TX
    PA10     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9|GPIO_PIN_10);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  }
  else if(uartHandle->Instance==USART2)
  {
    /* Peripheral clock disable */
    __HAL_RCC_USART2_CLK_DISABLE();
  
    /**USART2 GPIO Configuration    
    PA2     ------> USART2_TX
    PA3     ------> USART2_RX 
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2|GPIO_PIN_3);

    /* USART2 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART2_IRQn);
  }
} 

void usart_rx_interrupt(UART_HandleTypeDef *huart)
{
  uint8_t rev_data;
  if(huart->Instance == USART1)
  {
    //printf("Usart1 interrupt\n");
    if((__HAL_UART_GET_IT_SOURCE(&huart1, UART_IT_RXNE) != RESET) 
      && (__HAL_UART_GET_FLAG(&huart1, UART_FLAG_RXNE) != RESET))
    {
      __HAL_UART_CLEAR_FLAG(&huart1, UART_FLAG_RXNE);
      rev_data = (uint8_t)(huart->Instance->DR & (uint16_t)0x00FF);
      //printf("%c\n", rev_data);
      ringbuf_put(&ustRbRx, rev_data);
      ustTick = HAL_GetTick();
      ustFlag = 1;
    }
  }
  else if(huart->Instance == USART2)
  {
    //printf("Usart2 interrupt\n");
    if((__HAL_UART_GET_IT_SOURCE(&huart2, UART_IT_RXNE) != RESET) 
      && (__HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE) != RESET))
    {
      __HAL_UART_CLEAR_FLAG(&huart2, UART_FLAG_RXNE);
      rev_data = (uint8_t)(huart->Instance->DR & (uint16_t)0x00FF);
      //ringbuf_put(&xylanh.rxRb, rev_data);
      //xylanh.rxTick = HAL_GetTick();
      //xylanh.rxFlag = 1;
      // printf("%c\n", rev_data);
      // ringbuf_put(&ustRbRx, rev_data);
      // ustTick = HAL_GetTick();
      // ustFlag = 1;
    }
  }
}

void usart_manager(void)
{
  uint8_t c;
  uint32_t writelen=0, i=0;
  uint8_t getBuff[256];
  if((HAL_GetTick() - ustTick > 100) && (ustFlag == 1))
  {
    ustFlag = 0;
    ustTick = HAL_GetTick();
    printf("Usart get buff\n");
    while((ringbuf_get(&ustRbRx, &c))&&(writelen < 256))
    {
      writelen++;
      getBuff[i++] = c; 
    }
    if(writelen != 0)
    {
      printf("buff: %s\n", getBuff);
    }
  }
}
