/**
  ******************************************************************************
  * @file    ADC_Example/main.c 
  * @author  MCD Application Team
  * @version V1.1.0
  * @date    20-September-2012
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h> 
#include <string.h>

/** @addtogroup STM32F3_Discovery_Peripheral_Examples
  * @{
  */

/** @addtogroup ADC_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay = 0;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */

#define NOMEDIM 30
#define COGNDIM 40
#define CODDIM 5
#define DIPNUM 4
#define STUDDIM 10

typedef struct _studente
{
    char nome[NOMEDIM];
    char cognome[COGNDIM];
    char codice[CODDIM+1];
    int accesso_dip[DIPNUM];
    int access_counters[DIPNUM];
    int valid;
} Studente;

void init_array(Studente students[]);

void reset_counters(Studente *);
void insert_studente(Studente *studente, char nome[], char cognome[], char codice[], int a0, int a1, int a2, int a3, int valid);

int dipartimento = 1;
Studente studenti[STUDDIM];


int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f30x.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f30x.c file
     */ 
      
  /* Setup SysTick Timer for 1 µsec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1)
    {}
  }
  
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED5);
  STM_EVAL_LEDInit(LED6);
  
  int i, found = 0;
  
  Studente selected;
  char code[CODDIM+1];   
  init_array(studenti);
  
  /* Infinite loop */
  while (1)
  {
         found = 0;
         STM_EVAL_LEDOn(LED4);
         printf("Inserire il codice studente: ");
         scanf("%s", code);
         
          for(i=0; i<STUDDIM; i++)
           {
              if(strcmp((*(studenti + i)).codice , code) == 0)
              {
                  found = 1;
                  selected = *(studenti + i);
                  break;
              }
           }
          
          if (found == 1)
          {
              if (selected.valid == 0)
              {
                  printf("Studente non valido!\n");
                  STM_EVAL_LEDOff(LED4);
                  STM_EVAL_LEDOn(LED5);
                  Delay(2000);
              }
              else if (selected.accesso_dip[dipartimento] == 1)
              {
                  printf("Studente autorizzato!");
                  STM_EVAL_LEDOff(LED4);
                  STM_EVAL_LEDOn(LED6);
                  Delay(2000);
              }
              else
              {
                  printf("Studente non autorizzato!");
                  STM_EVAL_LEDOff(LED4);
                  STM_EVAL_LEDOn(LED3);
                  Delay(2000);
              }
          }
          else
          {
              printf("Studente non trovato nell'Archivio!\n");
              STM_EVAL_LEDOff(LED4);
              for(i=0; i< 3; i++)
              {
                  STM_EVAL_LEDOn(LED3);
                  Delay(200);
                  STM_EVAL_LEDOff(LED3);
                  Delay(200);
              }
          }
          STM_EVAL_LEDOff(LED3);
          STM_EVAL_LEDOff(LED5);
          STM_EVAL_LEDOff(LED6);
  }
}

void init_array(Studente students[])
{    
    insert_studente(&students[0], "Marco", "Salafia", "A0001", 1, 1, 0, 0, 1);
    insert_studente(&students[1], "Damiano", "DiStefano", "A0002", 1, 1, 1, 0, 1);
    insert_studente(&students[2], "Ciccio", "Caruso", "B0001", 1, 0, 0, 0, 1);
    insert_studente(&students[3], "Alessio", "Toscano", "B0002", 1, 1, 1, 1, 0);
    
}

void insert_studente(Studente *studente, char nome[], char cognome[], char codice[], int a0, int a1, int a2, int a3, int valid)
{
    strcpy(studente->nome, nome);
    strcpy(studente->cognome, cognome);
    strcpy(studente->codice, codice);
    studente->accesso_dip[0] = a0;
    studente->accesso_dip[1] = a1;
    studente->accesso_dip[2] = a2;
    studente->accesso_dip[3] = a3;
    reset_counters(studente);
    studente->valid = valid;
}

void reset_counters(Studente *studente)
{
    int i;
    
    for(i=0; i<DIPNUM; i++)
    {
       studente->access_counters[i] = 0;
    }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
