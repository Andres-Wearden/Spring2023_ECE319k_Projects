// ****************** Lab2.c ***************
// Program written by: Andres Wearden
// Date Created: 1/18/2017 
// Last Modified: 12/31/2022 
#include "Lab2.h"
// Put your name and EID in the next two lines
char Name[] = "Andres Wearden";
char EID[] = "aew3364";
#include <stdint.h>
// Inputs: x1,y1 is the first point
//         x2,y2 is the second point
// Output: calculate distance
// see UART window to see if you have square, Manhattan or ECE319K distance
// The input/output values will be displayed in the UART window
int32_t Distance(int32_t x1, int32_t y1, int32_t x2, int32_t y2){
	int32_t dis;
	int32_t subx = x1-x2;
	int32_t suby = y1-y2;
	if (subx < 0){
		subx *= -1;
		}
	if (suby < 0){
		suby *= -1;
		}
	if(subx >= suby){
		dis = subx;
	}else{
		dis = suby;
	}
  return dis;
}

// Inputs: rect1 is x1,y1,w1,h1 the first rectangle
//         rect2 is x2,y2,w2,h2 the second rectangle
// Output: 1 if the rectangles overlap
//         0 if the rectances do not overlap
// Notes: x1 is rect1[0]  x2 is rect2[0]
//        y1 is rect1[1]  y2 is rect2[1]
//        w1 is rect1[2]  w2 is rect2[2]
//        h1 is rect1[3]  h2 is rect2[3]
// The input/output values will be displayed in the UART window
int32_t OverLap(int32_t rect1[4], int32_t rect2[4]){
	int32_t x1 = rect1[0];
	int32_t y1 = rect1[1];
	int32_t w1 = rect1[2];
	int32_t h1 = rect1[3];
	int32_t x2 = rect2[0];
	int32_t y2 = rect2[1];
	int32_t w2 = rect2[2];
	int32_t h2 = rect2[3];
	int32_t var1 = x2 + w2;
	int32_t var2 = x1 + w1;
	int32_t var3 = y1 - h1;
	int32_t var4 = y2 - h2;
	if((x1 >= var1) || (x2 >= var2)){
		return 0;
	}
	if((y2 <= var3) || (y1 <= var4)){
		return 0;
	}
	return 1;
}
// do not edit this 2-D array
const char Hello[4][8] ={
   "Hello  ",    // language 0
   "\xADHola! ", // language 1
   "Ol\xA0    ", // language 2
   "All\x83   "  // language 3
};
// Hello[0][0] is 'H'
// Hello[0][1] is 'e'
// Hello[0][2] is 'l'
// Hello[0][3] is 'l'
// Hello[0][4] is 'o'
// Hello[1][0] is 0xAD
// Hello[1][1] is 'H'
// Hello[1][2] is 'o'
// Hello[1][3] is 'l'
// Hello[1][4] is 'a'
// Hello[1][5] is '!'
void LCD_OutChar(char letter);

// Print 7 characters of the hello message
// Inputs: language 0 to 3
// Output: none
// You should call LCD_OuChar exactly 7 times
void SayHello(uint32_t language){
	for(int i=0; i<7; i++){
		LCD_OutChar(Hello[language][i]);
	}
}


