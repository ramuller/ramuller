
import logging
import time

LOGDEBUG = logging.DEBUG
LOGNOTICE = logging.INFO
LOGERROR = logging.CRITICAL
LOGINFO = logging.INFO

def translatePath(path):
    return(path)

def log(msg, level):
    print(msg)
    # logging.log(logging.INFO, msg)

def sleep(to):
    time.sleep(to)
