English | [简体中文](https://github.com/ZingLix/LixTalk-client/blob/master/Readme.zh.md)

# LixTalk-client

![](https://travis-ci.com/ZingLix/LixTalk-client.svg?branch=master)

LixTalk is a high-performance secure IM system.

This repo is the source code of client which is cross-platform using Qt.

Still under developing.

[Server ->](https://github.com/ZingLix/LixTalk-server)

## Installation

LixTalk client supports Linux and Windows thanks to Qt. Therefore, you should install Qt first whatever platform you're using.

Everything below assumes that you have installed Qt and the environment is ready. Using QtCreator to build is feasible on both platforms, so it won't be listed below.

### Linux

The process of building is quite easy using Qmake.

```
qmake
make
```

And you'll get the client in the folder "build". Just run it.

```
cd build
./LixTalk-client
```

### Windows

There're several ways to build under Windows.

#### Visual Studio

The client is developed with Visual Studio 2017, so you can see a .sln file in the root directory. You can open it with VS2017 and run it like other normal c++ project. But in order to make VS able to us Qt, you may need [Qt VS Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools-19123).

> Low vesion VS may encounter problems when open it.

#### Command Line

Using command line to build LixTalk is also feasible. Open cmd/powershell if you have set nmake and cl related environment or VS native tools prompt which will help you set all dev environment, enter the project directory and run the commands below.

> other compilers may work as well (have not tested).

```
qmake
nmake
```

The exe file will appear in the folder 'build' if no errors occur.