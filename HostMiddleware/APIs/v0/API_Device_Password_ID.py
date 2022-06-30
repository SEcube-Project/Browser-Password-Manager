from .API_DeviceBase import *

class API_Device_Password_ID(API_DeviceBase):
    '''
    Class to manage a single password API endpoint.
    Supported methods: GET, PUT, DELETE
    '''

    def init(self, logger, l0: L0, l1: L1, utils: Utils) -> None:
        super().__init__(logger, l0, l1, utils)

    def get(self, indx: int, id: int):

        if not self._setdev_checklogin(indx, self._utils):
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

        if not self._setdev_checklogin(indx,self._utils):
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
            
        if not self._setdev_checklogin(indx, self._utils):
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
