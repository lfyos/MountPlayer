; �ű��� Inno Setup �ű��� ���ɣ�
; �йش��� Inno Setup �ű��ļ�����ϸ��������İ����ĵ���

#define MyAppName "��·��������ģ��ƴװϵͳ"
#define MyAppVersion "2011"
#define MyAppPublisher "�Ϻ���ѧ"
#define MyAppURL "http://www.shu.edu.cn"

[Setup]
; ע: AppId��ֵΪ������ʶ��Ӧ�ó���
; ��ҪΪ������װ����ʹ����ͬ��AppIdֵ��
; (�����µ�GUID����� ����|��IDE������GUID��)
AppId={{7C8AAD7C-CCEA-4D18-9656-C65E2559BD85}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName=C:\Program Files\�Ϻ���ѧ\��·��������ģ��ƴװϵͳ2011��
DefaultGroupName=��·��������ģ��ƴװϵͳ2011��
OutputDir=C:\Documents and Settings\Administrator\����
OutputBaseFilename=ƴװ����
SetupIconFile=H:\Shader_player\Shader_player\res\mount_player.ico
Compression=lzma
SolidCompression=yes

[Languages]
Name: "chinesesimp"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "H:\Shader_player\Debug\*"; DestDir: "{app}"; Flags: ignoreversion recursesubdirs createallsubdirs
; ע��: ��Ҫ���κι���ϵͳ�ļ���ʹ�á�Flags: ignoreversion��

