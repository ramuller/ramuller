#!/usr/bin/python
# po2xml.py
#
# Converts PO files into Android Style XML Language Files for use with http://www.colatris.com/
#
# @depends https://pypi.python.org/pypi/polib
# @see http://polib.readthedocs.org/en/latest/quickstart.html
#
# apt-get install python-polib
# apt-get install python3-polib
# emerge dev-python/polib
# pip install polib

import sys
import codecs
import polib # Make sure version 1.0.5 or better

if (len(sys.argv) != 3):
	print("Say: " + sys.argv[0] + " src_file.{po|mo} tgt_file.xml")
	sys.exit(0)

# Define the files
src_file = sys.argv[1]
tgt_file = codecs.open(sys.argv[2], 'w', 'utf-8')

# Detect the Extension (weak ass type detection)
src_kind = src_file[-2:]

# @todo Detect if MO or PO File
# file_source = './dio.pot'

if 'mo' == src_kind:
	src_data = polib.mofile(src_file)
elif 'po' == src_kind:
	src_data = polib.pofile(src_file)

# Spew
print >>tgt_file, "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
print >>tgt_file, "<resources>"

for line in src_data:
	print >>tgt_file, "  <string name=\"" + line.msgid + "\">" + line.msgstr + "</string>"

print >>tgt_file, "</resources>"
