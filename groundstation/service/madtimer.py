import rpyc
import time

class MADTimer():
    
    def __init__(self):
        self.con = rpyc.connect_by_service('MAD')
        self.asyncFunc = rpyc.async(self.con.root.timeTick)
        
    def start(self):
        while True:
            time.sleep(0.2)
            self.asyncFunc()
            

if __name__ == "__main__":
    time.sleep(6)
    print "Starting Clock"
    MADTimer().start()
            