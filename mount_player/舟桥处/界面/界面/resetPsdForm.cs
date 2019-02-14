using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.IO;
using System.Security;
using System.Security.Cryptography;
namespace 界面
{
    public partial class resetPsdForm : Form
    {
        public resetPsdForm()
        {
            InitializeComponent();
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

        private void btOK_Click(object sender, EventArgs e)
        {
            string oldPsd=null, newPsd=null, newPsd2=null,storePath=null,psdTrue=null;
            if (this.tbPsd.Text.Trim().ToString()!=null)
            { 
                newPsd = this.tbPsd.Text.Trim().ToString();
            }
           
            newPsd2 = this.tbPsd2.Text.Trim().ToString();
            storePath = Application.StartupPath + "//Data//" + "psd.DAT";
            StreamReader r = File.OpenText(storePath);
            psdTrue = r.ReadLine().Trim().ToString();
            r.Close();  
                oldPsd = GetMd5Hash(this.tbOldPsd.Text.Trim().ToString());
                if (oldPsd!=psdTrue)
                {
                    MessageBox.Show("旧密码不正确，请重新输入!");
                    this.tbOldPsd.Text = null;
                }
                else if (newPsd != newPsd2)
                {
                    MessageBox.Show("两次输入的新密码不一致，请重新输入!");
                    this.tbPsd.Text= null;
                    this.tbPsd2.Text = null;
                }
                else
                {
                    StreamWriter sw = File.CreateText(storePath);
                    sw.Write(GetMd5Hash(newPsd));
                    sw.Close();
                    MessageBox.Show("密码修改成功！");
                    this.Close();
                }
            

            
        }

        private void btCancel_Click(object sender, EventArgs e)
        {
            this.Close();
        }
    }
}
