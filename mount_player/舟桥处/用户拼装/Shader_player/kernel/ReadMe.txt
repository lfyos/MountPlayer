1 在开发环境的辅助include中，添加OpenCV的内容：
		C:\Program Files\OpenCV\cxcore\include ; C:\Program Files\OpenCV\cv\include ; C:\Program Files\OpenCV\cvaux\include ; C:\Program Files\OpenCV\ml\include ; C:\Program Files\OpenCV\otherlibs\highgui


2 在开发环境的辅助link中，添加opengl和OpenCV的内容：
		.\water\gl;C:\Program Files\OpenCV\lib

3 glew的动态链接库glew32.dll拷贝到系统目录，或者和生成的可执行文件放在一个目录

