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
#define volatile
#define GPIO_BASE 0x20200000
unsigned *gpio_fsel0 = (unsigned *)(GPIO_BASE);
unsigned *gpio_fsel1 = (unsigned *)(GPIO_BASE + 0x04);
unsigned *gpio_fsel2 = (unsigned *)(GPIO_BASE + 0x08);
unsigned *gpio_set0  = (unsigned *)(GPIO_BASE + 0x1C);
unsigned *gpio_clr0  = (unsigned *)(GPIO_BASE + 0x28);

// XXX might need memory barriers.
void gpio_set_output(unsigned pin) {
    // gpio_fsel1  --- set 'pin' to output
    int FSEL_offset = pin / 10;
    unsigned *gpio_fsel;
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
  *gpio_set0 = 1 << pin;
}
void gpio_set_off(unsigned pin) {
	// use gpio_clr0
  *gpio_clr0 = 1 << pin;
}

// countdown 'ticks' cycles; the asm probably isn't necessary.
void delay(unsigned ticks) {
  while(ticks-- > 0)
  asm("add r1, r1, #0");
}

int notmain ( void ) {
  int led = 5;
  int led2 = 16;
  gpio_set_output(led);
  gpio_set_output(led2);
  while(1) {
    gpio_set_on(led);
    gpio_set_off(led2);
    delay(1000000);
    gpio_set_off(led);
    gpio_set_on(led2);
    delay(1000000);
  }
  return 0;
}
