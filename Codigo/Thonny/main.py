from microdot_asyncio import Microdot, Response, send_file
from microdot_utemplate import render_template
from dht_module import DHTModule
import ujson
from mq135 import MQ135

dht_sensor = DHTModule(12)
mq135 = MQ135(32)


app = Microdot()
Response.default_content_type = 'text/html'


@app.route('/')
async def index(request):
    return render_template('index.html')


@app.route('/updateValues')
async def get_dht_reads(request):
    print("Receive get values request!")
    dht_temperature, dht_humidity = dht_sensor.get_values()
    sensor_reads = dht_sensor.get_values()
    rzero = mq135.get_rzero()
    corrected_rzero = mq135.get_corrected_rzero(dht_temperature, dht_humidity)
    resistance = mq135.get_resistance()
    ppm = mq135.get_ppm()
    corrected_ppm = mq135.get_corrected_ppm(dht_temperature, dht_humidity)
    
    sensor_reads = sensor_reads + (corrected_ppm,)
    return ujson.dumps(sensor_reads)


@app.route('/shutdown')
async def shutdown(request):
    request.app.shutdown()
    return 'The server is shutting down...'


@app.route('/static/<path:path>')
def static(request, path):
    if '..' in path:
        # directory traversal is not allowed
        return 'Not found', 404
    return send_file('static/' + path)

app.run()