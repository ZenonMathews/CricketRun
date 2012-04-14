#!/usr/bin/env python

import wx
from wxPython.wx import *
import sys, os
import subprocess
import time


class Hybsys(wx.Frame):
    def __init__(self,parent,id):
        wx.Frame.__init__(self,parent,id,'Hybrid-System',size=(690,400))
        panel=wx.Panel(self)
        #panel.SetBackgroundColour('BLACK')

        import subprocess as sp
        sp.Popen("sudo killall yarpserver3", shell=True)

        # log file text box
        LogFileName = wx.ComboBox(panel, -1, "ID_group_trial", (110, 10), (140,30))
        
        button1=wx.Button(panel,label="exit",pos=(645,10),size=(40,30))
        self.Bind(wx.EVT_BUTTON, self.closebutton, button1) 
        self.Bind(wx.EVT_CLOSE, self.closewindow)

        button2=wx.Button(panel,label="start",pos=(110,50),size=(50,30))
        self.Bind(wx.EVT_BUTTON, self.startYarp, button2) 

        btnStopYarp=wx.Button(panel,label="stop",pos=(170,50),size=(50,30))
        self.Bind(wx.EVT_BUTTON, self.stopYarp, btnStopYarp) 


        btnOgre=wx.Button(panel,label="virtual arena",pos=(110,130),size=(110,30))
        self.Bind(wx.EVT_BUTTON, self.ogre, btnOgre) 

        
        btnSoundOn=wx.Button(panel,label="On",pos=(110,90),size=(50,30))  
        self.Bind(wx.EVT_BUTTON, self.soundOn, btnSoundOn)
 
        btnSoundOff=wx.Button(panel,label="Off",pos=(170,90),size=(50,30))  
        self.Bind(wx.EVT_BUTTON, self.soundOff, btnSoundOff)        


        btnSensor1On=wx.Button(panel,label="On",pos=(110,170),size=(50,30))
        self.Bind(wx.EVT_BUTTON, self.sensor1On, btnSensor1On)

        btnSensor1Off=wx.Button(panel,label="Off",pos=(170,170),size=(50,30))
        self.Bind(wx.EVT_BUTTON, self.sensor1Off, btnSensor1Off)

        btnSensor2On=wx.Button(panel,label="On",pos=(110,210),size=(50,30))
        self.Bind(wx.EVT_BUTTON, self.sensor2On, btnSensor2On)

        btnSensor2Off=wx.Button(panel,label="Off",pos=(170,210),size=(50,30))
        self.Bind(wx.EVT_BUTTON, self.sensor2Off, btnSensor2Off)

        btnCameraOn=wx.Button(panel,label="On",pos=(110,250),size=(50,30))
        self.Bind(wx.EVT_BUTTON, self.cameraOn, btnCameraOn)   

        btnCameraOff=wx.Button(panel,label="Off",pos=(170,250),size=(50,30))
        self.Bind(wx.EVT_BUTTON, self.cameraOff, btnCameraOff)

        btnYarpCheck=wx.Button(panel,label="check",pos=(110,300),size=(110,30))
        self.Bind(wx.EVT_BUTTON, self.checkYarpPorts, btnYarpCheck)

        btnYarpClean=wx.Button(panel,label="clean",pos=(110,340),size=(110,30))
        self.Bind(wx.EVT_BUTTON, self.cleanYarpPorts, btnYarpClean)


        # yarp connections
#        portOut = input("Enter something: ")
        #print "you entered ", var  
        #portOut = []
