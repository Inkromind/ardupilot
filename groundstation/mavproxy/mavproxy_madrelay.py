#!/usr/bin/env python
'''MADRelay'''

import time, os

from MAVProxy.modules.lib import mp_module
from pymavlink import mavutil
import sys, traceback
import rpyc
import logging
import json
logging.basicConfig(filename="logs/%s.log" % time.strftime("%H-%M-%S_%d-%m-%Y"), level=logging.INFO, format='%(asctime)s - %(message)s')

class MadRelayModule(mp_module.MPModule):
    def __init__(self, mpstate):
        '''initialisation code'''
        super(MadRelayModule, self).__init__(mpstate, "MADRelay", "MADRelay module")
        self.add_command('registerRelay', self.cmd_registerRelay, "Register the relay with the groundstation")
        self.add_command('unregisterRelay', self.cmd_unregisterRelay, "Unregister the relay with the groundstation")
        self.connected = False
        self.droneId = -1
        self.resetMsgCounters()
        self.logFile = False
        
    def resetMsgCounters(self):
        self.receivedMsg = 0;
        self.sendMsg = 0;
    
    def cmd_registerRelay(self, args):
        '''Register the relay'''
        print("Registering relay")
        if self.connected:
            try:
                self.droneId = self.con.root.register_relay(self.relayCallback)
                print "Registered relay with id %d" % self.droneId
                if not self.logFile:
                    self.logFile = "logs/%s_%d" % (time.strftime("%H-%M-%S_%d-%m-%Y"), self.droneId)
                    print "Logging to file %s" % self.logFile
                logging.info("%d - Registered relay with id %d" % (int(time.time()), self.droneId))
            except:
                print "Connection error. Reopening connection..."
                self.connected = False
                try:
                    self.bgsrv.stop()
                finally:
                    pass
                try:
                    self.con.close()
                finally:
                    pass
                
        if not self.connected:
            try:
                self.con = rpyc.connect_by_service('MAD')
                self.bgsrv = rpyc.BgServingThread(self.con)
                self.droneId = self.con.root.register_relay(self.relayCallback)
                print "Registered relay with id %d" % self.droneId
                self.connected = True
                self.logFile = "logs/%s_%d" % (time.strftime("%H-%M-%S_%d-%m-%Y"), self.droneId)
                
                print "Logging to files %s_*.txt" % self.logFile
                logging.info("%d - Registered relay with id %d" % (int(time.time()), self.droneId))
            except:
                print "Unexpected error:", sys.exc_info()[0]
        
    def cmd_unregisterRelay(self, args):
        '''Unregister the relay'''
        print("Unregistering relay")
        logging.info("%d - Unregistered relay", int(time.time()))
        self.connected = False
        try:
            self.bgsrv.stop()
        finally:
            pass
        try:
            self.con.close()
        finally:
            pass
    
    def relayCallback(self, messageId, payload, senderId=-1):
        '''Callback for the MAD Service'''
        try:
            msg = json.loads(payload)
            if messageId == 'SYNC':
                syncId = msg["syncId"]
                if self.logFile:
                    with open(self.logFile + "_comm.txt", "a") as f:
                        f.write("%d,SYNC,%d\n" % (int(time.time()), syncId))
                    with open(self.logFile + "_log.txt", "a") as f:
                        f.write("%d,SYNC,%d\n" % (int(time.time()), syncId))
                print "Syncing logs"
                logging.info("%d - [SYNC] Syncing logs: %d" % (int(time.time()), syncId))
                self.master.mav.mad_sync_logging_send(syncId)
            elif messageId == 'SYNCANDRESET':
                syncId = msg["syncId"]
                if self.logFile:
                    with open(self.logFile + "_comm.txt", "a") as f:
                        f.write("%d,SYNC,%d\n" % (int(time.time()), syncId))
                    with open(self.logFile + "_log.txt", "a") as f:
                        f.write("%d,SYNC,%d\n" % (int(time.time()), syncId))
                print "Syncing logs and resetting counters"
                self.resetMsgCounters()
                logging.info("%d - [SYNC] Syncing logs and resetting counters: %d" % (int(time.time()), syncId))
                self.master.mav.mad_sync_and_reset_logging_send(syncId)
            elif messageId == 'START':
                if self.logFile:
                    with open(self.logFile + "_comm.txt", "a") as f:
                        f.write("%d,START\n" % int(time.time()))
                    with open(self.logFile + "_log.txt", "a") as f:
                        f.write("%d,START\n" % int(time.time()))
                print "Starting mission"
                logging.info("%d - [MISSION] Mission start" % int(time.time()))
                self.master.mav.mad_resume_mission_send(0, 0)
            elif messageId == 'REQUEST_ESTIMATE':
                # {"packageId" : id, "pickup": {"x": x, "y": y}, "delivery": {"x": x, "y": y}}
                id = int(msg["packageId"])
                pickupX = int(msg["pickup"]["x"])
                pickupY = int(msg["pickup"]["y"])
                deliveryX = int(msg["delivery"]["x"])
                deliveryY = int(msg["delivery"]["y"])
                print("Requesting package #%d to be moved from <%d, %d> to <%d, %d>" % (id, pickupX, pickupY, deliveryX, deliveryY))
                self.master.mav.mad_request_package_estimate_send(id, pickupX, pickupY, deliveryX, deliveryY)
                self.receivedMsg += 1
                if self.logFile:
                    with open(self.logFile + "_comm.txt", "a") as f:
                        f.write("%d,RECEIVED,CONTRACT\n" % int(time.time()))
                logging.info("%d - Requesting package #%d to be moved from <%d, %d> to <%d, %d>" % (int(time.time()), id, pickupX, pickupY, deliveryX, deliveryY))
            elif messageId == 'ASSIGN_PACKAGE':
                # {"packageId" : id, "pickup": {"x": x, "y": y}, "delivery": {"x": x, "y": y}}
                id = int(msg["packageId"])
                pickupX = int(msg["pickup"]["x"])
                pickupY = int(msg["pickup"]["y"])
                deliveryX = int(msg["delivery"]["x"])
                deliveryY = int(msg["delivery"]["y"])
                print("Assigning package #%d to be moved from <%d, %d> to <%d, %d>" % (id, pickupX, pickupY, deliveryX, deliveryY))
                logging.info("%d - Assigning package #%d to be moved from <%d, %d> to <%d, %d>" % (int(time.time()), id, pickupX, pickupY, deliveryX, deliveryY))
                self.master.mav.mad_assign_package_send(id, pickupX, pickupY, deliveryX, deliveryY)
                self.receivedMsg += 1
                if self.logFile:
                    with open(self.logFile + "_comm.txt", "a") as f:
                        f.write("%d,RECEIVED,CONTRACT\n" % int(time.time()))
            elif messageId == 'REQUEST_CORRIDOR_RESERVATION':
                # {'resId' : m.reservation_id, 'corId' : m.corridor_id, 'corType' : m.corridor_type,
                #     'alt' : m.alt, 'p1' : {'x' : m.p1x, 'y' : m.p1y}, 'p2' : {'x' : m.p2x, 'y' : m.p2y}}
                droneId = int(senderId)
                resId = int(msg["resId"])
                corId = int(msg["corId"])
                type = int(msg["corType"])
                alt = int(msg["alt"])
                x1 = int(msg["p1"]["x"])
                y1 = int(msg["p1"]["y"])
                x2 = int(msg["p2"]["x"])
                y2 = int(msg["p2"]["y"])
                print("Received reservation request for new corridor: DroneId: %d | ResId: %d | CorId: %d | Type: %d | Alt: %d | <%d, %d> | <%d, %d>" % \
                  (droneId, resId, corId, type, alt, x1, y1, x2, y2))
                logging.info("%d - Received reservation request for new corridor: DroneId: %d | ResId: %d | CorId: %d | Type: %d | Alt: %d | <%d, %d> | <%d, %d>" % \
                  (int(time.time()), droneId, resId, corId, type, alt, x1, y1, x2, y2))
                self.master.mav.mad_request_corridor_reservation_send(droneId, resId, corId, type, alt, x1, y1, x2, y2)
                self.receivedMsg += 1
                if self.logFile:
                    with open(self.logFile + "_comm.txt", "a") as f:
                        f.write("%d,RECEIVED,CORRIDOR\n" % int(time.time()))
            elif messageId == 'CORRIDOR_ANNOUNCEMENT':
                # {'corId' : m.corridor_id, 'corType' : m.corridor_type,
                #         'alt' : m.alt, 'p1' : {'x' : m.p1x, 'y' : m.p1y}, 'p2' : {'x' : m.p2x, 'y' : m.p2y}}
                droneId = int(senderId)
                corId = int(msg["corId"])
                type = int(msg["corType"])
                alt = int(msg["alt"])
                x1 = int(msg["p1"]["x"])
                y1 = int(msg["p1"]["y"])
                x2 = int(msg["p2"]["x"])
                y2 = int(msg["p2"]["y"])
