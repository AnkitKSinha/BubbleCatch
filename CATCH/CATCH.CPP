#include<graphics.h>
#include<time.h>
#include<math.h>
#include<conio.h>
#include<stdlib.h>
#include<dos.h>
#include<fstream.h>

union REGS in,out;

inline void INIT(int &maxx,int &maxy)
{
        int gdriver=DETECT,gmode;
        initgraph(&gdriver,&gmode,"c:\\turboc3\\bgi");
        maxx=getmaxx();
        maxy=getmaxy();

}
void callmouse();
void altermouse(int &,int &,int &);
void hidemouse();
void setlimits(int,int,int,int);
void outline(int,int);
void endgame(int,int,int);
void button(char *,int,int);
void readscore(int);
void displayscores(int,int);
void menu(int,int);
void game(int,int);

class score
{
        char name[20];
        int s;
        public:
               void setscore(int);
               void display(int,int);
               int retscore(){return s;}
};
void score::setscore(int x)
{
        cleardevice();
        settextstyle(DEFAULT_FONT,0,2);
        setcolor(BLUE);
        outtextxy(100,200,"ENTER NAME:");
        char ch;
        settextjustify(CENTER_TEXT,CENTER_TEXT);
        int i=0;
        name[0]='\0';
        while(1)
        {
                setcolor(RED);
                outtextxy(400,200,name);
                ch=getch();
                setcolor(BLACK);
                outtextxy(400,200,name);
                if(ch!=13)
                {
                        if(ch==8&&i!=0)
                        {
                                name[i-1]='\0';
                                i--;
                        }
                        else if(ch!=8)
                        {
                                name[i++]=ch;
                                name[i]='\0';
                        }
                }
                else
                {
                        break;
                }
        }
        s=x;
}
void score::display(int x,int y)
{
        settextstyle(DEFAULT_FONT,0,3);
        setcolor(BLUE);
        outtextxy(x,y,name);
        char ch[10];
        setcolor(RED);
        itoa(s,ch,10);
        outtextxy(x+200,y,ch);
}
void main()
{
        int mx,my;
        INIT(mx,my);
        menu(mx,my);


}
void callmouse()
{
        in.x.ax=1;
        int86(51,&in,&out);
}
void altermouse(int &x,int &y,int &click)
{
        in.x.ax=3;
        int86(51,&in,&out);
        x=out.x.cx;
        y=out.x.dx;
        click=out.x.bx;
}
void setlimits(int minx,int maxx,int miny,int maxy)
{
        in.x.ax=7;
        in.x.cx=minx;
        in.x.dx=maxx;
        int86(51,&in,&out);
        in.x.ax=8;
        in.x.cx=miny;
        in.x.dx=maxy;
        int86(51,&in,&out);
}
void hidemouse()
{
        in.x.ax=2;
        int86(51,&in,&out);
}
void outline(int maxx,int maxy)
{
        setcolor(WHITE);
        setlinestyle(SOLID_LINE,1,3);
        rectangle(1,1,maxx,maxy);
        rectangle(20,20,maxx-20,maxy-20);
        setfillstyle(SOLID_FILL,BLUE);
        floodfill(5,5,WHITE);
        setlinestyle(SOLID_LINE,1,1);

}
void endgame(int points,int mx,int my)
{
        char p[20];
        outline(mx,my);
        itoa(points,p,10);
        setcolor(BLUE);
        settextstyle(DEFAULT_FONT,0,3);
        outtextxy(200,100,"POINTS:");
        outtextxy(400,100,p);
        button("EXIT",300,300);
}
void button(char *data,int x,int y)
{
        setcolor(BLUE);
        int msx,msy,mscl=0;
        setlinestyle(SOLID_LINE,0,3);
        rectangle(x-60,y-20,x+60,y+20);
        setfillstyle(SOLID_FILL,LIGHTGRAY);
        floodfill(x,y,BLUE);
        settextstyle(DEFAULT_FONT,0,2);
        outtextxy(x-18,y-8,data);
        for(;;)
        {
                altermouse(msx,msy,mscl);
                if(mscl==1)
                {
                        if(msx<=x+60 && msx>=x-60 && msy>=y-20 && msy<=y+20)
                        {
                                setcolor(BLACK);
                                rectangle(x-60,y-20,x+60,y+20);
                                setfillstyle(SOLID_FILL,BLACK);
                                floodfill(x,y,BLACK);
                                setcolor(BLUE);
                                rectangle(x-58,y-18,x+58,y+18);
                                setfillstyle(SOLID_FILL,LIGHTGRAY);
                                floodfill(x,y,BLUE);
                                outtextxy(x-18,y-8,data);
                                delay(500);
                                break;
                        }
                }
        }
}
void readscore(int x)
{
        score sc;
        fstream file("scores2.dat",ios::in);
        if(!file)
        {
                 sc.setscore(x);
                 cleardevice();
                 ofstream file1("scores2.dat");
                 file1.write((char *)&sc,sizeof(sc));
                 file1.close();
        }
        else
        {
                file.read((char *)&sc,sizeof(sc));
                file.close();
                if(sc.retscore()<x)
                {
                        sc.setscore(x);
                        ofstream error("scores2.dat");
                        if((error.write((char *)&sc,sizeof(sc)))==0)
                        {
                                cout<<"\nERROR ERROR";
                                getch();
                        }
                        error.close();
                }

        }
        cleardevice();
        file.close();
}
void displayscores(int mx,int my)
{
        cleardevice();
        score sc;
        int y=200;
        outline(mx,my);
        setcolor(GREEN);
        settextstyle(DEFAULT_FONT,0,3);
        outtextxy((mx/2)-150,50,"HIGH SCORES");
        setlinestyle(SOLID_LINE,1,3);
        setcolor(MAGENTA);
        line((mx/2)-150,80,(mx/2)+100,80);
        setlinestyle(SOLID_LINE,1,1);
        ifstream file("scores2.dat");
        if(!file)
        {
                setcolor(RED);
                settextstyle(DEFAULT_FONT,0,3);
                outtextxy(100,250,"NO SCORE FOUND!");
                getch();
                cleardevice();
        }
        else
        {
                while(file.read((char *)&sc,sizeof(sc)) )
                {
                        sc.display(200,y);
                        y+=80;
                }
        }
        file.close();
        button("DONE",300,300);
        cleardevice();
        menu(mx,my);

}
void menu(int mx,int my)
{
        outline(mx,my);
        int x,y,cl;
        settextstyle(DEFAULT_FONT,0,3);
        setcolor(BLUE);
        callmouse();

        outtextxy((mx/2)-80,50,"MENU");
        outtextxy((mx/2)-220,150,"1.PLAY");
        outtextxy((mx/2)-220,250,"2.HIGH SCORES");
        outtextxy((mx/2)-220,350,"3.EXIT");
        char ch;
        while(1)
        {
                altermouse(x,y,cl);
                if(cl==1)
                {
                if(x>(mx/2)-220 && x<(mx/2)-80 && y>150 && y<200)
                {
                        game(mx,my);
                }
                else if(x>(mx/2)-220 && x<(mx/2)+90 && y>250 && y<300)
                {
                        displayscores(mx,my);
                }
                else if(x>(mx/2)-220 && x<(mx/2)-80 && y>350 && y<400)
                {
                        hidemouse();
                        closegraph();
                        exit(0);
                }
                }


        }
}

