/**
  ******************************************************************************
  * @file    SSD1306.c 
  * @author  Waveshare Team
  * @version 
  * @date    13-October-2014
  * @brief   This file includes the OLED driver for SSD1306 display moudle
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

/* Includes ------------------------------------------------------------------*/
#include "SSD1306.h"
#include "Fonts.h"

static I2C_HandleTypeDef hi2c1;
/* Private macro -------------------------------------------------------------*/
#define __SET_COL_START_ADDR() 	do { \
									                    I2CBuff[1] = 0x00; \
	                                    HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20); \
									                    I2CBuff[1] = 0x10; \
	                                    HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20); \
								                   } while(false)							


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  OLED turns on 
  *         
  * @param  None
  *         
  * @retval None
**/ 
void ssd1306_display_on(void)
{
	unsigned char I2CBuff[2]={0x00,0x00};
	
	I2CBuff[1] = 0x8D;
  HAL_I2C_Master_Transmit(&hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);  
	I2CBuff[1] = 0x14;
  HAL_I2C_Master_Transmit(&hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);  
	I2CBuff[1] = 0xAF;
  HAL_I2C_Master_Transmit(&hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);	
}
   
/**
  * @brief  OLED turns off
  *         
  * @param  None
  *         
  * @retval  None
**/
void ssd1306_display_off(void)
{
	unsigned char I2CBuff[2]={0x00,0x00};
	
	I2CBuff[1] = 0x8D;
  HAL_I2C_Master_Transmit(&hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);  
	I2CBuff[1] = 0x10;
	HAL_I2C_Master_Transmit(&hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xAE;
  HAL_I2C_Master_Transmit(&hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);  
}

/**
  * @brief  Refreshs the graphic ram
  *         
  * @param  None
  *         
  * @retval  None
**/

void ssd1306_refresh_gram(I2C_HandleTypeDef *hi2c1)
{
	unsigned char i, j;
	unsigned char I2CBuff[2]={0x40,0x00};
	
	for (i = 0; i < 8; i ++)
	{
    I2CBuff[0] = 0x00;		
		I2CBuff[1] = 0xB0 + i;
	  HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
		I2CBuff[1] = 0x00;
	  HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
    I2CBuff[1] = 0x10;
	  HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
		
		I2CBuff[0] = 0x40;
		for (j = 0; j < 128; j ++)
		{
			I2CBuff[1] = s_chDispalyBuffer[j][i];
			HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
		}
	}   
}


/**
  * @brief   Clears the screen
  *         
  * @param  None
  *         
  * @retval  None
**/

void ssd1306_clear_screen(unsigned char chFill)  
{ 
	unsigned char i, j;
	
	for (i = 0; i < 8; i ++)
		for (j = 0; j < 128; j ++)
			s_chDispalyBuffer[j][i] = chFill;
}

/**
  * @brief  Draws a piont on the screen
  *         
  * @param  chXpos: Specifies the X position
  * @param  chYpos: Specifies the Y position
  * @param  chPoint: 0: the point turns off    1: the piont turns on 
  *         
  * @retval None
**/

void ssd1306_draw_point(unsigned char chXpos, unsigned char chYpos, unsigned char chPoint)
{
	unsigned char chPos, chBx, chTemp = 0;
	
	if (chXpos > 127 || chYpos > 63) {
		return;
	}
	chPos = 7 - chYpos / 8; // 
	chBx = chYpos % 8;
	chTemp = 1 << (7 - chBx);
	
	if (chPoint) {
		s_chDispalyBuffer[chXpos][chPos] |= chTemp;
		
	} else {
		s_chDispalyBuffer[chXpos][chPos] &= ~chTemp;
	}
}
	  
/**
  * @brief  Fills a rectangle
  *         
  * @param  chXpos1: Specifies the X position 1 (X top left position)
  * @param  chYpos1: Specifies the Y position 1 (Y top left position)
  * @param  chXpos2: Specifies the X position 2 (X bottom right position)
  * @param  chYpos3: Specifies the Y position 2 (Y bottom right position)
  *         
  * @retval 
**/

void ssd1306_fill_screen(unsigned char chXpos1, unsigned char chYpos1, unsigned char chXpos2, unsigned char chYpos2, unsigned char chDot)  
{  
	unsigned char chXpos, chYpos; 
	
	for (chXpos = chXpos1; chXpos <= chXpos2; chXpos ++) {
		for (chYpos = chYpos1; chYpos <= chYpos2; chYpos ++) {
			ssd1306_draw_point(chXpos, chYpos, chDot);
		}
	}	
	
	//ssd1306_refresh_gram();
}


/**
  * @brief Displays one character at the specified position    
  *         
  * @param  chXpos: Specifies the X position
  * @param  chYpos: Specifies the Y position
  * @param  chSize: 
  * @param  chMode
  * @retval 
**/
void ssd1306_display_char(unsigned char chXpos, unsigned char chYpos, unsigned char chChr, unsigned char chSize, unsigned char chMode)
{      	
	unsigned char i, j;
	unsigned char chTemp, chYpos0 = chYpos;
	
	chChr = chChr - ' ';				   
    for (i = 0; i < chSize; i ++) {   
		if (chSize == 12) {
			if (chMode) {
				chTemp = c_chFont1206[chChr][i];
			} else {
				chTemp = ~c_chFont1206[chChr][i];
			}
		} else {
			if (chMode) {
				chTemp = c_chFont1608[chChr][i];
			} else {
				chTemp = ~c_chFont1608[chChr][i];
			}
		}
		
        for (j = 0; j < 8; j ++) {
			if (chTemp & 0x80) {
				ssd1306_draw_point(chXpos, chYpos, 1);
			} else {
				ssd1306_draw_point(chXpos, chYpos, 0);
			}
			chTemp <<= 1;
			chYpos ++;
			
			if ((chYpos - chYpos0) == chSize) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}  	 
    } 
}
static unsigned int pow(unsigned char m, unsigned char n)
{
	unsigned int result = 1;	 
	while(n --) result *= m;    
	return result;
}	


void ssd1306_display_num(unsigned char chXpos, unsigned char chYpos, unsigned int chNum, unsigned char chLen, unsigned char chSize)
{         	
	unsigned char i;
	unsigned char chTemp, chShow = 0;
	
	for(i = 0; i < chLen; i ++) {
		chTemp = (chNum / pow(10, chLen - i - 1)) % 10;
		if(chShow == 0 && i < (chLen - 1)) {
			if(chTemp == 0) {
				ssd1306_display_char(chXpos + (chSize / 2) * i, chYpos, ' ', chSize, 1);
				continue;
			} else {
				chShow = 1;
			}	 
		}
	 	ssd1306_display_char(chXpos + (chSize / 2) * i, chYpos, chTemp + '0', chSize, 1); 
	}
} 


/**
  * @brief  Displays a string on the screen
  *         
  * @param  chXpos: Specifies the X position
  * @param  chYpos: Specifies the Y position
  * @param  pchString: Pointer to a string to display on the screen
  * @param  chSize: Specifies the HIGHT of Font
  * @param  chMode: Specifies the CHAR COLOR and BG COLOR: 1 > CHAR COLOR=1 and BG COLOR=0 
  *                                                        0 > CHAR COLOR=0 and BG COLOR=1        
  * @retval  None
**/
void ssd1306_display_string(unsigned char chXpos, unsigned char chYpos, const unsigned char *pchString, unsigned char chSize, unsigned char chMode)
{
    while (*pchString != '\0') {       
        if (chXpos > (SSD1306_WIDTH - chSize / 2)) {
			chXpos = 0;
			chYpos += chSize;
			if (chYpos > (SSD1306_HEIGHT - chSize)) {
				chYpos = chXpos = 0;
				ssd1306_clear_screen(0x00);
			}
		}
		
        ssd1306_display_char(chXpos, chYpos, *pchString, chSize, chMode);
        chXpos += chSize / 2;
        pchString ++;
    }
}

void ssd1306_draw_1616char(unsigned char chXpos, unsigned char chYpos, unsigned char chChar)
{
	unsigned char i, j;
	unsigned char chTemp = 0, chYpos0 = chYpos, chMode = 0;

	for (i = 0; i < 32; i ++) {
		chTemp = c_chFont1612[chChar - 0x30][i];
		for (j = 0; j < 8; j ++) {
			chMode = chTemp & 0x80? 1 : 0; 
			ssd1306_draw_point(chXpos, chYpos, chMode);
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == 16) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
	}
}

void ssd1306_draw_3216char(unsigned char chXpos, unsigned char chYpos, unsigned char chChar)
{
	unsigned char i, j;
	unsigned char chTemp = 0, chYpos0 = chYpos, chMode = 0;

	for (i = 0; i < 64; i ++) {
		chTemp = c_chFont3216[chChar - 0x30][i];
		for (j = 0; j < 8; j ++) {
			chMode = chTemp & 0x80? 1 : 0; 
			ssd1306_draw_point(chXpos, chYpos, chMode);
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == 32) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
	}
}

void ssd1306_draw_bitmap(unsigned char chXpos, unsigned char chYpos, const unsigned char *pchBmp, unsigned char chWidth, unsigned char chHeight)
{
	int i, j, byteWidth = (chWidth + 7) / 8;
	
    for(j = 0; j < chHeight; j ++){
        for(i = 0; i < chWidth; i ++ ) {
            if(*(pchBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                ssd1306_draw_point(chXpos + i, chYpos + j, 1);
            }
        }
    }
}



/**
  * @brief  SSd1306 initialization
  *         
  * @param  None
  *         
  * @retval None
**/
void ssd1306_init(I2C_HandleTypeDef *hi2c1)
{
	unsigned char I2CBuff[2]={0x00,0x00};

	//display off
	I2CBuff[1] = 0xAE;//0xAF; //0xAE; //off
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x00;//---set low column address
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x10;//---set high column address
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x40;//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x81;//--set contrast control register
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xCF;// Set SEG Output Current Brightness
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xA1;//--Set SEG/Column Mapping     
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xC0;//Set COM/Row Scan Direction   
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xA6;//--set normal display
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xA8;//--set multiplex ratio(1 to 64)
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x3f;//--1/64 duty
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xD3;//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x00;//-not offset
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xd5;//--set display clock divide ratio/oscillator frequency
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x80;//--set divide ratio, Set Clock as 100 Frames/Sec
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xD9;//--set pre-charge period
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xF1;//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xDA;//--set com pins hardware configuration
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x12;
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xDB;//--set vcomh
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x40;//Set VCOM Deselect Level
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x20;//-Set Page Addressing Mode (0x00/0x01/0x02)
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x02;//
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x8D;//--set Charge Pump enable/disable
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0x14;//--set(0x10) disable
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xA4;// Disable Entire Display On (0xa4/0xa5)
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xA6;// Disable Inverse Display On (0xa6/a7) 
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	I2CBuff[1] = 0xAF;//--turn on oled panel
	HAL_I2C_Master_Transmit(hi2c1, SSD1306_CADDRESS, I2CBuff, 2, 20);
	
	ssd1306_clear_screen(0);
}

/*-------------------------------END OF FILE-------------------------------*/

