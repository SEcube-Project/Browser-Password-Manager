import sys
import logging
from flask import Flask
from flask_restful import Resource, Api, reqparse

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
        
class API_DeviceBase(Resource):

    def __init__(self, logger: logging, l0: L0, l1: L1) -> None:
        super().__init__()
        self._l0 : L0 = l0
        self._l1 : L1 = l1
        self._logger = logger

        self._parser = reqparse.RequestParser()
        self._parser.add_argument('pin', type=str, required=True, help='PIN argument required. Must be a string', location='args')
    
    def _setdev(self, indx: int):
        self._logger.info(f"Selecting device {indx}")
        
        if indx >= self._l0.getDeviceListSize():
            self._logger.error(f"Device {indx} does not exist")
            return False

        self._l1.SelectSECube(indx)
        return True

    def _dologin(self, pin: str, isAdmin: bool, force: bool):
        self._logger.info(f"Logging in with PIN {pin}")
        if not self._l1.Login(pin, isAdmin, force):
            self._logger.error(f"Login failed")
            return False
        
        return True

    def _setdev_login(self, indx: int, pin: str, isAdmin: bool, force: bool):
        if not self._setdev(indx):
            return False

        if not self._dologin(pin, isAdmin, force):
            return False

        return True

class API_Device_Passwords(API_DeviceBase):

    def init(self, logger, l0: L0, l1: L1):
        super().__init__(logger, l0, l1)

    def get(self, indx: int):

        args = self._parser.parse_args()
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 400

        l = self._l1.GetAllPasswords()
        self._l1.Logout()
        return {
            "count": len(l),
            "passwords": l
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
        logger.warning("No devices found!")
    else:
        logger.info(f"Found {device_cnt} devices")

    # logger.info("Selecting device 0")
    # l1.SelectSECube(0)

    api.add_resource(API_Devices, "/api/v0/devices", resource_class_args=[l0])
    api.add_resource(API_Device_Passwords, "/api/v0/device/<int:indx>/passwords", resource_class_args=[logger, l0, l1])
    app.run(ssl_context='adhoc', debug=True)
