import threading
from Servo import *
from threading import Thread, Lock
from Control import Control
from Ultrasonic import *
import time
import math
import socket
import sys
import errno
import pickle
import io
import struct
import picamera
forward = 100

# one step
#control = Control()
#time.sleep(1)
#control.forWard()
#control.forWard()
#print("ILERI")
#exit(-1)

#control = Control()
#for j in range(1000):
#	for i in range(20):
#		control.forWard()
	# control.turnRight()
#exit(-1)

flag = True

lock = threading.Lock()
mappingRunning = False
movementRunning = False
condition = threading.Condition(lock)

walkFlag = False

def remove_outliers(data):
    mean = sum(data) / len(data)
    std_dev = (sum((x - mean) ** 2 for x in data) / len(data)) ** 0.5
    if(std_dev == 0):
        return data
    threshold = 2

    filtered_data = [x for x in data if abs(x - mean) < threshold * std_dev]

    return filtered_data

def mappingStarted():
	global mappingRunning
	global movementRunning


	ultrasonic=Ultrasonic()    
	servo = Servo()

	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.bind(("0.0.0.0", 9050))
	while True:
		s.listen()
		print("Waiting for connection...")
		conn, addr = s.accept()
		print("Connected")
		while True:
			with lock:
				while movementRunning:
					condition.wait()
				mappingRunning = True
				try:
					degrees = [180, 80,75,70, 0]
					arr = []
					for i in degrees:
						#time.sleep(0.1)
						servo.setServoAngle(1, i)
						
						time.sleep(0.4)
						datas = []
						data=0
						for j in range(20):
							current_data = ultrasonic.getDistance()
							time.sleep(0.01)
							#if(sum(datas) / len(datas) - 5 < current_data and sum(datas) / len(datas) + 5 > current_data):
							datas.append(current_data)
							
						
						time.sleep(0.4)
						datass = remove_outliers(datas)
						
						if(len(datass) != 0):
							data = sum(datass) / len(datass)
						else:
							print(datas)
						#data = data / 20
						   #Get the value
						          
						print ("Obstacle distance is "+str(data)+"CM" + str(i) + "DEGREE")
						#time.sleep(1)
						if(i != 180 and i!=0):
							i = i + 15
						x=int(data* math.cos(math.radians((i/1.33333)+22.5)))
						y=int(data*math.sin(math.radians((i/1.33333)+22.5)))
						print(x, y)
						arr.append((x,y))			

					data_string = str(arr)
					print(sys.getsizeof(data_string.encode()))
					#print(data_string)
					conn.sendall(data_string.encode())
				except socket.error as e:
					break
				except IOError as e:
					break
				mappingRunning = False
				condition.notify()
			time.sleep(0.0000001)
		print("BITTI")

def newServer():
	global mappingRunning
	global movementRunning
	

	control = Control()

	# Create a socket object for receiving commands

	command_socket = socket.socket()
	command_socket.bind(('0.0.0.0', 8052))
	# Accept a single connection from a client for receiving commands
	command_socket.listen(5)  
	command_connection, _ = command_socket.accept()
	while True:
		recved = command_connection.recv(1024).decode()

		with lock:
			while mappingRunning:
				condition.wait()
			send_str = 'RECEIVED'
			movementRunning = True
			commands= recved.split(',')
			# Receive commands and control the robot
			for str_recv in commands:
				

				print(str_recv)
		    
				if str_recv == "FORWARD":
					#for i in range(0, 4):
					control.forWard()
					control.attitude(0,0, 0)
					print("ILERI")
				elif str_recv == "RIGHT":
					for i in range(0,6):
						control.turnRight()
						control.attitude(0,0,0)
					print("SAG")
				elif str_recv == 'LEFT':
					for i in range(0,8):
						control.turnLeft()
						control.attitude(0,0,0)
					print("SOL")
				elif str_recv == "BACKWARD":
					control.backWard()
					print("GERI")
				elif str_recv == "TURNRIGHT":
					control.turnRight()
				elif str_recv == "TURNLEFT":
					for k in range(2):
					    control.turnLeft()
				elif str_recv == "NAVIGATE":
					count = 0
					while count < 10:
						data = control.sonic.getDistance()
						int_data = int(data)
						print("Obstacle distance is ", end='')
						print(str(data))

						if int_data < 10 and int_data != 0:
							for i in range(40, -40, -5):
								control.attitude(0, 0, i)
								time.sleep(0.1)
						else:
							control.forWard()
						count += 1
					print("NAVIGATE")
				elif str_recv == "STEPRIGHT":
					for i in range(0,4):
						control.setpRight()
					print("SAĞ ADIM")
				elif str_recv == "STEPLEFT":
					for i in range(0,4):
						control.setpLeft()
					print("SOL ADIM")
				else:
					print("INVALID")
				temp= 'STOP'
				flag = True
				
			#time.sleep(0.5)
			command_connection.sendall(send_str.encode())
			movementRunning = False			
			condition.notify()
		
		time.sleep(0.0000001)

	command_socket.close()

def denemeVideo():
	server_socket = socket.socket()
	server_socket.bind(('0.0.0.0', 9031))  # Bind to an available address and port
	server_socket.listen(0)

	# Accept a single connection from a client
	connection = server_socket.accept()[0].makefile('wb')

	try:
		with picamera.PiCamera() as camera:
			camera.resolution = (1640, 1232)  # Set the video resolution
			camera.framerate = 15  # Adjust the framerate as needed
			time.sleep(2)  # Give the camera time to warm up
			start_time = time.time()
			
			# Start a continuous video stream
			stream = io.BytesIO()
			for _ in camera.capture_continuous(stream, 'jpeg', use_video_port=True):
				# Rewind the stream to the beginning
				stream.seek(0)
				# Calculate the timestamp of the current frame
				frame = stream.read()
				
				connection.write(struct.pack('<L', len(frame)))
				connection.flush()
				connection.write(frame)
				connection.flush()
				# Reset the stream for the next frame
				stream.seek(0)
				stream.truncate()

			# End the video stream
			connection.write(struct.pack('<L', 0))
	finally:
		connection.close()
		server_socket.close()


t1 = threading.Thread(target=mappingStarted)
t2 = threading.Thread(target=newServer)
t3 = threading.Thread(target=denemeVideo)
# starting thread 1
t1.start()
# starting thread 2
t2.start()
t3.start()


