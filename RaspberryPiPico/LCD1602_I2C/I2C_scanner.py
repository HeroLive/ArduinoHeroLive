import machine

sdaPIN=machine.Pin(16)  
sclPIN=machine.Pin(17)

i2c=machine.I2C(0,sda=sdaPIN, scl=sclPIN, freq=400000)   

devices = i2c.scan()
if len(devices) == 0:
 print("No i2c device !")
else:
 print('i2c devices found:',len(devices))
for device in devices:
 print("At address: ",hex(device))