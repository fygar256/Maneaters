# robots
The Raging Robots.

Cのゲームの習作です。

linuxのコンソールで動きます。ncursesを使っています。

ncursesがないと動かないので、

sudo apt-get install libncurses5-dev

として、インストールして下さい。

コンパイルは、Makefile があるので、make して下さい。

それか、cc robots.c -o robots -lncurses　と、コンパイルできます。

