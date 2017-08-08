import sys
from PyQt4 import QtCore, QtGui
from Ui_Form import Ui_Form
#---Import plot widget base class
from guiqwt.plot import CurveWidget
from guiqwt.builder import make
from guidata.configtools import get_icon

import numpy as np
import scipy.signal as sps, scipy.ndimage as spi
import serial
import re
from ctypes import *


global data

ds=0
data={}
#速度数据
data['Ldir']=[]           
data['Rdir']=[]
data['Hdir']=[]
#速度数据
data['Lspeed']=[]
data['Rspeed']=[]
data['Hspeed']=[]
data['DeltaSpeed']=[0]
#电流数据
data['Lcurrent']=[]          
data['Rcurrent']=[]
data['Hcurrent']=[]
#陀螺仪速度
data['Xgyro']=[]
data['Ygyro']=[]
data['Zgyro']=[]
#3轴加速度
data['Xacceleration']=[]
data['Yacceleration']=[]
data['Zacceleration']=[]

def decode(dstr):
    datatemp={}    
    temp=[]
    i=0
    j=0    
    #matchObj = re.search( r'\xfd(.*)\xf8', dstr, re.M|re.I)
    #print len(matchObj.group())
    fig=0
    changefig=0
    for achar in dstr:
        #print hex(ord(achar))        
        if achar=='\xf8' and fig==1:
            fig=0
            j=0
            datatemp[i]=temp
            temp=[]
            i+=1        
        if fig==1:
            if changefig==0:
                lastchar=''
                if achar=='\xfe':
                    changefig=1
                    lastchar='\xfe'
                else:
                    temp.append(achar)
            elif changefig==1:
                changefig=0
                asa=chr(0xf0+(0x0f&ord(achar)))
                #print hex(ord(achar))
                temp.append(asa)
        if achar=='\xfd':
            j+=1
            fig=1
            
    #print datatemp    
    for key,astr in datatemp.items():
        #print len(astr)
        #print astr
        try:            
            if astr[0]=='\x01':                
                #print astr
                #速度数据
                data['Ldir'].append(ord(astr[1]))           
                data['Rdir'].append(ord(astr[3]))
                data['Hdir'].append(ord(astr[5]))
                #速度数据
                if ord(astr[1])==1:
                    data['Lspeed'].append(ord(astr[2]))
                if ord(astr[1])==2:
                    data['Lspeed'].append(-ord(astr[2]))
                if ord(astr[3])==1:
                    data['Rspeed'].append(ord(astr[4]))
                if ord(astr[3])==2:
                    data['Rspeed'].append(-ord(astr[4]))                
                data['Hspeed'].append(ord(astr[6]))
                ds = data['DeltaSpeed'][-1] + (data['Lspeed'][-1]-data['Rspeed'][-1])
                data['DeltaSpeed'].append(ds)
                #电流数据                
                data['Lcurrent'].append((ord(astr[8])*0xff)+ord(astr[9]))
                #print hex(ord(astr[11]))
                #print hex(ord(astr[10]))
                #print hex((ord(astr[8])<<8)+ord(astr[9]))                
                data['Rcurrent'].append((ord(astr[10])*0xff)+ord(astr[11]))                                   
                data['Hcurrent'].append((ord(astr[8])<<8)+ord(astr[9]))
                #陀螺仪速度
                data['Xgyro'].append(ord(astr[1]))
                data['Ygyro'].append(ord(astr[1]))
                data['Zgyro'].append(ord(astr[1]))
                #3轴加速度
                data['Xacceleration'].append(ord(astr[1]))
                data['Yacceleration'].append(ord(astr[1]))
                data['Zacceleration'].append(ord(astr[1]))
                #
        except:
            pass        
            #print ord(astr[10])
            #print ord(astr[11])
        
    #print data
    del datatemp

