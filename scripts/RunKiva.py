#! /usr/bin/env python3
import shutil
import subprocess
import os

ScripsSrc="/home/carlos/archivos/Colciencias/Jovenes Investigadores 2015/KIVA-GUI/KIVA-GUI/build-Qt-GUI-Desktop_Qt_5_8_0_GCC_64bit-Debug"
ScrName='RunKiva.sh'

dst=os.getcwd()

shutil.copy(''.join([ScripsSrc,'/',ScrName]), dst)
subprocess.call("".join(["./",ScrName]))

os.remove(''.join([dst,'/',ScrName]))
os.mkdir(''.join([dst,'/','TERMINADO','RunKiva.py']))
