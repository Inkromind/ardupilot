import time
from rwlock import RWLock

class MADContract():
    
    def __init__(self, package):
        self.state = 'INIT'
        self.estimates = dict()
        self.winner = ()
        self.package = package
        self.failures = 0
        self.assignedTo = dict()
        self.completedBy = set()
        self.rwLock = RWLock()
        
    def openContract(self):
        # Requires locks:
        #   self.rwLock (write)
        self.state = 'OPEN'
        self.start = time.time()
        self.estimates = dict()
        
    def hasExpired(self):
        return (self.timeRemaining() <= 0)
    
    def timeRemaining(self):
        if not self.state == 'OPEN':
            return 0
        else:
            return (self.start + 10 - time.time())
    
    def addEstimate(self, droneId, estimate):
        # Requires locks:
        #   self.rwLock (write)
        if estimate >= 0:
            self.estimates[droneId] = estimate
            print "Estimate for package #%d received from drone %d: %.02f" % (self.package.id, droneId, estimate)
            
    def confirmAssignment(self, droneId, estimate):
        # Requires locks:
        #   self.rwLock (write)
        self.state = 'ASSIGNED'
        self.assignedTo[droneId] = estimate
        if self.winner:
            winnerId = self.winner[0]
            if winnerId == droneId:
                print "Package #%d succesfully assigned to drone %d (original estimate: %.2f | new estimate: %.2f)" % \
                (self.package.id, winnerId, self.winner[1], estimate)
            else:
                print "WARNING: Invalid assignment of package #%d to drone %d (winner: %d | original estimate: %.2f | new estimate: %.2f)" % \
                (self.package.id, droneId, winnerId, self.winner[1], estimate)
        else:
            print "WARNING: Invalid assignment of package #%d to drone %d (no winner)" % \
            (self.package.id, droneId)
        if len(self.assignedTo) > 1:
            print "WARNING: Package #%d assigned to more than 1 drone (%d)" % (self.package.id, len(self.assignedTo))
            
    def closeContract(self):
        # Requires locks:
        #   self.rwLock (write)
        if not self.state == 'OPEN':
            return True
        if not self.hasExpired():
            return False
        else:
            self.state = 'CALCULATING_WINNER'
            if (self.estimates):
                lowestEstimate = ()
                for drone, estimate in self.estimates.iteritems():
                    if not lowestEstimate:
                        lowestEstimate = drone, estimate
                    elif estimate < lowestEstimate[1]:
                        lowestEstimate = drone, estimate
                self.winner = lowestEstimate
                self.state = 'PENDING_ASSIGNMENT'
            else:
                self.failures += 1
                if self.failures >= 5:
                    self.state = 'FAILED_(UNASSIGNED)'
                else:
                    self.state = 'WAITING_FOR_RETRY'
            return True
        
    def completedContract(self, droneId):
        # Requires locks:
        #   self.rwLock (write)
        self.completedBy.add(droneId)
        self.state = 'COMPLETED'
        if self.winner:
            if not droneId == self.winner[0]:
                print "WARNING: Package #%d completed by wrong drone %d (assigned to %d)" % \
                (self.package.id, droneId, self.winner[0])
            else:
                print "Package #%d completed by drone %d" % \
                (self.package.id, droneId)
        else:
            print "WARNING: Package #%d completed by wrong drone %d (no winner)" % \
            (self.package.id, droneId)
        if len(self.completedBy) > 1:
            print "ERROR: Package #%d completed by more than 1 drone (%d)" % \
            (self.package.id, len(self.completedBy))
    
    def failedContract(self, droneId):
        # Requires locks:
        #   self.rwLock (write)
        if self.state == 'COMPLETED':
            print "Package #%d failed by drone %d but already completed by drone(s): %s" % \
            (self.package.id, droneId, ', '.join('{:d}'.format(droneId) for droneId in self.completedBy))
        elif not self.state == 'OPEN':
            self.state = 'FAILED'
            print "ERROR: Package #%d failed by drone %d" % \
            (self.package.id, droneId)
        else:
            print "ERROR: Package #%d failed by drone %d but contract still open" % \
            (self.package.id, droneId)
        
    def getState(self):
        # Requires locks:
        #   self.rwLock (read)
        returnString = "Package #{:d}: State: {}".format(self.package.id, self.state)

        if self.state == 'OPEN':
            returnString += " | Estimates: {:d} | Time Remaining: {:-.0f} | Failures: {:d}". format(len(self.estimates), self.timeRemaining(), self.failures)
        if self.winner:
            returnString += " | Won by: {:d} | Estimate: {:-.2f}".format(self.winner[0], self.winner[1])
        if self.assignedTo:
            returnString += " | Assigned To: "
            returnString += ', '.join('{:d} ({:-.2f})'.format(droneId, estimate) for droneId, estimate in self.assignedTo.iteritems())
        if self.completedBy:
            returnString += " | Completed by: " + ', '.join('{:d}'.format(droneId) for droneId in self.completedBy)
        return returnString