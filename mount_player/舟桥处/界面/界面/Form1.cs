using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;
using System.Diagnostics;

namespace 界面
{
    public partial class mainWindow : Form
    {
        public mainWindow()
        {           
            InitializeComponent();
           
        }

        private void mainWindow_Load(object sender, EventArgs e)
        {
            if (userFlag.chooseUserTypeFlag == -1)
            {
                chooseUserType cut = new chooseUserType();
                cut.ShowDialog();   
            }           
            
            try
            {
                pictureBox1.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\右侧界面图片.jpg");
                pictureBox1.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少右侧界面图片.jpg,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox2.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\基本知识介绍.png");
                pictureBox2.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少基本知识介绍.png,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox3.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\模拟拼组演示.png");
                pictureBox3.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少模拟拼组演示.png,请重新添加，下次启动时生效。");

            }

            try
            {
                pictureBox3.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\手动拼组演示.png");
                pictureBox3.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少手动拼组演示.png,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox4.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\整体架设演示.png");
                pictureBox4.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少整体架设演示.png,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox5.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\整体架设演示.png");
                pictureBox5.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少整体架设演示.png,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox6.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\运输装载演示.png");
                pictureBox6.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少运输装载演示.png,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox7.BackgroundImage = Image.FromFile(Application.StartupPath + "\\Data\\方案生成.png");
                pictureBox7.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少方案生成.png,请重新添加，下次启动时生效。");

            }


        }