def encode(astr):
    try:
        astr.replace('\xfd','\xfe\x7d')
    except:            
        #print 'error 0xfd'
        pass
    try:
        astr.replace('\xfe','\xfe\x7e')
    except:
        #print 'error 0xfe'
        pass
    try:    
        astr.replace('\xf8','\xfe\x78')
    except:
        #print 'error 0xf8'
        pass
    astr=''.join(['\xfd',astr,'\x00\xf8'])
    return astr


def double2bit32(x):
    x=int(x*100000)
    xarr=[chr((x&0xff000000)>>24),  chr((x&0x00ff0000)>>16), chr((x&0x0000ff00)>>8), chr((x&0x000000ff))]
    return ''.join(xarr)
    
class cmd(object):
    clas='\x01'
    Ldir='\x01'
    Lspeed='\x00'
    Rdir='\x01'
    Rspeed='\x00'
    Num1='\x01'
    Num2='\x01'
    cmdstr=''
    def cjoin(self):
        a=[self.clas,self.Ldir,self.Lspeed,self.Rdir,self.Rspeed,self.Num1,self.Num2]
        astr=''.join(a)
        astr=self.encode(astr)
        return astr
    def encode(slef,astr):        
        try:
            astr.replace('\xfd','\xfe\x7d')
        except:            
            #print 'error 0xfd'
            pass
        try:
            astr.replace('\xfe','\xfe\x7e')
        except:
            #print 'error 0xfe'
            pass
        try:    
            astr.replace('\xf8','\xfe\x78')
        except:
            #print 'error 0xf8'
            pass
        astr=''.join(['\xfd',astr,'\x00\xf8'])
        return astr

