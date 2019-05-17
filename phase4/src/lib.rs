#![feature(compiler_builtins_lib, lang_items, asm, pointer_methods)]
#![no_builtins]
#![no_std]

extern crate compiler_builtins;

pub mod lang_items;

const GPIO_BASE: usize = 0x3F000000 + 0x200000;

const GPIO_FSEL1: *mut u32 = (GPIO_BASE + 0x04) as *mut u32;
const GPIO_SET0: *mut u32 = (GPIO_BASE + 0x1C) as *mut u32;
const GPIO_CLR0: *mut u32 = (GPIO_BASE + 0x28) as *mut u32;

#[inline(never)]
fn spin_sleep_ms(ms: usize) {
    for _ in 0..(ms * 6000) {
        unsafe { asm!("nop" :::: "volatile"); }
    }
}

fn set_gpio_10_19_pin_as_out(pin: u8) -> Option<u8> {
    let shift = if pin < 10 {
        pin % 10
    } else {
        return None;
    };

    // let shift = pin % 10;
    unsafe {
        let old_val = GPIO_FSEL1.read_volatile() & (!(0b111 << shift));
        GPIO_FSEL1.write_volatile(old_val | (0b001 << shift));
    }

    Some(shift)
}

fn set_gpio(gpio_n: u8) -> Option<u8> {
    if gpio_n < 32u8 {
        unsafe {
            GPIO_SET0.write_volatile(1 << gpio_n);

            Some(gpio_n)
        }
    } else {
        None
    }
}

fn clr_gpio(gpio_n: u8) -> Option<u8> {
    if gpio_n < 32u8 {
        unsafe {
            GPIO_CLR0.write_volatile(1 << gpio_n);

            Some(gpio_n)
        }
    } else {
        None
    }
}


#[no_mangle]
pub unsafe extern "C" fn kmain() {
    // FIXME: STEP 1: Set GPIO Pin 16 as output.
    if let None = set_gpio_10_19_pin_as_out(16) { panic!() };
    // FIXME: STEP 2: Continuously set and clear GPIO 16.
    loop {
        if let Some(_offset) = set_gpio(16u8) {

        } else {
            loop {}
        };
        spin_sleep_ms(500usize);

        if let Some(_offset) = clr_gpio(16u8) {

        } else {
            loop {}
        };
        spin_sleep_ms(500usize);

    }
}
