class ListItem():
    def __init__(self, label = None):
        print('ListItem : {}'.format(label))
        self.art = {}
        self.property = {}

    def setArt(self, art):
        self.art = art

    def setProperty(self, key, val):
        self.property[key] = val

    def getProperty(self, key):
        return self.property[key]
