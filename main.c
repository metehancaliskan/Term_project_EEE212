#include <MKL25Z4.h>
#include <math.h>
#include <stdio.h>
#include <signal.h>
#include "switches.h"
#include "gpio_defs.h"
#include <stdlib.h>
#include <string.h>

#define RS 0x04     /* PTA2 mask */ 
#define RW 0x10     /* PTA4 mask */ 
#define EN 0x20     /* PTA5 mask */

void ADC0_init(void);
void LED_init(void);
void init_switch(void);
void PORTD_IRQHandler(void);
void timer_1(uint32_t F1,double D1);
void Delay(volatile unsigned int time_del);
volatile int button = 0;
double pos=8;
double pos2=2.5;
double pos3=2.5;

void timer_2(uint32_t F2,double D2);
int main (void)
{
	volatile short int result;
	timer_1(50,pos);
	LED_init(); /* Configure LEDs */
	ADC0_init(); /* Configure ADC0 */
	init_switch();
	PORTD_IRQHandler();
	while (1)
	{	
	while(1)
	{
		if(button %2 == 1)
		{
		PTA->PCOR=0x0002;
		ADC0->SC1[0] = 0; /* start conversion on channel 0 */
		while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
		result = ADC0->R[0]; /* read conversion result and clear COCO flag */
		if(result<3010)
		{
			PTB->PCOR = 0x40000; /* turn on red LED */
			PTB->PSOR = 0x80000; /* turn off green LED */
			PTD->PSOR = 0x02; /* turn off blue LED */
			Delay(100);		
			double i;
			for(i=pos;i<13.5;i+=0.5)
			{
			ADC0->SC1[0] = 0; /* start conversion on channel 0 */
			while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
			result = ADC0->R[0]; /* read conversion result and clear COCO flag */
			if(result<3010)
			{
			if(button%2==1)
			{
			Delay(1000000);
			pos+=0.5;
			timer_1(50,i);
			}
			}
			else
			{
				pos-=0.5;
				break;
			}
			}
		}
		if(result>3010 && result<3800)
		{
			PTD->PSOR = 0x02; /* turn off blue LED */
			PTB->PCOR = 0x80000; /* turn on green LED */
			PTB->PSOR = 0x40000; /* turn off red LED */
			Delay(100);
		}
		if(result>3800)
		{
			PTD->PCOR = 0x02; /* turn on blue LED */
			PTB->PSOR = 0x80000; /* turn off green LED */
			PTB->PSOR = 0x40000; /* turn off red LED */
			Delay(100);
			double j;
			for(j=pos;j>2.5;j-=0.5)
			{
			ADC0->SC1[0] = 0; /* start conversion on channel 0 */
			while(!(ADC0->SC1[0] & 0x80)) { } /* wait for conversion complete */
			result = ADC0->R[0]; /* read conversion result and clear COCO flag */
			if(result>3800)
			{
			if(button%2==1)
			{
			Delay(1000000);
			pos-=0.5;
			timer_1(50,j);
			}
			}
			else
			{
				pos+=0.5;
				break;
			}
			
			}
		}
	}
	else
		{
		break;
		}
	}
		while(1)
	{
	if(button %2 == 0)
	{
		volatile int dist1=0;
    volatile int count1=0;
		SIM->SCGC5 |= (1UL << 9);
		PORTA->PCR[1] = 0x100;
    PTA->PDDR=0X0002;      
		SIM->SCGC5 |= (1UL << 11);
		PORTC->PCR[2] = 0x100;
    PORTC->PCR[3] = 0x103; 
		PTC->PDDR=0X0004;      
		while(1){
			if(button %2 == 0)
			{
			while(((PTC->PDIR)&0x08)!=0)
			{ Delay(30);
				count1++;
			}
			while(((PTC->PDIR)&0x08)==0)
			{
			PTC->PSOR=0x0004;
			Delay(30);
			PTC->PCOR=0x0004;
				if(count1!=0)
				{
				Delay(30);
				dist1=count1*5/29;
				timer_1(50,pos3);
				if(dist1>42)
				{
					PTA->PCOR=0x0002;
					PTB->PCOR = 0x40000; /* turn on red LED */
					PTB->PSOR = 0x80000; /* turn off green LED */
					PTD->PSOR = 0x02; /* turn off blue LED */
					Delay(1000);
					double n;
					if(pos2<13.5)
					{
					for(n=pos2;n<13.5;n+=0.5)
					{
					while(((PTC->PDIR)&0x08)!=0)
					{ 
					Delay(30);
					count1++;
					}
					while(((PTC->PDIR)&0x08)==0)
					{
					PTC->PSOR=0x0004;
					Delay(30);
					PTC->PCOR=0x0004;
					if(count1!=0)
					{
					Delay(30);
					dist1=count1*5/29;
          if(button %2 == 0)
					{
					if(dist1>42)
						{
						Delay(2000000);		
						timer_2(50,n);
						pos2+=0.5;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
					count1 = 0;
					}
					}
					}
				}
				else
				{
				double m;
				for(m=pos3;m<13.5;m+=0.5)
				{
					while(((PTC->PDIR)&0x08)!=0)
					{ 
					Delay(30);
					count1++;
					}
					while(((PTC->PDIR)&0x08)==0)
					{
					PTC->PSOR=0x0004;
					Delay(30);
					PTC->PCOR=0x0004;
					if(count1!=0)
					{
					Delay(30);
					dist1=count1*5/29;
          if(button %2 == 0)
						{
						if(dist1>42)
						{
						Delay(2000000);		
						timer_1(50,m);
						pos3+=0.5;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
					count1 = 0;
					}
					}
				}
				if(pos3>=13.5)
				{
					pos2=3;
					pos3=3;
				}
				}
				}
				else
				{
					PTA->PSOR=0x0002;
					PTD->PCOR = 0x02; /* turn on blue LED */
					PTB->PSOR = 0x80000; /* turn off green LED */
					PTB->PSOR = 0x40000; /* turn off red LED */
					Delay(1000);
				}
				count1=0;
				}
			}
		}
	else
	{
			break;
	}
	
}

			
	}
	else
	{
	break;
	}
	}
	}
}

void ADC0_init(void)
{
	SIM->SCGC5 |= 0x2000; /* clock to PORTE */
	PORTE->PCR[20] = 0; /* PTE20 analog input */
	SIM->SCGC6 |= 0x8000000; /* clock to ADC0 */
	ADC0->SC2 &= ~0x40; /* software trigger */
	/* clock div by 4, long sample time, single ended 12 bit, bus clock */
	ADC0->CFG1 = 0x40 | 0x10 | 0x04 | 0x00;
}
void Delay(volatile unsigned int time_del) {
  while (time_del--) 
		{
  }
}
void init_switch(void) {
	SIM->SCGC5 |=  SIM_SCGC5_PORTD_MASK; /* enable clock for port D */

	/* Select GPIO and enable pull-up resistors and interrupts 
		on falling edges for pins connected to switches */
	
	/* Set port D switch bit to inputs */
	PTD->PDDR &= ~(1UL << (6));
		PORTD->PCR[SW_POS] |= PORT_PCR_MUX(1) | PORT_PCR_PS_MASK | PORT_PCR_PE_MASK | PORT_PCR_IRQC(0x0a);

	/* Enable Interrupts */
	NVIC_SetPriority(PORTD_IRQn, 128); // 0, 64, 128 or 192
	NVIC_ClearPendingIRQ(PORTD_IRQn); 
	NVIC_EnableIRQ(PORTD_IRQn);
}

void PORTD_IRQHandler(void) {  
		button ++;
		Delay(1000);

	PORTD->ISFR = 0xffffffff;
}

void LED_init(void) 
{
	SIM->SCGC5 |= 0x400; /* enable clock to Port B */
	SIM->SCGC5 |= 0x1000; /* enable clock to Port D */
	PORTB->PCR[18] = 0x100; /* make PTB18 pin as GPIO */
	PTB->PDDR |= 0x40000; /* make PTB18 as output pin */
	PORTB->PCR[19] = 0x100; /* make PTB19 pin as GPIO */
	PTB->PDDR |= 0x80000; /* make PTB19 as output pin */
	PORTD->PCR[1] = 0x100; /* make PTD1 pin as GPIO */
	PTD->PDDR |= 0x02; /* make PTD1 as output pin */
}

void timer_1(uint32_t F1,double D1)
{
	  uint32_t M1= (10488000/(128*F1)); 
    uint32_t du1= ((D1*M1)/100);
    SIM->SCGC5 |= (1UL << 13); 
    PORTE->PCR[21] = 0x0300; 
    SIM->SCGC6 |= (1UL << 25); 
    SIM->SOPT2 |= 0x01000000; 
    TPM1->SC = 0; /* disable timer */
    TPM1->CONTROLS[1].CnSC = 0x20 | 0x08; 
    TPM1->MOD = M1; 
    TPM1->CONTROLS[1].CnV = du1;
    TPM1->SC = 0x0F | 0x20;
}
void timer_2(uint32_t F2,double D2)
{
		uint32_t M2= (10488000/(128*F2)); 
    uint32_t du2= ((D2*M2)/100);
    SIM->SCGC5 |= 0x2000; 
    PORTE->PCR[31] = 0x0300; 
    SIM->SCGC6 |= 0X01000000; 
    SIM->SOPT2 |= 0x01000000; 
    TPM0->SC = 0; /* disable timer */
    TPM0->CONTROLS[4].CnSC = 0x20 | 0x08; 
    TPM0->MOD = M2; 
    TPM0->CONTROLS[4].CnV = du2;
    TPM0->SC = 0x0F | 0x20;
}