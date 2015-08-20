import cmd
from madgroundstationservice import MADGroundstationService
import subprocess
import time
import sys

class HelloWorld(cmd.Cmd):
    """Simple command processor example."""
    
    def do_greet(self, line):
        print "hello"
        
    def do_exit(self, line):
        "Exit the groundstation"
        registry.kill()
        service.kill()
        sys.exit()
    
    def do_EOF(self, line):
        return True

if __name__ == '__main__':
    registry = subprocess.Popen(['python', 'registryServer.py', '-q', 'True'])
    print('Registry started')
    time.sleep(5)
    service = subprocess.Popen(['python', 'madgroundstationservice.py'])
    print('Service started')
    time.sleep(5)
    HelloWorld().cmdloop()