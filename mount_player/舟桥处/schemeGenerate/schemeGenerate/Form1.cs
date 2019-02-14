using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.IO;
using System.Reflection;
using System.Windows.Forms;
using Microsoft.Win32;

using Microsoft.Office.Core;
using Word = Microsoft.Office.Interop.Word;
using System.Text.RegularExpressions;
using System.Diagnostics;



                    

namespace schemeGenerate
{
    public partial class MainWindow : Form
    {

        killAllWinword killer = new killAllWinword();

        public MainWindow()
        {

            //killer.killAllProcess();             
            InitializeComponent();
        }



        private void MainWindow_SizeChanged(object sender, EventArgs e)
        {
            tabControl1.Refresh();
            layout1.Refresh();
        }
                      
        //获得括号之前的数字，从换行符/r之前获得
        public int getNumBeforeBracket(string src)
        {   
            string s = src.Trim().Substring(0, src.IndexOf("\r"));
            return Convert.ToInt16(s);

        }

        public int getNumBeforeBracketNoR(string src)
        {
            string s = src.Trim().Substring(0, src.IndexOf("("));
            return Convert.ToInt16(s);
        }

       //获得括号内的数字
        public int getNumInBracket(string src)
        {
            string temp = null;
            char[] a = src.ToCharArray();
            try
            {
                for (int i = 0; i < a.Length; i++)
                {
                    if (a[i] == '(')
                    {
                        i++;
                        while (a[i] != ')')
                        {
                            temp += a[i++];

                        }
                    }
                }


            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return 0;
            }

            return Convert.ToInt16(temp);
        }


