/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "stdarg.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct {
	uint32_t header;
	uint8_t data;
	uint8_t cnt;
}DATA_Typedef;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

osThreadId Task01Handle;
//osMessageQId myQueue01Handle;
//osSemaphoreId myBinarySem01Handle;
/* USER CODE BEGIN PV */
osThreadId Task02Handle;
osMailQId myQueue01Handle;
osMutexId myMutexHandle;

EventGroupHandle_t eventGroupHandle;
EventBits_t event_B;
char buffer[40];
/*Unsigned long*/
#define  EVENT0 (1UL<<0)
#define  EVENT1 (1UL<<1)
#define  EVENT2 (1UL<<2)
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartTask01(void const * argument);

/* USER CODE BEGIN PFP */
void StartTask02(void const * argument);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void mPrintf(const char* format,...)
{
	for(uint8_t i=0;i<40;i++){
		buffer[i] = 0;
	}
	va_list arg;
	va_start(arg,format);
	vsnprintf(buffer,sizeof(buffer),format,arg);
	va_end(arg);
	HAL_UART_Transmit(&huart1,(uint8_t*)buffer,sizeof(buffer),HAL_MAX_DELAY);
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* USER CODE BEGIN RTOS_MUTEX */
  osMutexDef(myMutextt);
  myMutexHandle = osMutexCreate(osMutex(myMutextt));
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* Create the semaphores(s) */
  /* definition and creation of myBinarySem01 */
//  osSemaphoreDef(myBinarySem01);
//  myBinarySem01Handle = osSemaphoreCreate(osSemaphore(myBinarySem01), 1);

  /* USER CODE BEGIN RTOS_SEMAPHORES */

  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* definition and creation of myQueue01 */
  osMailQDef(myQueue01, 14, DATA_Typedef);
  myQueue01Handle = osMailCreate(osMailQ(myQueue01), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */

  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of Task01 */
  osThreadDef(Task01, StartTask01, 3, 0, 128);
  Task01Handle = osThreadCreate(osThread(Task01), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  osThreadDef(Task02, StartTask02, 2, 0, 128);
  Task02Handle = osThreadCreate(osThread(Task02), NULL);
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
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
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
//osEvent recv_task1_data;
//DATA_Typedef *receiver;
//nhan struct
void StartTask02(void const * argument)
{
  /* USER CODE BEGIN 5 */
//	//osEvent recv_task1_data;
//	osEvent event;
	//mPrintf("cnt semaphore: %d\n", osSemaphoreGetCount(myBinarySem01Handle));

  /* Infinite loop */
  while(1)
  {
	//state1= eTaskGetState((TaskHandle_t)Task01Handle);
	//state2= eTaskGetState((TaskHandle_t)Task02Handle);
	//mPrintf("State task1 %d\nState task2 %d \n",state1,state2);
	//mPrintf("Task2 \n");
//	osSemaphoreWait(myBinarySem01Handle, osWaitForever);
	mPrintf("Task 2 is running \n");
//	osSemaphoreRelease(myBinarySem01Handle);
//	event = osMailGet(myQueue01Handle, osWaitForever);
	//recv_task1_data = osMessageGet(myQueue01Handle, osWaitForever);
	//mPrintf("Data = %d\n" , recv_task1_data.value.v);
//	if(event.status == osEventMail){
//		receiver = (DATA_Typedef*)event.value.p;
		/*Nếu mà để là void* thì không khác gì nhận theo kiểu message queue
		 * và khi nó trả về con trỏ thì có thể đọc bẩt kì dạng dữ liệu nào*/
		/*ở đây trong event.value.p nó đọc và trả về kiểu */
//		mPrintf("%d cnt is:\n ", receiver->header);
//		osMailFree(myQueue01Handle, receiver);
//	}
	//*receiver = recv_task1_data.value.p;
    osDelay(1000);
  }
  /* USER CODE END 5 */
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartTask01 */
/**
  * @brief  Function implementing the Task01 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartTask01 */
void StartTask01(void const * argument)
{
  /* USER CODE BEGIN 5 */
	//osEvent recv_task1_data;
	//osEvent event;
//  mPrintf("cnt semaphore: %d\n", osSemaphoreGetCount(myBinarySem01Handle));
	eventGroupHandle = xEventGroupCreate();
  /* Infinite loop */
  while(1)
  {
	//state1= eTaskGetState((TaskHandle_t)Task01Handle);
	//state2= eTaskGetState((TaskHandle_t)Task02Handle);
	//mPrintf("State task1 %d\nState task2 %d \n",state1,state2);
	//mPrintf("Task2 \n");
//	osSemaphoreWait(myBinarySem01Handle, osWaitForever);
	event_B = xEventGroupWaitBits(eventGroupHandle, event_B, event_B, event_B, event_B);
	mPrintf("Task 1 is running \n");

//	event = osMailGet(myQueue01Handle, osWaitForever);
	//recv_task1_data = osMessageGet(myQueue01Handle, osWaitForever);
	//mPrintf("Data = %d\n" , recv_task1_data.value.v);
//	if(event.status == osEventMail){
//		receiver = (DATA_Typedef*)event.value.p;
		/*Nếu mà để là void* thì không khác gì nhận theo kiểu message queue
		 * và khi nó trả về con trỏ thì có thể đọc bẩt kì dạng dữ liệu nào*/
		/*ở đây trong event.value.p nó đọc và trả về kiểu */
//		mPrintf("%d cnt is:\n ", receiver->header);
//		osMailFree(myQueue01Handle, receiver);
	//}
	//*receiver = recv_task1_data.value.p;
//	osSemaphoreRelease(myBinarySem01Handle);
    osDelay(1000);

  }
  /* USER CODE END 5 */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM4 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM4)
  {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
