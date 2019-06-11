/*
 * blink for arbitrary pins.    
 * Implement:
 *	- gpio_set_output;
 *	- gpio_set_on;
 * 	- gpio_set_off.
 *
 *
 * - try deleting volatile.
 * - change makefile to use -O3
 * - get code to work by calling out to a set32 function to set the address.
 * - initialize a structure with all the fields.
 */

// see broadcomm documents for magic addresses.
#define GPIO_BASE 0x20200000
volatile unsigned *gpio_fsel0 = (volatile unsigned *)(GPIO_BASE);
volatile unsigned *gpio_fsel1 = (volatile unsigned *)(GPIO_BASE + 0x04);
volatile unsigned *gpio_fsel2 = (volatile unsigned *)(GPIO_BASE + 0x08);
volatile unsigned *gpio_set0  = (volatile unsigned *)(GPIO_BASE + 0x1C);
volatile unsigned *gpio_clr0  = (volatile unsigned *)(GPIO_BASE + 0x28);

// XXX might need memory barriers.
void gpio_set_output(unsigned pin) {
    // gpio_fsel1  --- set 'pin' to output
    int FSEL_offset = pin / 10;
    volatile unsigned *gpio_fsel;
    switch (FSEL_offset) {
    case 0:
      gpio_fsel = gpio_fsel0;
      break;
    case 1:
      gpio_fsel = gpio_fsel1;
      break;
    case 2:
      gpio_fsel = gpio_fsel2;
      break;
    }
    *gpio_fsel &= ~(0b111 << ((pin % 10) * 3));
    *gpio_fsel |= 0b001 << ((pin % 10) * 3);  // set the pin out
}

void gpio_set_on(unsigned pin) {
	// use gpio_set0
  *gpio_set0 &= ~(1 << pin);
  *gpio_set0 |= 1 << pin;
}
void gpio_set_off(unsigned pin) {
	// use gpio_clr0
  *gpio_clr0 &= ~(1 << pin);
  *gpio_clr0 |= 1 << pin;
}

// countdown 'ticks' cycles; the asm probably isn't necessary.
void delay(unsigned ticks) {
	while(ticks-- > 0)
		asm("add r1, r1, #0");
}

int notmain ( void ) {
	int led = 5;
  gpio_set_output(led);
  while(1) {
    gpio_set_on(led);
    delay(5000000);
    gpio_set_off(led);
    delay(5000000);
  }
	return 0;
}
