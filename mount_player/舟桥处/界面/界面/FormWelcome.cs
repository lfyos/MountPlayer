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
    public partial class FormWelcome : Form
    {
        public FormWelcome()
        {
            InitializeComponent();
        }

        private void FormWelcome_Load(object sender, EventArgs e)
        {
            try
            {
                this.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\启动界面.jpg");
                this.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch(Exception ex)
            {
                MessageBox.Show("缺少欢迎界面.jpg，请重新添加，下次启动时生效。");
            }

            userFlag.chooseUserTypeFlag = -1;
        }

        private void FormWelcome_Click(object sender, EventArgs e)
        {
            

            mainWindow mw = new mainWindow();
            mw.Show();
            
            this.Hide();
        }
    }
}
