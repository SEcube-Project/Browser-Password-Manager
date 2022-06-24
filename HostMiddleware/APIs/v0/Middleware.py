import re
import logging

from urllib.parse import urlparse
from flask import Response


class Middleware:

    def __init__(self, app, logger: logging.Logger):
        self._app = app
        self._logger = logger

    def __call__(self, environ, start_response):
        
        # print(f"HEADERS: {environ}")
        try:
            orig = environ.get("HTTP_ORIGIN", None)
            requ = environ["REQUEST_URI"]
        except Exception as e:
            self._logger.critical(f"Some strane error happened: {e}")
            return False

        self._logger.debug(f"Origin: {orig}")
        self._logger.debug(f"Request: {requ}")

        if orig is not None:
            orig_hname = urlparse(orig).hostname
            
            # allowed paths: tuple (allowed_endpoint, allowed_origin, is_regex)
            allowed_paths = [
                (f"/api/v0/device/0/passwords?hostname={orig_hname}", None, False),
                (None, r"^chrome-extension://.+", True)
            ]

            self._logger.debug(f"Allowed paths: {allowed_paths}")

            match = lambda x, y, z : ((x and re.match(y, z)) or (not x and y == z))
            for path in allowed_paths:

                a = path[0] is None and (path[1] is not None and re.match(path[1], orig))
                b = path[0] is not None and match(path[2], path[0], requ) and path[1] is None
                c = path[0] is not None and match(path[2], path[0], requ) and path[1] is not None and match(path[2], path[1], orig)
                if a or b or c: 
                    self._logger.info(f"Request {requ} is allowed from {orig}")
                    return self._app(environ, start_response)


            self._logger.warn(f"Request {requ} not allowed from {orig}")
            res = Response({'error': 'Request not allowed'}, 403, mimetype='application/json')
            return res(environ, start_response)

        self._logger.info(f"Request {requ} is allowed from {orig}")
        return self._app(environ, start_response)
        
