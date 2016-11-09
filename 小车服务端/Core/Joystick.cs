using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;
using System.Runtime.InteropServices;
using JoyKeys.Core;

namespace JoyKeys.Core
{
    public class Joystick : IMessageFilter,IDisposable
    {
        #region 事件定义
        /// <summary>
        /// 按钮被单击
        /// </summary>
        public event EventHandler<JoystickEventArgs> Click;
        /// <summary>
        /// 按钮被弹起
        /// </summary>
        public event EventHandler<JoystickEventArgs> ButtonUp;
        /// <summary>
        /// 按钮已被按下
        /// </summary>
        public event EventHandler<JoystickEventArgs> ButtonDown;
        /// <summary>
        /// 触发单击事件
        /// </summary>
        /// <param name="e"></param>
        protected void OnClick(JoystickEventArgs e)
        {
            EventHandler<JoystickEventArgs> h = this.Click;
            if (h != null) h(this, e);
        }
        /// <summary>
        /// 触发按钮弹起事件
        /// </summary>
        /// <param name="e"></param>
        protected void OnButtonUp(JoystickEventArgs e)
        {
            EventHandler<JoystickEventArgs> h = this.ButtonUp;
            if (h != null) h(this, e);
        }
        /// <summary>
        /// 触发按钮按下事件
        /// </summary>
        /// <param name="e"></param>
        protected void OnButtonDown(JoystickEventArgs e)
        {
            EventHandler<JoystickEventArgs> h = this.ButtonDown;
            if (h != null) h(this, e);
        }
        /// <summary>
        /// 是否已注册消息
        /// </summary>
        private bool IsRegister = false;
        /// <summary>
        /// 注册
        /// </summary>
        /// <param name="hWnd">需要捕获手柄消息的窗口</param>
        /// <param name="joystickId">要捕获的手柄Id</param>
        public bool Register(IntPtr hWnd, int joystickId)
        {
            bool flag = false;
            int result = 0;
            API.JOYCAPS caps = new API.JOYCAPS();
            if (API.joyGetNumDevs() != 0)
            {
                //拥有手柄.则判断手柄状态
                result = API.joyGetDevCaps(joystickId, ref caps, Marshal.SizeOf(typeof(API.JOYCAPS)));
                if (result == API.JOYERR_NOERROR)
                {
                    //手柄处于正常状态
                    flag = true;
                }

            }

            if (flag)
            {
                //注册消息
                if (!this.IsRegister)
                {
                    Application.AddMessageFilter(this);
                }
                this.IsRegister = true;

                result = API.joySetCapture(hWnd, joystickId, caps.wPeriodMin * 2, false);
                if (result != API.JOYERR_NOERROR)
                {
                    flag = false;
                }
            }
            return flag;
        }

        /// <summary>
        /// 取消注册
        /// </summary>
        /// <param name="joystickId"></param>
        public void UnRegister(int joystickId)
        {
            if (this.IsRegister)
            {
                API.joyReleaseCapture(joystickId);
            }
        }
        #endregion

        #region 消息处理
        #region IMessageFilter 成员
        /// <summary>
        /// 处理系统消息.
        /// </summary>
        /// <param name="m"></param>
        /// <returns></returns>
        /// 
        public JoyKeys.Core.API.JOYINFOEX infoEx;

        public JoyKeys.Core.API.JOYINFOEX InfoEx
        {
            get { return infoEx; }
            set { infoEx = value; }
        }
        public Joystick()
        {
            infoEx = new API.JOYINFOEX();
            infoEx.dwSize = Marshal.SizeOf(typeof(API.JOYINFOEX));
            infoEx.dwFlags = (int)API.JOYSTICKID1;
        }


