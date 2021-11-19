# Tetris 俄罗斯方块
A C++ Tetris for Windows  
For those who cannot compile C++ files, see our [release files](https://github.com/Czile-create/Tetris/releases)

一个为Windows系统写的C++俄罗斯方块  
对于那些未安装C++编译器的用户而言，参阅我们的[发行文件](https://github.com/Czile-create/Tetris/releases)

## Pics 部分图片

> Pics follows may NOT screenshots of the latest version.  
> 下列图片可能不是最新版本

![Cover](https://github.com/Czile-create/Tetris/tree/master/TetrisPics/6.png)  
![Game](https://github.com/Czile-create/Tetris/tree/master/TetrisPics/4.png)  
![Game Over](https://github.com/Czile-create/Tetris/tree/master/TetrisPics/5.png)  

## How to use? 使用方法
**重要：在游戏反应页面中键入 `start` 即可开始游戏。**

Here are commands you can use （全部命令如下）:
|名称|描述（英文）|描述（中文）|
|-|-|-|
|start|To start the game|开始游戏|
|showSetting|To show Setting|显示当前设置值|
|changeSetting|To change Setting|更改设置|
|honor|To show your honor|查看荣誉|
|about|To show the info of author|查看开发者信息|
|exit|To exit the game|退出游戏|

**控制方块的方法**  How to control the cube?  
|键位|描述（英文）|描述（中文）|
|-|-|-|
|`←`|Move the cube to the left|向左移动一格|
|`→`|Move the cube to the right|向右移动一格|
|`↓`|Move the cube to the down|软着陆（快速下降）|
|`space`|Transform the cube|旋转方块|
|`q`|Quit the game|结束本次游戏并显示分数页面|
|`/`|Draw the cube to the down|硬着陆（快速使方块掉落到最底端)|

## Compile 编译

Go to the directory of the 'Tetris.cpp', and then enter: 
```
g++ Tetris.cpp -std=c++11 -o Tetris.exe
```

## License 许可证
See the file License.
