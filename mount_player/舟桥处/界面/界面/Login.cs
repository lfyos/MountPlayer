using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Security.Cryptography;
using System.IO;
namespace 界面
{
    public partial class Login : Form
    {
        public Login()
        {
            InitializeComponent();
        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }
        //md5加密函数
        public static string GetMd5Hash(string input)
        {
            MD5 md5Hasher = MD5.Create();
            byte[] data = md5Hasher.ComputeHash(Encoding.Default.GetBytes(input));

            StringBuilder sBuilder = new StringBuilder();
            for (int i = 0; i < data.Length; i++)
                sBuilder.Append(data[i].ToString("x2"));

            return sBuilder.ToString();
        }

        private void btRegister_Click(object sender, EventArgs e)
        {
            resetPsdForm rgForm = new resetPsdForm();
            rgForm.ShowDialog();
        }

        private void btLogin_Click(object sender, EventArgs e)
        {
            string psd = this.tbPsd.Text.Trim().ToString();
            string psdMd5 = GetMd5Hash(psd);
            string storePath = Application.StartupPath + "//data//" + "psd.DAT";
            if(!File.Exists(storePath))
            {
                StreamWriter sw = File.CreateText(storePath);
                sw.Write(GetMd5Hash("123"));
                sw.Close();
            }
            else
            {
                StreamReader r = File.OpenText(storePath);
                string psdTrue = r.ReadLine().Trim().ToString();           
            if(psdMd5 == psdTrue)
            {
                this.Hide();
                //mainWindow mw = new mainWindow();
                //mw.Show();
            }
            else
            {
                MessageBox.Show("密码输入错误，请重新输入！");
                this.tbPsd.Text = null;                
            }
            }

        }

        private void Login_Load(object sender, EventArgs e)
        {
            this.tbUsername.Text = "铁道战备舟桥处";
            this.tbUsername.Enabled = false;
            string storePath = Application.StartupPath + "//Data//" + "psd.DAT";
            if (!File.Exists(storePath))
            {
                StreamWriter sw = File.CreateText(storePath);
                sw.Write(GetMd5Hash("123"));
                sw.Close();
            }
        }

        private void Login_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }

        private void btPsdreset_Click(object sender, EventArgs e)
        {
            resetPsdForm rpf = new resetPsdForm();            
            rpf.ShowDialog();
            
        }
    }
}
