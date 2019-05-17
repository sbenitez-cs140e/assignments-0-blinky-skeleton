#define GPIO_BASE (0x3F000000 + 0x200000)

volatile unsigned *GPIO_FSEL1 = (volatile unsigned *)(GPIO_BASE + 0x04);
volatile unsigned *GPIO_SET0  = (volatile unsigned *)(GPIO_BASE + 0x1C);
volatile unsigned *GPIO_SET1  = (volatile unsigned *)(GPIO_BASE + 0x20);
volatile unsigned *GPIO_CLR0  = (volatile unsigned *)(GPIO_BASE + 0x28);
volatile unsigned *GPIO_CLR1  = (volatile unsigned *)(GPIO_BASE + 0x2C);

#define GPIO_FSEL_VAL_OUT 0b001
#define GPIO_FSEL_N_OFF(N) ( 3 * ((N) % 10) )
#define GPIO_FSEL_N_MASK(N) (0x7 << GPIO_FSEL_N_OFF(N))

static int clr_gpio(unsigned char gpioN) {
  if (gpioN < 32) {
    *GPIO_CLR0 = 1 << gpioN;
  } else if (gpioN < 54) {
    *GPIO_CLR1 = 1 << (gpioN - 32);
  } else {
    return -1;
  }

  return 0;
}

static int set_gpio(unsigned char gpioN) {
  if (gpioN < 32) {
    *GPIO_SET0 = 1 << gpioN;
  } else if (gpioN < 54) {
    *GPIO_SET1 = 1 << (gpioN - 32);
  } else {
    return -1;
  }

  return 0;
}

static void spin_sleep_us(unsigned int us) {
  for (unsigned int i = 0; i < us * 6; i++) {
    asm volatile("nop");
  }
}

static void spin_sleep_ms(unsigned int ms) {
  spin_sleep_us(ms * 1000);
}

int main(void) {
  // FIXME: STEP 1: Set GPIO Pin 16 as output.
  // GPIO function select register 1 - for GPIO 10-19
  (*GPIO_FSEL1) = (*GPIO_FSEL1) & (~GPIO_FSEL_N_MASK(16)) | (GPIO_FSEL_VAL_OUT << GPIO_FSEL_N_OFF(16));

  // FIXME: STEP 2: Continuously set and clear GPIO 16.
  while(1) {
    set_gpio(16);
    spin_sleep_ms(500);

    clr_gpio(16);
    spin_sleep_ms(500);
  }
}