#                 print("Received corridor announcement: DroneId: %d | CorId: %d | Type: %d | Alt: %d | <%d, %d> | <%d, %d>" % \
#                   (droneId, corId, type, alt, x1, y1, x2, y2))
#                 logging.info("Received corridor announcement: DroneId: %d | CorId: %d | Type: %d | Alt: %d | <%d, %d> | <%d, %d>" % \
#                   (droneId, corId, type, alt, x1, y1, x2, y2))
                self.master.mav.mad_corridor_announcement_send(droneId, corId, type, alt, x1, y1, x2, y2)
                self.receivedMsg += 1
                if self.logFile:
                    with open(self.logFile + "_comm.txt", "a") as f:
                        f.write("%d,RECEIVED,CORRIDOR\n" % int(time.time()))
            elif messageId == 'CORRIDOR_RESERVATION_CONFLICT':
                #{'droneId' : m.drone_id, 'resId' : m.reservation_id,
                #     'id1' : m.preliminary_id, 'type1' : m.preliminary_type, 'alt1' : m.preliminary_alt,
                #     'id2' : m.conflicting_id, 'type2' : m.conflicting_type, 'alt2' : m.conflicting_alt}
                droneId = int(senderId)
                resId = int(msg["resId"])
                prelId = int(msg["id1"])
                prelType = int(msg["type1"])
                prelAlt = int(msg["alt1"])
                id = int(msg["id2"])
                type = int(msg["type2"])
                alt = int(msg["alt2"])
                self.master.mav.mad_corridor_reservation_conflict_send(self.droneId, droneId, resId, prelId, prelType, prelAlt, id, type, alt)
                print("Received reservation conflict: DroneId: %d | ResId: %d | PrelId: %d | PrelType: %d | PrelAlt: %d | Id: %d | Type: %d | Alt: %d" % \
                  (droneId, resId, prelId, prelType, prelAlt, id, type, alt))
                logging.info("%d - Received reservation conflict: DroneId: %d | ResId: %d | PrelId: %d | PrelType: %d | PrelAlt: %d | Id: %d | Type: %d | Alt: %d" % \
                  (int(time.time()), droneId, resId, prelId, prelType, prelAlt, id, type, alt))
                self.receivedMsg += 1
                if self.logFile:
                    with open(self.logFile + "_comm.txt", "a") as f:
                        f.write("%d,RECEIVED,CORRIDOR\n" % int(time.time()))
            elif messageId == 'RESET_COUNTERS':
                print("Resetting logging counters")
                logging.info("%d - Resetting logging counters" % int(time.time()))
                self.master.mav.mad_reset_logging_send(0)
            elif messageId == 'GET_COUNTERS':
                print("Resetting logging counters")
                self.master.mav.mad_get_logging_send(0)
            elif messageId == 'RESET_MSG_COUNTERS':
                print("Resetting msg counters")
                self.resetMsgCounters()
            elif messageId == 'GET_MSG_COUNTERS':
                print("Getting msg counters")
                asyncRelay = rpyc.async(self.con.root.msg_logging_reply)
                asyncRelay(self.sendMsg, self.receivedMsg)
            else:
                print "Received message: SenderId: %d | MessageId: %s | Payload: %s" % \
                (senderId, messageId, payload)
                logging.info("%d - Received message: SenderId: %d | MessageId: %s | Payload: %s" % \
                (int(time.time()), senderId, messageId, payload))
        except:
            print "Unexpected error:", sys.exc_info()[0]
            traceback.print_exc()
            raise

    def mavlink_packet(self, m):
        '''handle a mavlink packet'''
        try:
            if self.connected:
                if m.get_type() == 'MAD_PACKAGE_ESTIMATE':
                    asyncRelay = rpyc.async(self.con.root.add_estimate)
                    asyncRelay(m.package_id, m.estimate)
                    self.sendMsg += 1
                    if self.logFile:
                        with open(self.logFile + "_comm.txt", "a") as f:
                            f.write("%d,SENT,CONTRACT\n" % int(time.time()))
                    logging.info("%(date)d - Package estimate: ID #%(id)d | Estimate: %(estimate).2f" % \
                {'date' : int(time.time()), 'id' : m.package_id, 'estimate' : m.estimate})
                elif m.get_type() == 'MAD_COMPLETED_PACKAGE':
                    asyncRelay = rpyc.async(self.con.root.completed_package)
                    asyncRelay(m.package_id)
                    self.sendMsg += 1
                    logging.info("%d - Completed package #%d" % (int(time.time()), m.package_id))
                    if self.logFile:
                        with open(self.logFile + "_comm.txt", "a") as f:
                            f.write("%d,SENT,CONTRACT\n" % int(time.time()))
                elif m.get_type() == 'MAD_FAILED_PACKAGE':
                    asyncRelay = rpyc.async(self.con.root.failed_package)
                    asyncRelay(m.package_id)
                    self.sendMsg += 1
                    logging.info("%d - Failed package #%d" % (int(time.time()), m.package_id))
                    if self.logFile:
                        with open(self.logFile + "_comm.txt", "a") as f:
                            f.write("%d,SENT,CONTRACT\n" % int(time.time()))
                elif m.get_type() == 'MAD_CONFIRM_PACKAGE':
                    asyncRelay = rpyc.async(self.con.root.confirm_package_assignment)
                    asyncRelay(m.package_id, m.estimate)
                    self.sendMsg += 1
                    logging.info("%(date)d - Package confirmed: ID #%(id)d | Estimate: %(estimate).2f" % \
                                 {'date' : int(time.time()), 'id' : m.package_id, 'estimate' : m.estimate})
                    if self.logFile:
                        with open(self.logFile + "_comm.txt", "a") as f:
                            f.write("%d,SENT,CONTRACT\n" % int(time.time()))
                elif m.get_type() == 'MAD_REQUEST_CORRIDOR_RESERVATION':
                    msg = {'resId' : m.reservation_id, 'corId' : m.corridor_id, 'corType' : m.corridor_type,
                           'alt' : m.alt, 'p1' : {'x' : m.p1x, 'y' : m.p1y}, 'p2' : {'x' : m.p2x, 'y' : m.p2y}}
                    asyncRelay = rpyc.async(self.con.root.broadcast)
                    asyncRelay('REQUEST_CORRIDOR_RESERVATION', json.dumps(msg))
                    self.sendMsg += 1
                    logging.info("%(date)d - Corridor Requested: ResId: %(resId)d | Id: %(corId)d | Type: %(corType)d | Alt: %(alt).2f | <%(p1x).2f,%(p1y).2f> - <%(p2x).2f,%(p2y).2f>" % \
                                 {'date' : int(time.time()), 'resId' : m.reservation_id, 'corId' : m.corridor_id, 'corType' : m.corridor_type,
                                  'alt' : m.alt, 'p1x' : m.p1x, 'p1y' : m.p1y, 'p2x' : m.p2x, 'p2y' : m.p2y})
                    if self.logFile:
                        with open(self.logFile + "_comm.txt", "a") as f:
                            f.write("%d,SENT,CORRIDOR\n" % int(time.time()))
                elif m.get_type() == 'MAD_CORRIDOR_ANNOUNCEMENT':
                    msg = {'corId' : m.corridor_id, 'corType' : m.corridor_type,
                           'alt' : m.alt, 'p1' : {'x' : m.p1x, 'y' : m.p1y}, 'p2' : {'x' : m.p2x, 'y' : m.p2y}}
                    asyncRelay = rpyc.async(self.con.root.broadcast)
                    asyncRelay('CORRIDOR_ANNOUNCEMENT', json.dumps(msg))
                    self.sendMsg += 1
                    if self.logFile:
                        with open(self.logFile + "_comm.txt", "a") as f:
                            f.write("%d,SENT,CORRIDOR\n" % int(time.time()))
                elif m.get_type() == 'MAD_CORRIDOR_RESERVATION_CONFLICT':
                    msg = {'droneId' : m.drone_id, 'resId' : m.reservation_id,
                         'id1' : m.preliminary_id, 'type1' : m.preliminary_type, 'alt1' : m.preliminary_alt,
                         'id2' : m.conflicting_id, 'type2' : m.conflicting_type, 'alt2' : m.conflicting_alt}
                    asyncRelay = rpyc.async(self.con.root.unicast)
                    asyncRelay(m.drone_id, 'CORRIDOR_RESERVATION_CONFLICT', json.dumps(msg))
                    self.sendMsg += 1
                    logging.info("%(date)d - Reservation Conflict: Drone Id: %(droneId)d | ResId: %(resId)d | Id1: %(id1)d | Type1: %(type1)d | Alt1: %(alt1).2f | Id2: %(id2)d | Type2: %(type2)d | Alt2: %(alt2).2f" % \
                {'date' : int(time.time()), 'droneId' : m.drone_id, 'resId' : m.reservation_id,
                 'id1' : m.preliminary_id, 'type1' : m.preliminary_type, 'alt1' : m.preliminary_alt,
                 'id2' : m.conflicting_id, 'type2' : m.conflicting_type, 'alt2' : m.conflicting_alt})
                    if self.logFile:
                        with open(self.logFile + "_comm.txt", "a") as f:
                            f.write("%d,SENT,CORRIDOR\n" % int(time.time()))
                elif m.get_type() == 'MAD_LOGGING_REPLY':
                    asyncRelay = rpyc.async(self.con.root.logging_reply)
                    asyncRelay(m.retries, m.rounds, m.res_failures, m.res_succes, m.flight_levels, m.returns, 
                               m.lands, m.pack_completed, m.pack_failed, m.distance)
                    logging.info("%d - Logging Reply: %d;%d;%d;%d;%.2f;%d;%d;%d;%d;%.2f" % \
                                 (int(time.time()), m.retries, m.rounds, m.res_failures, m.res_succes, m.flight_levels, m.returns, m.lands, m.pack_completed, m.pack_failed, m.distance))

                elif m.get_type() == 'MAD_HEARTBEAT':
                    if self.logFile:
                        with open(self.logFile + "_log.txt", "a") as f:
                            f.write("%d\n" % int(time.time()))
                elif m.get_type() == 'STATUSTEXT':
                    logging.info("%d - APM: %s" % (int(time.time()), m.text))
        except:
            print "Unexpected error:", sys.exc_info()[0]
            traceback.print_exc()
            raise

def init(mpstate):
    '''initialise module'''
    return MadRelayModule(mpstate) 