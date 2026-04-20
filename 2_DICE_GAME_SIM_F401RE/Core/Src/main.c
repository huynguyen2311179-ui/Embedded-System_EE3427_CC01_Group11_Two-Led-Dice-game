/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : BẢN CODE CHUẨN XÁC NHẤT - ÁNH XẠ 1:1 TỪ PC0 ĐẾN PC13
  ******************************************************************************
  */
/* USER CODE END Header */
#include "main.h"
#include <stdlib.h>
#include <stdio.h>

/* --- BIẾN TRẠNG THÁI GAME --- */
uint8_t gameStep = 0;
uint8_t p1_sum = 0, p2_sum = 0;
int8_t p1_status = 0, p2_status = 0;

/* MÃ LED LOGIC TỪNG HẠT XÚC XẮC */
const uint16_t PATTERN[] = {
    0x0000, // [0]: Tắt
    0x0008, // [1]: Chỉ D4 sáng (Tâm)
    0x0041, // [2]: D1, D7 sáng
    0x002A, // [3]: D2, D4, D6 sáng
    0x0063, // [4]: 4 góc
    0x006B, // [5]: 4 góc + tâm
    0x0077  // [6]: 2 cột bên
};

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
void Custom_Delay(uint32_t ms);
void Display_All_Dice(uint8_t valA, uint8_t valB);
void Lcd_Init(void);
void Lcd_Print(char* str, uint8_t line);
void Play_End_Sound(int8_t is_draw);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  Lcd_Init();

  /* LAMP TEST: BẬT MỞ HẾT 14 CHÂN TỪ PC0 ĐẾN PC13 */
  Lcd_Print("SYSTEM CHECK...", 0);
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET); // Bật MOSFET

  // Ép sáng cả 14 bóng (0x3FFF = 11111111111111 trong hệ nhị phân)
  GPIOC->ODR = 0x3FFF;
  Custom_Delay(1000);

  Display_All_Dice(0, 0); // Tắt sạch

  Lcd_Print("== DICE GAME ==", 0);
  Lcd_Print("P1 Turn: ROLL", 1);

  while (1) {
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
      if (gameStep < 2) {
        srand(HAL_GetTick());
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);

        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) {
          Display_All_Dice((rand()%6)+1, (rand()%6)+1);
          Custom_Delay(60);
        }
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);

        uint8_t r1 = (rand()%6)+1;
        uint8_t r2 = (rand()%6)+1;
        Display_All_Dice(r1, r2);

        int8_t cur_stat = 0;
        if (r1 == r2) cur_stat = (r1==1 || r1==6) ? -1 : 1;

        if (gameStep == 0) {
          p1_sum = r1 + r2; p1_status = cur_stat;
          char buf[16]; sprintf(buf, "P1:%d %s", p1_sum, (p1_status==1?"(W)":(p1_status==-1?"(L)":"")));
          Lcd_Print(buf, 0); Lcd_Print("P2 Turn: ROLL", 1);
          gameStep = 1;
        } else {
          p2_sum = r1 + r2; p2_status = cur_stat;
          char buf[16]; sprintf(buf, "P2:%d %s", p2_sum, (p2_status==1?"(W)":(p2_status==-1?"(L)":"")));
          Lcd_Print(buf, 1); Custom_Delay(1000);

          int8_t res = 0;
          if (p1_status == -1 && p2_status == -1) res = 0;
          else if (p1_status == -1) res = 2; else if (p2_status == -1) res = 1;
          else if (p1_status == 1 && p2_status == 1) res = 0;
          else if (p1_status == 1) res = 1; else if (p2_status == 1) res = 2;
          else { if (p1_sum > p2_sum) res = 1; else if (p2_sum > p1_sum) res = 2; else res = 0; }

          Lcd_Print(res == 0 ? "MATCH DRAW!" : (res == 1 ? "PLAYER 1 WINS!" : "PLAYER 2 WINS!"), 0);
          Play_End_Sound(res == 0); Lcd_Print("RESET TO REPLAY", 1);
          gameStep = 2;
        }
      }
      while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET) Custom_Delay(10);
    }

    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) {
      gameStep = 0;
      Display_All_Dice(0, 0);
      Lcd_Print("== NEW MATCH ==", 0); Lcd_Print("P1 Turn: ROLL", 1);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET); Custom_Delay(100);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
      while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == GPIO_PIN_RESET) Custom_Delay(10);
    }
  }
}