        private void btnHeZhong_Click(object sender, EventArgs e)
        {
            //取得要架设的舟桥的长度，判断是否输入正确
            string meter = this.textBox1.Text.Trim().ToString();
             double temp ;
            bool isNum = true;
            try
            {
              temp = Convert.ToDouble(meter);

            }
            catch (System.Exception ex)
            {

                MessageBox.Show("输入错误，请重新输入");
                isNum = false;
                return;
                
            }
            if (isNum)
            {
                temp = Convert.ToDouble(meter);
                if (temp < 101.26 || temp > 1000)
                {
                    MessageBox.Show("输入错误：河中部分最小宽度为101.26米，最大为1000米");
                    this.textBox1.Text = null;
                    return;
                }
            }



            //根据输入的米数，计算需要架设多少米的桥
            //这里有两个数字，门桥数量和边孔数量
            //门桥长度=16.2，边孔长度=34.28，门桥数量需要计算，边孔数量则固定为2个
            //粗略计算出来的门桥长度 = （要架设的桥长度-边孔长度/2）/门桥长度，这里只需要取整数部分即可
            double lengthOfmenqiao = 16.2;   //门桥长度
            double lengthOfbiankong = 34.28;//边孔长度
            int numOfbiankong = 2;          //边孔数量固定，不需要变
            double doubleOfmenqiao = (Convert.ToDouble(meter) - 2 * lengthOfbiankong) / lengthOfmenqiao;
            //int numOfmenqiao = Convert.ToInt16(doubleOfmenqiao);//这里只需要取整，得到门桥数量
            int numOfmenqiao = Convert.ToInt16(Math.Truncate(doubleOfmenqiao));
            double numofbeiyong = 1.2;
            int K = 1;

            Word._Application appCreate;// = new Word.ApplicationClass();//= new Word.Application();
            Word._Document doc = null;
            appCreate = new Word.Application();
            object missing = System.Reflection.Missing.Value;
            object readOnly = false;
            object isVisable = false;
            object index = 1;

            File.Copy(Application.StartupPath + "\\PrimData\\铁路舟桥河中部分器材数量表模板.doc",
                Application.StartupPath + "\\PrimData\\temp.doc", true);
            object FileName = Application.StartupPath + "\\PrimData\\temp.doc";
            string finalFileName;
                try
                {
                    this.textBox1.Enabled = false;
                    this.btnHeZhong.Enabled = false;
                   
                    doc = appCreate.Documents.Open(ref FileName, ref missing, ref readOnly,
                                ref missing, ref missing, ref missing, ref missing, ref missing,
                                ref missing, ref missing, ref missing, ref isVisable, ref missing,
                                ref missing, ref missing, ref missing);
                   

                    //在文档的首行写入门桥数量，边孔数量，以及架设桥的长度
                    string strTextOfFirstLine = "舟桥总长度为： " + (numOfmenqiao * lengthOfmenqiao + numOfbiankong * lengthOfbiankong).ToString() +"米" +"\r\n";
                    string strTextOfSecondtLine = "门桥数量为： " + numOfmenqiao.ToString() + "     " + "边孔数量：" + numOfbiankong.ToString() +"\r\n";
                    doc.Paragraphs[1].Range.Select();
                    doc.Paragraphs[1].Range.Text = strTextOfFirstLine;
                    doc.Paragraphs[2].Range.Select();
                    doc.Paragraphs[2].Range.Text = strTextOfSecondtLine;
                    //string teststr = doc.Paragraphs[1].Range.Text.ToString();    
                    //object bookMarksIndex = 1;
                    //doc.Bookmarks.get_Item(ref bookMarksIndex).Range.Text= strTextOfFirstLine;
                    //bookMarksIndex = 2;
                    //doc.Bookmarks.get_Item(ref bookMarksIndex).Range.Text = strTextOfSecondtLine;
                    //doc.Paragraphs[1].Range.Text = strTextOfFirstLine.ToString();
                    
                  // MessageBox.Show(doc.Tables.Count.ToString()); 统计word内的表格数o

                    //获取第一个表格，并修改其相应内容
                    Word.Table tempTable = doc.Tables[1];

                    //填充第一个表的表头，舟桥长度， 2行6列
                    tempTable.Cell(2, 6).Range.Text = meter.ToString();
                    #region //填充第一张表的其他部分，第一张表只需要填充下表头即可，数据不用修改，注释掉先

                    //填充第一个表的3-14行
                    //for (int i = 3; i <= 14; i++)
                    //{

                    //    string tableContent = tempTable.Cell(i, 6).Range.Text.Trim().ToString();
                    //    tableContent = tableContent.Remove(tableContent.Length - 2, 2);

                    //    int num = Convert.ToInt16(tableContent) * K;
                    //    tempTable.Cell(i, 6).Range.Text = num.ToString();
                    //}
                    #endregion

                    int numBeforeBracket, numInBracket;

                    #region                   //获取第二个表格，只需要填充第9列，把7,8列的对应相乘即可
                    tempTable = doc.Tables[2];
                    //填充第二个表的舟桥长度， 1行7列
                    tempTable.Cell(1, 7).Range.Text = "架设" + meter.ToString() + "米舟桥数量";


                    //单独处理10行7列和26行7列的小数，再相加
                    for (int i = 3; i <= 34; i++)
                    {
                        if (i == 10 || i == 26)  //过滤掉第10列和第26列
                        {
                            i = i + 1;
                            string rowTen = tempTable.Cell(10, 7).Range.Text.ToString();
                            rowTen = rowTen.Remove(rowTen.Length - 2, 2);
                            tempTable.Cell(10, 9).Range.Text = (Convert.ToDouble(rowTen) * numOfmenqiao).ToString();

                            rowTen = tempTable.Cell(26, 7).Range.Text.ToString();
                            rowTen = rowTen.Remove(rowTen.Length - 2, 2);
                            tempTable.Cell(26, 9).Range.Text = (Convert.ToDouble(rowTen) * numOfmenqiao + Convert.ToInt16(tempTable.Cell(26, 8).Range.Text.ToString().Remove(tempTable.Cell(26, 8).Range.Text.ToString().Length - 2, 2))).ToString();
                        }

                        string tableContent = tempTable.Cell(i, 9).Range.Text.Trim().ToString();
                        if (tableContent.Length < 2)
                        {
                            tempTable.Cell(i, 9).Range.Text = null;
                        }
                        else if (tableContent.Length >= 8)
                        {
                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;

                            string rowOfEIStr = null;
                            if (tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfEIStr = tempTable.Cell(i, 8).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfEIStr = null;
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            int rowIofEI = Convert.ToInt16(rowOfEIStr) * numOfbiankong; //处理第8列的边孔数量
                            tempTable.Cell(i, 9).Range.Text = (rowIofSV + rowIofEI).ToString() + "\n" + "(" + (Convert.ToInt16((rowIofSV + rowIofEI) * numofbeiyong)).ToString() + ")";

                        }
                        else
                        {
                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;

                            string rowOfEIStr = null;
                            if (tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfEIStr = tempTable.Cell(i, 8).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfEIStr = null;
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            int rowIofEI = Convert.ToInt16(rowOfEIStr) * numOfbiankong; //处理第8列的边孔数量
                            // tableContent = tableContent.Remove(tableContent.Length - 2, 2);
                            // int num = Convert.ToInt16(tableContent) * K;
                            tempTable.Cell(i, 9).Range.Text = (rowIofSV + rowIofEI).ToString();
                        }


                    }

                    //单独处理10行7列和26行7列的小数

                    #endregion   //第二个表格处理完毕


                    #region  //处理第三个表格
                    tempTable = doc.Tables[3];
                    //表头
                    tempTable.Cell(1, 7).Range.Text = "架设" + meter.ToString() + "米舟桥数量";
                    //处理表格数据
                    for (int i = 3; i <= 11; i++)
                    {
                        if (i == 10 || i == 26)  //过滤掉第10列和第26列
                        {
                            i = i + 1;
                        }

                        string tableContent = tempTable.Cell(i, 9).Range.Text.Trim().ToString();
                        if (tableContent.Length < 2)
                        {
                            tempTable.Cell(i, 9).Range.Text = null;
                        }
                        else if (tableContent.Length >= 8)
                        {
                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;

                            string rowOfEIStr = null;
                            if (tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfEIStr = tempTable.Cell(i, 8).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfEIStr = null;
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            int rowIofEI = Convert.ToInt16(rowOfEIStr) * numOfbiankong; //处理第8列的边孔数量
                            tempTable.Cell(i, 9).Range.Text = (rowIofSV + rowIofEI).ToString() + "\n" + "(" + (Convert.ToInt16((rowIofSV + rowIofEI) * numofbeiyong)).ToString() + ")";

                        }
                        else
                        {
                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;

                            string rowOfEIStr = null;
                            if (tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfEIStr = tempTable.Cell(i, 8).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfEIStr = null;
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            int rowIofEI = Convert.ToInt16(rowOfEIStr) * numOfbiankong; //处理第8列的边孔数量
                            // tableContent = tableContent.Remove(tableContent.Length - 2, 2);
                            // int num = Convert.ToInt16(tableContent) * K;
                            tempTable.Cell(i, 9).Range.Text = (rowIofSV + rowIofEI).ToString();
                        }


                    }
                    #endregion //第三个表格无特殊情况，处理完毕


                    #region  //处理第四个表格
                    tempTable = doc.Tables[4];
                    //表头
                    tempTable.Cell(1, 7).Range.Text = "架设" + meter.ToString() + "米舟桥数量";
                    //处理表格数据,4-26行 ,16行单独处理
                    for (int i = 4; i <= 26; i++)
                    {

                        if (i == 16)
                        {
                            i = i + 1;
                            //16行有括号，在这里单独处理第16行
                            string tempstrSixteen = tempTable.Cell(16, 7).Range.Text.ToString();//.Remove(tempTable.Cell(16,7).Range.Text.ToString().Length-2,2);//16行7列
                            numBeforeBracket = getNumBeforeBracketNoR(tempstrSixteen);//每门桥
                            tempstrSixteen = tempTable.Cell(16, 8).Range.Text.ToString().Remove(tempTable.Cell(16, 8).Range.Text.ToString().Length - 2, 2);//16行7列
                            int tempSixteenEightC = Convert.ToInt16(tempstrSixteen); //16行8列，每边孔
                            tempTable.Cell(16, 9).Range.Text = (numBeforeBracket * numOfmenqiao + tempSixteenEightC * numOfbiankong).ToString();

                        }
                        //有些行不是9列，跳过
                        if (i == 19 || i == 23)
                        {
                            i = i + 1;

                        }

                        string tableContent = tempTable.Cell(i, 9).Range.Text.Trim().ToString();
                        if (tableContent.Length < 2)
                        {
                            tempTable.Cell(i, 9).Range.Text = null;
                        }
                        else if (tableContent.Length >= 8)
                        {
                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;

                            string rowOfEIStr = null;
                            if (tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfEIStr = tempTable.Cell(i, 8).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfEIStr = null;
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            int rowIofEI = Convert.ToInt16(rowOfEIStr) * numOfbiankong; //处理第8列的边孔数量
                            tempTable.Cell(i, 9).Range.Text = (rowIofSV + rowIofEI).ToString() + "\n" + "(" + (Convert.ToInt16((rowIofSV + rowIofEI) * numofbeiyong)).ToString() + ")";

                        }
                        else
                        {
                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;

                            string rowOfEIStr = null;
                            if (tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfEIStr = tempTable.Cell(i, 8).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfEIStr = null;
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            int rowIofEI = Convert.ToInt16(rowOfEIStr) * numOfbiankong; //处理第8列的边孔数量
                            // tableContent = tableContent.Remove(tableContent.Length - 2, 2);
                            // int num = Convert.ToInt16(tableContent) * K;
                            tempTable.Cell(i, 9).Range.Text = (rowIofSV + rowIofEI).ToString();
                        }
                    }


                    #endregion  //第四个表格处理完毕


                    #region //处理第五个表格,无特殊情况，直接处理

                    tempTable = doc.Tables[5];
                    //表头
                    tempTable.Cell(1, 7).Range.Text = "架设" + meter.ToString() + "米舟桥数量";
                    //处理表格数据,4-25行
                    for (int i = 4; i <= 25; i++)
                    {


                        //第10行不需要填充，，跳过
                        if (i == 10)
                        {
                            i = i + 1;
                        }
                        string tableContent = tempTable.Cell(i, 9).Range.Text.Trim().ToString();
                        if (tableContent.Length < 2)
                        {
                            tempTable.Cell(i, 9).Range.Text = null;
                        }
                        else if (tableContent.Length >= 8)
                        {
                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;

                            string rowOfEIStr = null;
                            if (tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfEIStr = tempTable.Cell(i, 8).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfEIStr = null;
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            int rowIofEI = Convert.ToInt16(rowOfEIStr) * numOfbiankong; //处理第8列的边孔数量
                            tempTable.Cell(i, 9).Range.Text = (rowIofSV + rowIofEI).ToString() + "\n" + "(" + (Convert.ToInt16((rowIofSV + rowIofEI) * numofbeiyong)).ToString() + ")";

                        }
                        else
                        {
                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;

                            string rowOfEIStr = null;
                            if (tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfEIStr = tempTable.Cell(i, 8).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 8).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfEIStr = null;
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            int rowIofEI = Convert.ToInt16(rowOfEIStr) * numOfbiankong; //处理第8列的边孔数量
                            // tableContent = tableContent.Remove(tableContent.Length - 2, 2);
                            // int num = Convert.ToInt16(tableContent) * K;
                            tempTable.Cell(i, 9).Range.Text = (rowIofSV + rowIofEI).ToString();
                        }

                    }

                    #endregion             //第五个表格处理完毕


                    #region             //处理第六个表格
                    tempTable = doc.Tables[6];
                    //处理表格数据,3-12行，第9行特殊，单独处理
                    for (int i = 3; i <= 12; i++)
                    {
                        if (i == 9)
                        {
                            i = i + 1;
                            //单独处理第六个表格的第9行
                            string sixTableNineRowS = tempTable.Cell(9, 7).Range.Text.ToString(); //获得第七列的字符串
                            string sixTableNineRowENum = (getNumBeforeBracket(sixTableNineRowS) * numOfbiankong).ToString(); //计算出第八列的结果
                            tempTable.Cell(9, 8).Range.Text = sixTableNineRowENum + "\r" + sixTableNineRowENum + "\r" + sixTableNineRowENum + "\r";
                        }

                        string tableContent = tempTable.Cell(i, 8).Range.Text.Trim().ToString();
                        if (tableContent.Length < 2)
                        {
                            tempTable.Cell(i, 8).Range.Text = null;
                        }
                        else if (tableContent.Length >= 8)
                        {
                            string rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            tempTable.Cell(i, 8).Range.Text = (rowIofSV * numOfbiankong).ToString() + "\n" + "(" + (Convert.ToInt16((rowIofSV * numOfbiankong) * numofbeiyong)).ToString() + ")";

                        }
                        else
                        {
                            string rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);
                            int rowIofSV = Convert.ToInt16(rowOfSvStr) * numOfmenqiao; //处理第7列的门桥数量
                            // tableContent = tableContent.Remove(tableContent.Length - 2, 2);
                            // int num = Convert.ToInt16(tableContent) * K;
                            tempTable.Cell(i, 8).Range.Text = (rowIofSV * numOfbiankong).ToString();
                        }

                    }

                    #endregion  //第六个表格处理完毕


                    #region  //处理第7个表格
                    //获取第七个表格
                    tempTable = doc.Tables[7];
                    //处理表头
                    tempTable.Cell(1, 6).Range.Text = "架设" + meter.ToString() + "米舟桥数量";
                    //3-32行
                    //单独处理18，19，21,23,24
                    for (int i = 3; i <= 32; i++)
                    {
                        //先处理18，19,21,23，24行
                        if (i == 18 || i == 19 || i == 21 || i == 23 || i == 24)
                        {
                            string rowstrSix = tempTable.Cell(i, 6).Range.Text.ToString();
                            double rowNumSix = Convert.ToDouble(rowstrSix.Remove(rowstrSix.Length - 2, 2));
                            string rowstrSeven = tempTable.Cell(i, 7).Range.Text.ToString();
                            int rowNumSeven = Convert.ToInt16(rowstrSeven.Remove(rowstrSeven.Length - 2, 2));
                            tempTable.Cell(i, 8).Range.Text = (rowNumSix * numOfmenqiao + rowNumSeven * numOfbiankong).ToString();
                        }

                        if (i == 18 || i == 21 || i == 23)
                        {
                            i = i + 1;                                //18行

                        }
                        if (i == 19 || i == 24)
                        {
                            i = i + 1;
                        }
                        string tableContent = tempTable.Cell(i, 8).Range.Text.Trim().ToString();
                        if (tableContent.Length < 2)
                        {
                            tempTable.Cell(i, 8).Range.Text = null;
                        }
                        else if (tableContent.Length >= 8)
                        {
                            string rowOfSixStr = null;
                            if (tempTable.Cell(i, 6).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSixStr = tempTable.Cell(i, 6).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 6).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSixStr = null;

                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;
                            int rowIofSix = Convert.ToInt16(rowOfSixStr) * numOfmenqiao; //处理第6列的门桥数量
                            int rowIofSv = Convert.ToInt16(rowOfSvStr) * numOfbiankong; //处理第7列的边孔数量
                            tempTable.Cell(i, 8).Range.Text = (rowIofSix + rowIofSv).ToString() + "\n" + "(" + (Convert.ToInt16((rowIofSix + rowIofSv) * numofbeiyong)).ToString() + ")";

                        }
                        else
                        {
                            string rowOfSixStr = null;
                            if (tempTable.Cell(i, 6).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSixStr = tempTable.Cell(i, 6).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 6).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSixStr = null;

                            string rowOfSvStr = null;
                            if (tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length > 2)
                            {
                                rowOfSvStr = tempTable.Cell(i, 7).Range.Text.Trim().ToString().Remove(tempTable.Cell(i, 7).Range.Text.Trim().ToString().Length - 2, 2);

                            }
                            else
                                rowOfSvStr = null;
                            int rowIofSix = Convert.ToInt16(rowOfSixStr) * numOfmenqiao; //处理第6列的门桥数量
                            int rowIofSv = Convert.ToInt16(rowOfSvStr) * numOfbiankong; //处理第7列的边孔数量
                            // tableContent = tableContent.Remove(tableContent.Length - 2, 2);
                            // int num = Convert.ToInt16(tableContent) * K;
                            tempTable.Cell(i, 8).Range.Text = (rowIofSix + rowIofSv).ToString();
                        }

                    }
                    #endregion //第七张表处理结束


                    doc.Save();
                    this.textBox1.Enabled = true;
                    this.btnHeZhong.Enabled = true;

                    finalFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + meter.ToString() + "米铁路舟桥河中部分器材数量表.doc";
                 
                        File.Copy(Application.StartupPath+"\\PrimData\\temp.doc",
                           finalFileName,
                            true);

                        MessageBox.Show( meter.ToString() + "米铁路舟桥河中部分器材数量表生成成功，已保存至桌面");


                }
                    catch(Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                    return;
                }


                finally
                {
                    if (doc != null)
                    {
                        doc.Close(ref missing, ref missing, ref missing);
                        doc = null;
                    }

                    if (appCreate != null)
                    {

                        appCreate.Quit(ref missing, ref missing, ref missing);
                        appCreate = null;
                    }
                }

            }

        //判断是否为正整数
        public static bool isIntegerNumber(string strValue)
        {

            Regex regex = new Regex("^[0-9]*[1-9][0-9]*$");
            return regex.IsMatch(strValue.Trim());

        }

        private void button2_Click(object sender, EventArgs e)
            
        {
            string lunduNum = this.tbLunduNum.Text.Trim().ToString();
            if (!isIntegerNumber(lunduNum))
            {
                MessageBox.Show("输入错误，请输入一个整数！");
                this.tbLunduNum.Text = null;
                return;
            }


  
            //操作文档
            Word.ApplicationClass appCreate = new Word.ApplicationClass();
            Word.Document doc = null;
            object missing = System.Reflection.Missing.Value;
            object readOnly = false;
            object isVisable = false;
            object index = 1;

            File.Copy(Application.StartupPath +"\\PrimData\\丙式轮渡器材数量表模板.doc",Application.StartupPath+ "\\PrimData\\temp.doc", true);

            //object FileName = @"C:\Users\ww\Desktop\铁路舟桥方案生成\schemeGenerate\schemeGenerate\bin\Debug\PrimData\temp.doc";

            object FileName = Application.StartupPath + "\\PrimData\\temp.doc";
            //object FileName = "PrimData/temp1.doc";
            try
            {
                //轮渡数量

                int K = Convert.ToInt16(lunduNum);

                this.tbLunduNum.Enabled = false;
                this.btnLundu.Enabled = false;
                this.button3.Enabled = false;



                doc = appCreate.Documents.Open(ref FileName, ref missing, ref readOnly,
                            ref missing, ref missing, ref missing, ref missing, ref missing,
                            ref missing, ref missing, ref missing, ref isVisable, ref missing,
                            ref missing, ref missing, ref missing);

                //设置标题  "艘丙式轮渡器材配套数量表"
                doc.Paragraphs[1].Range.InsertParagraphBefore();
                doc.Paragraphs[1].Range.Select();
                doc.Paragraphs[1].Range.Text = K.ToString();

                //获取第一个表格，并修改其相应内容
                //这个方案里只有一张表
                Word.Table tempTable = doc.Tables[1];
                //填充3-32行
                for (int i = 3; i <= 32;i++ )
                {

                    string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                    temp = temp.Remove(temp.Length - 2, 2);

                    int num = Convert.ToInt16(temp) * K;
                    tempTable.Cell(i, 7).Range.Text = num.ToString();
                }

                //填充34-40行
                for (int i = 34; i <= 40; i++)
                {

                    string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                    temp = temp.Remove(temp.Length - 2, 2);

                    int num = Convert.ToInt16(temp) * K;
                    tempTable.Cell(i, 7).Range.Text = num.ToString();
                }

                //填充42-58行
                for (int i = 42; i <= 58; i++)
                {

                    string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                    temp = temp.Remove(temp.Length - 2, 2);

                    int num = Convert.ToInt16(temp) * K;
                    tempTable.Cell(i, 7).Range.Text = num.ToString();
                }

                //填充61-79
                for (int i = 61; i <= 79; i++)
                {

                    string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                    temp = temp.Remove(temp.Length - 2, 2);

                    int num = Convert.ToInt16(temp) * K;
                    tempTable.Cell(i, 7).Range.Text = num.ToString();
                }

                //填充82-92

                for (int i = 82; i <= 92; i++)
                {

                    string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                    temp = temp.Remove(temp.Length - 2, 2);

                    int num = Convert.ToInt16(temp) * K;
                    tempTable.Cell(i, 7).Range.Text = num.ToString();
                }
                //MessageBox.Show(doc.Paragraphs[1].Range.Text.ToString());

                doc.Save();
                this.tbLunduNum.Enabled = true;
                this.btnLundu.Enabled = true;
                this.button3.Enabled = true;

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
                return;
            }


            finally
            {
                if (doc != null)
                {
                    doc.Close(ref missing, ref missing, ref missing);
                    doc = null;
                }

                if (appCreate != null)
                {
                    appCreate.Quit(ref missing, ref missing, ref missing);
                    appCreate = null;
                }
            }


            //最后，将修改后的文档复制到桌面

            string destFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + this.tbLunduNum.Text.Trim().ToString() + "艘丙式轮渡器材数量表.doc";
            try
            {
                File.Copy("PrimData\\temp.doc", 
                   destFileName,
                    true);

                MessageBox.Show(this.tbLunduNum.Text.ToString()+"艘丙式轮渡器材数量表生成成功，已保存至桌面");

            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void button3_Click(object sender, EventArgs e)
        {

            string zhanqiaoStrNum = this.tbzhanqiaoNum.Text.Trim().ToString();
            if (!isIntegerNumber(zhanqiaoStrNum))
            {
                MessageBox.Show("输入错误，请输入一个整数！");
                this.tbzhanqiaoNum.Text = null;
                return;
            }

            int K = 0;
            //确定有几座码头           
            switch (Convert.ToInt16(zhanqiaoStrNum))
            {

                case 1: K = 1; break;
                default:
                    K = Convert.ToInt16(zhanqiaoStrNum);
                    break;

            }
            if (K == 1)
            {
                string destFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\一座丙式轮渡码头器材数量表.doc";
                try
                {
                    File.Copy(Application.StartupPath + "\\PrimData\\丙式轮渡码头器材数量表模板.doc",
                       destFileName,
                        true);

                    MessageBox.Show("一座丙式轮渡码头器材数量表生成成功，已保存至桌面");
                    return;

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                }
            }
            else
            {
                Word.ApplicationClass appCreate = new Word.ApplicationClass();
                Word.Document doc = null;
                object missing = System.Reflection.Missing.Value;
                object readOnly = false;
                object isVisable = false;
                object index = 1;

                File.Copy(Application.StartupPath + "\\PrimData\\丙式轮渡码头器材数量表模板2.doc", Application.StartupPath + "\\PrimData\\temp.doc", true);

                //object FileName = @"C:\Users\ww\Desktop\铁路舟桥方案生成\schemeGenerate\schemeGenerate\bin\Debug\PrimData\temp.doc";

                object FileName = Application.StartupPath + "\\PrimData\\temp.doc";
                try
                {
                    this.tbLunduNum.Enabled = false;
                    this.btnLundu.Enabled = false;
                    this.button3.Enabled = false;

                    doc = appCreate.Documents.Open(ref FileName, ref missing, ref readOnly,
                                ref missing, ref missing, ref missing, ref missing, ref missing,
                                ref missing, ref missing, ref missing, ref isVisable, ref missing,
                                ref missing, ref missing, ref missing);



                    
                    //设置标题  "丙式轮渡码头器材数量表"
                    doc.Paragraphs[1].Range.InsertParagraphBefore();
                    doc.Paragraphs[1].Range.Select();
                    doc.Paragraphs[1].Range.Text = K.ToString();
                    //获取第一个表格，并修改其相应内容
                    //这个方案里只有一张表

                    Word.Table tempTable = doc.Tables[1];
                    //填充3-15行
                    for (int i = 3; i <= 15; i++)
                    {

                        string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                        temp = temp.Remove(temp.Length - 2, 2);

                        int num = Convert.ToInt16(temp) * K;
                        tempTable.Cell(i, 7).Range.Select();
                        tempTable.Cell(i, 7).Range.Text = num.ToString();
                    }

                    //填充17-22行
                    for (int i = 17; i <= 22; i++)
                    {

                        string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                        temp = temp.Remove(temp.Length - 2, 2);

                        int num = Convert.ToInt16(temp) * K;
                        tempTable.Cell(i, 7).Range.Select();
                        tempTable.Cell(i, 7).Range.Text = num.ToString();
                    }

                    //填充第18行
                    if (K  == 2)
                    {
                        tempTable.Cell(23, 7).Range.Text = "各 2";
                    } 
                    else if (K == 3)
                    {
                        tempTable.Cell(23, 7).Range.Text = "各 3";
                    }

                    //填充24-37行
                    for (int i = 24; i <= 37; i++)
                    {

                        string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                        temp = temp.Remove(temp.Length - 2, 2);

                        int num = Convert.ToInt16(temp) * K;
                        tempTable.Cell(i, 7).Range.Text = num.ToString();
                    }

                    //填充41-55
                    for (int i = 41; i <= 55; i++)
                    {

                        string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                        temp = temp.Remove(temp.Length - 2, 2);

                        int num = Convert.ToInt16(temp) * K;
                        tempTable.Cell(i, 7).Range.Text = num.ToString();
                    }

                    //填充57-64行
                    for (int i = 57; i <= 64; i++)
                    {

                        string temp = tempTable.Cell(i, 7).Range.Text.Trim().ToString();
                        temp = temp.Remove(temp.Length - 2, 2);

                        int num = Convert.ToInt16(temp) * K;
                        tempTable.Cell(i, 7).Range.Text = num.ToString();
                    }

                    doc.Save();
                    this.tbLunduNum.Enabled = true;
                    this.btnLundu.Enabled = true;
                    this.button3.Enabled = true;

                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.ToString());
                    return;
                }


                finally
                {
                    if (doc != null)
                    {
                        doc.Close(ref missing, ref missing, ref missing);
                        doc = null;
                    }

                    if (appCreate != null)
                    {
                        appCreate.Quit(ref missing, ref missing, ref missing);
                        appCreate = null;
                    }
                }
            }


            //最后，将修改后的文档复制到桌面

            string MaTouFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" +  K.ToString()+"座丙式轮渡码头器材数量.doc";
            try
            {
                File.Copy(Application.StartupPath + "\\PrimData\\temp.doc",
                   MaTouFileName,
                    true);

                MessageBox.Show(K.ToString() + "座丙式轮渡码头器材数量生成成功，已保存至桌面");

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
            
            //try
            //{
            //    File.Copy("PrimData\\丙式轮渡码头器材数量表.doc",
            //       destFileName,
            //        true);

            //    MessageBox.Show("一座丙式轮渡码头器材数量表生成成功，已保存至桌面");

            //}
            //catch (Exception ex)
            //{
            //    MessageBox.Show(ex.ToString());
            //}
        }

        private void MainWindow_Load(object sender, EventArgs e)
        {
            this.cbbLaoShiZhanQiao.SelectedIndex = 0;
            //Image backImage
            try
            {
                pictureBox1.BackgroundImage = Image.FromFile(Application.StartupPath + "\\PrimData\\河中部分.jpg");
                pictureBox1.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少河中部分.jpg,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox2.BackgroundImage = Image.FromFile(Application.StartupPath + "\\PrimData\\老式栈桥.jpg");
                pictureBox2.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少老式栈桥.jpg,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox4.BackgroundImage = Image.FromFile(Application.StartupPath + "\\PrimData\\新式栈桥.jpg");
                pictureBox4.BackgroundImageLayout = ImageLayout.Stretch;
            }
            catch (Exception ex)
            {
                MessageBox.Show("缺少新式栈桥.jpg,请重新添加，下次启动时生效。");

            }
            try
            {
                pictureBox5.BackgroundImage = Image.FromFile(Application.StartupPath + "\\PrimData\\丙式渡轮.jpg");
                pictureBox5.BackgroundImageLayout = ImageLayout.Stretch;
            }
 
            catch (Exception ex)
            {
                MessageBox.Show("缺少丙式渡轮.jpg,请重新添加，下次启动时生效。");

            }   



        }

        private void button2_Click_1(object sender, EventArgs e)
        {
            string destFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\"  + "1号升降墩1座数量表.doc";
            string srcFileName = Application.StartupPath + "\\PrimData" + "\\1号升降塔1座.doc";
            try
            {
                File.Copy(srcFileName,
                   destFileName,
                    true);

                MessageBox.Show("1号升降墩1座数量表生成成功，已保存至桌面");

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void button4_Click(object sender, EventArgs e)
        {
            string destFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + "1、2号升降墩各1座数量表.doc";
            string srcFileName = Application.StartupPath + "\\PrimData" + "\\1号和2号升降塔各1座.doc";
            try
            {
                File.Copy(srcFileName,
                   destFileName,
                    true);

                MessageBox.Show("1、2号升降墩各1座数量表生成成功，已保存至桌面");

            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void btnLayout2_Paint(object sender, PaintEventArgs e)
        {

        }

        private void btnLaoShi_Click(object sender, EventArgs e)
        {
            int selectFlag = this.cbbLaoShiZhanQiao.SelectedIndex;
            try
            {
                string destFileName, srcFileName;
                switch (selectFlag)
                {
                    case 0:
                        destFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + "1座升降塔数量表.doc";
                        srcFileName = Application.StartupPath + "\\PrimData" + "\\1座升降塔.doc";
                        File.Copy(srcFileName, destFileName, true);
                        MessageBox.Show("1座升降塔数量表生成成功，已保存至桌面");
                        break;
                    case 1:
                        destFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + "1座升降塔和一座A型活动墩数量表.doc";
                        srcFileName = Application.StartupPath + "\\PrimData" + "\\1座升降塔和一座A型活动墩.doc";
                        File.Copy(srcFileName, destFileName, true);
                        MessageBox.Show("1座升降塔和一座A型活动墩数量表生成成功，已保存至桌面");
                        break;

                    case 2:
                        destFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + "1座升降塔和两座A型活动墩数量表.doc";
                        srcFileName = Application.StartupPath + "\\PrimData" + "\\1座升降塔和两座A型活动墩.doc";
                        File.Copy(srcFileName, destFileName, true);
                        MessageBox.Show("1座升降塔和两座A型活动墩数量表生成成功，已保存至桌面");
                        break;
                    case 3:
                        destFileName = Environment.GetFolderPath(Environment.SpecialFolder.Desktop) + "\\" + "1座升降塔和三座A型活动墩数量表.doc";
                        srcFileName = Application.StartupPath + "\\PrimData" + "\\1座升降塔和三座A型活动墩.doc";
                        File.Copy(srcFileName, destFileName, true);
                        MessageBox.Show("1座升降塔和三座A型活动墩数量表生成成功，已保存至桌面");
                        break;
                    default:
                        break;
                }

            }
            catch(Exception ex)
            {
                MessageBox.Show(ex.ToString());
            }
        }

        private void tabControl1_Selected(object sender, TabControlEventArgs e)
        {
            int i = this.tabControl1.SelectedIndex;
            switch(i){
                case 0: MainWindow.ActiveForm.Text = "河中部分器材数量表";
                    break;
                case 1: MainWindow.ActiveForm.Text = "一岸栈桥器材数量表";
                    break;
                case 2: MainWindow.ActiveForm.Text = "新式栈桥器材数量表";
                    break;
                case 3: MainWindow.ActiveForm.Text = "丙式轮渡器材数量表";
                    break;
                default:
                    MainWindow.ActiveForm.Text = "方案生成";
                    break;
            }

        }

        private void textBox1_Click(object sender, EventArgs e)
        {
            textBox1.Text = null;
            
        }

        private void tbLunduNum_Click(object sender, EventArgs e)
        {
            tbLunduNum.Text = null;
        }

        private void textBox1_TextChanged(object sender, EventArgs e)
        {

        }

        private void btnLayout1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void tableLayoutPanel1_Paint(object sender, PaintEventArgs e)
        {

        }

        private void button5_Click(object sender, EventArgs e)
        {

        }
    }

        



    }

