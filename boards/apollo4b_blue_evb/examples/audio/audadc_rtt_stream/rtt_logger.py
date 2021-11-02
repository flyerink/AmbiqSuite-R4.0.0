#
# Helper script for jlinkrttlogger
#
import subprocess
import time

timestr = time.strftime("%Y%m%d-%H-%M-%S")

#file name for pcm raw data capture
bin_name = timestr + ".pcm"

#run rtt logger
subprocess.run(["jlinkrttlogger.exe", "-Device", 'AMAPH1KK-KBR', "-If", "SWD", "-speed", "4000", "-RTTAddress", "0x10060404", "-RTTChannel", "1", bin_name])