#        portIn = input("Enter something: ")

        portOut = str()         

        YarpPortListOut = ['/opticMouse/out/z', '/opticMouse/out/yaw', '/opticMouse2/out/z', '/opticMouse2/out/yaw']
        yarpOut = wx.ComboBox(panel, -1, portOut, (240, 210), wx.DefaultSize,YarpPortListOut, wx.CB_DROPDOWN)
        print yarpOut
        YarpPortListIn = ['/ogre/trackball/in/z', '/ogre/trackball/in/yaw', '/controlPuck/in']
        yarpIn = wx.ComboBox(panel, -1, "", (430, 210), wx.DefaultSize,YarpPortListIn, wx.CB_DROPDOWN   ) 
  
        self.Txt = wx.StaticText(panel, -1, 'yarp connections:')
        self.Txt.SetPosition((240, 170))
        self.Txt = wx.StaticText(panel, -1, 'from:')
        self.Txt.SetPosition((240, 190))
        self.Txt = wx.StaticText(panel, -1, 'to:')
        self.Txt.SetPosition((430, 190))

        btnConnect=wx.Button(panel,label="connect",pos=(620,210),size=(65,30))
        self.Bind(wx.EVT_BUTTON, self.yarpConnectPorts, btnConnect)


        

        # text

        self.Txt = wx.StaticText(panel, -1, 'log file name:')
        self.Txt.SetPosition((10, 20))   
        self.Txt = wx.StaticText(panel, -1, 'yarp server')
        self.Txt.SetPosition((10, 60))
        self.Txt = wx.StaticText(panel, -1, 'Pd sound app')
        self.Txt.SetPosition((10, 100)) 
        self.Txt = wx.StaticText(panel, -1, 'Virtual arena')
        self.Txt.SetPosition((10, 140))
        self.Txt = wx.StaticText(panel, -1, 'read mouse 1')
        self.Txt.SetPosition((10, 180))
        self.Txt = wx.StaticText(panel, -1, 'read mouse 2')
        self.Txt.SetPosition((10, 220))
        self.Txt = wx.StaticText(panel, -1, 'Camera app')
        self.Txt.SetPosition((10, 260))
        self.Txt = wx.StaticText(panel, -1, 'yarp ports:')
        self.Txt.SetPosition((10, 300))
        self.Show(True)




        #functions
 
    def closebutton(self, event):
        self.Close(True)
        import subprocess as sp
        sp.Popen("sudo killall yarpserver3", shell=True) 
        sp.Popen("exit", shell=True)  

    def closewindow(self, event):
        self.Destroy()



    def startYarp(self, event):
        subprocess.Popen("sudo killall yarpserver", shell=True)
        subprocess.Popen('"yarpserver3"', shell=True)       

    def stopYarp(self, event):
        subprocess.Popen("sudo killall yarpserver3", shell=True)

    def checkYarpPorts(self, event):
        subprocess.Popen('"./checkYarpPorts"', shell=True)

    def cleanYarpPorts(self, event):
        subprocess.Popen('"./cleanYarpPorts"', shell=True)


    def yarpConnectPorts(self, event):
        subprocess.Popen("yarp connect portOut portIn" , shell=True)
        #subprocess.Popen('"yarpserver3"', shell=True)   
        

    def ogre(self, event):
        subprocess.Popen('"./launchOgre"', shell=True)     
      

    def sensor1On(self, event):
        import subprocess
        subprocess.Popen('"./launchMouse1"', shell=True)

    def sensor1Off(self, event):
        import subprocess
        subprocess.Popen('"./stopMouse1"', shell=True)

    def sensor2On(self, event):
        import subprocess
        subprocess.Popen('"./launchMouse2"', shell=True)

    def sensor2Off(self, event):
        import subprocess
        subprocess.Popen('"./stopMouse2"', shell=True)


    def cameraOn(self, event):
        subprocess.Popen('"./launchCamera"', shell=True)

    def cameraOff(self, event):
        subprocess.Popen('"./stopCamera"', shell=True)


    def soundOn(self, event):
        subprocess.Popen('"./launchSound"', shell=True)


    def soundOff(self, event):
        subprocess.Popen('"./stopSound"', shell=True)





if __name__=='__main__':
    app=wx.PySimpleApp()
    frame= Hybsys(parent=None,id=-1)
    frame.Show()
    app.MainLoop()
