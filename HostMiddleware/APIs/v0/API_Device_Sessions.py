from .API_DeviceBase import *

class API_Device_Sessions(API_DeviceBase):

    def init(self, logger: logging, l0: L0, l1: L1, utils: Utils) -> None:
        super().__init__(logger, l0, l1, utils)

    def get(self, indx: int):

        state = self._setdev_checklogin(indx, self._utils)

        if state:
            self._l1.Logout()

        return {
            'session': state
        }

    def post(self, indx: int):

        parser = self._parser.copy()
        parser.add_argument('endtime', type=int, required=True, help='Endtime argument required. Must be a UNIX timestamp', location='args')

        args = parser.parse_args()
        if Utils.HAS_EXPIRED(args["endtime"], self._utils.tick):
            self._logger.error(f"Endtime is in the past")
            return {'error': 'Endtime is in the past'}, 400

        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        session[self._utils.pinkeystr] = self._utils.encrypt(args["pin"])
        session[self._utils.endtimekeystr] = self._utils.encrypt(str(args["endtime"]))
        
        self._l1.Logout()

        return {
            'session': True
        }

    def delete(self, indx: int):
        
        if not self._setdev_checklogin(indx, self._utils):
            return {'error': 'Could not logout: wrong pin or device not found'}, 403

        session.pop(self._utils.pinkeystr, None)
        self._l1.Logout()

        return {
            'session': False
        }

