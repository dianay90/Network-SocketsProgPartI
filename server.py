#DIANA OH CS372 Program 1 server.py. This program acts as the server in a mini chat project.  
from socket import *
from sys import argv 
import time, sys

# Cite: CS372 Class Lecture Notes 
# TESTED ON FLIP 1

quit = False
# Start up socket connection and send message saying read to receive

def startUpConnection(serverSocket,serverPort):
    #If the connection is closed, chatserve repeats from 2 (until a SIGINT is received).
    serverSocket.bind(('',serverPort))
    serverSocket.listen(1)
    print ("The server is ready to receive")
    #chatserve starts on host A 

# Receive and Display client message 
def receiveAndDisplayMessage(connectionSocket): 
     global quit 
     #Receive Message
     sentence = connectionSocket.recv(555)
     # Display Host B Message 
     string = sentence 

     if '\quit' not in string:
        print(sentence)
     # If client wants to quit set quit flag to true 
     else:
         quit = True 

#Send message to client
def  sendMessagetoClient(serverUserName,connectionSocket):
     global quit 

     while(True):
         # Send message from host A. Ensure it's not greater than 500 chars 
        hostASentence = raw_input(serverUserName + ">")
        #Did this to more specifically make hostASentence be treated internally as a string 
        string = hostASentence
        
        if '\quit' not in string:
            if len(hostASentence) > 500: 
                print("Message should be up to 500 characters.It currently exceeds that.Please try again")
                        
            else:
                connectionSocket.send(serverUserName + "> " + hostASentence) 
                break
        else:
            quit = True
            break

while True: 
    try: 
        if len(argv) > 1:
            serverPort = int(argv[1])
            print("Port Number entered is " + str(serverPort))

        else: 
            print("Incorrect number of arguments in command line. Please try again.")
            exit(1) 

        # Get username and ensure it's not more than 10 characters 
        #serverUserName = raw_input("What is the username?")
        serverUserName = "Server"
        # if len(serverUserName) > 10:
          #  serverUserName = serverUserName[0:10]
        
        serverSocket = socket(AF_INET,SOCK_STREAM)
        startUpConnection(serverSocket,serverPort) 

        while(True): 
            #Accept connections from outside
            quit = False
            print("Server is accepting connections")
            connectionSocket, addr = serverSocket.accept()	
    
            while(True):

                receiveAndDisplayMessage(connectionSocket)
                
                if quit == False:
                    sendMessagetoClient(serverUserName,connectionSocket)
                
                if quit == True: #User has opted to quit 
                    connectionSocket.close() 
                    break

    except KeyboardInterrupt:
        print ("Sig Int has been caught.")
        sys.exit()



