// PORTE_PCR29 - MUX = 001
// Address = 4004_D074
// Register = 0x100
#include "MKL46Z4.h"

// team last mock

/*
 -*  IRS for SW1 -> toggle led when click button  - Falling edge   with porth PC3
 PC3 - Port C -  IRQ number = 31, EXception number = 47.
config IRQC in port   
1 config interup for i/o port
2 config  interup in NVIC 
3 write ISR 
*/

#define Mode1 0
#define Mode2 1
volatile uint8_t Stt_Flag_Mode = 0;
// ham delay 
void delay_200ms()
{
    uint32_t i;
    for (i = 0; i < 200000; i++)
    {
        __asm("nop");
    }
}




void PortC3_INTConfig(void)
{
	// clear register ve 0
  *(uint32_t*)0x4004B00C  &= 0;
  // Select Mux PortC3 = GPIO 
      *(uint32_t*)0x4004B00C  &= ~(7 << 8);
      *(uint32_t*)0x4004B00C  |= (1 << 8 );
  // select Pull up cho PortC3    
      *(uint32_t*)0x4004B00C  |= (3 << 0 );
  // enable IRQC  port C3
      *(uint32_t*)0x4004B00C &= ~(0xF << 16);
      *(uint32_t*)0x4004B00C  |= (10  << 16);   
}

void PortC12_INTConfig(void)
{
	// clear register ve 0
  *(uint32_t*)0x4004B030 &= 0;
  // Select PortC12  GPIO
      *(uint32_t*)0x4004B030   &= ~(7 << 8);
      *(uint32_t*)0x4004B030  |= (1 << 8 );
  // seclect pull up cho PortC12
      *(uint32_t*)0x4004B030  |= (3 << 0 );
  // enable   IRQC  port C3
      *(uint32_t*)0x4004B030 &= ~(0xF << 16);
      *(uint32_t*)0x4004B030  |= (10  << 16);   
}


void PORTC_PORTD_IRQHandler(void)
{
	
  if((*(uint32_t *)0x4004B00C) == 0x010A0103) //  mode1
  {
  	// tat het 2 den led
   *(uint32_t *)0x400FF0C0 |= (1  << 5);
   *(uint32_t *)0x400FF100 |= (1  << 29);
    Stt_Flag_Mode = Mode1; // chon mode 1
  }
  if((*(uint32_t *)0x4004B030) == 0x010A0103)  // mode 2
  {
  	// tat het 2 den led
   *(uint32_t *)0x400FF0C0 |= (1  << 5);  
   *(uint32_t *)0x400FF100 |= (1  << 29);
    Stt_Flag_Mode = Mode2; // chon mode 2
  }
*(uint32_t*)0x4004B030 |= (1 << 24); // writer 1 vao Flag
*(uint32_t*)0x4004B00C |= (1 << 24);
}

void Led_Green_Config(void)
{
	// xet mux = GPIO
  *(uint32_t * )0x4004C014 &= 0;
  *(uint32_t * )0x4004C014 &= ~(7 << 8);
   *(uint32_t*)0x4004C014  |= (1 << 8 );
  // xet GPIO = OUTPUT
   *(uint32_t *)0x400FF0D4 |= (1 << 5);
   *(uint32_t *)0x400FF0C0 |= (1 << 5);
   *(uint32_t *)0x400FF0C0 |= (0 << 5);
}

void Led_Red_Config(void)
{
  // mux = GPIO
  *(uint32_t*)0x4004D074  &= 0;
   *(uint32_t*)0x4004D074  &= ~(7 << 8);
   *(uint32_t*)0x4004D074  |= (1 << 8 );
  // xet GPIO = OUTPUT
   *(uint32_t *)0x400FF114 |= (1 << 29);
   *(uint32_t *)0x400FF100 |= (1 << 29);
   *(uint32_t *)0x400FF100 |= (0 << 29);
}
void main()
{
  
  // enble  internal clock
  *(uint8_t*)0x40064000 |= (1 << 6);
  // select fast CLK form internal clock
  *(uint8_t *)0x40064001 |= 1; 
  // sset FCRDIV = 0
  *(uint8_t * )0x40064008 |= (1 << 1);
  // set OUTDIV1 with OUTDIV4 for signal CLK
  // set OUTDIV1
    *(uint32_t*)0x40048044 &= ~(15 << 28);
  // set OUTDIV4
    *(uint32_t*)0x40048044 |= (0 << 16);
      SystemCoreClockUpdate(); // Clock = 2 MHz
   // NVIC set thanh ghi NVIC_ISER   boi vi IRQ cua port C va portD la  31, xem tai chuong 3 tai lieu KL46 RM   
   *(uint32_t*)0xE000E100  |= (1u  <<  31);
   // enable port C,D,E
    *(uint32_t*)0x40048038  |=  ( 7 << 11 );
  PortC3_INTConfig();
  PortC12_INTConfig();
  // cau hinh LED  ---------------------------------------------------------------------------
  Led_Red_Config();
  Led_Green_Config();
  
    
  while(1)
  {
  // set 

      while(Stt_Flag_Mode == Mode1)
     {
     	// toggle 2 led
      *(uint32_t *)0x400FF0CC |= ( 1 << 5); 
      *(uint32_t *)0x400FF10C |= ( 1 << 29);
      delay_200ms();
      }
  
    
    while(Stt_Flag_Mode ==  Mode2)
  {  
      // toggle 1 led 
    *(uint32_t *)0x400FF0CC |= ( 1 << 5);
      delay_200ms();
   }

  } 

  
  
  
 
}
