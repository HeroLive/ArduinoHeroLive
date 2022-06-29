from machine import I2C, Pin
from time import sleep, sleep_us
from pico_i2c_lcd import I2cLcd

import _thread

i2c = I2C(0, sda=Pin(0), scl=Pin(1), freq=100000) 
I2C_ADDR = i2c.scan()[0]
lcd = I2cLcd(i2c, I2C_ADDR, 2, 16)

_dir = machine.Pin(18,machine.Pin.OUT)
_pul = machine.Pin(19,machine.Pin.OUT)
pulPerRound = 200
_dirStatus=1

def core0_thread():
    global cnt
    while True:
        _pul.value(1)
        sleep_us(1000)
        _pul.value(0)
        sleep_us(1000)
        if cnt<=0:
            _dir.value(1)
            _dirStatus = 1
            sleep(2)
        elif cnt>=2000:
            _dir.value(0)
            _dirStatus=0
            sleep(2)
        if _dirStatus == 1:
            cnt += 1
        else:
            cnt -= 1

def core1_thread():
    counter = 0
    while True:
        counter += 1
        lcd.putstr(str(counter) + "\n")
        lcd.putstr(str(round(cnt/pulPerRound,1)))
        sleep(0.5)
        lcd.clear()

cnt=0
second_thread = _thread.start_new_thread(core1_thread, ())

core0_thread()
    