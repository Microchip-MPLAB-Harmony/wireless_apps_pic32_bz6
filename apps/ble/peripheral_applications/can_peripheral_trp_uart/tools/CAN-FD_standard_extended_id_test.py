"""*****************************************************************************
* Copyright (C) 2025 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*****************************************************************************"""

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
canfd_payload = [0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
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
print("This is a BigBuck CAN-FD test")
            
#################################################
#  Setup CAN-FD interface
#################################################

#sets up CAN-FD channel, mode, nominal bit rate and bit switching rate
device.can_set_speed(mba.CAN0, 500, 2000)
device.can_set_mode(mba.CAN0, mba.CAN_MODE_FD, mba.CAN_TESTMODE_NORMAL, False)

#sets the CAN-FD message ID, DLC and payload
canfd_id = 0x10000096        #0x469, 0x10000096
canfd_dlc = 16          #this value specifies the actual amount of data being sent, max 64 for CAN-FD
canfd_payload[0] = 0x55
canfd_payload[1] = 0x0a
canfd_payload[2] = 0x02
canfd_payload[3] = 0x03
canfd_payload[4] = 0x04
canfd_payload[5] = 0x05
canfd_payload[6] = 0x06
canfd_payload[7] = 0x07
canfd_payload[8] = 0x08
canfd_payload[9] = 0x09
canfd_payload[10] = 0x0a
canfd_payload[11] = 0x0b
canfd_payload[12] = 0xff
canfd_payload[13] = 0xff
canfd_payload[14] = 0xff
canfd_payload[15] = 0xff 

#commands to send frame with standard vs extended message IDs
if canfd_id == 0x469:
  device.can_send_frame(mba.CAN0, canfd_id, canfd_payload, canfd_dlc, mba.CANFRAME_FLAG_FD | mba.CANFRAME_FLAG_BRS, 1000)
elif canfd_id == 0x10000096:
  device.can_send_frame(mba.CAN0, canfd_id, canfd_payload, canfd_dlc, mba.CANFRAME_FLAG_FD | mba.CANFRAME_FLAG_EXTENDED | mba.CANFRAME_FLAG_BRS, 1000)

#print message ID and payload
print("Transmitted message on channel 0: ID = %s and Message = " %hex(canfd_id) + " " .join(hex(x) for x in canfd_payload))