using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace schemeGenerate
{
    class killAllWinword
    {
        public void killAllProcess() // 杀掉所有winword.exe进程
        {
            System.Diagnostics.Process[] myPs;
            myPs = System.Diagnostics.Process.GetProcesses();
            foreach (System.Diagnostics.Process p in myPs)
            {
                if (p.Id != 0)
                {
                    string myS = "WINWORD.EXE" + p.ProcessName + "  ID:" + p.Id.ToString();
                    try
                    {
                        if (p.Modules != null)
                            if (p.Modules.Count > 0)
                            {
                                System.Diagnostics.ProcessModule pm = p.Modules[0];
                                myS += "\n Modules[0].FileName:" + pm.FileName;
                                myS += "\n Modules[0].ModuleName:" + pm.ModuleName;
                                myS += "\n Modules[0].FileVersionInfo:\n" + pm.FileVersionInfo.ToString();
                                if (pm.ModuleName.ToLower() == "winword.exe")
                                    p.Kill();
                            }
                    }
                    catch
                    {

                    }
                    finally
                    {
                        ;
                    }
                }
            }
        }

    }
}
