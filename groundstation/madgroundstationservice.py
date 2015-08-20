import rpyc
import logging
from __builtin__ import dict
import sys
import time
import json
logging.basicConfig()

class MADGroundstationService(rpyc.Service):
    
    droneRelays = dict()
    packages = dict()
    openContracts = dict()
    contracts = dict()
    pendingAssignments = dict()
    
    nextRelayId = 0
        
  
    def on_connect(self):
        # code that runs when a connection is created
        # (to init the serivce, if needed)
        self.relayId = -1
        pass

    def on_disconnect(self):
        # code that runs when the connection has already closed
        # (to finalize the service, if needed)
        self.exposed_unregister_relay()
        pass
    
    def exposed_register_relay(self, relayCallback):
        if (self.relayId == -1):
            self.relayId = MADGroundstationService.nextRelayId
            MADGroundstationService.nextRelayId += 1
            MADGroundstationService.droneRelays[self.relayId] = rpyc.async(relayCallback)
            print "Relay for drone %(id)d registered" % {"id" : self.relayId}
        return self.relayId
        
    def exposed_unregister_relay(self):
        if self.relayId >= 0 and self.relayId in MADGroundstationService.droneRelays:
            del MADGroundstationService.droneRelays[self.relayId]
            print "Relay for drone %(id)d unregistered" % {"id" : self.relayId}
        
    def exposed_get_relays(self):
        return MADGroundstationService.droneRelays.keys()

    #     def exposed_get_locations(self):
    #         for relay in MADGroundstationService.droneRelays.itervalues():
    #             relay()
    def exposed_request_new_package(self, pickupX, pickupY, deliveryX, deliveryY):
        package = MADPackage(pickupX, pickupY, deliveryX, deliveryY)
        MADGroundstationService.packages[package.id] = package
        contract = MADContract(package)
        MADGroundstationService.contracts[package.id] = contract
        
        self.openContract(contract)
            
        print "Started contract net for package #%d from <%.0f, %.0f> to <%.0f, %.0f>" % (package.id, pickupX, pickupY, deliveryX, deliveryY)
            
    def exposed_add_estimate(self, packageId, estimate):
        if self.relayId >= 0 and packageId in MADGroundstationService.openContracts:
            MADGroundstationService.openContracts[packageId].addEstimate(self.relayId, estimate)
    
    def exposed_get_package_status(self, packageId):
        if not packageId in MADGroundstationService.contracts:
            return "No contract for package #%d" % packageId
        else:
            contract = MADGroundstationService.contracts[packageId]
            if contract.state == "INIT":
                return "Package #%d: State: Init" % packageId
            elif contract.state == "OPEN":
                return "Package #%d: State: Open | Estimates: %d | Time Remaining: %d | Failures: %d" % \
                    (packageId, len(contract.estimates), contract.timeRemaining(), contract.failures)
            else:
                winner = contract.winner
                if winner:
                    return "Package #%d: State: %s | Won by: %s | Estimate: %.0f" % \
                        (packageId, contract.state, winner[0], winner[1])
                else:
                    return "Package #%d: State: %s" % \
                        (packageId, contract.state)
                        
    def exposed_confirm_package_assignment(self, packageId, estimate):
        if packageId in MADGroundstationService.pendingAssignments:
            contract, start, failures = MADGroundstationService.pendingAssignments[packageId]
            del MADGroundstationService.pendingAssignments[packageId]
            contract.state = "ASSIGNED"
            winnerId = contract.winner[0]
            if winnerId == self.relayId:
                print "Package #%d succesfully assigned to drone %d (original estimate: %.2f | new estimate: %.2f)" % \
                (packageId, winnerId, contract.winner[1], estimate)
            else:
                print "WARNING: Invalid assignment of package #%d to drone %d (winner: %d | original estimate: %.2f | new estimate: %.2f)" % \
                (packageId, self.relayId, winnerId, contract.winner[1], estimate)
            
    def exposed_timeTick(self):
        for packageId in list(MADGroundstationService.openContracts.keys()):
            contract = MADGroundstationService.openContracts[packageId]
            if contract.closeContract():
                del MADGroundstationService.openContracts[packageId]
                if contract.state == "PENDING ASSIGNMENT":
                    droneId, estimate = contract.winner
                    print "Contract for package #%d won by drone #%d with estimate %.0f" % \
                    (packageId, droneId, estimate)
                    self.assignContract(contract)
                elif contract.state == "FAILED (UNASSIGNED)":
                    print "Contract assignment for package #%d failed due to no assignments" % packageId
                elif contract.state == "WAITING FOR RETRY":
                    print "No estimates for package #%d. Retrying (failures: %d)" % (packageId, contract.failures)
                    self.openContract(contract)
                else:
                    print "WARNING: Contract for package #%d in unknown state after closing: %s" % (packageId, contract.state)
                    
        for packageId in list(MADGroundstationService.pendingAssignments.keys()):
            contract, start, failures = MADGroundstationService.pendingAssignments[packageId]
            if time.time() - 10 >= start:
                del MADGroundstationService.pendingAssignments[packageId]
                failures += 1
                winner = contract.winner
                if failures >= 5:
                    contract.state = "UNCONFIRMED ASSIGNMENT"
                    print "WARNING: Assignment of package #%d to drone %d unconfirmed" % (packageId, winner[0])
                else:
                    print "Retrying assignment of package #%d to drone %d (failures: %d)" % (packageId, winner[0], failures)
                    self.assignContract(contract, failures)
        
    def openContract(self, contract):
        contract.openContract()
        package = contract.package
        MADGroundstationService.openContracts[package.id] = contract
        
        payload = package.convertToJson()
        
        for relay in MADGroundstationService.droneRelays.itervalues():
            relay("REQUEST_ESTIMATE", payload)
            
    def assignContract(self, contract, failures = 0):
        droneId, estimate = contract.winner
        package = contract.package
        if droneId in MADGroundstationService.droneRelays:
            MADGroundstationService.pendingAssignments[package.id] = contract, time.time(), failures
            MADGroundstationService.droneRelays[droneId]("ASSIGN_PACKAGE", package.convertToJson())
        else:
            print "ERROR: Contract for package #%d won by invalid drone: %d" % (package.id, droneId)
            
    def exposed_broadcast(self, messageId, payload):
        for relayId, relay in MADGroundstationService.droneRelays.iteritems():
            if not relayId == self.relayId:
                relay(messageId, payload, self.relayId)
    
    def exposed_unicast(self, recipientId, messageId, payload):
        if recipientId in MADGroundstationService.droneRelays:
            MADGroundstationService.droneRelays[recipientId](messageId, payload, self.relayId)
        

