#!/usr/bin/env python
'''MAD'''

import time, os

from MAVProxy.modules.lib import mp_module
from pymavlink import mavutil
import sys, traceback

class MadModule(mp_module.MPModule):
    def __init__(self, mpstate):
        super(MadModule, self).__init__(mpstate, "MAD", "MAD module")
        self.add_command('initPlanner', self.cmd_init, "Initialize Planners")
        self.add_command('pauseMission', self.cmd_pause, "Pause mission")
        self.add_command('returnHome', self.cmd_returnHome, "Pause mission and return home")
        self.add_command('resumeMission', self.cmd_resume, "Resume mission")
        self.add_command('getLocation', self.cmd_getLocation, "Get current location")
        self.add_command('requestPackage', self.cmd_requestPackage, "Request a new package to be transported", ["<id> <pickupX> <pickupY> <deliveryX> <deliveryY>"])
        '''initialisation code'''
    
    def cmd_init(self, args):
        'Initialize Planners'''
        print("Initializing Planners")
        self.master.mav.mad_init_planner_send(0, 0)

    def cmd_pause(self, args):
        '''Pause mission'''
        print("Pausing mission")
        self.master.mav.mad_pause_mission_send(0, 0)
      
    def cmd_resume(self, args):
        '''Resume mission'''
        print("Resuming mission")
        self.master.mav.mad_resume_mission_send(0, 0)
        
    def cmd_returnHome(self, args):
        '''Return home'''
        print("Returning Home")
        self.master.mav.mad_return_home_send(0, 0)
    
    def cmd_getLocation(self, args):
        'Get location'''
        print("Requesting location")
        self.master.mav.mad_get_current_location_send(0, 0)
    
    def cmd_requestPackage(self, args):
        '''Request new package'''
        usage = "Usage: <id> <pickupX> <pickupY> <deliveryX> <deliveryY>"
    
        if len(args) != 5:
            print(usage)
            return;
            
        id = int(args[0])
        if id < 0:
            print("Invalid package id %u" % id)
            return
        
        pickupX = int(args[1])
        pickupY = int(args[2])
        deliveryX = int(args[3])
        deliveryY = int(args[4])
        
        print("Requesting package #%u to be moved from <%u, %u> to <%u, %u>" % (id, pickupX, pickupY, deliveryX, deliveryY))
        self.master.mav.mad_request_package_send(id, pickupX, pickupY, deliveryX, deliveryY)
        

    def mavlink_packet(self, m):
        'handle a mavlink packet'''
        if m.get_type() == 'MAD_CURRENT_LOCATION':
            print "Current Location: <%(x).2f,%(y).2f> | Altitude: %(altitude).2f" % \
                {"x" : m.x, "y": m.y, "altitude": m.alt}

def init(mpstate):
    '''initialise module'''
    return MadModule(mpstate) 