from .API_DeviceBase import *

class API_Device_Sessions(API_DeviceBase):
    '''
    Class to manage the Sessions API endpoint.
    Supported methods: GET, POST, DELETE
    '''

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
        # Check if the endtime is in the future
        if Utils.HAS_EXPIRED(args["endtime"], self._utils.tick):
            self._logger.error(f"Endtime is in the past")
            return {'error': 'Endtime is in the past'}, 400

        # Try to login (to check if the pin is correct)
        if not self._setdev_login(indx, args["pin"], True, True):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        # Create a new session
        # Session is a dictionary with the following keys:
        #  - pin: the pin of the device
        #  - endtime: the endtime of the session
        # both key and values are encrypted 
        session[self._utils.pinkeystr] = self._utils.encrypt(args["pin"])
        session[self._utils.endtimekeystr] = self._utils.encrypt(str(args["endtime"]))
        
        # Logout from the device
        self._l1.Logout()

        return {
            'session': True
        }

    def delete(self, indx: int):
        
        if not self._setdev_checklogin(indx, self._utils):
            return {'error': 'Could not logout: wrong pin or device not found'}, 403

        # Delete the session by removing the pin key from the session
        session.pop(self._utils.pinkeystr, None)
        self._l1.Logout()

        return {
            'session': False
        }

