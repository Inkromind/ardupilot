import rpyc
import logging
import json
logging.basicConfig()

class TestClient:
    
    def init(self):
        self.con = rpyc.connect_by_service('MAD')
        self.relayId = self.con.root.register_relay(self.relayCallback)
        self.bgsrv = rpyc.BgServingThread(self.con)
        print "Register relay with id %d" % self.relayId
        
    def relayCallback(self, messageId, payload, senderId=-1):
        '''Message received from service'''
        print "Received message: SenderId: %d | MessageId: %s | Payload: %s" % \
        (senderId, messageId, payload)
        
    def makeEstimate(self, packageId, estimate):
        asyncFunc = rpyc.async(self.con.root.add_estimate)
        asyncFunc(packageId, estimate)
        
    def requestCorridor(self, resId, corId, corType, alt, x1, y1, x2, y2):
        msg = {'resId' : resId, 'corId' : corId, 'corType' : corType,
                 'alt' : alt, 'p1' : {'x' : x1, 'y' : y1}, 'p2' : {'x' : x2, 'y' : y2}}
        asyncRelay = rpyc.async(self.con.root.broadcast)
        asyncRelay('REQUEST_CORRIDOR_RESERVATION', json.dumps(msg))
        
    def announceCorridor(self, corId, corType, alt, x1, y1, x2, y2):
        msg = {'corId' : corId, 'corType' : corType,
                 'alt' : alt, 'p1' : {'x' : x1, 'y' : y1}, 'p2' : {'x' : x2, 'y' : y2}}
        asyncRelay = rpyc.async(self.con.root.broadcast)
        asyncRelay('CORRIDOR_ANNOUNCEMENT', json.dumps(msg))
        
    def corridorConflict(self, droneId, resId, prelId, prelType, prelAlt, conId, conType, conAlt):
        msg = {'droneId' : droneId, 'resId' : resId,
                 'id1' : prelId, 'type1' : prelType, 'alt1' : prelAlt,
                 'id2' : conId, 'type2' : conType, 'alt2' : conAlt}
        asyncRelay = rpyc.async(self.con.root.unicast)
        asyncRelay(droneId, 'CORRIDOR_RESERVATION_CONFLICT', json.dumps(msg))
        
    def completedPackage(self, packageId):
        asyncFunc = rpyc.async(self.con.root.completedPackage)
        asyncFunc(packageId)
        
    def failedPackage(self, packageId):
        asyncFunc = rpyc.async(self.con.root.failedPackage)
        asyncFunc(packageId)