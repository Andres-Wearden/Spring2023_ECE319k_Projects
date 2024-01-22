// SpaceInvaders.c
// Runs on TM4C123
// Jonathan Valvano and Daniel Valvano
// This is a starter project for the ECE319K Lab 10

// Last Modified: 1/2/2023 
// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php

// ******* Possible Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PD2/AIN5
// Slide pot pin 3 connected to +3.3V 
// buttons connected to PE0-PE3
// 32*R resistor DAC bit 0 on PB0 (least significant bit)
// 16*R resistor DAC bit 1 on PB1
// 8*R resistor DAC bit 2 on PB2 
// 4*R resistor DAC bit 3 on PB3
// 2*R resistor DAC bit 4 on PB4
// 1*R resistor DAC bit 5 on PB5 (most significant bit)
// LED on PD1
// LED on PD0


#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "../inc/ADC.h"
#include "Images.h"
#include "../inc/wave.h"
#include "Timer1.h"
#include "EdgeInterrupt.h"
#include "Timer3AInitialization.h"
#include "Timer2A.h"
#include "DAC.h"
#include "SoundFile.h"
#include "Enemy_external.h"


void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
void WaitForInterrupt(void);

typedef enum {dead,alive} status_t;
struct sprite {
  int32_t x;      // x coordinate
  int32_t y;      // y coordinate
  int32_t vx,vy;  // pixels/30Hz
  const unsigned short *image; // ptr->image
  const unsigned short *black;
  status_t life;        // dead/alive
  int32_t w; // width
  int32_t h; // height
  uint32_t needDraw; // true if need to draw, false if already drawn
};
typedef struct sprite sprite_t;

int main2(void){
	uint32_t *pt = 0;
	uint32_t var;
	uint32_t* var2 = &var;
	if(var){
	return 1;
	}
	return 0;
}

int main1(void){
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Random_Init(1);

  Output_Init();
  ST7735_FillScreen(0x0000);            // set screen to black
  
  ST7735_DrawBitmap(22, 159, PlayerShip0, 18,8); // player ship bottom
  ST7735_DrawBitmap(53, 151, Bunker0, 18,5);
  ST7735_DrawBitmap(42, 159, PlayerShip1, 18,8); // player ship bottom
  ST7735_DrawBitmap(62, 159, PlayerShip2, 18,8); // player ship bottom
  ST7735_DrawBitmap(82, 159, PlayerShip3, 18,8); // player ship bottom

  ST7735_DrawBitmap(0, 9, SmallEnemy10pointA, 16,10);
  ST7735_DrawBitmap(20,9, SmallEnemy10pointB, 16,10);
  ST7735_DrawBitmap(40, 9, SmallEnemy20pointA, 16,10);
  ST7735_DrawBitmap(60, 9, SmallEnemy20pointB, 16,10);
  ST7735_DrawBitmap(80, 9, SmallEnemy30pointA, 16,10);
  ST7735_DrawBitmap(100, 9, SmallEnemy30pointB, 16,10);

  Delay100ms(50);              // delay 5 sec at 80 MHz

  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }

}


// You can't use this timer, it is here for starter code only 
// you must use interrupts to perform delays
void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
      time--;
    }
    count--;
  }
}
typedef enum {English, Spanish, Portuguese, French} Language_t;
Language_t myLanguage=3;
typedef enum {HELLO, GOODBYE, LANGUAGE} phrase_t;
const char Hello_English[] ="Hello";
const char Hello_Spanish[] ="\xADHola!";
const char Hello_Portuguese[] = "Ol\xA0";
const char Hello_French[] ="All\x83";
const char Goodbye_English[]="Goodbye";
const char Goodbye_Spanish[]="Adi\xA2s";
const char Goodbye_Portuguese[] = "Tchau";
const char Goodbye_French[] = "Au revoir";
const char Language_English[]="English";
const char Language_Spanish[]="Espa\xA4ol";
const char Language_Portuguese[]="Portugu\x88s";
const char Language_French[]="Fran\x87" "ais";
const char Please_Select_Language[]="Please Select Language:";
const char *Phrases[3][4]={
  {Hello_English,Hello_Spanish,Hello_Portuguese,Hello_French},
  {Goodbye_English,Goodbye_Spanish,Goodbye_Portuguese,Goodbye_French},
  {Language_English,Language_Spanish,Language_Portuguese,Language_French}
};

