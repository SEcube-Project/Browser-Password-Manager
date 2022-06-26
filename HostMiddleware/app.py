import os
import sys
import logging
import pathlib
from urllib.parse import urlparse
from flask import Flask, Response
from flask_session import Session
from flask_restful import Api
from flask_cors import CORS

from L0 import L0
from L1 import L1
from CustomFormatter import CustomFormatter

from APIs import Utils
from APIs.v0 import API_Time, API_Devices, API_Device_Generate, API_Device_Password_ID, API_Device_Sessions, API_Device_Passwords
from APIs.v0 import Middleware as APIV0_Middleware

def wipe_out():
    if os.path.exists("flask_session"):
        for f in os.listdir("flask_session"):
            os.remove(os.path.join("flask_session", f))

if __name__ == "__main__":

    logger = logging.getLogger("main")
    logger.setLevel(logging.DEBUG)
    c_handler = logging.StreamHandler(stream=sys.stdout)
    c_handler.setFormatter(CustomFormatter())
    c_handler.setLevel(logging.DEBUG)
    logger.addHandler(c_handler)

    # determine if application is a script file or frozen exe
    if getattr(sys, 'frozen', False):
        logger.info("Found running in a frozen env")
        # If the application is run as a bundle, the PyInstaller bootloader
        # extends the sys module by a flag frozen=True and sets the app 
        # path into variable _MEIPASS'.
        application_path = sys._MEIPASS
    else:
        logger.info("Found running in a clear env")
        application_path = os.path.dirname(os.path.abspath(__file__))

    # remove flask_session directory if it exists
    wipe_out()
    
    app = Flask(__name__)
    app.wsgi_app = APIV0_Middleware(app.wsgi_app, logger)

    app.config["SESSION_PERMANENT"] = False
    app.config["SESSION_TYPE"] = "filesystem"
    app.config["SESSION_COOKIE_SAMESITE"] = "None"
    app.config["SESSION_COOKIE_SECURE"] = True

    api = Api(app)
    CORS(app, supports_credentials=True)
    Session(app)

    if os.name == 'nt':
        path_lib = ".\\high_quality_engineering_lib.dll"
    else:
        path_lib = f"{os.getcwd()}/lib.so"
    
    path_lib = os.path.join(application_path, path_lib)
    l0 = L0(path_lib=path_lib)
    l1 = L1(path_lib=path_lib)
    utils = Utils()

    device_cnt = l0.getDeviceListSize()
    if not device_cnt > 0:
        logger.warning("No devices found!")
    else:
        logger.info(f"Found {device_cnt} devices")

    api.add_resource(API_Time, "/api/v0/time", resource_class_args=[utils])
    api.add_resource(API_Devices, "/api/v0/devices", resource_class_args=[l0])
    api.add_resource(API_Device_Sessions, "/api/v0/device/<int:indx>/sessions", resource_class_args=[logger, l0, l1, utils])
    api.add_resource(API_Device_Generate, "/api/v0/device/<int:indx>/generate", resource_class_args=[logger, l0, l1, utils])
    api.add_resource(API_Device_Passwords, "/api/v0/device/<int:indx>/passwords", resource_class_args=[logger, l0, l1, utils])
    api.add_resource(API_Device_Password_ID, "/api/v0/device/<int:indx>/password/<int:id>", resource_class_args=[logger, l0, l1, utils])

    certpath = os.path.join(application_path, "cert.pem")
    keypath = os.path.join(application_path, "key.pem")
    app.run(ssl_context=(certpath, keypath), debug=False, threaded=False)

    print()
    logger.info("Exiting...")
    utils.stop_tick()
    wipe_out()
