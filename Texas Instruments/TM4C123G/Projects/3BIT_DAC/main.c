/* 3bit DAC - Github.com/glennlopez */
// Shape of the wave is a function of an index

/************************
 * PREPROCESSOR DIRECTIVES
 ************************/

// System Control Legacy base address: 0x400F.E000 (Datasheet pg. 234)
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))

// INPUT PortE(APB) base address: 0x40024000 (Datasheet pg. 657)
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_PDR_R        (*((volatile unsigned long *)0x40024514))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))

// OUTPUT PortB(APB) base address: 0x40005000 (Datasheet pg. 657)
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))

// Systick & NVIC Registers (pg 132)
#define NVIC_SYS_PRI3_R         (*((volatile unsigned long *)0xE000ED20))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))

// DAC Bit-specific address: (7|200, 6|100, 5|80, 4|40, 3|20, 2|10, 1|08, 0|04)
#define DACOUT                  (*((volatile unsigned long *)0x4000503C))

// INPUT Bit-specific Address: (7|200, 6|100, 5|80, 4|40, 3|20, 2|10, 1|08, 0|04)
#define G_KEY /* E3: (783.991) */ (*((volatile unsigned long *)0x40024020))
#define E_KEY /* E2: (659.255) */ (*((volatile unsigned long *)0x40024010))
#define D_KEY /* E1: (587.330) */ (*((volatile unsigned long *)0x40024008))
#define C_KEY /* E0: (523.251) */ (*((volatile unsigned long *)0x40024004))


/************************
 * PROTOTYPES & GLOBAL
 ************************/

// Prototypes
void initPortBOut(void);
void SysTick_Init(unsigned long period);

/************************
 * ISR HANDLERS
 ************************/
void SysTick_Handler(void){
    GPIO_PORTB_DATA_R ^= 0x001;
}


/************************
 * MAIN ROUTINE
 ************************/
int main(void) {

    // Initialization routine
    SYSCTL_RCGC2_R |= 0x00000012;
    initPortBOut();
    SysTick_Init(8000);
    EnableInterrupts();

    // Loop routine
    while(1){

        //GPIO_PORTB_DATA_R ^= 0x001;
        //delay(500);

    }
	
	return 0;
}



/************************
 * SUB ROUTINES
 ************************/

// SysTick Interrupt Routine (pg. 132)
void SysTick_Init(unsigned long period){

  NVIC_ST_CTRL_R = 0;
  NVIC_ST_RELOAD_R = period-1;
  NVIC_ST_CURRENT_R = 0;

  NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000;
  NVIC_ST_CTRL_R = 0x07;
}

// PortB OUTPUT Initiazation Routine (pg. 657)
void initPortBOut(void){    unsigned long volatile delay;

    // GPIO Digital Control
    GPIO_PORTB_DEN_R        |=      0x0F;
    GPIO_PORTB_DIR_R        |=      0x0F;

    // GPIO Alternate function control
    GPIO_PORTB_AMSEL_R      &=      ~0x0F;
    GPIO_PORTB_AFSEL_R      &=      ~0x0F;
    GPIO_PORTB_PCTL_R       &=      ~0x0000FFFF;
}