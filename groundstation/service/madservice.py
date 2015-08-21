import rpyc
import logging
from __builtin__ import dict
import time
import json
from madcontract import MADContract
from threading import Lock
from rwlock import RWLock
logging.basicConfig()

class MADService(rpyc.Service):
    
    drones = dict()
    dronesLock = RWLock()
    openContracts = dict()
    openContractsLock = RWLock()
    contracts = dict()
    contractsLock = RWLock()
    pendingAssignments = dict()
    pendingAssignmentsLock = Lock()
    
    nextDroneIdLock = Lock()
    nextDroneId = 0
        
  
    def on_connect(self):
        # code that runs when a connection is created
        # (to init the serivce, if needed)
        self.droneId = -1
        pass

    def on_disconnect(self):
        # code that runs when the connection has already closed
        # (to finalize the service, if needed)
        self.exposed_unregister_relay()
        pass
    
    def exposed_register_relay(self, droneCallback):
        if (self.droneId == -1):
            MADService.nextDroneIdLock.acquire() # Acquire lock
            MADService.dronesLock.acquire_write() # Acquire write lock
            self.droneId = MADService.nextDroneId
            MADService.nextDroneId += 1
            MADService.drones[self.droneId] = rpyc.async(droneCallback)
            MADService.dronesLock.release() # Release write lock
            MADService.nextDroneIdLock.release() # Release lock
            print "Relay for drone %(id)d registered" % {'id' : self.droneId}
        return self.droneId
        
    def exposed_unregister_relay(self):
        MADService.dronesLock.acquire_write() # Acquire write lock
        if self.droneId >= 0 and self.droneId in MADService.drones:
            del MADService.drones[self.droneId]
            print "Relay for drone %(id)d unregistered" % {'id' : self.droneId}
        MADService.dronesLock.release() # Release write lock
        
    def exposed_get_drones(self):
        return MADService.drones.keys()
    
    def exposed_get_package_status(self, packageId):
        MADService.contractsLock.acquire_read() # Acquire read lock
        if not packageId in MADService.contracts:
            returnString = "No contract for package #%d" % packageId
        else:
            contract = MADService.contracts[packageId]
            ': :type contract: MADContract'
            contract.rwLock.acquire_read() # Acquire read lock
            returnString = contract.getState()
            contract.rwLock.release() # Release read lock
        MADService.contractsLock.release() # Release read lock
        return returnString
        
    def exposed_add_new_package(self, pickupX, pickupY, deliveryX, deliveryY):
        package = MADPackage(pickupX, pickupY, deliveryX, deliveryY)
        contract = MADContract(package)
        
        MADService.contractsLock.acquire_write() #Acquire write lock
        MADService.openContractsLock.acquire_write() #Acquire write lock
        contract.rwLock.acquire_write() # Acquire write lock
        
        MADService.contracts[package.id] = contract
        self.openContract(contract)
        
        contract.rwLock.release() # Release write lock
        MADService.openContractsLock.release() # Release write lock
        MADService.contractsLock.release() # Release write lock
            
        print "Started contract net for package #%d from <%.0f, %.0f> to <%.0f, %.0f>" % (package.id, pickupX, pickupY, deliveryX, deliveryY)
        
    def exposed_print_contracts(self, state = 'ALL'):
        MADService.contractsLock.acquire_read() # Acquire read lock
        for contract in MADService.contracts.itervalues():
            ': :type contract: MADContract'
            contract.rwLock.acquire_read() # Acquire read lock
            if state.lower() == 'all' or contract.state.lower() == state.lower():
                print contract.getState()
            contract.rwLock.release() # Release read lock
        MADService.contractsLock.release() # Release read lock
        
    def exposed_cancel_package(self, packageId):
        MADService.contractsLock.acquire_read() # Acquire read lock
        if packageId in MADService.contracts:
            MADService.openContractsLock.acquire_write() # Acquire write lock
            MADService.pendingAssignmentsLock.acquire() # Acquire write lock
            contract = MADService.contracts[packageId]
            ': :type contract: MADContract'
            contract.rwLock.acquire_write() # Acquire write lock
            if packageId in MADService.openContracts:
                del MADService.openContracts[packageId]
            if packageId in MADService.pendingAssignments:
                del MADService.pendingAssignments[packageId]
            if contract.state == 'OPEN' or contract.state == 'INIT' or contract.state == 'FAILED_(UNASSIGNED)' or contract.state == 'WAITING_FOR_RETRY':
                print "Package #%d cancelled" % packageId
            else:
                print "Package #%d cancelled but was already in state %s" % (packageId, contract.state)
            contract.state = 'CANCELLED'
            contract.rwLock.release() # Release write lock
            MADService.pendingAssignmentsLock.release() # Release write lock
            MADService.openContractsLock.release() # Release write lock
        else:
            print "Package #%d does not exist" % packageId
        MADService.contractsLock.release() # Release read lock
      
    def exposed_add_estimate(self, packageId, estimate):
        MADService.openContractsLock.acquire_read() #Acquire read lock
        if self.droneId >= 0 and packageId in MADService.openContracts:
            contract = MADService.openContracts[packageId]
            ': :type contract: MADContract'
            contract.rwLock.acquire_write() #Acquire write lock
            contract.addEstimate(self.droneId, estimate)
            contract.rwLock.release() # Release write lock
        MADService.openContractsLock.release() # Release read lock
               
    def exposed_confirm_package_assignment(self, packageId, estimate):
        MADService.contractsLock.acquire_read() # Acquire read lock
        if packageId in MADService.contracts:
            MADService.openContractsLock.acquire_write() # Acquire write lock
            MADService.pendingAssignmentsLock.acquire() # Acquire write lock
            contract = MADService.contracts[packageId]
            ': :type contract: MADContract'
            contract.rwLock.acquire_write() # Acquire write lock
            contract.confirmAssignment(self.droneId, estimate)
            if packageId in MADService.openContracts:
                del MADService.openContracts[packageId]
            if packageId in MADService.pendingAssignments:
                del MADService.pendingAssignments[packageId]
            contract.rwLock.release() # Release write lock
            MADService.pendingAssignmentsLock.release() # Release write lock
            MADService.openContractsLock.release() # Release write lock
        MADService.contractsLock.release() # Release read lock
        
    def exposed_completed_package(self, packageId):
        MADService.contractsLock.acquire_read() # Acquire read lock
        if packageId in MADService.contracts:
            MADService.openContractsLock.acquire_write() # Acquire write lock
            MADService.pendingAssignmentsLock.acquire() # Acquire write lock
            contract = MADService.contracts[packageId]
            contract.rwLock.acquire_write() # Acquire write lock
            contract = MADService.contracts[packageId]
            ': :type contract: MADContract'
            contract.completedContract(self.droneId)
            if packageId in MADService.openContracts:
                del MADService.openContracts[packageId]
            if packageId in MADService.pendingAssignments:
                del MADService.pendingAssignments[packageId]
            contract.rwLock.release() # Release write lock
            MADService.pendingAssignmentsLock.release() # Release write lock
            MADService.openContractsLock.release() # Release write lock
        MADService.contractsLock.release() # Release read lock
    
    def exposed_failed_package(self, packageId):
        MADService.contractsLock.acquire_read() # Acquire read lock
        if packageId in MADService.contracts:
            contract = MADService.contracts[packageId]
            ': :type contract: MADContract'
            contract.rwLock.acquire_write() # Acquire write lock
            contract.failedContract(self.droneId)
            contract.rwLock.release() # Release write lock
        MADService.contractsLock.release() # Release read lock
                      
    def exposed_broadcast(self, messageId, payload):
        MADService.dronesLock.acquire_read() # Acquire read lock
        for droneId, relay in MADService.drones.iteritems():
            if not droneId == self.droneId:
                relay(messageId, payload, self.droneId)
        MADService.dronesLock.release() # Release read lock
    
    def exposed_unicast(self, recipientId, messageId, payload):
        MADService.dronesLock.acquire_read() # Acquire read lock
        if recipientId in MADService.drones:
            MADService.drones[recipientId](messageId, payload, self.droneId)
        MADService.dronesLock.release() # Release read lock

    def exposed_timeTick(self):
        self.checkOpenContracts()
        self.checkPendingAssignments()
 
    def checkOpenContracts(self):
        MADService.dronesLock.acquire_read() # Acquire read lock
        MADService.openContractsLock.acquire_write() # Acquire write lock
        MADService.pendingAssignmentsLock.acquire() # Acquire write lock
        for packageId in list(MADService.openContracts.keys()):
            contract = MADService.openContracts[packageId]
            ': :type contract: MADContract'
            contract.rwLock.acquire_write() # Acquire write lock
            if contract.closeContract():
                del MADService.openContracts[packageId]
                if contract.state == 'PENDING_ASSIGNMENT':
                    droneId, estimate = contract.winner
                    print "Contract for package #%d won by drone #%d with estimate %.0f" % \
                    (packageId, droneId, estimate)
                    self.assignContract(contract)
                elif contract.state == 'FAILED_(UNASSIGNED)':
                    print "Contract assignment for package #%d failed due to no assignments" % packageId
                elif contract.state == 'WAITING_FOR_RETRY':
                    print "No estimates for package #%d. Retrying (failures: %d)" % (packageId, contract.failures)
                    self.openContract(contract)
                else:
                    print "WARNING: Contract for package #%d in unknown state after closing: %s" % (packageId, contract.state)
            contract.rwLock.release() # Release write lock
        MADService.pendingAssignmentsLock.release() # Release write lock
        MADService.openContractsLock.release() # Release write lock
        MADService.dronesLock.release() # Release read lock
                    
    def checkPendingAssignments(self):
        MADService.dronesLock.acquire_read() # Acquire read lock
        MADService.pendingAssignmentsLock.acquire() # Acquire write lock
        for packageId in list(MADService.pendingAssignments.keys()):
            contract, start, failures = MADService.pendingAssignments[packageId]
            ': :type contract: MADContract'
            contract.rwLock.acquire_write() # Acquire write lock
            if time.time() - 10 >= start:
                del MADService.pendingAssignments[packageId]
                failures += 1
                winner = contract.winner
                if failures >= 5:
                    contract.state = 'UNCONFIRMED_ASSIGNMENT'
                    print "WARNING: Assignment of package #%d to drone %d unconfirmed" % (packageId, winner[0])
                else:
                    print "Retrying assignment of package #%d to drone %d (failures: %d)" % (packageId, winner[0], failures)
                    self.assignContract(contract, failures)
            contract.rwLock.release() # Release write lock
        MADService.pendingAssignmentsLock.release() # Release write lock
        MADService.dronesLock.release() # Release read lock
                    
    def openContract(self, contract):
        # Requires locks:
        #   MADService.openContractsLock (write)
        #   contract.rwLock (write)
        contract.openContract()
        package = contract.package
        MADService.openContracts[package.id] = contract
        
        payload = package.convertToJson()
        
        for callback in MADService.drones.itervalues():
            callback('REQUEST_ESTIMATE', payload)

    def assignContract(self, contract, failures = 0):
        # Requires locks:
        #   MADService.dronesLock (read)
        #   MADService.pendingAssignmentsLock (write)            
        #   contract.rwLock (read)
        droneId = contract.winner[0]
        package = contract.package
        if droneId in MADService.drones:
            MADService.pendingAssignments[package.id] = contract, time.time(), failures
            MADService.drones[droneId]('ASSIGN_PACKAGE', package.convertToJson())
        else:
            print "ERROR: Contract for package #%d won by invalid drone: %d" % (package.id, droneId)
            
class MADPackage():
    
    nextId = 0
    nextIdLock = Lock()
    
    def __init__(self, pickupX, pickupY, deliveryX, deliveryY):
        MADPackage.nextIdLock.acquire()
        self.id = MADPackage.nextId
        MADPackage.nextId += 1
        MADPackage.nextIdLock.release()
        self.pickup = {'x' : pickupX, 'y' : pickupY}
        self.delivery = {'x' : deliveryX, 'y' : deliveryY}
        
    def convertToJson(self):
        return json.dumps({'packageId' : self.id, 'pickup': self.pickup, 'delivery': self.delivery})
                       
    
if __name__ == "__main__":
    from rpyc.utils.server import ThreadedServer
    t = ThreadedServer(MADService, port = 18861, auto_register = True)
    time.sleep(5)
    print "Starting MAD Service"
    t.start()