        public int result;   
        bool IMessageFilter.PreFilterMessage(ref Message m)
        {
            result = API.joyGetPosEx(API.JOYSTICKID1, ref infoEx);


            bool flag = false;
            if (m.HWnd != IntPtr.Zero && (m.WParam != IntPtr.Zero || m.LParam != IntPtr.Zero))
            {

                Action<JoystickEventArgs> action = null;
                JoystickButtons buttons = JoystickButtons.None;
                int joystickId = -1;
                switch (m.Msg)
                {
                    case API.MM_JOY1MOVE:
                    case API.MM_JOY2MOVE:
                        //单击事件
                        buttons = GetButtonsStateFromMessageParam(m.WParam.ToInt32(), m.LParam.ToInt32());
                        action = this.OnClick;
                        joystickId = m.Msg == API.MM_JOY1MOVE ? API.JOYSTICKID1 : API.JOYSTICKID2;
                        break;
                    case API.MM_JOY1BUTTONDOWN:
                    case API.MM_JOY2BUTTONDOWN:
                        //按钮被按下
                        buttons = GetButtonsPressedStateFromMessageParam(m.WParam.ToInt32(), m.LParam.ToInt32());
                        action = this.OnButtonDown;
                        joystickId = m.Msg == API.MM_JOY1BUTTONDOWN ? API.JOYSTICKID1 : API.JOYSTICKID2;
                        break;
                    case API.MM_JOY1BUTTONUP:
                    case API.MM_JOY2BUTTONUP:
                        //按钮被弹起
                        buttons = GetButtonsPressedStateFromMessageParam(m.WParam.ToInt32(), m.LParam.ToInt32());
                        action = this.OnButtonUp;
                        joystickId = m.Msg == API.MM_JOY1BUTTONUP ? API.JOYSTICKID1 : API.JOYSTICKID2;
                        break;
                }
                if (action != null && joystickId != -1 && buttons != JoystickButtons.None)
                {
                    //阻止消息继续传递
                    flag = true;
                    //触发事件
                    action(new JoystickEventArgs(joystickId, buttons));
                }
            }
            return flag;
        }
        #endregion
        /// <summary>
        /// 根据消息的参数获取按钮的状态值
        /// </summary>
        /// <param name="wParam"></param>
        /// <param name="lParam"></param>
        /// <returns></returns>
        private JoystickButtons GetButtonsStateFromMessageParam(int wParam, int lParam)
        {
            JoystickButtons buttons = JoystickButtons.None;
            if ((wParam & API.JOY_BUTTON1) == API.JOY_BUTTON1)
            {
                buttons |= JoystickButtons.B1;
            }
            if ((wParam & API.JOY_BUTTON2) == API.JOY_BUTTON2)
            {
                buttons |= JoystickButtons.B2;
            }
            if ((wParam & API.JOY_BUTTON3) == API.JOY_BUTTON3)
            {
                buttons |= JoystickButtons.B3;
            }
            if ((wParam & API.JOY_BUTTON4) == API.JOY_BUTTON4)
            {
                buttons |= JoystickButtons.B4;
            }
            if ((wParam & API.JOY_BUTTON5) == API.JOY_BUTTON5)
            {
                buttons |= JoystickButtons.B5;
            }
            if ((wParam & API.JOY_BUTTON6) == API.JOY_BUTTON6)
            {
                buttons |= JoystickButtons.B6;
            }
            if ((wParam & API.JOY_BUTTON7) == API.JOY_BUTTON7)
            {
                buttons |= JoystickButtons.B7;
            }
            if ((wParam & API.JOY_BUTTON8) == API.JOY_BUTTON8)
            {
                buttons |= JoystickButtons.B8;
            }
            if ((wParam & API.JOY_BUTTON9) == API.JOY_BUTTON9)
            {
                buttons |= JoystickButtons.B9;
            }
            if ((wParam & API.JOY_BUTTON10) == API.JOY_BUTTON10)
            {
                buttons |= JoystickButtons.B10;
            }

            GetXYButtonsStateFromLParam(lParam, ref buttons);

            return buttons;
        }
        /// <summary>
        /// 根据消息的参数获取按钮的按压状态值
        /// </summary>
        /// <param name="wParam"></param>
        /// <param name="lParam"></param>
        /// <returns></returns>
        private JoystickButtons GetButtonsPressedStateFromMessageParam(int wParam, int lParam)
        {
            JoystickButtons buttons = JoystickButtons.None;
            if ((wParam & API.JOY_BUTTON1CHG) == API.JOY_BUTTON1CHG)
            {
                buttons |= JoystickButtons.B1;
            }
            if ((wParam & API.JOY_BUTTON2CHG) == API.JOY_BUTTON2CHG)
            {
                buttons |= JoystickButtons.B2;
            }
            if ((wParam & API.JOY_BUTTON3CHG) == API.JOY_BUTTON3CHG)
            {
                buttons |= JoystickButtons.B3;
            }
            if ((wParam & API.JOY_BUTTON4CHG) == API.JOY_BUTTON4CHG)
            {
                buttons |= JoystickButtons.B4;
            }

            GetXYButtonsStateFromLParam(lParam, ref buttons);

            return buttons;
        }
        /// <summary>
        /// 获取X,Y轴的状态
        /// </summary>
        /// <param name="lParam"></param>
        /// <param name="buttons"></param>
        private void GetXYButtonsStateFromLParam(int lParam, ref JoystickButtons buttons)
        {
            //处理X,Y轴
            int x = lParam & 0x0000FFFF;                //低16位存储X轴坐标
            int y = (int)((lParam & 0xFFFF0000) >> 16); //高16位存储Y轴坐标(不直接移位是为避免0xFFFFFF)
            int m = 0x7EFF;                             //中心点的值,
            if (x > m)
            {
                buttons |= JoystickButtons.Right;
            }
            else if (x < m)
            {
                buttons |= JoystickButtons.Left;
            }
            if (y > m)
            {
                buttons |= JoystickButtons.Down;
            }
            else if (y < m)
            {
                buttons |= JoystickButtons.UP;
            }
        }
        #endregion

        #region IDisposable 成员

        public void Dispose()
        {
            Application.RemoveMessageFilter(this);
        }

        #endregion
    }
}