void game(int mx,int my)
{
        cleardevice();
        int x,y,cl=0,i,j,ctr=0,points=0,lives=3,dt=100;
        int *ycoord,*bomby;
        int delaytime[20];
        int randomcolor;

        int *xcoord,*bombx,numctr=0,playtime=0;
        int nob=0,bombctr=0,temp,selbomb;

        callmouse();
        setlimits(23,mx-23,23,my-23);
        delay(100);

        // ASSIGN THE SPEED
        for(i=0;i<10;i++){delaytime[i]=i+10;}

        // ALOCATE MEMORY FOR COORDINATES

        if((ycoord=(int *)malloc(10))==NULL||(xcoord=(int *)malloc(10))==NULL||(bomby=(int *)malloc(10))==NULL||(bombx=(int *)malloc(10))==NULL)
        {
                cout<<"\nMEMORY ALLOCATION ERROR!";
                getch();
                exit(0);

        }

        randomize();
        outline(mx,my);

//THE GAME BEGINS

        int start=clock(),end;
        for(i=0;i<lives;i++)
        {
                setcolor(WHITE);
                circle((mx-200)+(i*20),my-10,6);
                setfillstyle(SOLID_FILL,RED);
                floodfill((mx-200)+(i*20),my-10,WHITE);
        }

        for(;;)
        {



        //      ASSIGN COORDINATES TO BOMBS
                selbomb=random(10);
                if( (selbomb%7)==0 && nob<=3)
                {
                        *(bombx+bombctr)=50;
                        *(bomby+bombctr)=random(my-100)+50;
                        bombctr++;
                        nob++;
                }


        //      ASSIGN COORDINATES TO BUBLES
                if(numctr<3)
                {
                        *(xcoord+ctr)=48;
                        *(ycoord+ctr)=random(my-100)+48;
                        numctr++;
                        ctr++;
                }
        //      DROP THE BOMBS
                for(i=0;i<nob;i++)
                {
                        setcolor(WHITE);
                        circle(*(bombx+i),*(bomby+i),22);
                }

        //      CREATE BUBLES
                for(i=0;i<numctr;i++)
                {
                        randomcolor=random(5)+1;
                        setcolor(randomcolor);
                        circle(*(xcoord+i),*(ycoord+i),20);

                }
       //       MOUSE
                altermouse(x,y,cl);
                delay(dt);

       //       CHECK WHAT HAPPENED WITH BUBLES
                temp=numctr;
                for(i=0;i<temp;i++)
                {
                //      CHECK IF BUBLES WERE HIT
                        if(x < *(xcoord+i)+20 && x > *(xcoord+i)-20 && y< *(ycoord+i)+20 && y>*(ycoord+i)-20)
                        {
                                points+=10;
                                numctr--;
                                ctr=i;
                                cleardevice();
                                outline(mx,my);
                                for(j=0;j<lives;j++)
                                {
                                        setcolor(WHITE);
                                        circle((mx-200)+(j*20),my-10,6);
                                        setfillstyle(SOLID_FILL,RED);
                                        floodfill((mx-200)+(j*20),my-10,WHITE);
                                }

                        }
                //      DE-CREATE THE BUBLES
                        setcolor(BLACK);
                        circle(*(xcoord+i),*(ycoord+i),20);

                //      LET THE BUBLES MOVE
                        *(xcoord+i)+=delaytime[i];

                //      CHECK FOR PLAYER'S DEATH
                        if(xcoord[i]>=mx-48)
                        {
                                numctr--;
                                lives--;

                                if(lives==0){goto again;}
                                for(j=0;j<lives+1;j++)
                                {
                                        setcolor(BLUE);
                                        circle((mx-200)+(i*20),my-10,6);
                                        setfillstyle(SOLID_FILL,BLUE);
                                        floodfill((mx-200)+(j*20),my-10,BLUE);
                                }
                                for(j=0;j<lives;j++)
                                {
                                        setcolor(WHITE);
                                        circle((mx-200)+(j*20),my-10,6);
                                        setfillstyle(SOLID_FILL,RED);
                                        floodfill((mx-200)+(j*20),my-10,WHITE);
                                }
                                ctr=i;
                        }


                }

       //       CHECK WHAT HAPPENED WITH THE BOMB
                temp=nob;
                for(i=0;i<temp;i++)
                {
                //      CHECK FOR PLAYERS DEATH AND THE END OF GAME
                        if(x < *(bombx+i)+22 && x > *(bombx+i)-22 && y< *(bomby+i)+22 && y>*(bomby+i)-22)
                        {
                                goto again;

                        }
                //      DE-CREATE THE BOMBS
                        setcolor(BLACK);
                        circle(*(bombx+i),*(bomby+i),22);
                //      LET THE BOMBS MOVE
                        *(bombx+i)+=delaytime[i];
                //      CHECK FOR THE BOMBS DEATH
                        if(*(bombx+i)>=mx-45)
                        {
                                bombctr=i;
                                nob--;
                                *(bombx+bombctr)=48;
                                *(bomby+bombctr)=random(my-100)+50;
                                nob++;
                        }

                }
                end=clock();
                playtime=ceill((end-start)/CLK_TCK);
                if(playtime>10)
                {
                        dt-=10;
                        start=clock();
                }

        }
//      THIS IS THE END :

        again:
        cleardevice();
        outline(mx,my);
        endgame(points,mx,my);
        readscore(points);
        hidemouse();
        delete ycoord,xcoord;
        menu(mx,my);
}