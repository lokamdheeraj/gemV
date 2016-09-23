#!/usr/bin/env python3

import subprocess
import os
import time

home = os.environ["HOME"]

# files / first_line; edit if necessaary
golden_run = "/home/lokam/Desktop/GEM5/gem5/out.txt"
first_line = "**** REAL SIMULATION ****"

# don't change anything below
typescript_outputfile = home+"/"+"scriptlog.txt"
# commands
startup_command = "gnome-terminal -x script -f "+typescript_outputfile
clean_textcommand = "col -bp <"+typescript_outputfile+" | less -R"
# remove old outputfile
try:
    os.remove(typescript_outputfile)
except Exception:
    pass
# initiate typescript
subprocess.Popen(["/bin/bash", "-c", startup_command])
time.sleep(1)
# read golden run
with open(golden_run) as src:
    original = src.read()
orig_section = original[original.find(first_line):]
# read last output of current results so far
def get_last():
    read = subprocess.check_output(["/bin/bash", "-c", clean_textcommand]).decode("utf-8")
    if not first_line+"\n" in read:
        return "Waiting for first line"
    else:
        return read[read.find(first_line):]
    with open(typescript_outputfile, "wt") as clear:
        clear.write("\n")
# loop
while True:
    current = get_last()
    if current == "\n":
        pass
    else:
        if not current in orig_section and current != "Waiting for first line":
            orig = orig_section.split("\n")
            breakpoint = current.split("\n")
            diff = [(orig[i], breakpoint[i]) for i in range(len(breakpoint)) \
                    if not orig[i] == breakpoint[i]]
            print("error: "+str(diff[0]))
            break
        else:
            pass
    time.sleep(5)