class Main(QtGui.QMainWindow):
    def __init__(self):
        super(Main, self).__init__()

        self.opencom("com5")

        # build ui
        self.ui = Ui_Form()
        self.ui.setupUi(self)

        # connect signals        
        self.connect(self.ui.obUpdate, QtCore.SIGNAL('clicked()'), self.obUpdateEXE)
        self.ui.speed.valueChanged.connect(self.speedSlideMove)
        self.ui.pushButton_2.clicked.connect(self.goforward)
        self.ui.pushButton.clicked.connect(self.goback)
        self.ui.pushButton_3.clicked.connect(self.stop)
        self.ui.lineUpdate.clicked.connect(self.lineUpdateEXE)
        
        self.startTimer(200)
        
    def keyPressEvent(self, e):
        print e.key()
        if e.key() == 87:
            print 'goForward'
            self.goforward()
        if e.key() == 83:
            print 'goBack'
            self.goback()
        if e.key() == 65:
            print 'turnLeft'
            self.turnLeft()
        if e.key() == 68:
            print 'turnRight'
            self.turnRight()
        if e.key() == 32:
            print 'Stop'
            self.stop()
            
            
    def speedSlideMove(self):
        print self.ui.speed.value()
        self.ui.lcdSpeed.setDecMode()
        speed=str('{:.2f}'.format(50*float(self.ui.speed.value())/100))
        self.ui.lcdSpeed.display(speed)

    def goforward(self):
        gofwd=cmd()
        gofwd.clas='\x01'
        gofwd.Ldir='\x01\x00'
        gofwd.Lspeed=chr(int(self.ui.lcdSpeed.value()))
        print self.ui.lcdSpeed.value()
        gofwd.Rdir='\x01\x00'
        gofwd.Rspeed=chr(int(self.ui.lcdSpeed.value()))
        ss=gofwd.cjoin()
        
        for ads in ss:
            print hex(ord(ads))
        self.conn.write(ss)
        
    def goback(self):
        gobk=cmd()
        gobk.clas='\x01'
        gobk.Ldir='\x02\x00'
        gobk.Lspeed=chr(int(self.ui.lcdSpeed.value()))
        gobk.Rdir='\x02\x00'
        gobk.Rspeed=chr(int(self.ui.lcdSpeed.value()))
        ss=gobk.cjoin()
        for ads in ss:
            print hex(ord(ads))
        self.conn.write(ss)

    def turnLeft(self):
        gofwd=cmd()
        gofwd.clas='\x01'
        gofwd.Ldir='\x02\x00'
        gofwd.Lspeed=chr(int(self.ui.lcdSpeed.value()))
        print self.ui.lcdSpeed.value()
        gofwd.Rdir='\x01\x00'
        gofwd.Rspeed=chr(int(self.ui.lcdSpeed.value()))
        ss=gofwd.cjoin()
        
        for ads in ss:
            print hex(ord(ads))
        self.conn.write(ss)
        
    def turnRight(self):
        gofwd=cmd()
        gofwd.clas='\x01'
        gofwd.Ldir='\x01\x00'
        gofwd.Lspeed=chr(int(self.ui.lcdSpeed.value()))
        print self.ui.lcdSpeed.value()
        gofwd.Rdir='\x02\x00'
        gofwd.Rspeed=chr(int(self.ui.lcdSpeed.value()))
        ss=gofwd.cjoin()
        
        for ads in ss:
            print hex(ord(ads))
        self.conn.write(ss)
        
    def stop(self):
        stp=cmd()
        stp.clas='\x01'
        stp.Ldir='\x01'
        stp.Lspeed='\x00\x00'
        stp.Rdir='\x01'
        stp.Rspeed='\x00\x00'
        ss=stp.cjoin()
        for ads in ss:
            print hex(ord(ads))
        self.conn.write(ss)
        
    def obUpdateEXE(self):
        print float(self.ui.obCrossKp.value())

    def lineUpdateEXE(self):
        print float(self.ui.LineKp.text())
        kp=double2bit32(float(self.ui.LineKp.text()))
        ki=double2bit32(float(self.ui.LineKi.text()))
        kd=double2bit32(float(self.ui.LineKd.text()))
        ss='\xcf'+kp+ki+kd
        ss=encode(ss)
        for ads in ss:
            print hex(ord(ads))
        self.conn.write(ss)

    def opencom(self,comnum):
        self.conn = serial.Serial(comnum,115200)

    def closecom(self):
        self.conn.close

    def timerEvent(self, event):
        dstr=self.conn.read(15*12)        
        decode(dstr)      

        num=500         
        if len(data['Lspeed'])>num:
            data['Lspeed']=data['Lspeed'][-num:]
        if len(data['Rspeed'])>num:
            data['Rspeed']=data['Rspeed'][-num:]
        if len(data['DeltaSpeed'])>num:
            data['DeltaSpeed']=data['DeltaSpeed'][-num:]
        x = np.linspace(0, 100, len(data['Lspeed']))
        curve_name='Lspeed'
        self.ui.curve_item[curve_name].set_data(x, data['Lspeed'])        
        curve_name='Rspeed'
        self.ui.curve_item[curve_name].set_data(x, data['Rspeed'])
        curve_name='DeltaSpeed'
        #self.ui.curve_item[curve_name].set_data(x, data['DeltaSpeed'])
        self.ui.realtimecurve.do_autoscale()
        
        num=500        
        if len(data['Lcurrent'])>num:
            data['Lcurrent']=data['Lcurrent'][-num:]
        if len(data['Rcurrent'])>num:
            data['Rcurrent']=data['Rcurrent'][-num:]
        x = np.linspace(0, 100, len(data['Lcurrent']))
        curve_name='Lcurrent'
        self.ui.curve_item[curve_name].set_data(x, data['Lcurrent'])        
        curve_name='Rcurrent'
        self.ui.curve_item[curve_name].set_data(x, data['Rcurrent'])
        
        self.ui.realtimecurve2.do_autoscale()
        
        
    def closeEvent(self, event):        
        print 'Close!'
    

if __name__ == '__main__':
    app = QtGui.QApplication(sys.argv)
    main = Main()
    main.show()
    sys.exit(app.exec_())
