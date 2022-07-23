import machine

LED = machine.Pin(16,machine.Pin.OUT)

LED.value(0)

p16 = LED.value()
print(p16)