#!/usr/bin/env python
'''MAD'''

import time, os

from MAVProxy.modules.lib import mp_module
from pymavlink import mavutil
import sys, traceback

class MadModule(mp_module.MPModule):
    def __init__(self, mpstate):
        '''initialisation code'''
        super(MadModule, self).__init__(mpstate, "MAD", "MAD module")
        self.add_command('setDroneId', self.cmd_setDroneId, "Set the drone id", ["<id>"])
        self.add_command('hideCorridors', self.cmd_hideCorridors, "Hide the corridor announcement")
        self.add_command('showCorridors', self.cmd_showCorridors, "Show the corridor announcement")
        self.add_command('initPlanner', self.cmd_init, "Initialize Planners")
        self.add_command('setHomebase', self.cmd_setHomebase, "Set the homebase-coordinates", ["<x> <y>"])
        self.add_command('resetHomebase', self.cmd_resetHomebase, "Reset homebase to last takeoff location")
        self.add_command('pauseMission', self.cmd_pause, "Pause mission")
        self.add_command('returnHome', self.cmd_returnHome, "Pause mission and return home")
        self.add_command('resumeMission', self.cmd_resume, "Resume mission")
        self.add_command('getLocation', self.cmd_getLocation, "Get current location")
        self.add_command('requestPackage', self.cmd_requestPackage, "Request a new package to be transported", ["<id> <pickupX> <pickupY> <deliveryX> <deliveryY>"])
        self.add_command('assignPackage', self.cmd_assignPackage, "Assign a new package to a drone", ["<id> <pickupX> <pickupY> <deliveryX> <deliveryY>"])
        self.add_command('requestCorridor', self.cmd_requestCorridor, "Request a new corridor", ["<droneId> <resId> <id> <type> <x1> <y1> <x2> <y2>"])
        self.add_command('corridorConflict', self.cmd_corridorConflict, "Announce a new corridor conflict", ["<droneId> <resId> <prelId> <prelType> <prelAlt> <id> <type> <alt>"])
        self.add_command('announceCorridor', self.cmd_announceCorridor, "Announce a reserved corridor", ["<droneId> <id> <type> <x1> <y1> <x2> <y2>"])
        self.add_command('resetCounters', self.cmd_resetCounters, "Reset Logging Counters")
        self.add_command('getCounters', self.cmd_getCounters, "Get Logging Counters")
        self.droneId = 0
        self.showCorridors = False
        
    def cmd_hideCorridors(self, args):
        '''Hide corridor announcements'''
        print("Hiding Corridor Announcements")
        self.showCorridors = False
        
    def cmd_showCorridors(self, args):
        '''Show corridor announcements'''
        print("Showing Corridor Announcements")
        self.showCorridors = True
        
    def cmd_init(self, args):
        '''Initialize Planners'''
        print("Initializing Planners")
        self.master.mav.mad_init_planner_send(0, 0)
        
    def cmd_setDroneId(self, args):
        '''Set the drone id'''
        usage = "Usage: <id>"
        
        if len(args) != 1:
            print(usage)
            return;
    
        id = int(args[0])
        if id < 0:
            print("Invalid drone id %d" % id)
            return
            
        self.droneId = id
        
        print("Drone ID set to %d" % (id))
        
    def cmd_setHomebase(self, args):
        '''Set the homebase-coordinates'''
        usage = "Usage: <x> <y>"
    
        if len(args) != 2:
            print(usage)
            return;
            
        xCo = int(args[0])
        yCo = int(args[1])
        
        print("Setting homebase to <%i, %i>" % (xCo, yCo))
        self.master.mav.mad_set_homebase_send(xCo, yCo)

    def cmd_resetHomebase(self, args):
        '''Reset homebase to last takeoff location'''
        print("Resetting homebase to last takeoff location")
        self.master.mav.mad_reset_homebase_send(0, 0)
        
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
        ''''Get location'''
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
            print("Invalid package id %d" % id)
            return
        
        pickupX = int(args[1])
        pickupY = int(args[2])
        deliveryX = int(args[3])
        deliveryY = int(args[4])
        
        print("Requesting package #%d to be moved from <%d, %d> to <%d, %d>" % (id, pickupX, pickupY, deliveryX, deliveryY))
        self.master.mav.mad_request_package_estimate_send(id, pickupX, pickupY, deliveryX, deliveryY)
        
    def cmd_assignPackage(self, args):
        '''Assign new package'''
        usage = "Usage: <id> <pickupX> <pickupY> <deliveryX> <deliveryY>"
    
        if len(args) != 5:
            print(usage)
            return;
            
        id = int(args[0])
        if id < 0:
            print("Invalid package id %d" % id)
            return
        
        pickupX = int(args[1])
        pickupY = int(args[2])
        deliveryX = int(args[3])
        deliveryY = int(args[4])
        
        print("Assigning package #%d to be moved from <%d, %d> to <%d, %d>" % (id, pickupX, pickupY, deliveryX, deliveryY))
        self.master.mav.mad_assign_package_send(id, pickupX, pickupY, deliveryX, deliveryY)
        
    def cmd_requestCorridor(self, args):
        '''Request a new corridor'''
        usage = "Usage: <droneId> <resId> <id> <type> <alt> <x1> <y1> <x2> <y2>"
    
        if len(args) != 9:
            print(usage)
            return;
            
        droneId = int(args[0])
        if droneId < 0:
            print("Invalid drone id %d" % droneId)
            return
        resId = int(args[1])
        if resId < 0:
            print("Invalid reservation id %d" % resId)
            return
        corId = int(args[2])
        if corId < 0:
            print("Invalid corridor id %d" % corId)
            return
        type = int(args[3])
        if type < 0:
            print("Invalid type %d" % type)
            return
        alt = int(args[4])
        if alt < 0:
            print("Invalid altitude %d" % alt)
            return
        
        x1 = int(args[5])
        y1 = int(args[6])
        x2 = int(args[7])
        y2 = int(args[8])
        
        print("Requesting new corridor: DroneId: %d | ResId: %d | CorId: %d | Type: %d | Alt: %d | <%d, %d> | <%d, %d>" % \
              (droneId, resId, corId, type, alt, x1, y1, x2, y2))
        self.master.mav.mad_request_corridor_reservation_send(droneId, resId, corId, type, alt, x1, y1, x2, y2)
        
    def cmd_corridorConflict(self, args):
        '''Announce a new corridor conflict'''
        usage = "Usage: <droneId> <resId> <prelId> <prelType> <prelAlt> <id> <type> <alt>"
    
        if len(args) != 8:
            print(usage)
            return;
            
        droneId = int(args[0])
        if droneId < 0:
            print("Invalid drone id %d" % droneId)
            return
        resId = int(args[1])
        if resId < 0:
            print("Invalid reservation id %d" % resId)
            return
        prelId = int(args[2])
        if prelId < 0:
            print("Invalid preliminary corridor id %d" % prelId)
            return
        prelType = int(args[3])
        if prelType < 0:
            print("Invalid preliminary corridor type %d" % prelType)
            return
        prelAlt = int(args[4])
        if prelAlt < 0:
            print("Invalid preliminary corridor altitude %d" % prelAlt)
            return
        id = int(args[5])
        if id < 0:
            print("Invalid corridor id %d" % id)
            return
        type = int(args[6])
        if type < 0:
            print("Invalid corridor type %d" % type)
            return
        alt = int(args[7])
        if alt < 0:
            print("Invalid corridor altitude %d" % alt)
            return
        
        print("Announcing corridor conflict: DroneId: %d | ResId: %d | PrelId: %d | PrelType: %d | PrelAlt: %d | Id: %d | Type: %d | Alt: %d" % \
              (droneId, resId, prelId, prelType, prelAlt, id, type, alt))
        self.master.mav.mad_corridor_reservation_conflict_send(droneId, 0, resId, prelId, prelType, prelAlt, id, type, alt)
        
    def cmd_announceCorridor(self, args):
        '''Announce a reserved corridor'''
        usage = "Usage: <droneId> <id> <type> <alt> <x1> <y1> <x2> <y2>"
    
        if len(args) != 8:
            print(usage)
            return;
            
        droneId = int(args[0])
        if droneId < 0:
            print("Invalid drone id %d" % droneId)
            return
        corId = int(args[1])
        if corId < 0:
            print("Invalid corridor id %d" % corId)
            return
        type = int(args[2])
        if type < 0:
            print("Invalid type %d" % type)
            return
        alt = int(args[3])
        if alt < 0:
            print("Invalid altitude %d" % alt)
            return
        
        x1 = int(args[4])
        y1 = int(args[5])
        x2 = int(args[6])
        y2 = int(args[7])
        
        print("Announcing corridor: DroneId: %d | CorId: %d | Type: %d | Alt: %d | <%d, %d> | <%d, %d>" % \
              (droneId, corId, type, alt, x1, y1, x2, y2))
        self.master.mav.mad_corridor_announcement_send(droneId, corId, type, alt, x1, y1, x2, y2)
        
    def cmd_resetCounters(self, args):
        '''Reset Logging Counters'''
        print("Resetting logging counters")
        self.master.mav.mad_reset_logging_send(0)
        
    def cmd_getCounters(self, args):
        '''Get Logging Counters'''
        print("Getting logging counters")
        self.master.mav.mad_get_logging_send(0)
        

    def mavlink_packet(self, m):
        '''handle a mavlink packet'''
        if m.get_type() == 'MAD_CURRENT_LOCATION':
            print "Current Location: <%(x).2f,%(y).2f> | Altitude: %(altitude).2f" % \
                {'x' : m.x, 'y': m.y, 'altitude': m.alt}
        elif m.get_type() == 'MAD_FAILED_PACKAGE':
            print "Failed package #%d" % m.package_id
        elif m.get_type() == 'MAD_COMPLETED_PACKAGE':
            print "Completed package #%d" % m.package_id
        elif m.get_type() == 'MAD_PACKAGE_ESTIMATE':
            print "Package estimate: ID #%(id)d | Estimate: %(estimate).2f" % \
                {'id' : m.package_id, 'estimate' : m.estimate}
        elif m.get_type() == 'MAD_CONFIRM_PACKAGE':
            print "Package confirmed: ID #%(id)d | Estimate: %(estimate).2f" % \
                {'id' : m.package_id, 'estimate' : m.estimate}
        elif m.get_type() == 'MAD_REQUEST_CORRIDOR_RESERVATION':
            print "Corridor Requested: ResId: %(resId)d | Id: %(corId)d | Type: %(corType)d | Alt: %(alt).2f | <%(p1x).2f,%(p1y).2f> - <%(p2x).2f,%(p2y).2f>" % \
                {'resId' : m.reservation_id, 'corId' : m.corridor_id, 'corType' : m.corridor_type,
                 'alt' : m.alt, 'p1x' : m.p1x, 'p1y' : m.p1y, 'p2x' : m.p2x, 'p2y' : m.p2y}
        elif m.get_type() == 'MAD_CORRIDOR_ANNOUNCEMENT':
            if self.showCorridors:
                print "Corridor Announced: Id: %(corId)d | Type: %(corType)d | Alt: %(alt).2f | <%(p1x).2f,%(p1y).2f> - <%(p2x).2f,%(p2y).2f>" % \
                    {'corId' : m.corridor_id, 'corType' : m.corridor_type,
                     'alt' : m.alt, 'p1x' : m.p1x, 'p1y' : m.p1y, 'p2x' : m.p2x, 'p2y' : m.p2y}
        elif m.get_type() == 'MAD_CORRIDOR_RESERVATION_CONFLICT':
            print "Reservation Conflict: Drone Id: %(droneId)d | ResId: %(resId)d | Id1: %(id1)d | Type1: %(type1)d | Alt1: %(alt1).2f | Id2: %(id2)d | Type2: %(type2)d | Alt2: %(alt2).2f" % \
                {'droneId' : m.drone_id, 'resId' : m.reservation_id,
                 'id1' : m.preliminary_id, 'type1' : m.preliminary_type, 'alt1' : m.preliminary_alt,
                 'id2' : m.conflicting_id, 'type2' : m.conflicting_type, 'alt2' : m.conflicting_alt}
        elif m.get_type() == 'MAD_LOGGING_REPLY':
            print "Logging Reply: %d;%d;%d;%d;%.2f;%d;%d;%d;%d" % \
            (m.retries, m.rounds, m.res_failures, m.res_succes, m.flight_levels, m.returns, m.lands, m.pack_completed, m.pack_failed)

def init(mpstate):
    '''initialise module'''
    return MadModule(mpstate) 