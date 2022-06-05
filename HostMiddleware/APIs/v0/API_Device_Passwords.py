from .API_DeviceBase import *

class API_Device_Passwords(API_DeviceBase):

    def init(self, logger, l0: L0, l1: L1, utils: Utils):
        super().__init__(logger, l0, l1, utils)

    def get(self, indx: int):

        if not self._setdev_checklogin(indx, self._utils):
            return {'error': 'Could not login: device not found or invalid login'}, 403

        parser = reqparse.RequestParser()
        parser.add_argument('hostname', type=str, required=False, help='Hostname argument required. Must be a string', location='args')
        args = parser.parse_args()

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

        if not self._setdev_checklogin(indx, self._utils):
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
