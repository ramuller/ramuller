import xbmc as x

class DialogProgressBG():
    def __init__(self):
        pass

    def create(self, heading, message):
        x.log('heading : {}'.format(heading), x.LOGDEBUG)
        x.log('message : {}'.format(message), x.LOGDEBUG)

    def update(self, percent = None, heading = '', message = ''):
        x.log('percent : {}'.format(percent), x.LOGDEBUG)
        x.log('heading : {}'.format(heading), x.LOGDEBUG)
        x.log('message : {}'.format(message), x.LOGDEBUG)

    def close(self):
        pass
