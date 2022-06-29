import machine

LED = machine.Pin(25,machine.Pin.OUT)
BUTTON = machine.Pin(18,machine.Pin.IN)

while True:
    val = BUTTON.value()
    print(val)
    if val == 1:
        LED.value(1)
    else:
        LED.value(0)