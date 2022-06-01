from .API_DeviceBase import *

class API_Device_Sessions(API_DeviceBase):

    def init(self, logger: logging, l0: L0, l1: L1, utils: Utils) -> None:
        super().__init__(logger, l0, l1, utils)

    def get(self, indx: int):

        args = self._parser.parse_args()
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        session[self._utils.pinkeystr] = self._utils.encrypt(args["pin"])
        self._l1.Logout()

        return {
            'session': True
        }

    def post(self, indx: int):
        self._logger.info(f"session: {session[self._utils.pinkeystr]}")
        self._logger.info(f"session: {self._utils.decrypt(session[self._utils.pinkeystr])}")
        return {
            'session': True
        }

