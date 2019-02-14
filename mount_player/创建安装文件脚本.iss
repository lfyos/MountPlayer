; 脚本由 Inno Setup 脚本向导 生成！
; 有关创建 Inno Setup 脚本文件的详细资料请查阅帮助文档！

#define MyAppName "铁路桥梁抢修模拟拼装系统"
#define MyAppVersion "2011"
#define MyAppPublisher "上海大学"
#define MyAppURL "http://www.shu.edu.cn"

[Setup]
; 注: AppId的值为单独标识该应用程序。
; 不要为其他安装程序使用相同的AppId值。
; (生成新的GUID，点击 工具|在IDE中生成GUID。)
AppId={{7C8AAD7C-CCEA-4D18-9656-C65E2559BD85}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:\Program Files\上海大学\铁路桥梁抢修模拟拼装系统2011版
DefaultGroupName=铁路桥梁抢修模拟拼装系统2011版
OutputDir=C:\Documents and Settings\Administrator\桌面
OutputBaseFilename=拼装内容
SetupIconFile=H:\Shader_player\Shader_player\res\mount_player.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "H:\Shader_player\Debug\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; 注意: 不要在任何共享系统文件上使用“Flags: ignoreversion”

