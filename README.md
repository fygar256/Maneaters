# robots
The Raging Robots.

Cのcursesのゲームの習作です。

linuxのコンソールで動きます。ncurseswを使っています。

ncurseswがないと動かないので、

sudo apt-get install libncursesw5-dev

として、インストールして下さい。

コンパイルは、Makefile があるので、make して下さい。

それか、cc robots.c -o robots -lncursesw　と、コンパイルできます。

Instruction of Raging Robots Ver 0.8

Mission : survive from raging robots.

＃ --- Rock , which robot dies if robots touch.

＠ --- Raging Robot , which chases you step by step.

Ｏ --- You , control and run away from the robots.

<a href="https://imgur.com/oaD71xN"><img src="https://i.imgur.com/oaD71xN.png" title="source: imgur.com" /></a>
