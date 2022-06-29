import machine
import utime

LED  = machine.Pin(25, machine.Pin.OUT)

while True:
    LED.value(1)
    utime.sleep(0.1)
    LED.value(0)
    utime.sleep(0.1)