//prototype declarations
void ST7735_OutPhrase(phrase_t message, Language_t myLanguage);
void StringCenterX(char center[], uint8_t Ypos);
void ShowFirstScreen(void);
void DrawEnemies(uint8_t xPos);
typedef struct enemy enemy_t;
void GameInit(void);
void GameMove(void);
void GameDraw(void);
void Attack(void);
void shootBullet(uint32_t x);
uint32_t Convert(uint32_t x);
int32_t OverLap(int32_t rect1[4], int32_t rect2[4]);
uint8_t checkIfHitEnemyOrBackWall(void);
uint8_t CenterX(char center[], uint8_t Ypos);
void checkLoss(void);
uint8_t Flag = 0; //Semaphore which == 1 if PORT E Edge interrupt occured
sprite_t Enemy[6][4]; //declaring an "Enemy" array of size 18, essentially creating 18 enemies
uint32_t Semaphore = 0;
uint32_t Anyalive = 0; //1 if any of the enemies are alive, 0 if all dead
uint32_t Position;
uint32_t Data;
uint32_t TimerFlag = 0; //1 if Timer3A occurred
sprite_t Player;
uint32_t numberOfBullets = 40;
sprite_t Shooter[40];
uint32_t numBullets = 0;
uint32_t GameInitFlag = 0;
uint32_t FlagForTimer1 = 0;
uint32_t counter = 0;
uint32_t HasReached[40] = {0};
uint32_t HasSet[40] = {0};
uint32_t FlagShoot[40] = {0};
int32_t foo[4];
int32_t bar[4];
int32_t CheckCollision(void);
int32_t CheckCollision_2(void);
int32_t OverLap(int32_t rect1[4], int32_t rect2[4]);
uint32_t score = 0;
uint32_t Amo = 0;
uint8_t PauseFlag = 0;
uint32_t NumberDead = 0;
uint32_t EndFlag = 0;
uint32_t LoseFlag = 0;
//MAIN FUNCTON!!!!!
int main(void){ char l;
  DisableInterrupts();
  TExaS_Init(NONE);       // Bus clock is 80 MHz 
  Output_Init();
	PortE_Init();
	Timer3A_Init(8000000, 2);
	Timer1_Init(80000000/30, 3);
	//Timer0_Init(800000000, 5);
	PortF_Init();
	ADC_Init();
	ADC0_SAC_R = 5;
	Sound_Init();
	ShowFirstScreen();
	EnableInterrupts();
	while(1){
		WaitForInterrupt();
		if(myLanguage == English){
			Sound_Start(4800);
			Flag = 0;
			GameInit();
			GameInitFlag = 1;
			ST7735_FillScreen(0x0000);
			while(1){
				checkLoss();
				if(PauseFlag == 1){
					StringCenterX("Game Paused", 7);
				while(PauseFlag == 1){}
				ST7735_FillScreen(0x0000);
				}
				GameDraw();
				ST7735_SetCursor(11,0);
				ST7735_OutString("Score: ");
				ST7735_OutUDec(score);
				ST7735_SetCursor(15, 10);
				ST7735_OutUDec(Amo);
				ST7735_OutString("/");
				ST7735_OutUDec(40);
				GameMove();
				if(score == 24){
					EndFlag = 1;
					ST7735_FillScreen(0x0000);
					StringCenterX("YOU WIN!!", 7);
					while(EndFlag == 1){}
				}
				uint8_t var = checkIfHitEnemyOrBackWall();
				if(((score != 24 && Amo == 40) && var == 1) || LoseFlag == 1){
					EndFlag = 1;
					ST7735_FillScreen(0x0000);
					StringCenterX("YOU LOSE!!", 7);
					StringCenterX("Score: ", 9);
					ST7735_SetCursor(13, 9);
					ST7735_OutUDec(score);
					while(EndFlag == 1){};
				}
			}
		}
		if(myLanguage == Spanish){
			Sound_Start(4800);
			Flag = 0;
			GameInit();
			GameInitFlag = 1;
			ST7735_FillScreen(0x0000);
			while(1){
				checkLoss();
				if(PauseFlag == 1 && EndFlag == 0){
				StringCenterX("Juego Pausado", 7);
				while(PauseFlag == 1){}
				ST7735_FillScreen(0x0000);
				}
				GameDraw();
				ST7735_SetCursor(8,0);
				ST7735_OutString("Puntaje: ");
				ST7735_OutUDec(score);
				ST7735_SetCursor(15, 10);
				ST7735_OutUDec(Amo);
				ST7735_OutString("/");
				ST7735_OutUDec(40);
				GameMove();
				if(score == 24){
					EndFlag = 1;
					ST7735_FillScreen(0x0000);
					StringCenterX("T\xA3 Ganas!!", 7);
					while(EndFlag == 1){}
				}
				uint8_t var = checkIfHitEnemyOrBackWall();
				if(((score != 24 && Amo == 40) && var == 1) || LoseFlag == 1){
					EndFlag = 1;
					ST7735_FillScreen(0x0000);
					StringCenterX("T\xA3 Pierdes!!", 7);
					StringCenterX("Puntos: ", 9);
					ST7735_SetCursor(14, 9);
					ST7735_OutUDec(score);
					while(EndFlag == 1){};
				}
			}
		}
}
	}


