import sys
import logging
from flask import Flask
from flask_restful import Resource, Api

from L0 import L0
from L1 import L1
from CustomFormatter import CustomFormatter

class API_Devices(Resource):

    def __init__(self, l0: L0) -> None:
        super().__init__()
        self._l0 : L0 = l0;

    def get(self):
        return {
            'count': self._l0.getDeviceListSize(),
            'devices': [{
                'index': i,
                'path': x, 
                'serial': y
            } for i, x, y in self._l0.getDeviceList()]
        }
        

if __name__ == "__main__":
    
    app = Flask(__name__)
    api = Api(app)
    l0 = L0()
    l1 = L1()
    # middleware = MiddlewareAPI()

    logger = logging.getLogger("main")
    logger.setLevel(logging.DEBUG)
    c_handler = logging.StreamHandler(stream=sys.stdout)
    c_handler.setFormatter(CustomFormatter())
    c_handler.setLevel(logging.DEBUG)
    logger.addHandler(c_handler)

    device_cnt = l0.getDeviceListSize()
    if not device_cnt > 0:
        logger.error("No devices found")
        exit(1)

    logger.info(f"Found {device_cnt} devices")
    # logger.info("Selecting device 0")
    # l1.SelectSECube(0)

    api.add_resource(API_Devices, "/v0/devices", resource_class_args=[l0])
    app.run(ssl_context='adhoc', debug=True)
