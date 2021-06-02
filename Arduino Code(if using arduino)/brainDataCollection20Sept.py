import serial 


def main():

    # declare port, baud, and file path
    serial_port = 'COM8'
    baud_rate = 57600
    fileName= "BHUMI_12.csv"

    # open file for writing and  get serial feed
    ser = serial.Serial(serial_port, baud_rate)
    file = open(fileName, "w")
    print("Created File")

    samples = 6 #how many samples to collect
    print_labels = True
    line = 0 #start at 0 because our header is 0 (not real data)
    while line <= samples:
        if print_labels:
            if line==0:
                 file = open(fileName, "a")
                 file.write( "Attention,Meditation,Delta,Theta,lowAlpha, highAlpha,lowBeta,highBeta,lowGamma,midGamma" + "\n" )
                 file.close()
            else:
                print("Line " + str(line) + ": writing...")
        
        getData=ser.readline()
        getData=getData.decode("utf-8")
        data=getData[0:][:-2]
        print(data)
 
        file = open(fileName, "a")
        file.write(data + "\n") #write data with a newline
        line = line+1

    file.close()
    print("Data collection complete!")



main()
