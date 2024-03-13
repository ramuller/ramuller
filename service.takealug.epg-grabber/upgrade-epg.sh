#!/bin/bash


cd /local/data/src/ramuller/service.takealug.epg-grabber
python service.py
export SSHPASS=".Teufel"
sshpass -e scp /local/data/src/service.takealug.epg-grabber/guide.xml root@192.168.123.128:/www

