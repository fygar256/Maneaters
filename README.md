# robots
The Raging Robots.

Cのゲームの習作です。

linuxのコンソールで動きます。ncursesを使っています。

ncursesがないと動かないので、

sudo apt-get install libncurses5-dev

として、インストールして下さい。

コンパイルは、Makefile があるので、make して下さい。

それか、cc robots.c -o robots -lncurses　と、コンパイルできます。

# Instruction of Raging Robots Ver 0.8

Mission : survive from raging robots.

\## --- Rock , which robot dies if robots touch.

<> --- Raging Robot , which chases you step by step.

() --- You , control and run away from the robots.

             if you touch robots and rocks then you die.
             
Key Control: 7   8   9    tenkey: 7  8  9

             u   i   o            4  5  6

             j   k   l            1  2  3
             
             
these keys move you to each direction. 'i' and '5' take no move.

and 'q' key to quit.