void ST7735_OutPhrase(phrase_t message, Language_t myLanguage){
	ST7735_OutString((char *)Phrases[message][myLanguage]);
}
//function will center string in X position in display
//inputs: string of character and Ypos on screen
void StringCenterX(char center[], uint8_t Ypos){
	uint32_t sizeOfArray = 0;
	uint8_t i = 0;
	uint8_t Xpos = 0;
	while(center[i] != 0){
		sizeOfArray++;
		i++;
	}
	sizeOfArray--;
	Xpos = 10 - (sizeOfArray/2);
	ST7735_SetCursor(Xpos,Ypos);
	ST7735_OutString(center);
}

uint8_t CenterX(char center[], uint8_t Ypos){
	uint32_t sizeOfArray = 0;
	uint8_t i = 0;
	uint8_t Xpos = 0;
	while(center[i] != 0){
		sizeOfArray++;
		i++;
	}
	sizeOfArray--;
	Xpos = 10 - (sizeOfArray/2);
	return Xpos;
}
//Will show the screen with languages
void ShowFirstScreen(void){
	ST7735_FillScreen(0x0000);            // set screen to black
	ST7735_SetCursor(3,10);
	ST7735_OutPhrase(LANGUAGE, English);
	ST7735_SetCursor(11,10);
  ST7735_OutPhrase(LANGUAGE, Spanish);
	StringCenterX("Select Language:", 7);
	StringCenterX("SPACE INVADERS", 3);
}

void GPIOPortE_Handler(void){
	if((GPIO_PORTE_RIS_R & 0x04)==0x04){
		PauseFlag ^= 1;
	}
	if((GPIO_PORTE_RIS_R & 0x02)==0x02){
		myLanguage = Spanish;
	}
	if((GPIO_PORTE_RIS_R & 0x08)==0x08){
		if(GameInitFlag == 1){
		numBullets++;
		if(numBullets > numberOfBullets){
		numBullets = numberOfBullets;
		}
		FlagShoot[numBullets-1] = 1;
		}
		myLanguage = English;
	}
	GPIO_PORTE_ICR_R = 0x0E;      // acknowledge flag4
}
//Struct which defines a sprite



