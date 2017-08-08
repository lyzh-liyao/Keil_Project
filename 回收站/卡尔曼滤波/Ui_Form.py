# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'controller.ui'
#
# Created: Wed Aug 19 14:32:28 2015
#      by: PyQt4 UI code generator 4.9.6
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui
import time

try:
    _fromUtf8 = QtCore.QString.fromUtf8
except AttributeError:
    def _fromUtf8(s):
        return s

try:
    _encoding = QtGui.QApplication.UnicodeUTF8
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig, _encoding)
except AttributeError:
    def _translate(context, text, disambig):
        return QtGui.QApplication.translate(context, text, disambig)

class Ui_Form(object):
    curve_item = {}
    def setupUi(self, Form):

        #-----------------------------------------------
        Form.setObjectName(_fromUtf8("Form"))
        Form.resize(1329, 856)
        self.realtimecurve = CurvePlot(Form)
        self.realtimecurve.setGeometry(QtCore.QRect(10, 10, 1251, 261))
        self.realtimecurve.setObjectName(_fromUtf8("realtimecurve"))      
 
        
        curve_name='Lspeed'
        curve=self.curve_item[curve_name] = make.curve([0], [0], color='b', title=curve_name)
        self.realtimecurve.add_item(curve)                              
        curve_name='Rspeed'
        curve=self.curve_item[curve_name] = make.curve([0], [0], color='r', title=curve_name)
        self.realtimecurve.add_item(curve)
        curve_name='DeltaSpeed'
        curve=self.curve_item[curve_name] = make.curve([0], [0], color='g', title=curve_name)
        self.realtimecurve.add_item(curve)
        self.realtimecurve.add_item(make.legend("TR"))

        #curve
        self.realtimecurve2 = CurvePlot(Form)
        self.realtimecurve2.setGeometry(QtCore.QRect(10, 280, 1251, 261))
        self.realtimecurve2.setObjectName(_fromUtf8("realtimecurve2"))      
 
        
        curve_name='Lcurrent'
        curve=self.curve_item[curve_name] = make.curve([0], [0], color='b', title=curve_name)
        self.realtimecurve2.add_item(curve)                              
        curve_name='Rcurrent'
        curve=self.curve_item[curve_name] = make.curve([0], [0], color='r', title=curve_name)
        self.realtimecurve2.add_item(curve)
        self.realtimecurve2.add_item(make.legend("TR"))
        #------------------------------------------------
        
        self.Controller = QtGui.QGroupBox(Form)
        self.Controller.setGeometry(QtCore.QRect(20, 580, 311, 271))
        self.Controller.setObjectName(_fromUtf8("Controller"))
        self.Lspeed = QtGui.QLineEdit(self.Controller)
        self.Lspeed.setGeometry(QtCore.QRect(150, 40, 121, 22))
        self.Lspeed.setObjectName(_fromUtf8("Lspeed"))
        self.Rspeed = QtGui.QLineEdit(self.Controller)
        self.Rspeed.setGeometry(QtCore.QRect(150, 70, 121, 22))
        self.Rspeed.setObjectName(_fromUtf8("Rspeed"))
        self.label = QtGui.QLabel(self.Controller)
        self.label.setGeometry(QtCore.QRect(20, 40, 111, 20))
        self.label.setObjectName(_fromUtf8("label"))
        self.label_2 = QtGui.QLabel(self.Controller)
        self.label_2.setGeometry(QtCore.QRect(20, 70, 111, 20))
        self.label_2.setObjectName(_fromUtf8("label_2"))
        self.checkBox = QtGui.QCheckBox(self.Controller)
        self.checkBox.setGeometry(QtCore.QRect(20, 100, 91, 16))
        self.checkBox.setObjectName(_fromUtf8("checkBox"))
        self.speed = QtGui.QSlider(self.Controller)
        self.speed.setGeometry(QtCore.QRect(130, 140, 160, 22))
        self.speed.setOrientation(QtCore.Qt.Horizontal)
        self.speed.setObjectName(_fromUtf8("speed"))
        self.lcdSpeed = QtGui.QLCDNumber(self.Controller)
        self.lcdSpeed.setGeometry(QtCore.QRect(20, 140, 91, 23))
        self.lcdSpeed.setStyleSheet(_fromUtf8("background-color: rgb(0, 0, 0);\n"
"border-color: rgb(255, 0, 0);\n"
"selection-color: rgb(255, 0, 0);"))
        self.lcdSpeed.setSmallDecimalPoint(False)
        self.lcdSpeed.setObjectName(_fromUtf8("lcdSpeed"))
        self.pushButton = QtGui.QPushButton(self.Controller)
        self.pushButton.setGeometry(QtCore.QRect(200, 230, 75, 23))
        self.pushButton.setObjectName(_fromUtf8("pushButton"))
        self.pushButton_2 = QtGui.QPushButton(self.Controller)
        self.pushButton_2.setGeometry(QtCore.QRect(20, 230, 75, 23))
        self.pushButton_2.setObjectName(_fromUtf8("pushButton_2"))
        self.pushButton_3 = QtGui.QPushButton(self.Controller)
        self.pushButton_3.setGeometry(QtCore.QRect(110, 230, 75, 23))
        self.pushButton_3.setObjectName(_fromUtf8("pushButton_3"))
        self.LineStrategy = QtGui.QGroupBox(Form)
        self.LineStrategy.setGeometry(QtCore.QRect(330, 580, 241, 271))
        self.LineStrategy.setObjectName(_fromUtf8("LineStrategy"))
        self.LineKp = QtGui.QLineEdit(self.LineStrategy)
        self.LineKp.setGeometry(QtCore.QRect(100, 30, 121, 22))
        self.LineKp.setObjectName(_fromUtf8("LineKp"))
        self.LineKi = QtGui.QLineEdit(self.LineStrategy)
        self.LineKi.setGeometry(QtCore.QRect(100, 60, 121, 22))
        self.LineKi.setObjectName(_fromUtf8("LineKi"))
        self.LineKd = QtGui.QLineEdit(self.LineStrategy)
        self.LineKd.setGeometry(QtCore.QRect(100, 90, 121, 22))
        self.LineKd.setObjectName(_fromUtf8("LineKd"))
        self.label_4 = QtGui.QLabel(self.LineStrategy)
        self.label_4.setGeometry(QtCore.QRect(20, 30, 61, 21))
        self.label_4.setObjectName(_fromUtf8("label_4"))
        self.label_5 = QtGui.QLabel(self.LineStrategy)
        self.label_5.setGeometry(QtCore.QRect(20, 60, 61, 21))
        self.label_5.setObjectName(_fromUtf8("label_5"))
        self.label_6 = QtGui.QLabel(self.LineStrategy)
        self.label_6.setGeometry(QtCore.QRect(20, 90, 61, 21))
        self.label_6.setObjectName(_fromUtf8("label_6"))
        self.lineUpdate = QtGui.QPushButton(self.LineStrategy)
        self.lineUpdate.setGeometry(QtCore.QRect(80, 230, 75, 23))
        self.lineUpdate.setObjectName(_fromUtf8("lineUpdate"))
        self.lineStragery = QtGui.QRadioButton(self.LineStrategy)
        self.lineStragery.setGeometry(QtCore.QRect(20, 130, 89, 16))
        self.lineStragery.setObjectName(_fromUtf8("lineStragery"))
        self.lineStragery_2 = QtGui.QRadioButton(self.LineStrategy)
        self.lineStragery_2.setGeometry(QtCore.QRect(90, 130, 89, 16))
        self.lineStragery_2.setObjectName(_fromUtf8("lineStragery_2"))
        self.lineStragery_3 = QtGui.QRadioButton(self.LineStrategy)
        self.lineStragery_3.setGeometry(QtCore.QRect(160, 130, 71, 16))
        self.lineStragery_3.setObjectName(_fromUtf8("lineStragery_3"))
        self.ObstacleCrossing = QtGui.QGroupBox(Form)
        self.ObstacleCrossing.setGeometry(QtCore.QRect(570, 580, 241, 271))
        self.ObstacleCrossing.setObjectName(_fromUtf8("ObstacleCrossing"))
        self.obCrossKd = QtGui.QLineEdit(self.ObstacleCrossing)
        self.obCrossKd.setGeometry(QtCore.QRect(100, 90, 121, 22))
        self.obCrossKd.setObjectName(_fromUtf8("obCrossKd"))
        self.obCrossKi = QtGui.QLineEdit(self.ObstacleCrossing)
        self.obCrossKi.setGeometry(QtCore.QRect(100, 60, 121, 22))
        self.obCrossKi.setObjectName(_fromUtf8("obCrossKi"))
        self.label_7 = QtGui.QLabel(self.ObstacleCrossing)
        self.label_7.setGeometry(QtCore.QRect(20, 60, 61, 21))
        self.label_7.setObjectName(_fromUtf8("label_7"))
        self.label_8 = QtGui.QLabel(self.ObstacleCrossing)
        self.label_8.setGeometry(QtCore.QRect(20, 90, 61, 21))
        self.label_8.setObjectName(_fromUtf8("label_8"))
        self.label_9 = QtGui.QLabel(self.ObstacleCrossing)
        self.label_9.setGeometry(QtCore.QRect(20, 30, 61, 21))
        self.label_9.setObjectName(_fromUtf8("label_9"))
        self.obCrossKp = QtGui.QLineEdit(self.ObstacleCrossing)
        self.obCrossKp.setGeometry(QtCore.QRect(100, 30, 121, 22))
        self.obCrossKp.setObjectName(_fromUtf8("obCrossKp"))
        self.label_10 = QtGui.QLabel(self.ObstacleCrossing)
        self.label_10.setGeometry(QtCore.QRect(20, 120, 54, 21))
        self.label_10.setObjectName(_fromUtf8("label_10"))
        self.obTriggerCurrent = QtGui.QLineEdit(self.ObstacleCrossing)
        self.obTriggerCurrent.setGeometry(QtCore.QRect(100, 120, 121, 22))
        self.obTriggerCurrent.setObjectName(_fromUtf8("obTriggerCurrent"))
        self.label_11 = QtGui.QLabel(self.ObstacleCrossing)
        self.label_11.setGeometry(QtCore.QRect(20, 150, 61, 21))
        self.label_11.setObjectName(_fromUtf8("label_11"))
        self.obCurrent = QtGui.QLineEdit(self.ObstacleCrossing)
        self.obCurrent.setGeometry(QtCore.QRect(100, 150, 121, 22))
        self.obCurrent.setObjectName(_fromUtf8("obCurrent"))
        self.obStragery = QtGui.QRadioButton(self.ObstacleCrossing)
        self.obStragery.setGeometry(QtCore.QRect(30, 190, 89, 16))
        self.obStragery.setObjectName(_fromUtf8("obStragery"))
        self.obStragery_2 = QtGui.QRadioButton(self.ObstacleCrossing)
        self.obStragery_2.setGeometry(QtCore.QRect(120, 190, 89, 16))
        self.obStragery_2.setObjectName(_fromUtf8("obStragery_2"))
        self.obUpdate = QtGui.QPushButton(self.ObstacleCrossing)
        self.obUpdate.setGeometry(QtCore.QRect(70, 230, 75, 23))
        self.obUpdate.setObjectName(_fromUtf8("obUpdate"))

        self.retranslateUi(Form)
        QtCore.QMetaObject.connectSlotsByName(Form)
        
    def retranslateUi(self, Form):
        Form.setWindowTitle(_translate("Form", "Form", None))
        self.Controller.setTitle(_translate("Form", "控制命令窗口", None))
        self.label.setText(_translate("Form", "左电机速度（cm/s）", None))
        self.label_2.setText(_translate("Form", "右电机速度（cm/s）", None))
        self.checkBox.setText(_translate("Form", "左右轮同步", None))
        self.pushButton.setText(_translate("Form", "后退", None))
        self.pushButton_2.setText(_translate("Form", "前进", None))
        self.pushButton_3.setText(_translate("Form", "急停", None))
        self.LineStrategy.setTitle(_translate("Form", "直线运动策略", None))
        self.label_4.setText(_translate("Form", "PID参数Kp", None))
        self.label_5.setText(_translate("Form", "PID参数Ki", None))
        self.label_6.setText(_translate("Form", "PID参数Kd", None))
        self.lineUpdate.setText(_translate("Form", "更新参数", None))
        self.lineStragery.setText(_translate("Form", "上坡策略", None))
        self.lineStragery_2.setText(_translate("Form", "下坡策略", None))
        self.lineStragery_3.setText(_translate("Form", "平地策略", None))
        self.ObstacleCrossing.setTitle(_translate("Form", "越障策略", None))
        self.label_7.setText(_translate("Form", "PID参数Ki", None))
        self.label_8.setText(_translate("Form", "PID参数Kd", None))
        self.label_9.setText(_translate("Form", "PID参数Kp", None))
        self.label_10.setText(_translate("Form", "触发电流", None))
        self.label_11.setText(_translate("Form", "越障轮电流", None))
        self.obStragery.setText(_translate("Form", "速度策略", None))
        self.obStragery_2.setText(_translate("Form", "电流策略", None))
        self.obUpdate.setText(_translate("Form", "更新参数", None))


#---Import plot widget base class
from guiqwt.plot import CurveWidget, PlotManager, CurvePlot
from guiqwt.builder import make
from guidata.configtools import get_icon

import numpy as np
import scipy.signal as sps, scipy.ndimage as spi
