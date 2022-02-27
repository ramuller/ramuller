import xbmc as x

class Dialog():
    def __init__(self):
        pass

    
    def notification(self, head, msg, icon = None, time = None, sound = None):
        print('Head : {}'.format(head))
        print('MSG : {}'.format(msg))

    def yesno(self, head, msg, nl = None, yl = None, cl = None, ac = None):
        print('Head : {}'.format(head))
        print('MSG : {}'.format(msg))
        return True

    def multiselect(self, heading, options, autoclose = 0, preselect = [], useDetails = False):
        print('Head : {}'.format(heading))
        print('opt : {}'.format(options))
        print('autoclose : {}'.format(autoclose))
        print('preselect : {}'.format(preselect))
        print('usDetails : {}'.format(useDetails))
        print('size of options : {}'.format(len(options)))
        select = list(range(len(options)))
        x.log('list of options : {}'.format(select), x.LOGDEBUG)
        import sys
        return select

    def ok(self, heading, message):
        pass
        
