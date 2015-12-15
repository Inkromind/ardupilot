import cmd
import sys
import rpyc
import time
import random
import logging
import math
logging.basicConfig(filename="logs/autoRequest_%s.log" % time.strftime("%H-%M-%S_%d-%m-%Y"), level=logging.INFO, format='%(asctime)s - %(message)s')

class MADAutoRequest(cmd.Cmd):
    """CLI for the MAD Service."""
        
    def do_exit(self, line):
        "Exit the groundstation"
        sys.exit()
    
    def do_EOF(self, line):
        return True
    
    def connectToService(self):
        self.configured = False
        try:
            self.con = rpyc.connect_by_service('MAD')
        except:
            print "Unexpected error:", sys.exc_info()[0]
        else:
            print "Connected AutoRequest to service"
            
    def do_setParam(self, line):
        args = line.split()
        
        usage = "Usage: <interval> <amount> <minX> <maxX> <minY> <maxY> <minDist> [homebases]"
    
        if len(args) != 8 and len(args) != 7:
            print(usage)
            return;
            
        self.interval = int(args[0])
        self.amount = int(args[1])
        self.minX = int(args[2])
        self.maxX = int(args[3])
        self.minY = int(args[4])
        self.maxY = int(args[5])
        self.minDist = int(args[6])
        
        if len(args) == 8:
            self.homebases = self.processHomebases(args[7])
        else:
            self.homebases = []
            
        self.configured = True
        
    def processHomebases(self, rawHomebases):
        rawList = rawHomebases.split(";")
        
        homebases = []
        
        for rawHomebase in rawList:
            coordinates = rawHomebase.split(",")
            x = int(coordinates[0])
            y = int(coordinates[1])
            homebases.append({'x' : x, 'y' : y})
        return homebases
            
        
    def help_setParam(self):
        print "Set parameters in one line"
        print "Usage: <interval> <amount> <minX> <maxX> <minY> <maxY> <minDist> [homebases]"
        
    def setSeed(self, seed):
        self.seed = seed
        
        print "Setting seed to %d" % seed
        random.seed(seed)
        
    def do_setSeed(self, line):
        args = line.split()
        
        usage = "Usage: <seed>"
    
        if len(args) != 1:
            print(usage)
            return;
        
        self.setSeed(int(args[0]))
    
    def help_setSeed(self):
        print "Set seed"
        print "Usage: <seed>"
            
    def do_setup(self, line):
        self.interval = input("Please enter an interval (s): ")
        self.amount = input("Please enter number of packages: ")
        self.minX = input("Please enter minimum X-coordinate: ")
        self.maxX = input("Please enter maximum X-coordinate: ")
        self.minY = input("Please enter minimum Y-coordinate: ")
        self.maxY = input("Please enter maximum Y-coordinate: ")
        self.minDist = input("Please enter minimum distance (m): ")
        
        self.homebases = self.processHomebases(raw_input("Please enter homebases in the format x,y. Seperate homebases with ; : "))
        
        self.configured = True
        
    def do_start(self, line):
        if (not self.configured):
            print "Please configure requests prior to starting. Use setup or setParam"
        else:
            self.startRequests();
            
    def startRequests(self):
        print "Requesting %d packages between <%d,%d> and <%d,%d> with interval %d s, seed %d and min distance %d" % \
        (self.amount, self.minX, self.minY, self.maxX, self.maxY, self.interval, self.seed, self.minDist)
        print "Homebases:"
        for homebase in self.homebases:
            print "<%d,%d>" % (homebase['x'], homebase['y'])
        logging.info("%d - Requesting %d packages between <%d,%d> and <%d,%d> with interval %d s, seed %d and min distance %d" % \
        (int(time.time()), self.amount, self.minX, self.minY, self.maxX, self.maxY, self.interval, self.seed, self.minDist))
        logging.info("%d - Homebases:" % int(time.time()))
        for homebase in self.homebases:
            logging.info("%d - <%d,%d>" % (int(time.time()), homebase['x'], homebase['y']))
        for i in range(1, self.amount + 1):
            package = self.getNewPackage()
            print "Requesting package #%d from <%d,%d> to <%d,%d> (dist: %.2f)" % \
            (i, package['pickup']['x'], package['pickup']['y'], package['delivery']['x'], package['delivery']['y'], package['dist'])
            logging.info("%d - Requesting package #%d from <%d,%d> to <%d,%d> (dist: %.2f)" % \
                         (int(time.time()), i, package['pickup']['x'], package['pickup']['y'], package['delivery']['x'], package['delivery']['y'], package['dist']))
            try:
                self.con.root.add_new_package(package['pickup']['x']*100, package['pickup']['y']*100, package['delivery']['x']*100, package['delivery']['y']*100)
            except:
                print "Unexpected error:", sys.exc_info()[0]
            time.sleep(self.interval)
        print("Finished requesting all packages")
        logging.info("%d - Finished requesting all packages" %  int(time.time()))
        
            
    def getNewPackage(self):
        
        pickup = self.getPoint()
        
        validDeliveryPoint = False
        delivery = {'x' : 0, 'y' : 0}
        dist = 0
        while not validDeliveryPoint:
            delivery = self.getPoint()
            dist = math.sqrt( (delivery['x'] - pickup['x'])**2 + (delivery['y'] - pickup['y'])**2 )
            if (dist >= self.minDist):
                validDeliveryPoint = True
        return {'pickup' : pickup, 'delivery' : delivery, 'dist' : dist}
    
    def getPoint(self):
        validPoint = False
        point = {'x' : 0, 'y' : 0}
        while not validPoint:
            point = {'x' : random.randint(self.minX, self.maxX), 'y' : random.randint(self.minY, self.maxY)}
            validPoint = True
            for homebase in self.homebases:
                dist = math.sqrt( (point['x'] - homebase['x'])**2 + (point['y'] - homebase['y'])**2 )
                if (dist < self.minDist):
                    validPoint = False
                    break
        return point
        
    def emptyline(self):
        pass        

if __name__ == '__main__':
    autoRequest = MADAutoRequest()
    autoRequest.connectToService()
    autoRequest.setSeed(1)
    autoRequest.cmdloop()
