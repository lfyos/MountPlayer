using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace 界面
{
    public partial class chooseUserType : Form
    {
        public chooseUserType()
        {
            InitializeComponent();
        }

        private void btGuest_Click(object sender, EventArgs e)
        {
            userFlag.flag = -1;

            this.Hide();            
            //mainWindow mw = new mainWindow();
            //mw.Show();
        }

        private void chooseUserType_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void btAdmin_Click(object sender, EventArgs e)
        {
            userFlag.flag = 0;
           
            this.Hide();
            Login lg = new Login();
            lg.ShowDialog();
        }

        private void chooseUserType_Load(object sender, EventArgs e)
        {
            userFlag.chooseUserTypeFlag = 1;

        }
    }
}
