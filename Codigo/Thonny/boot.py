# boot.py -- run on boot-up
import network, utime

# Replace the following with your WIFI Credentials
SSID = "CAMILO"
SSI_PASSWORD = "9XKUQ8THKZHYFZJ"

def connect_wifi():
    import network
    sta_if = network.WLAN(network.STA_IF)
    if not sta_if.isconnected():
        print('connecting to network...')
        sta_if.active(True)
        sta_if.connect(SSID, SSI_PASSWORD)
        while not sta_if.isconnected():
            pass
    print('Connected! Network configuration:', sta_if.ifconfig())
    
print("Connecting to your wifi...")
connect_wifi()

