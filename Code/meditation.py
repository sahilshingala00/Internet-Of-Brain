#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Feb 20 17:19:33 2021

@author: apple
"""

import numpy as np

import datetime
import pyrebase
import serial
import time
import requests
import json
from statistics import mean
import time
import random

config={
  "apiKey": "AIzaSyC0Js9fP5X89Pmbulj3OLkjY81LA3Ibkl0",
  "authDomain": "iobproject.firebaseapp.com",
  "databaseURL": "https://iobproject.firebaseio.com",
  "projectId": "iobproject",
  "storageBucket": "iobproject.appspot.com",
  "messagingSenderId": "445113035915",
  "appId": "1:445113035915:web:d54e9ab90293932a1df88e",
  "measurementId": "G-DX6PW8291Z"
}

firebase=pyrebase.initialize_app(config)
db=firebase.database()

current_time = datetime.datetime.now()
last_min=current_time.minute
first_min=last_min

last_pause=1
pause=1
icnt=0
dcnt=0

list_c=[]
list_c_avg=[]
avg_5=0
avg=0


l=0
list_c_total=0


# Initializing the arrays required to store the data.
attention_values = np.array([])
meditation_values = np.array([])
delta_values = np.array([])
theta_values = np.array([])
lowAlpha_values = np.array([])
highAlpha_values = np.array([])
lowBeta_values = np.array([])
highBeta_values = np.array([])
lowGamma_values = np.array([])
highGamma_values = np.array([])
blinkStrength_values = np.array([])
time_array = np.array([])


####MODEL LOADING AND STANDARD SCALER LOADER#######
import joblib

#IMPORTANT
classifier = joblib.load('model15k.pkl')
import matplotlib.pyplot as plt




X_model = np.loadtxt('X_model.txt', dtype=float)

y = np.loadtxt('y_model.txt', dtype=int)                 

# Splitting the dataset into the Training set and Test set
from sklearn.model_selection import train_test_split
X_train, X_test, y_train, y_test = train_test_split(X_model, y, test_size = 0.1, random_state = 0)

# Feature Scaling
from sklearn.preprocessing import StandardScaler

#IMPORTANT OBJECT
sc = StandardScaler()
X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)


##################COLLECTION OF DATA######################
import sys
import json
import time
from telnetlib import Telnet

tn=Telnet('localhost',13854)

start=time.process_time()

i=0
# app registration step (in this instance unnecessary) 
#tn.write('{"appName": "Example", "appKey": "9f54141b4b4c567c558d3a76cb8d715cbde03096"}')
tn.write(b'{"enableRawOutput": true, "format": "Json"}')

#blink_or_not = raw_input('Non-zero blink(1) or zero blink(0): ')

outfile="null"
if len(sys.argv) > 1:
    outfile=sys.argv[len(sys.argv)-1]
    outfptr=open(outfile,'w')

eSenseDict={'attention':0, 'meditation':0}
waveDict={'lowGamma':0, 'highGamma':0, 'highAlpha':0, 'delta':0, 'highBeta':0, 'lowAlpha':0, 'lowBeta':0, 'theta':0}
signalLevel=0

values_list = []

iterations = 0
all_values = 0
right_values = 0
calculate=0
while (datetime.datetime.now().minute-first_min)<3:
    blinkStrength=0
    #timediff=time.process_time()-start+1

    line=tn.read_until(b'\r')
    if len(line) > 20:
        timediff=time.process_time()-start
        dict = json.loads(line)
        if "poorSignalLevel" in dict:
            signalLevel=dict['poorSignalLevel']
        if "blinkStrength" in dict:
            blinkStrength=dict['blinkStrength']
        if "eegPower" in dict:
            waveDict=dict['eegPower']
            eSenseDict=dict['eSense']
        if signalLevel>=100:
            db.child("SignalQuality").set(1)
            continue
        else:
            db.child("SignalQuality").set(0)
        
        
        outputstr=str(timediff)+ ", "+ str(signalLevel)+", "+str(blinkStrength)+", " + str(eSenseDict['attention']) + ", " + str(eSenseDict['meditation']) + ", "+str(waveDict['lowGamma'])+", " + str(waveDict['highGamma'])+", "+ str(waveDict['highAlpha'])+", "+str(waveDict['delta'])+", "+ str(waveDict['highBeta'])+", "+str(waveDict['lowAlpha'])+", "+str(waveDict['lowBeta'])+ ", "+str(waveDict['theta'])
        if blinkStrength==0 and eSenseDict['attention'] ==0 and eSenseDict['meditation'] == 0 and waveDict['lowGamma'] == 0 and waveDict['highGamma']==0 and waveDict['highAlpha']==0 and waveDict['lowAlpha']==0 and waveDict['lowBeta']==0 and waveDict['highBeta']==0 and waveDict['delta']==0 and waveDict['theta']==0:
            continue
        time_array = np.append(time_array, [timediff])
        blinkStrength_values = np.append(blinkStrength_values, [blinkStrength])
        lowGamma_values = np.append(lowGamma_values, [waveDict['lowGamma']])
        highGamma_values = np.append(highGamma_values, [waveDict['highGamma']])
        highAlpha_values = np.append(highAlpha_values, [waveDict['highAlpha']])
        delta_values = np.append(delta_values, [waveDict['delta']])
        lowBeta_values = np.append(lowBeta_values, [waveDict['lowBeta']])
        highBeta_values = np.append(highBeta_values, [waveDict['highBeta']])
        theta_values = np.append(theta_values, [waveDict['theta']])
        lowAlpha_values = np.append(lowAlpha_values, [waveDict['lowAlpha']])
        attention_values = np.append(attention_values, [eSenseDict['attention']])
        meditation_values = np.append(meditation_values, [eSenseDict['meditation']])
        print (outputstr)
        values_list.append(np.array([blinkStrength, delta_values[-1], highAlpha_values[-1], highBeta_values[-1], highGamma_values[-1], lowAlpha_values[-1], lowBeta_values[-1], lowGamma_values[-1], theta_values[-1]]))
        iterations += 1
        print (iterations)
        list_c.append(eSenseDict['meditation'])
        
        tme=datetime.datetime.now()
        if tme.minute==(last_min+1):
            print("httyt")
            for i in range(0,len(list_c)):
                avg_5=avg_5+list_c[i]
            avg_5=avg_5/len(list_c)
            list_c.clear()
            data={"Meditation_value":eSenseDict['meditation'],"Avg_5":avg_5,"Avg":avg}
            db.child("Meditation").set(data)
            list_c_avg.append(avg_5)
            last_min=tme.minute
                
        if iterations == 1 or iterations == 2:
            continue
        else:
            if blinkStrength_values[-2] == 0:
                continue
		#print str(timediff) + " ," + str(blinkStrength_values[-2])
            else:
                X_new = []
                print ()
                for i in range(0, len(values_list[-1])):
                    for j in [3,2,1]:
                        X_new.append(values_list[-1*j][i])
                X_new = np.array([X_new])
                X_new = sc.transform(X_new)
                value = classifier.predict(X_new)
                print (value)
                all_values += 1
                if value[0] == 2:
                    right_values += 1
                    print ("right blink")
                elif value[0] == 1:
                    print ("left blink")
            
            
          
            
            
                

for i in range(0,len(list_c_avg)):
    avg=avg+list_c_avg[i]
avg=avg/len(list_c_avg)
data={"Meditation_value":eSenseDict['meditation'],"Avg_5":avg_5,"Avg":avg}
db.child("Meditation").set(data)
print(list_c_avg)

list_c_avg.clear()
list_c.clear()


