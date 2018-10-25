[English](https://github.com/ZingLix/LixTalk-client/blob/master/Readme.md) | 简体中文

# LixTalk-client

[![Build Status](https://travis-ci.com/ZingLix/LixTalk-client.svg?branch=master)](https://travis-ci.com/ZingLix/LixTalk-client)

LixTalk 是一个高效、安全的即时通讯系统。

这个 repo 是 LixTalk 的客户端，利用 Qt 开发，支持多平台。

仍在开发中。。。

[Server ->](https://github.com/ZingLix/LixTalk-server)

## 安装

LixTalk 客户端用 Qt 开发，所以支持 Linux 和 Windows。因此如果你想构建 LixTalk 客户端，无论哪个平台你都需要先安装 Qt。

下面所有教程都假设你已经成功安装 Qt 并已经设置好所有的环境变量。在所有平台上都可以利用 QtCreator 直接构建项目，所以下面不赘述。 

### Linux

利用 Qmake 整个构建过程十分简单。

```
qmake
make
```

然后客户端就会生成在 build 文件夹中，可以直接运行他。

```
cd build
./LixTalk-client
```

### Windows

在 Windows 下有多种方式构建项目。

#### Visual Studio

客户端是在 Visual Studio 2017 下开发的，所以你能在根目录看到一个 sln 文件。像其他 VS 的项目一样，可以通过它直接打开然后按下 F5 编译并运行。不过为了能够让 VS 使用 Qt，你可能需要 [Qt VS Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123)。

> 低版本的 VS 可能会在打开项目时遇到问题。

#### Command Line

用命令行同样可以构建 LixTalk 客户端。如果你已经设置好 nmake 和 cl 相关的环境变量，可以打开 CMD/PowerShell，或者用 VS 自带的 VS native tools prompt 进入项目目录并执行下面的指令。 

> 其他编译器可能也可以（未测试）。

```
qmake
nmake
```

如果没有发生错误，那么可执行文件就会在 build 文件夹中。