void GameInit(void){
	//Established initial conditions for all 18 enemies
	for(uint32_t j = 0; j<4; j++){
		for(uint32_t i = 0; i < 6; i++){
		Enemy[i][j].x = 16+i*(16);
		Enemy[i][j].y = 10+10+j*(10);
		Enemy[i][j].vx = 2;
		Enemy[i][j].vy = 0;
		Enemy[i][j].image = SmallEnemy10pointA;
		Enemy[i][j].black = BlackEnemy;
		Enemy[i][j].life = alive;
		Enemy[i][j].w = 18;
		Enemy[i][j].h = 10;
		Enemy[i][j].needDraw = 1;
		}
	}
	Player.x = 55;
	Player.y = 160;
	Player.vx = 0;
	Player.vy = 2;
	Player.image = PlayerShip0;
	Player.black = BlackEnemy;
	Player.life = alive;
	Player.w = 18;
	Player.h = 8;
	Player.needDraw = 1;
	for(uint32_t i = 0; i<numberOfBullets; i++){
	Shooter[i].x = 64;
	Shooter[i].y = 150;
	Shooter[i].vx = 0;
	Shooter[i].vy = 2;
	Shooter[i].image = Bullet;
	Shooter[i].black = BlackEnemy;
	Shooter[i].life = alive;
	Shooter[i].w = 3;
	Shooter[i].h = 5;
	Shooter[i].needDraw = 1;
	}
}
// will move the enemies, but will not display them on screen
void GameMove(void){
	Anyalive = 0; //at the beginning of the game, all enemies are dead
	for(uint32_t j = 0; j<4; j++){
		for(uint32_t i = 0; i<6; i++){
			Enemy[i][j].x += Enemy[i][j].vx;
			Enemy[i][j].y += Enemy[i][j].vy;
			Enemy[i][j].needDraw = 1;
			if((Enemy[i][j].x + 10 > 127) || (Enemy[i][j].x < 0)){
				Enemy[i][j].vx = -1*Enemy[i][j].vx;
				}
			}
		}
	if(FlagShoot[numBullets-1] == 1){
	FlagForTimer1 = 1;
	shootBullet(Position);
		}
	
	}
//displays the enemies on screen
void GameDraw(void){
	for(uint32_t j = 0; j<4; j++){
		for(uint32_t i = 0; i<6; i++){
			if(Enemy[i][j].needDraw == 1){
				if(Enemy[i][j].life == alive){
					ST7735_DrawBitmap(Enemy[i][j].x, Enemy[i][j].y, Enemy[i][j].image, Enemy[i][j].w, Enemy[i][j].h);
				}else{
					ST7735_DrawBitmap(Enemy[i][j].x, Enemy[i][j].y, Enemy[i][j].black, Enemy[i][j].w, Enemy[i][j].h);
					//Enemy[i][j].x = 0;
					Enemy[i][j].y = 0;
				}
				Enemy[i][j].needDraw = 0;
			}
		}
	}
	if(TimerFlag == 1){
		TimerFlag = 0;
		ST7735_DrawBitmap(Position, Player.y, Player.black, Player.w, Player.h);
		Position = Convert(Data);
		ST7735_DrawBitmap(Position, Player.y, Player.image, Player.w, Player.h);
		}
		for(uint32_t i = 0; i<numBullets; i++){
		if(HasSet[i] == 1){
		ST7735_DrawBitmap(Shooter[i].x, Shooter[i].y, Shooter[i].image, Shooter[i].w, Shooter[i].h);
		}
	}
}

void GameTasks(void){// 30Hz
	// playsound, slidepot, buttons
	GameMove();
	//collision
	Semaphore = 1;
}
void Timer3A_Handler(void){
	TIMER3_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
  // write this
	GPIO_PORTF_DATA_R ^= 0x02; // toggle PF1
  Data = ADC_In();      // Sample ADC
	if(GameInitFlag == 1){
	Attack();
}
  TimerFlag = 1;
	TIMER3_ICR_R = 0x00000001;
}

void Timer1A_Handler(void){ // can be used to perform tasks in background
  TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER1A timeout
	GPIO_PORTF_DATA_R ^= 0x04; // toggle PF1
	if(CheckCollision() == 1){
		score++;
	}
}
//void Timer1_Stop(void){
//  NVIC_DIS1_R = 1<<(35-32);   // 9) disable interrupt 35 in NVIC
//  TIMER1_CTL_R = 0x00000000;  // 10) disable timer3
//}

