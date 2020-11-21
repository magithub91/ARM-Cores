/**
  ******************************************************************************
  * @file    SSD1306.h
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief   This file contains all the functions prototypes for the SSD1306 OLED firmware driver.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, WAVESHARE SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _SSD1306_H_
#define _SSD1306_H_

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define SSD1306_WIDTH         128
#define SSD1306_HEIGHT        64
#define SSD1306_CADDRESS      0x78
#define SSD1306_DADDRESS      0x7A

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"
#include <stdbool.h>
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static unsigned char s_chDispalyBuffer[128][8];


/* Exported macro ------------------------------------------------------------*/

/* Exported functions ------------------------------------------------------- */

extern void ssd1306_clear_screen  (unsigned char chFill);
extern void ssd1306_refresh_gram  (I2C_HandleTypeDef *hi2c1);
extern void ssd1306_draw_point    (unsigned char chXpos,  unsigned char chYpos,  unsigned char chPoint);
extern void ssd1306_fill_screen   (unsigned char chXpos1, unsigned char chYpos1, unsigned char chXpos2,          unsigned char chYpos2, unsigned char chDot);
extern void ssd1306_display_char  (unsigned char chXpos,  unsigned char chYpos,  unsigned char chChr,            unsigned char chSize,  unsigned char chMode);
extern void ssd1306_display_num   (unsigned char chXpos,  unsigned char chYpos,  unsigned int chNum,             unsigned char chLen,   unsigned char chSize);
extern void ssd1306_display_string(unsigned char chXpos,  unsigned char chYpos,  const unsigned char *pchString, unsigned char chSize,  unsigned char chMode);
extern void ssd1306_draw_1616char (unsigned char chXpos,  unsigned char chYpos,  unsigned char chChar);
extern void ssd1306_draw_3216char (unsigned char chXpos,  unsigned char chYpos,  unsigned char chChar);
extern void ssd1306_draw_bitmap   (unsigned char chXpos,  unsigned char chYpos,  const unsigned char *pchBmp,    unsigned char chWidth, unsigned char chHeight);
void ssd1306_display_off(void);
void ssd1306_display_on(void);

extern void ssd1306_init(I2C_HandleTypeDef *hi2c1);



#endif

/*-------------------------------END OF FILE-------------------------------*/


