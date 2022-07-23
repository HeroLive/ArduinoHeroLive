from machine import Pin, I2C
from ssd1306 import SSD1306_I2C
from time import sleep

i2c=I2C(0,sda=Pin(16), scl=Pin(17), freq=400000)
oled = SSD1306_I2C(128, 64, i2c)
counter = 0
oled.text("WELCOME!", 0, 2)
oled.text("I'm Hero Live", 0, 18)
oled.text("Test by a text", 0, 34)
oled.text("GOOD BYE", 0, 50)
oled.show()
sleep(2)
oled.fill(0)

while True:
    counter = counter + 1
    #Displaying a Pixel
    oled.pixel(64,32,1)
    oled.show()
    sleep(1)
    oled.fill(0)
    
    #Draw Line
    oled.hline(0,32,128,1)
    oled.show()
    sleep(1)
    oled.fill(0)

    oled.vline(0, 0, 64, 1)
    oled.show()
    sleep(1)
    oled.fill(0)

    oled.line(0, 0, 128, 64, 1)
    oled.show()
    sleep(1)
    oled.fill(0)

    #Draw Rectangle (Unfilled)
    oled.rect(20, 20, 64, 32, 1)
    oled.show()
    sleep(1)
    oled.fill(0)
    #Draw Rectangle (filled)
    oled.fill_rect(20, 20, 64, 32, 1)
    oled.show()
    sleep(1)
    oled.fill(0)
    
    oled.text(str(counter),64,32)
    oled.show()
    sleep(1)
    oled.fill(0)
      
