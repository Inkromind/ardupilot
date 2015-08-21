import cmd
import subprocess
import sys
import rpyc
import time

class MADCLI(cmd.Cmd):
    """CLI for the MAD Service."""
        
    def do_addPackage(self, line):
        args = line.split()
        
        usage = "Usage: <pickupX> <pickupY> <deliveryX> <deliveryY>"
    
        if len(args) != 4:
            print(usage)
            return;
            
        pickupX = int(args[0])
        pickupY = int(args[1])
        deliveryX = int(args[2])
        deliveryY = int(args[3])
        
        try:
            self.con.root.add_new_package(pickupX, pickupY, deliveryX, deliveryY)
        except:
            print "Unexpected error:", sys.exc_info()[0]
    
    def help_addPackage(self):
        print "Add a new package to be serviced"
        print "Usage: <pickupX> <pickupY> <deliveryX> <deliveryY>"
        
    def do_getDrones(self, line):
        "Get the registered drones"
        try:
            print self.con.root.get_drones()
        except:
            print "Unexpected error:", sys.exc_info()[0]
        
    def do_getPackageStatus(self, line):
        args = line.split()
        
        usage = "Usage: <packageId>"
    
        if len(args) != 1:
            print(usage)
            return;
        
        packageId = int(args[0])
        if packageId < 0:
            print("Invalid package id %d" % packageId)
            return
        try:
            print self.con.root.get_package_status(packageId)
        except:
            print "Unexpected error:", sys.exc_info()[0]
        
    def help_getPackageStatus(self):
        print "Get the status of a package"
        print "Usage: <packageId>"
        
    def do_cancelPackage(self, line):
        args = line.split()
        
        usage = "Usage: <packageId>"
    
        if len(args) != 1:
            print(usage)
            return;
        
        packageId = int(args[0])
        if packageId < 0:
            print("Invalid package id %d" % packageId)
            return
        
        try:
            self.con.root.cancel_package(packageId)
        except:
            print "Unexpected error:", sys.exc_info()[0]
        
    def help_cancelPackage(self):
        print "Cancel a package"
        print "Usage: <packageId>"
        
    def do_printContracts(self, line):
        args = line.split()
        
        usage = "Usage: [STATE=ALL]"
    
        if len(args) > 1:
            print(usage)
            return;
        
        try:
            if len(args) == 1:
                self.con.root.print_contracts(args[0])
            else:
                self.con.root.print_contracts()
        except:
            print "Unexpected error:", sys.exc_info()[0]
        
    def help_printContracts(self):
        print "Print contracts"
        print "Usage: [STATE=ALL]"
        
    def complete_printContracts(self, text, line, begidx, endidx):
        available_states = ('ALL', 'INIT', 'OPEN', 'FAILED_(UNASSIGNED)', 'WAITING_FOR_RETRY', 'CANCELLED', \
                            'PENDING_ASSIGNMENT', 'UNCONFIRMED_ASSIGNMENT', 'ASSIGNED', 'CALCULATING_WINNER', \
                            'COMPLETED', 'FAILED')
        return [i for i in available_states if i.startswith(text)]
        
    def do_exit(self, line):
        "Exit the groundstation"
        clockTicker.kill()
        service.kill()
        registry.kill()
        sys.exit()
    
    def do_EOF(self, line):
        return True
    
    def connectToService(self):
        try:
            self.con = rpyc.connect_by_service('MAD')
        except:
            print "Unexpected error:", sys.exc_info()[0]
        else:
            print "Connected CLI to service"
        
    def do_reconnect(self, line):
        "Reconnect the CLI to the Service"
        try:
            self.con = self.con.close()
        except:
            print "Unexpected error:", sys.exc_info()[0]
        else:
            print "Disconnect CLI from service"
        self.connectToService()
        
    def do_restartClock(self, line):
        "Restart the clock"
        global clockTicker
        try:
            clockTicker.kill()
        finally:
            clockTicker = subprocess.Popen(['python', 'madclock.py'])
        
    def emptyline(self):
        pass        

if __name__ == '__main__':
    try:
        registry = subprocess.Popen(['python', 'registryServer.py', '-q', 'True'])
        service = subprocess.Popen(['python', 'madservice.py'])
        clockTicker = subprocess.Popen(['python', 'madtimer.py'])
        time.sleep(7)
        cli = MADCLI()
        cli.connectToService()
        cli.cmdloop()
    finally:
        try:
            clockTicker.kill()
        finally:
            pass
        try:
            service.kill()
        finally:
            pass
        try:
            registry.kill()
        finally:
            pass
        
