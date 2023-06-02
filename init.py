import sys
import subprocess
import os
from shutil import rmtree

subprocess.check_call([sys.executable, '-m', 'pip', 'install', '-r', 'requirements.txt'])
binpath = os.path.dirname(os.path.abspath(__file__)) + '/bin'
if os.path.exists(binpath):
    rmtree(binpath)

if not os.path.exists(binpath + '/build'):
    os.makedirs(binpath + '/build')


os.chdir(binpath + '/build')
subprocess.call(['cmake', '../..'])
subprocess.call(['make'])
os.rename(binpath + "/build/CHESS", binpath + "/validator")
os.rename(binpath + "/build/CHESS_TEST", binpath + "/tests")
os.rename(binpath + "/build/ENGINE", binpath + "/bot")


data = '{' + f"\"bot\": \"{binpath}/bot\", \"multiplayer\": \"{binpath}/validator\"" + '}'
with open(os.path.dirname(os.path.abspath(__file__)) + "/server/binary_paths", "w") as file:
    print(data, file=file)
