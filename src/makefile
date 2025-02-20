# makefile 
# yooaroma : 20230104 
# https://www.st.com/en/evaluation-tools/stm8s-discovery.html
# https://www.st.com/en/microcontrollers-microprocessors/stm8s105c6.html
# https://www.st.com/resource/en/datasheet/stm8s105c6.pdf
# https://www.st.com/resource/en/reference_manual/rm0016-stm8s-series-and-stm8af-series-8bit-microcontrollers-stmicroelectronics.pdf
# https://github.com/yooaroma/yoo_stm8s

# 
OBJ_DIR = obj
EXE_DIR = exe

LIST = AWU
# LIST += 7seg
# LIST += ADC
# LIST += beCAN
LIST += BEEP
LIST += blink
LIST += blink_key
LIST += blink_key_irq
LIST += blink_key_timer
LIST += blink_key_tli_irq
LIST += EEPROM
LIST += FLASH
LIST += GPIO
# LIST += I2C
# LIST += i2c_gpio_DHT11
# LIST += i2c_irq_DHT11
# LIST += i2c_irq_eeprom
LIST += IWDG
# LIST += joystick_adc
# LIST += key_full
# LIST += lan
# LIST += led_full
LIST += mem_reg
# LIST += PWM
# LIST += RFID
# LIST += SPI
# LIST += spi_eeprom
LIST += stm8s_debug_top
LIST += TIM1
# LIST += TIM2_3_5
# LIST += TIM4_6
LIST += timer_irq
LIST += uart_irq_no
LIST += uart_irq_rtx
LIST += uart_irq_rx
LIST += vsprintf_test
LIST += WWDG
# LIST += zigbee

all:
	for i in $(LIST); do \
		rm -f ./$$i/$(OBJ_DIR)/*.* ./$$i/$(EXE_DIR)/*.* ; \
	done