        private void treeView1_AfterSelect(object sender, TreeViewEventArgs e)
        {

        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void pictureBox2_DoubleClick(object sender, EventArgs e)
        {

        }
        PictureBox old = null;

        private void pictureBox2_Click(object sender, EventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
                return;
            if (old != null)
            {
                old.Width -= 10;
                old.Height -= 10;
                old.Location = new Point(old.Location.X + 5, old.Location.Y + 5);
            }
            old = p;
            p.Width += 10;
            p.Height += 10;
            p.Location = new Point(p.Location.X - 5, p.Location.Y - 5);
            p.BringToFront();
            Process.Start(Application.StartupPath + "\\基本知识介绍.chm");
        }

        private void pictureBox3_Click(object sender, EventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
                return;
            if (old != null)
            {
                old.Width -= 10;
                old.Height -= 10;
                old.Location = new Point(old.Location.X + 5, old.Location.Y + 5);
            }
            old = p;
            p.Width += 10;
            p.Height += 10;
            p.Location = new Point(p.Location.X - 5, p.Location.Y - 5);
            p.BringToFront();
            if (userFlag.flag == 0)
            {
                //管理员程序启动
                Process.Start(Application.StartupPath + "\\Shader_player.exe", "/铁路舟桥   /parts.lfy /merge.lfy  /模拟拼组.configure.lfy  /模拟拼组.last.lfy");
            }
            else
                //用户程序启动
                Process.Start(Application.StartupPath + "\\Shader_player_user.exe", "/铁路舟桥   /parts.lfy /merge.lfy  /模拟拼组.configure.lfy  /模拟拼组.last.lfy");
        }

        private void pictureBox4_Click(object sender, EventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
                return;
            if (old != null)
            {
                old.Width -= 10;
                old.Height -= 10;
                old.Location = new Point(old.Location.X + 5, old.Location.Y + 5);
            }
            old = p;
            p.Width += 10;
            p.Height += 10;
            p.Location = new Point(p.Location.X - 5, p.Location.Y - 5);
            p.BringToFront();

            Process.Start(Application.StartupPath + "\\Shader_player.exe", "/铁路舟桥   /parts.lfy /merge.lfy  /手动拼组.configure.lfy  /手动拼组.last.lfy");
        }

        private void pictureBox5_Click(object sender, EventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
                return;
            if (old != null)
            {
                old.Width -= 10;
                old.Height -= 10;
                old.Location = new Point(old.Location.X + 5, old.Location.Y + 5);
            }
            old = p;
            p.Width += 10;
            p.Height += 10;
            p.Location = new Point(p.Location.X - 5, p.Location.Y - 5);
            p.BringToFront();

            Process.Start(Application.StartupPath + "\\Shader_player.exe", "/200米舟桥架设   /parts.lfy   /merge.lfy  /configure.lfy  /last.lfy");
        }

        private void pictureBox6_Click(object sender, EventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
                return;
            if (old != null)
            {
                old.Width -= 10;
                old.Height -= 10;
                old.Location = new Point(old.Location.X + 5, old.Location.Y + 5);
            }
            old = p;
            p.Width += 10;
            p.Height += 10;
            p.Location = new Point(p.Location.X - 5, p.Location.Y - 5);
            p.BringToFront();

            Process.Start(Application.StartupPath + "\\Shader_player.exe", "/铁路舟桥   /parts.lfy /merge.lfy  /运输装载.configure.lfy  /运输装载.last.lfy");
        }

        private void pictureBox7_Click(object sender, EventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
                return;
            if (old != null)
            {
                old.Width -= 10;
                old.Height -= 10;
                old.Location = new Point(old.Location.X + 5, old.Location.Y + 5);
            }
            old = p;
            p.Width += 10;
            p.Height += 10;
            p.Location = new Point(p.Location.X - 5, p.Location.Y - 5);
            p.BringToFront();

            Process.Start(Application.StartupPath + "\\schemeGenerate.exe");
        }

        private void label2_Click(object sender, EventArgs e)
        {
            Process.Start(Application.StartupPath + "\\基本知识介绍.chm");
        }

        private void label3_Click(object sender, EventArgs e)
        {
            if (userFlag.flag == 0)
            {
                //管理员程序启动
                Process.Start(Application.StartupPath + "\\Shader_player.exe", "/铁路舟桥   /parts.lfy /merge.lfy  /模拟拼组.configure.lfy  /模拟拼组.last.lfy");
            }
            else
                //用户程序启动
                Process.Start(Application.StartupPath + "\\Shader_player_user.exe", "/铁路舟桥   /parts.lfy /merge.lfy  /模拟拼组.configure.lfy  /模拟拼组.last.lfy");
        }

        private void label5_Click(object sender, EventArgs e)
        {
            Process.Start(Application.StartupPath + "\\Shader_player.exe", "/200米舟桥架设   /parts.lfy /merge.lfy  /configure.lfy  /last.lfy");
        }

        private void label4_Click(object sender, EventArgs e)
        {
            Process.Start(Application.StartupPath + "\\Shader_player.exe", "/铁路舟桥   /parts.lfy /merge.lfy  /手动拼组.configure.lfy  /手动拼组.last.lfy");
        }

        private void label6_Click(object sender, EventArgs e)
        {
            Process.Start(Application.StartupPath + "\\Shader_player.exe", "/铁路舟桥   /parts.lfy /merge.lfy  /运输装载.configure.lfy  /运输装载.last.lfy");
        }

        private void label7_Click(object sender, EventArgs e)
        {
            Process.Start(Application.StartupPath + "\\schemeGenerate.exe");
        }

        private void pictureBox2_Paint(object sender, PaintEventArgs e)
        {
             PictureBox p = (PictureBox)sender;  
            if (p == old)  
            {  
                Pen pp = new Pen(Color.Red);  
                e.Graphics.DrawRectangle(pp, e.ClipRectangle.X, e.ClipRectangle.Y, e.ClipRectangle.X + e.ClipRectangle.Width - 1, e.ClipRectangle.Y + e.ClipRectangle.Height - 1);  
            }  

        }

        private void pictureBox3_Paint(object sender, PaintEventArgs e)
        {
            PictureBox p = (PictureBox)sender;  
            if (p == old)  
            {  
                Pen pp = new Pen(Color.Red);  
                e.Graphics.DrawRectangle(pp, e.ClipRectangle.X, e.ClipRectangle.Y, e.ClipRectangle.X + e.ClipRectangle.Width - 1, e.ClipRectangle.Y + e.ClipRectangle.Height - 1);  
            }  
        }

        private void pictureBox4_Paint(object sender, PaintEventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
            {
                Pen pp = new Pen(Color.Red);
                e.Graphics.DrawRectangle(pp, e.ClipRectangle.X, e.ClipRectangle.Y, e.ClipRectangle.X + e.ClipRectangle.Width - 1, e.ClipRectangle.Y + e.ClipRectangle.Height - 1);
            }  
        }

        private void pictureBox5_Paint(object sender, PaintEventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
            {
                Pen pp = new Pen(Color.Red);
                e.Graphics.DrawRectangle(pp, e.ClipRectangle.X, e.ClipRectangle.Y, e.ClipRectangle.X + e.ClipRectangle.Width - 1, e.ClipRectangle.Y + e.ClipRectangle.Height - 1);
            }  
        }

        private void pictureBox7_Paint(object sender, PaintEventArgs e)
        {
            PictureBox p = (PictureBox)sender;
            if (p == old)
            {
                Pen pp = new Pen(Color.Red);
                e.Graphics.DrawRectangle(pp, e.ClipRectangle.X, e.ClipRectangle.Y, e.ClipRectangle.X + e.ClipRectangle.Width - 1, e.ClipRectangle.Y + e.ClipRectangle.Height - 1);
            }  
        }

        private void mainWindow_FormClosed(object sender, FormClosedEventArgs e)
        {
            Application.Exit();
        }









    }
}