//void Timer1_Handler(void){
//	TIMER1_ICR_R = TIMER_ICR_TATOCINT;// acknowledge TIMER2A timeout
//  // write this
//  GameTasks();
//}

uint32_t Convert(uint32_t x){
  // write this
	int16_t Position = (x*2685)/100000;
	return Position;
}

//void Timer2A_Handler(void){
//  TIMER2_ICR_R = 0x00000001;  // acknowledge
//// run some background stuff here
//}

//void Timer2A_Stop(void){
//  TIMER2_CTL_R &= ~0x00000001; // disable
//}

//void Timer2A_Start(void){
//  TIMER2_CTL_R |= 0x00000001;   // enable
//}

void shootBullet(uint32_t x){
		for(uint32_t i = 0; i<numBullets; i++){
		if(HasSet[i] == 0){
		Shooter[i].x = x + 8;
		HasSet[i] = 1;
		Amo++;
		SystickFlag = 1;
		}
		Shooter[i].y -= Shooter[i].vy;
		if(Shooter[i].y < 0){
			//FlagShoot[i] = 0;
			//HasSet[i] = 0;
			Shooter[i].y = 0;
		}
	}
}

int32_t CheckCollision(void){
	for(uint32_t i = 0; i<6; i++){
		for(uint32_t j = 0; j<4; j++){
			for(uint32_t h = 0; h<numberOfBullets; h++){
	foo[0] = Enemy[i][j].x;
	foo[1] = Enemy[i][j].y;
	foo[2] = Enemy[i][j].w;
	foo[3] = Enemy[i][j].h;
	bar[0] = Shooter[h].x;
	bar[1] = Shooter[h].y;
	bar[2] = Shooter[h].w;
	bar[3] = Shooter[h].h;
				if(OverLap(foo, bar) == 1 && Enemy[i][j].life == alive){
					Enemy[i][j].life = dead;
					ST7735_DrawBitmap(Shooter[h].x, Shooter[h].y, Shooter[h].black, Shooter[h].w, Shooter[h].h);
					Shooter[h].y = -1;
					SystickFlag_2 = 1;
					return 1;
				}
			}
		}
	}
	return 0;
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

uint8_t checkIfHitEnemyOrBackWall(void){
	if(Shooter[39].y == 0){
		return 1;
	}
	if(CheckCollision_2() == 1){
		return 2;
	}
	return 0;
}

int32_t CheckCollision_2(void){
	for(uint32_t i = 0; i<6; i++){
		for(uint32_t j = 0; j<4; j++){
			for(uint32_t h = 0; h<numberOfBullets; h++){
	foo[0] = Enemy[i][j].x;
	foo[1] = Enemy[i][j].y;
	foo[2] = Enemy[i][j].w;
	foo[3] = Enemy[i][j].h;
	bar[0] = Shooter[h].x;
	bar[1] = Shooter[h].y;
	bar[2] = Shooter[h].w;
	bar[3] = Shooter[h].h;
				if(OverLap(foo, bar) == 1 && Enemy[i][j].life == alive){
					return 1;
				}
			}
		}
	}
	return 0;
}

//void Timer0A_Handler(void){
//	if(GameInitFlag == 1){
//	uint32_t i = (Random32()>>16)%6;
//	uint32_t j = (Random32()>>16)%4;
//		Enemy[i][j].vy = 1;
//}
//	 TIMER1_ICR_R = TIMER_ICR_TATOCINT;
//}

void checkLoss(void){
	for(uint32_t i = 0; i<6; i++){
		for(uint32_t j = 0; j<4; j++){
		if(Enemy[i][j].y == 160){
		LoseFlag = 1;
			}
		}
	}
}

void Attack(void){
	if((Random32()>>16)%50 == 49){
	uint32_t i = (Random32()>>16)%6;
	uint32_t j = (Random32()>>16)%4;
		Enemy[i][j].vy = 1;
	}
}






	
