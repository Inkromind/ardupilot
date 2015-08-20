#!/usr/bin/env python
'''MADRelay'''

import time, os

from MAVProxy.modules.lib import mp_module
from pymavlink import mavutil
import sys, traceback
import rpyc
import logging
logging.basicConfig()

class MadRelayModule(mp_module.MPModule):
    def __init__(self, mpstate):
        super(MadRelayModule, self).__init__(mpstate, "MADRelay", "MADRelay module")
        self.add_command('registerRelay', self.cmd_registerRelay, "Register the relay with the groundstation")
        self.add_command('unregisterRelay', self.cmd_unregisterRelay, "Unregister the relay with the groundstation")
        self.connected = False
        self.droneId = 1
        '''initialisation code'''
    
    def cmd_registerRelay(self, args):
        '''Register the relay'''
        print("Registering the relay")
        if (not self.connected):
            self.connected = True
            self.con = rpyc.connect_by_service("MADGroundstation")
            self.con.root.register_relay(self.droneId, self.relayCallback)
            self.bgsrv = rpyc.BgServingThread(self.con)
        
    def cmd_unregisterRelay(self, args):
        '''Unregister the relay'''
        print("Unregistering the relay")
        if (self.connected):
            self.connected = False
            self.bgsrv.stop()
            self.con.root.unregister_relay(self.droneId)
            self.con.close()
            
    def relayCallback(self, messageId, payload):
        '''Message received from service'''
        print "Received message: Id: %(id)s | Payload: %(data)s" % {'id' : messageId, 'data' : payload}
        
    def getLocation(self):
        '''Get location'''
        print("Requesting location")
        self.master.mav.mad_get_current_location_send(0, 0)

    def mavlink_packet(self, m):
        'handle a mavlink packet'''
        if m.get_type() == 'MAD_CURRENT_LOCATION':
            if (self.connected):
                print("Forwarded location")
                asyncCall = rpyc.async(self.con.root.locationCallback)
                asyncCall(self.droneId, m.x, m.y, m.alt)

def init(mpstate):
    '''initialise module'''
    return MadRelayModule(mpstate) 