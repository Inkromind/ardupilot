import rpyc
import logging
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