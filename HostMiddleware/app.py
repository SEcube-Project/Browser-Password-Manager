import sys
import logging
from flask import Flask
from flask_restful import Resource, Api, reqparse
from werkzeug.exceptions import BadRequest

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

        self._bodyargs_template = reqparse.RequestParser()
        self._bodyargs_template.add_argument('hostname', type=str, required=True, help='Hostname argument required. Must be a string', location='json')
        self._bodyargs_template.add_argument('username', type=str, required=True, help='Username argument required. Must be a string', location='json')
        self._bodyargs_template.add_argument('password', type=str, required=True, help='Password argument required. Must be a string', location='json')
    
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

        parser = self._parser.copy()
        parser.add_argument('hostname', type=str, required=False, help='Hostname argument required. Must be a string', location='args')

        args = parser.parse_args()
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        try:
            l = self._l1.GetAllPasswords(hostname=args["hostname"])
        except BadRequest as e:
            raise
        except Exception as e:
            self._logger.error(f"Could not get passwords: {e}")
            return {'error': f'Could not get passwords {e}'}, 400
        finally:
            self._l1.Logout()

        return {
            "count": len(l),
            "passwords": [{
                "id": x[0],
                "hostname": x[1],
                "username": x[2],
                "password": x[3]
            } for x in l]
        }

    def post(self, indx: int):

        args = self._parser.parse_args()
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        try:
            bodyargs = self._bodyargs_template.parse_args()
            res = self._l1.AddPassword(bodyargs["hostname"], bodyargs["username"], bodyargs["password"])

            return {'success': res}, 201 if res else 400
        except BadRequest as e:
            raise
        except Exception as e:
            self._logger.error(f"Could not add password: {e}")
            return {'error': f'Could not add password: {e}'}, 400
        finally:
            self._l1.Logout()

class API_Device_Password_ID(API_DeviceBase):

    def init(self, logger, l0: L0, l1: L1):
        super().__init__(logger, l0, l1)

    def get(self, indx: int, id: int):

        args = self._parser.parse_args()
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        try:
            res = self._l1.GetPassword(id)
            if not res[0]:
                return {'error': f'Could not get password with id {id}'}, 400
            
            return {
                "id": id,
                "hostname": res[1],
                "username": res[2],
                "password": res[3]
            }
            
        except BadRequest as e:
            raise
        except Exception as e:
            self._logger.error(f"Could not get password: {e}")
            return {'error': f'Could not get password: {e}'}, 400
        finally:
            self._l1.Logout()

    def put(self, indx: int, id: int):

        args = self._parser.parse_args()
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        try:
            bodyargs = self._bodyargs_template.parse_args()
            res = self._l1.ModifyPassword(id, bodyargs["hostname"], bodyargs["username"], bodyargs["password"])

            return {'success': res}, 201 if res else 400
        except BadRequest as e:
            raise
        except Exception as e:
            self._logger.error(f"Could not update password: {e}")
            return {'error': f'Could not update password: {e}'}, 400
        finally:
            self._l1.Logout()

    def delete(self, indx: int, id: int):
            
        args = self._parser.parse_args()
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        try:
            res = self._l1.DeletePassword(id)
            return {'success': res}, 200 if res else 400
        except BadRequest as e:
            raise
        except Exception as e:
            self._logger.error(f"Could not delete password: {e}")
            return {'error': f'Could not delete password: {e}'}, 400
        finally:
            self._l1.Logout()

class API_Device_Generate(API_DeviceBase):

    def init(self, logger, l0: L0, l1: L1):
        super().__init__(logger, l0, l1)

    def get(self, indx: int):

        argument_parser = self._parser.copy()
        argument_parser.add_argument('length', type=int, required=False, help='Length argument required. Must be an integer', location='args')
        argument_parser.add_argument('upper', type=int, required=False, help='Upper argument required. Must be an intenger', location='args')
        argument_parser.add_argument('special', type=int, required=False, help='Special argument required. Must be an integer', location='args')
        argument_parser.add_argument('numbers', type=int, required=False, help='Numbers argument required. Must be an integer', location='args')

        args = argument_parser.parse_args()
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        try:
            llen = args["length"] if args["length"] is not None else 64
            upper = args["upper"] == 1 if args["upper"] is not None else True
            special = (args["special"] == 1) if args["special"] is not None else True
            numbers = args["numbers"] == 1 if args["numbers"] is not None else True

            res = self._l1.GeneratePassword(llen, upper, special, numbers)
            return ({'generated': res[1]}, 200) if res[0] else ({'success': res[0]}, 400)

        except BadRequest as e:
            raise
        except Exception as e:
            self._logger.error(f"Could not generate passwords: {e}")
            return {'error': f'Could not generate passwords: {e}'}, 400
        finally:
            self._l1.Logout()

if __name__ == "__main__":
    
    app = Flask(__name__)
    api = Api(app)
    l0 = L0()
    l1 = L1()

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

    api.add_resource(API_Devices, "/api/v0/devices", resource_class_args=[l0])
    api.add_resource(API_Device_Generate, "/api/v0/device/<int:indx>/generate", resource_class_args=[logger, l0, l1])
    api.add_resource(API_Device_Passwords, "/api/v0/device/<int:indx>/passwords", resource_class_args=[logger, l0, l1])
    api.add_resource(API_Device_Password_ID, "/api/v0/device/<int:indx>/password/<int:id>", resource_class_args=[logger, l0, l1])
    app.run(ssl_context='adhoc', debug=False)
