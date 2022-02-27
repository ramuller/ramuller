import xml.dom.minidom
import polib
import xbmc as x
import re

POFILE = './resources/language/resource.language.en_gb/strings.po'
# POFILE = './resources/language/resource.language.de_de/strings.po'

class Addon():
    def __init__(self, id):
        self.id = id
        self.addon = xml.dom.minidom.parse('addon.xml')
        self.settings = {}
        for s in xml.dom.minidom.parse('settings.xml').getElementsByTagName('setting'):
            try:
                self.settings[s.getAttribute('id')] = s.firstChild.nodeValue
            except:
                self.settings[s.getAttribute('id')] = ''
        self.po = polib.pofile('resources/language/resource.language.de_de/strings.po')
        for entry in self.po.translated_entries():
            print('id : {} , str : {}'.format(entry.msgid, entry.msgstr))
        self.poDict = self._initLocalization(POFILE)
        # sys.exit(0)
            

    def _getMsgValues(self, regex, lines, start, size = 10):
        cnt = 1
        ret = ''
        while start + cnt < len(lines) and cnt < size + 1:
            l = lines[start + cnt]
            val = regex.match(l)
            if val:
                ret = re.sub('.* "', '', l)
                ret = re.sub('"\n', '', l)
                return ret
            cnt += 1
        return ret

    def _initLocalization(self, fn):
        poDict = {}
        reCtxt = re.compile('^msgctxt.*')
        reId = re.compile('^msgid.*')
        reStr = re.compile('^msgstr.*')
        with open(POFILE) as fp:
            lines = fp.readlines()
            cnt = 0
            # print(len(lines))
            while cnt < len(lines):
                ctxt = reCtxt.match(lines[cnt])
                if ctxt:
                    tid = re.sub('.*#', '', ctxt[0])
                    tid = re.sub('"', '', tid)
                    poDict[tid] = {'msgid' : '', 'msgstr' : ''}
                    poDict[tid]['msgid'] = self._getMsgValues(reId, lines, cnt)
                    poDict[tid]['msgstr'] = self._getMsgValues(reStr, lines, cnt)
                    if poDict[tid]['msgstr'] == '':
                        poDict[tid]['msgstr'] = poDict[tid]['msgid']
                cnt += 1
        return poDict

    def getAddonInfo(self, field):
        addon = self.addon.getElementsByTagName('addon')
        return addon[0].getAttribute(field)

    def getLocalizedString(self, id):
        # print('Localize ID : {}'.format(self.poDict[str(id)]['msgstr']))
        # x.log('Localize ID : {}'.format(id), x.LOGINFO)
        # import pdb; pdb.set_trace()
        return self.poDict[str(id)]['msgstr']

    def getSetting(self, id):
        print('setting ID : {}; value : {}'.format(id, self.settings[id]))
        return self.settings[id]

