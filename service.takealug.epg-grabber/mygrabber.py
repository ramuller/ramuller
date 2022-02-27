#!/usr/bin/env python

import xbmc
import xbmcaddon
import time
from datetime import datetime
import os
import json
import re
import socket
from collections import Counter

# ADDON = xbmcaddon.Addon(id="service.takealug.epg-grabber")

# from resources.lib import xml_structure
from resources.providers import tvspielfilm_DE
