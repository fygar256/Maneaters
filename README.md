# robots
The Raging Robots.

Cのcursesのゲームの習作です。

linuxのコンソールで動きます。ncursesを使っています。

ncursesがないと動かないので、

sudo apt-get install libncurses5-dev

として、インストールして下さい。

コンパイルは、Makefile があるので、make して下さい。

それか、cc robots.c -o robots -lncurses　と、コンパイルできます。

Instruction of Raging Robots Ver 0.8

Mission : survive from raging robots.

\## --- Rock , which robot dies if robots touch.

<> --- Raging Robot , which chases you step by step.

() --- You , control and run away from the robots.

<a href="https://imgur.com/oaD71xN"><img src="https://i.imgur.com/oaD71xN.png" title="source: imgur.com" /></a>
