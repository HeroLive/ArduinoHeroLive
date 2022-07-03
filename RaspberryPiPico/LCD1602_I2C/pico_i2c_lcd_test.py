from machine import I2C, Pin
from time import sleep, sleep_us
from pico_i2c_lcd import I2cLcd

i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=100000) 
I2C_ADDR = i2c.scan()[0]
lcd = I2cLcd(i2c, I2C_ADDR, 2, 16)
counter = 0;

while True:
    if counter >=100:
        counter=0
        lcd.clear()
    else:
        counter +=1
    lcd.move_to(0,0)
    lcd.putstr("Hello" + "\n")
    lcd.move_to(5,1)
    lcd.putstr(str(counter))
    sleep(0.05)
    