/* HÀM ÁNH XẠ TRỰC TIẾP RA PORT C (TỪ PC0 ĐẾN PC13) */
void Display_All_Dice(uint8_t valA, uint8_t valB) {
    // Ghép mã Dice A (7 bit thấp) và Dice B (7 bit cao) thành 1 cục data 14-bit
    uint32_t out = (PATTERN[valA] & 0x7F) | ((uint32_t)(PATTERN[valB] & 0x7F) << 7);

    // Đẩy thẳng ra Port C (PC0 -> PC13)
    GPIOC->ODR = out;

    // Đóng/Mở cầu dao MOSFET (PB6)
    if (valA > 0 || valB > 0) {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
    }
}

/* KHỞI TẠO GPIO CHUẨN XÁC */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  __HAL_RCC_GPIOC_CLK_ENABLE(); __HAL_RCC_GPIOA_CLK_ENABLE(); __HAL_RCC_GPIOB_CLK_ENABLE();

  // Kích hoạt Port C: PC0 đến PC13 làm Output cho 14 LED
  GPIO_InitStruct.Pin = 0x3FFF;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  // SET TỐC ĐỘ MAX ĐỂ ĐẢM BẢO DÒNG ĐIỆN ỔN ĐỊNH CHO TẤT CẢ CÁC CHÂN (Đặc biệt PC13)
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // Kích hoạt Port B: PB0->PB6 (LCD & MOSFET)
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // Nút bấm và Còi
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/* CÁC HÀM CẤU HÌNH CLOCK, LCD & DELAY GIỮ NGUYÊN */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  __HAL_RCC_PWR_CLK_ENABLE(); __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);
}

void Lcd_Write(uint8_t data, uint8_t rs) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, rs);
    GPIOB->ODR = (GPIOB->ODR & ~0x3C) | ((data & 0x0F) << 2);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 1); Custom_Delay(1);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, 0); Custom_Delay(1);
}
void Lcd_Init(void) {
    Custom_Delay(50); Lcd_Write(0x03, 0); Custom_Delay(5);
    Lcd_Write(0x03, 0); Custom_Delay(1); Lcd_Write(0x03, 0); Lcd_Write(0x02, 0);
    Lcd_Write(0x2, 0); Lcd_Write(0x8, 0); Lcd_Write(0x0, 0); Lcd_Write(0xC, 0);
    Lcd_Write(0x0, 0); Lcd_Write(0x1, 0); Custom_Delay(2);
}
void Lcd_Print(char* str, uint8_t line) {
    Lcd_Write((line == 0 ? 0x80 : 0xC0) >> 4, 0); Lcd_Write(line == 0 ? 0x0 : 0x0, 0);
    Lcd_Write((line == 0 ? 0x80 : 0xC0) >> 4, 0); Lcd_Write((line == 0 ? 0x80 : 0xC0), 0);
    while(*str) { Lcd_Write((*str) >> 4, 1); Lcd_Write(*str, 1); str++; }
}
void Play_End_Sound(int8_t is_draw) {
    if (is_draw) { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1); Custom_Delay(500); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0); }
    else { for(int i=0; i<3; i++) { HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 1); Custom_Delay(100); HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, 0); Custom_Delay(100); } }
}
void Custom_Delay(uint32_t ms) { for(volatile uint32_t i=0; i < ms * 1600; i++) __NOP(); }
