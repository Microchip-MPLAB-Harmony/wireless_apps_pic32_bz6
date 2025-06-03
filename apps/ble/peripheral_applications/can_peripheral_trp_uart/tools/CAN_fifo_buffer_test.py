import argparse
import logging
import queue
import sys
import threading
import secrets
import datetime
import sys, re
import os
import mba
import time
from mba import CanFrame

os. system('CLS')

#initialization
aliveCounter = 0
deviceCounter = 0
deviceCounterTarget = 6
can_payload = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
stop_program = False

#for debug
def _device_callback(data):
  global aliveCounter
  global deviceCounter
  global deviceCounterTarget
  global stop_program
  print("Tool Rx a Msg", data.command)
  if(data.command  == mba.CAN_MSG_RX):
    print("1")
    if(data.msg.busId == 0):
      print("2")
      if(data.msg.id == 0x555):
         print("Rx Data on channel 0 %s" %hex(data.msg.id))
         stop_program = True
  return

#checks if a CAN-FD Bus Analyzer is detected
num_devices, types, serial_nums = mba.enum_devices()
if 0 >= num_devices:
  raise Exception('No bus analyzers detected.')
print("Found %d connected bus analyzer " %num_devices + "with serial number %s" %serial_nums[0])

#checks if the CAN-FD Bus Analyzer can be opened
device = mba.Mba()
instance = device.open_device(serial_nums[0])
if 0 > instance:
  logging.error("Failed to open bus analyzer: %s" %str(instance))
  raise Exception('Failed to open bus analyzer.')
  
#uncomment below only when debugging  
#rc = device.register_callback(_device_callback, None) 

#should ALWAYS print
print("This is a BigBuck CAN test")
            
#################################################
#  Setup CAN interface
#################################################

#sets up CAN channel, mode and nominal bit rate
device.can_set_speed(mba.CAN0, 500, 0)
device.can_set_mode(mba.CAN0, mba.CAN_MODE_CLASSIC, mba.CAN_TESTMODE_NORMAL, False)

#sets the CAN message ID, DLC and payload
can_id = 0x7ff        #0x469, 0x45a, 0x7ff
can_dlc = 8           #this value specifies the actual amount of data being sent, max 8 for CAN
can_payload[0] = 0x55
can_payload[1] = 0x0a
can_payload[2] = 0x02
can_payload[3] = 0x03
can_payload[4] = 0x04
can_payload[5] = 0x05
can_payload[6] = 0x06
can_payload[7] = 0x07

#command to send frame
device.can_send_frame(mba.CAN0, can_id, can_payload, can_dlc, mba.CANFRAME_FLAG_NONE, 1000)

#print message ID and payload
print("Transmitted message on channel 0: ID = %s and Message = " %hex(can_id) + " " .join(hex(x) for x in can_payload))