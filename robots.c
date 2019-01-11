#include	<stdio.h>
#include	<stdlib.h>
#include	<ncurses.h>
#include	<time.h>
#include	<locale.h>

#define	ENEMY		"＠"
#define	PLAYER	"Ｏ"
#define	ROCK		"＃"
#define	CRASH		"Ｘ"
#define	SPACE		"　"
#define rnd(x) (rand()%x)

static	int	enemyl[40];
static	int	enemyx[40];
static	int	enemyy[40];
static	int	rockl[320];
static	int	rockx[320];
static	int	rocky[320];
static	int	enemymax=12;
static	int	rockmax=120;
static	int	playerx,playery;
static	int	h,w;
static	int	xsize,ysize;
static	int	ecount;
static	int	gamestat;

int	check_player_crash() {
	for(int i=0;i<enemymax;i++) {
		if((enemyl[i]==1)&&(playerx==enemyx[i]) && (playery==enemyy[i]))
			return(1);
		}
	for(int i=0;i<rockmax;i++) {
		if((rockl[i]==1)&&(playerx==rockx[i]) && (playery==rocky[i]))
			return(1);
		}
	return(0);
}

int	putchara(int x,int y,char *str){
	move(y,x*2);
	printw("%s",str);
}

int	init_game() {
	clear();
	gamestat=0;
	srand((unsigned)time(NULL));
	for(int i=0;i<rockmax;i++) {
		putchara(rockx[i]=rnd(xsize),rocky[i]=rnd(ysize),ROCK);
		rockl[i]=1;
		}
	for(int i=0;i<enemymax;i++) {
		putchara(enemyx[i]=rnd(xsize),enemyy[i]=rnd(ysize),ENEMY);
		enemyl[i]=1;
		}
	putchara(playerx=xsize/2+1,playery=ysize/2+1,PLAYER);
}

int	robot_crash_check(int x,int y) {
	for(int i=0;i<enemymax;i++) {
		if (x==enemyx[i] && y==enemyy[i])
			return(1);
		}
	for(int i=0;i<rockmax;i++) {
		if (x==rockx[i] && y==rocky[i])
			return(1);
		}
	return(0);
}


void	move_robots() {
	int	dx,dy,f,x,y;
	for(int i=0;i<enemymax;i++) {
		if (enemyl[i]!=0) {
			dx=0,dy=0;
			if (enemyx[i]<playerx) dx=1;
			if (enemyx[i]>playerx) dx=-1;
			if (enemyy[i]<playery) dy=1;
			if (enemyy[i]>playery) dy=-1;
			putchara(enemyx[i],enemyy[i],SPACE);
			x=enemyx[i]+dx,y=enemyy[i]+dy;

			for(int j=0;j<enemymax;j++) {
				if ((i!=j)&&(enemyl[j]==1) && (x==enemyx[j]) && (y==enemyy[j])) {
					putchara(x,y,SPACE);
					enemyl[i]=0;
					}
				}
			for(int j=0;j<rockmax;j++) {
				if ((rockl[j]==1)&&(x==rockx[j]) && (y==rocky[j])) {
					rockl[j]=enemyl[i]=0;
					putchara(x,y,SPACE);
					}
				}
			f=0;
			for(int j=0;j<enemymax;j++) {
				f|=enemyl[j];
				}
			if (f==0) {
				gamestat=3;
				return;
				}
			enemyx[i]=x;
			enemyy[i]=y;
			if (enemyl[i]==1) {
				if ((x==playerx) && (y==playery)) gamestat=1;
				putchara(enemyx[i],enemyy[i],ENEMY);
				}
			}
		}
}

int	main_game() {
	int	c,dx,dy;
	ecount=enemymax;
	while(1) {
	move(0,0);
	c=getch();
	switch(c) {
		case	'q':
			return(-1);
		case	'7': dx=-1,dy=-1; break;
		case	'8': dx= 0,dy=-1; break;
		case	'9': dx= 1,dy=-1; break;
		case	'u':
		case	'4': dx=-1,dy=0; break;
		case	'i':
		case	'5': dx= 0,dy=0; break;
		case	'o':
		case	'6': dx= 1,dy=0; break;
		case	'j':
		case	'1': dx=-1,dy=1; break;
		case	'k':
		case	'2': dx= 0,dy=1; break;
		case	'l':
		case	'3': dx= 1,dy=1; break;
		default:
			continue;
		}
		if ((playerx+dx)>=0 && (playerx+dx)<=(xsize-1) &&
				(playery+dy)>=0 && (playery+dy)<=(ysize-1)) {
			putchara(playerx,playery,SPACE);
			playerx+=dx;
			playery+=dy;
			if (check_player_crash()==0)
							putchara(playerx,playery,PLAYER);
			else	{
							gamestat=1;
							return(0);
						}
			}
		move_robots();
		if (gamestat!=0) return(0);
	}
}
		
int	tryagainp() {
	printw("Try Again? [y/n]");
	while(1) {
		switch(getch()) {
			case	'n':
			case	'q':
				return(0);
			case	'y':
				return(1);
			default:
				continue;
			}
		}
}

int	play_game() {
	int	f;
	while(1) {
		init_game();
		f=main_game();
		if (gamestat==1) {
			putchara(playerx,playery,CRASH);
			move(0,0);
			printw("You Lose ");
			if (tryagainp()==0) return(0);
			}
		if (gamestat==3) {
			move(0,0);
			printw("You Win!! ");
			if (tryagainp()==0) return(0);
			}
		
		if (f==-1) {
			return(0);
			}
		}
}

void	instruction() {
		clear();
		printw("Raging Robots Ver 0.8\n");
		printw("Mission : survive from raging robots.\n");
		printw("%s --- Rock , which robot dies if robots touch.\n",ROCK);
		printw("%s --- Raging Robot , which chases you step by step\n",ENEMY);
		printw("%s --- You , control and run away from the robots.\n",PLAYER);
		printw("             if you touch robots and rocks then you die.\n");
		printw("Key Control: 7   8   9    tenkey: 7  8  9\n\n");
		printw("             u   i   o            4  5  6\n\n");
		printw("             j   k   l            1  2  3\n\n");
		printw("             these keys move you to each direction.\n");
		printw("             'i' and '5' take no move.\n");
		printw("             and 'q' key to quit.\n");
		printw("             Good Luck\n");
		printw("Hit a key to start game\n");
		getch();
	}

int	main() {
	setlocale( LC_ALL,"" );
	initscr();
	noecho();
	getmaxyx(stdscr,h,w);
	xsize=w/2;
	ysize=h;
	instruction();
	play_game();
	endwin();
	exit(0);
}

