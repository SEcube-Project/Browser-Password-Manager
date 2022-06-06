from .API_DeviceBase import *

class API_Device_Generate(API_DeviceBase):

    def init(self, logger, l0: L0, l1: L1, utils: Utils):
        super().__init__(logger, l0, l1, utils)

    def get(self, indx: int):

        if not self._setdev_checklogin(indx, self._utils):
            return {'error': 'Could not login: wrong pin or device not found'}, 403

        argument_parser = reqparse.RequestParser()
        argument_parser.add_argument('length', type=int, required=False, help='Length argument required. Must be an integer', location='args')
        argument_parser.add_argument('upper', type=int, required=False, help='Upper argument required. Must be an intenger', location='args')
        argument_parser.add_argument('special', type=int, required=False, help='Special argument required. Must be an integer', location='args')
        argument_parser.add_argument('numbers', type=int, required=False, help='Numbers argument required. Must be an integer', location='args')

        args = argument_parser.parse_args()
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
