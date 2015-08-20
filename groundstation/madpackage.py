class MADPackage():
    
    nextId = 0
    
    def __init__(self, pickupX, pickupY, deliveryX, deliveryY):
        self.id = MADPackage.nextId
        MADPackage.nextId += 1
        self.pickupX = pickupX
        self.pickupY = pickupY
        self.deliveryX = deliveryX
        self.deliveryY = deliveryY
        self.state = "INIT"