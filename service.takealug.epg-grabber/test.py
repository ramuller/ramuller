#!/bin/python

import re

POFILE = './resources/language/resource.language.en_gb/strings.po'
POFILE = './resources/language/resource.language.de_de/strings.po'

reCtxt = re.compile('^msgctxt.*')
reId = re.compile('^msgid.*')
reStr = re.compile('^msgstr.*')

po_dict = {}

def getMsgValues(regex, lines, start, size = 10):
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
        

with open(POFILE) as fp:
   lines = fp.readlines()
   cnt = 0
   print(len(lines))
   while cnt < len(lines):
       ctxt = reCtxt.match(lines[cnt])
       if ctxt:
           tid = re.sub('.*#', '', ctxt[0])
           tid = re.sub('"', '', tid)
           po_dict[tid] = {'msgid' : '', 'msgstr' : ''}
           po_dict[tid]['msgid'] = getMsgValues(reId, lines, cnt)
           po_dict[tid]['msgstr'] = getMsgValues(reStr, lines, cnt)
       cnt += 1

print('{}'.format(po_dict))
