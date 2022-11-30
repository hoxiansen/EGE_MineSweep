# EGE_MineSweep
一个用[EGE](https://xege.org/)实现的简易扫雷游戏

## Windows下使用VSCode编译步骤

### 环境准备
1. 进入[mingw64下载页](https://sourceforge.net/projects/mingw-w64/files/mingw-w64/mingw-w64-release/)，下载*x86_64-win32-seh*并解压，然后把/path/to/mingw64/bin目录添加到系统变量path中
2. 进入[ege下载页](https://xege.org/install_and_config)下载*ege20.08_all*，解压后把/ege/include/\*\*和/ege/lib/mingw64/\*\*分别复制到/path/to/mingw64/**x86_64-w64-mingw32**/下的include和lib目录下面

> 注意：一定是mingw64/x86_64-w64-mingw32目录下面的lib和include，而**不是**mingw64下面的lib和include

### VSCode配置
1. 使用VSCode打开文件夹，安装C/C++扩展
2. 按F1（或者Ctrl+Shift+P），输入`C/C++: Edit Configurations (UI)` 进入配置
   - 配置名称：win32
   - 编译器路径：/path/to/mingw64/bin/g++.exe （**这里注意默认是gcc.exe要改成g++.exe**）
   - 编译器参数：(来自[ege安装文档](https://xege.org/beginner-lesson-1.html))
   ```
   -lgraphics64
   -lgdiplus
   -luuid
   -lmsimg32
   -lgdi32
   -limm32
   -lole32
   -loleaut32
   -lwinmm
   ```
   - IntelliSense 模式：windows-gcc-x64
   - 包含路径：（添加复制了ege.h等文件的include路径）
   ```
   ${workspaceFolder}/**
   /path/to/mingw64/x86_64-w64-mingw32/include/**
   ```
   - C标准：c11
   - C++标准：c++14
   - 其他：默认就行
3. 按F1，输入`Tasks: Configure Default Build Task`，然后选择`C/C++: g++.exe 生成活动文件`，不需要改动
4. 点击Ctrl+Shift+B编译，正常的话就会生成.exe文件啦~