class MADPackage():
    
    nextId = 0
    
    def __init__(self, pickupX, pickupY, deliveryX, deliveryY, id=-1):
        if (id == -1):
            self.id = MADPackage.nextId
            MADPackage.nextId += 1
        else:
            self.id = id
        self.pickup = {'x' : pickupX, 'y' : pickupY}
        self.delivery = {'x' : deliveryX, 'y' : deliveryY}
        
    def convertToJson(self):
        return json.dumps({"packageId" : self.id, "pickup": self.pickup, "delivery": self.delivery})
        
class MADContract():
    
    def __init__(self, package):
        self.state = "INIT"
        self.estimates = dict()
        self.winner = ()
        self.package = package
        self.failures = 0
        
    def openContract(self):
        self.state = "OPEN"
        self.start = time.time()
        self.estimates = dict()
        
    def hasExpired(self):
        return (self.timeRemaining() <= 0)
    
    def timeRemaining(self):
        if not self.state == "OPEN":
            return 0
        else:
            return (self.start + 10 - time.time())
    
    def addEstimate(self, relayId, estimate):
        if estimate >= 0:
            self.estimates[relayId] = estimate
            print "Estimate for package #%d received from relay %d: %.02f" % (self.package.id, relayId, estimate)
            
    def closeContract(self):
        if not self.state == "OPEN":
            return True
        if not self.hasExpired():
            return False
        else:
            self.state = "CALCULATING WINNER"
            if (self.estimates):
                highestEstimate = ()
                for relay, estimate in self.estimates.iteritems():
                    if not highestEstimate:
                        highestEstimate = relay, estimate
                    elif estimate > highestEstimate[1]:
                        highestEstimate = relay, estimate
                self.winner = highestEstimate
                self.state = "PENDING ASSIGNMENT"
            else:
                self.failures += 1
                if self.failures >= 5:
                    self.state = "FAILED (UNASSIGNED)"
                else:
                    self.state = "WAITING FOR RETRY"
            return True
        
    
    
if __name__ == "__main__":
    from rpyc.utils.server import ThreadedServer
    t = ThreadedServer(MADGroundstationService, port = 18861, auto_register = True)
    t.start()