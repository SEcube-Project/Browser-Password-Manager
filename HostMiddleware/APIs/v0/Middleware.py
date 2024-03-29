import re
import logging

from urllib.parse import urlparse
from flask import Response


class Middleware:
    '''
    Middleware for Flask.

    When a request is made to the server, the middleware checks if the request is allowed.
    If the request is allowed, the request is passed to the Flask application.
    If the request is not allowed, the request is denied and a 403 error is returned.

    How it works:
    1. obtain the origin of the request
    2. obtain the request path (i.e. the endpoint of the request)
    3. check if the request is allowed

    3.1. if the origin is null, it is allowed (i.e. the request is from a chrome extension)

    3.2. if the origin is not null, check if the origin is allowed

    3.2.1. the endpoint is allowed if the tuple (origin, endpoint) is matched in the allowed_paths list

    '''

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
            orig_hname = urlparse(orig).hostname.replace("www.", "")
            
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
        
