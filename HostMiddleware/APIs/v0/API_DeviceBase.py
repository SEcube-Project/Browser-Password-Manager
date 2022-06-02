from datetime import timedelta
import logging

from flask import session
from flask_restful import Resource, reqparse
from werkzeug.exceptions import BadRequest

from L0 import L0
from L1 import L1

from ..Utils import Utils

class API_DeviceBase(Resource):

    def __init__(self, logger: logging, l0: L0, l1: L1, utils: Utils) -> None:
        super().__init__()
        self._l0 : L0 = l0
        self._l1 : L1 = l1
        self._logger = logger
        self._utils = utils

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
        self._logger.info(f"Logging in with PIN '{pin}'")
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

    def _setdev_checklogin(self, indx: int):
        
        # self._logger.debug("HEADERS: ")
        # for k, v, in request.headers.items():
        #     self._logger.debug(f"    {k}: {v}")

        if not self._setdev(0):
            return False

        if not self._utils.pinkeystr in session.keys() or not self._utils.endtimekeystr in session.keys():
            self._logger.error(f"PIN not set. Maybe login not done?")
            return False

        # checking if endtime has been reached
        endtime = int(self._utils.decrypt(session[self._utils.endtimekeystr]))
        actual = int(Utils.NTP_TIME())
        self._logger.debug(f"endtime: {endtime}, reamining seconds: {timedelta(seconds=endtime - actual - 10)}")
        if Utils.HAS_EXPIRED(endtime, actual):
            self._logger.error(f"Session expired")
            session.pop(self._utils.pinkeystr, None) # remove pin from session
            return False

        if not self._dologin(self._utils.decrypt(session[self._utils.pinkeystr]), True, True):
            return False

        return True
