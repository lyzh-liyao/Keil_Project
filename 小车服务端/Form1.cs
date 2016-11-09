using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using JoyKeys.Core;
using System.IO.Ports;

namespace JoyKeys
{
    public partial class Form1 : Form
    {
        private Core.Joystick joystick;
        public Form1()
        {
            InitializeComponent();
        }
        protected override void OnLoad(EventArgs e)
        {
            base.OnLoad(e);
            joystick = new JoyKeys.Core.Joystick();
            joystick.Click += new EventHandler<JoyKeys.Core.JoystickEventArgs>(joystick_Click);
            joystick.Register(base.Handle, Core.API.JOYSTICKID1);
            joystick.Register(base.Handle, Core.API.JOYSTICKID2);
        }

        protected override void OnClosing(CancelEventArgs e)
        {
            joystick.UnRegister(Core.API.JOYSTICKID1);
            joystick.UnRegister(Core.API.JOYSTICKID2);
            base.OnClosing(e);
        }
        void joystick_Click(object sender, JoyKeys.Core.JoystickEventArgs e)
        {
            if (e.JoystickId == Core.API.JOYSTICKID1)
            {
                this.Text = "1号手柄";
            }
            else if (e.JoystickId == Core.API.JOYSTICKID2)
            {
                this.Text = "2号手柄";
            }
            

           
           // if ((e.Buttons & JoyKeys.Core.JoystickButtons.UP) == JoyKeys.Core.JoystickButtons.UP) y--;
           // if ((e.Buttons & JoyKeys.Core.JoystickButtons.Down) == JoyKeys.Core.JoystickButtons.Down) y++;
           // if ((e.Buttons & JoyKeys.Core.JoystickButtons.Left) == JoyKeys.Core.JoystickButtons.Left) x--;
           // if ((e.Buttons & JoyKeys.Core.JoystickButtons.Right) == JoyKeys.Core.JoystickButtons.Right) x++;

            //this.label1.Text = "+";

            this.label2.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B1) == JoyKeys.Core.JoystickButtons.B1) ? Color.Red : SystemColors.Control;
            this.label3.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B2) == JoyKeys.Core.JoystickButtons.B2) ? Color.Red : SystemColors.Control;
            this.label4.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B3) == JoyKeys.Core.JoystickButtons.B3) ? Color.Red : SystemColors.Control;
            this.label5.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B4) == JoyKeys.Core.JoystickButtons.B4) ? Color.Red : SystemColors.Control;
            this.label6.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B5) == JoyKeys.Core.JoystickButtons.B5) ? Color.Red : SystemColors.Control;
            this.label7.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B6) == JoyKeys.Core.JoystickButtons.B6) ? Color.Red : SystemColors.Control;
            this.label8.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B7) == JoyKeys.Core.JoystickButtons.B7) ? Color.Red : SystemColors.Control;
            this.label9.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B8) == JoyKeys.Core.JoystickButtons.B8) ? Color.Red : SystemColors.Control;
            this.label10.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B9) == JoyKeys.Core.JoystickButtons.B9) ? Color.Red : SystemColors.Control;
            this.label11.BackColor = ((e.Buttons & JoyKeys.Core.JoystickButtons.B10) == JoyKeys.Core.JoystickButtons.B10) ? Color.Red : SystemColors.Control;
        }

        SerialPort sp ;
        int com = 1;
        private void Form1_Load(object sender, EventArgs e)
        {
            //comboBox1.SelectedIndex = 10;
            for (; com < comboBox1.Items.Count; com++)
            {
                comboBox1.SelectedIndex = com;
                if (com == 0)
                {
                    break;
                }
            }
            if (com != 0)
            {
               // MessageBox.Show("没有找到可用的串口");
            }
         
        }

        int Xpos=0;
        int Ypos=0;
        int Xdev = 0;
        int Ydev = 0;
        int pwm = 0;
        int speed = 0;
        char dir = '0';
        int left_speed = 0;

        public int Left_speed
        {
            get
            {
                if (left_speed > 95)
                {
                    return 95;
                } return left_speed;
            }
            set { left_speed = value; }
        }
        int right_speed = 0;

        public int Right_speed
        {
            get {
                if (right_speed>95)
                 {
                     return 95;
                 }
                return right_speed; }
            set { right_speed = value; }
        }

        public int Pwm
        {
            get
            {
                if (pwm > 95)
                {
                    return 95;
                }
                return pwm;
            }
            set { pwm = value; }
        }
        byte[] data = { (byte)'(', 0, 0, 0, (byte)')' };
        byte[] rea = new byte[5];
        private void timer1_Tick(object sender, EventArgs e)
        {
            try
            {
                Xpos = (int)System.Math.Round(joystick.InfoEx.dwXpos / 1000.0);
               Ypos = (int)System.Math.Round(joystick.InfoEx.dwYpos / 1000.0);
                //Xpos = label16.Location.X;
                //Ypos = label16.Location.Y;
                Xdev = System.Math.Abs(Xpos - 33);
                Ydev = System.Math.Abs(Ypos - 33);
                Pwm = (int)(System.Math.Sqrt(Xdev * Xdev + Ydev * Ydev) * 3);
                speed = (int)System.Math.Round(System.Math.Atan((Xdev * 1.0 / Ydev)) / System.Math.PI * 180);
                // MessageBox.Show();//System.Math.PI*180

                
                
                if(Xpos==33&&Ypos==33)
                {
                    pwm = 0;
                    speed = 0;
                    Left_speed = 0;
                    Right_speed = 0;
                    dir = 'n';
                }else if (Xpos == 33&& Ypos < 33)//正前
                {
                    dir = 'w';
                    Left_speed = Ydev * 2 + 29;
                    Right_speed = Ydev * 2 + 29;
                }else if (Xpos == 33&& Ypos > 33)//正后
                {
                    dir = 's';
                    Left_speed = Ydev * 2 + 29;
                    Right_speed = Ydev * 2 + 29;
                }
                else if (Xpos < 33 && Ypos == 33)//正右
                {
                    dir = 'w';
                    Left_speed = 0;
                    Right_speed = Pwm;
                }else if (Xpos > 33 && Ypos == 33)//正左
                {
                    dir = 'w';
                    Left_speed = Pwm;
                    Right_speed = 0;
                }else if (Xpos > 33 && Ypos < 33)//右上
                {
                    dir = 'w';
                    Left_speed = Pwm;
                    Right_speed = 90 - speed;
                }else if(Xpos < 33 && Ypos < 33)//左上
                {
                    dir = 'w';
                    Left_speed = 90 - speed;
                    Right_speed = Pwm;
                }else if(Xpos < 33 && Ypos > 33)//左下
                {
                    dir = 's';
                    Left_speed = 90 - speed;
                    Right_speed = Pwm;
                }else if(Xpos > 33 && Ypos > 33)//右下
                {
                    dir = 's';
                    Left_speed = Pwm;
                    Right_speed = 90 - speed;
                }
                data[1] = (byte)dir;
                data[2] = (byte)Left_speed;
                data[3] = (byte)Right_speed;
                label1.Text = "左轮：" + Left_speed.ToString();
                label17.Text = "右轮" + Right_speed.ToString();
                label12.Text = "X轴：" + Xpos.ToString();
                label13.Text = "Y轴：" + Ypos.ToString();
                label14.Text = "深度：" + Pwm.ToString();
                label15.Text = "减速" + speed.ToString();
                label16.Location = new Point(Xpos, Ypos);
                
                sp.Write(data,0,5);
                for (int i = 0; i < sp.BytesToRead;i++ )
                {
                    tem[i] = (byte)sp.ReadByte();
                }
           
                //if (tem == 0x28)
                //{
                    //label18.Text = "";
                    label18.Text +=Encoding.ASCII.GetString(tem);
                    if (label18.Text.Length > 200)
                    {
                        label18.Text = "";

                    }
                    
                    //label18.Text += sp.ReadByte().ToString();
                    //label18.Text += sp.ReadByte().ToString();
                    //label18.Text += sp.ReadByte().ToString();
                    //label18.Text += sp.ReadByte().ToString();
                //}
               // label18.Text = Encoding.ASCII.GetString(rea, 0, 5);
            }
            catch (System.Exception ex)
            {
                //MessageBox.Show(ex.ToString());
            }
            

        }
        byte []tem = new byte [100];
        private void comboBox1_SelectedIndexChanged(object sender, EventArgs e)
        {
            try
            {
                sp = new SerialPort(comboBox1.Text,1200, Parity.None, 8, StopBits.One);
                sp.Open();
                sp.ReadTimeout = 100;
                com = 0;
            }
            catch (System.Exception ex)
            {
               
            }
            
        }

        private void button1_Click(object sender, EventArgs e)
        {
            label18.Text = "";
        }

        private void panel1_MouseDown(object sender, MouseEventArgs e)
        {
            label16.Location = new Point(e.X, e.Y);
        }

        private void button2_Click(object sender, EventArgs e)
        {
            try
            {
                sp.Open();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("串口打开失败！异常："+ex.ToString());
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {
            try
            {
                sp.Close();
            }
            catch (System.Exception ex)
            {
                MessageBox.Show("串口关闭失败！异常：" + ex.ToString());
            }
        }
    }
}
