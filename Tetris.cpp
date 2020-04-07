/*
    Copyright (c) Purelight.Chan Zee Lok.
    Latest Update: 2020 - 04 - 07
*/

#include <iostream>
#include <thread>
#include <mutex>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <Windows.h>
#include <math.h>
#include <conio.h>
#include <string>
#include <iomanip>

using namespace std;

class set;
class point;
class map;
class cube;

void shell();
void autodown(cube &);
void create(cube &);
void start();
void printfailed();
int main();

/*令光标停留在x,y*/
void gotoxy(HANDLE hOut1, int y, int x)
{
    COORD pos;
    pos.X = x+1;
    pos.Y = y+1;
    SetConsoleCursorPosition(hOut1, pos);
}
HANDLE hOut1 = GetStdHandle(STD_OUTPUT_HANDLE);

/*隐藏光标*/
void hide()
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 0};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);    //隐藏光标
}
void show()
{
    CONSOLE_CURSOR_INFO cursor_info = {1, 1};
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);    //显示光标
}

class set {
    public:
        set()
        {
            updateGap=200;
            LargestPoint=0;
            n=30;
            m=15;
            LastPoint=0;
        }
        set & operator=(const set & t)
        {
            updateGap=t.updateGap;
            LargestPoint=t.LargestPoint;
            n=t.n;
            m=t.m;
            LastPoint=t.LastPoint;
            return *this;
        }
        void readSetting();
        void changeSetting();
        void saveSetting();
        void showSetting();
        int culculateVerify();
        int updateGap, n, m, Verify;
        double LargestPoint, LastPoint;
        mutex mtx;
} Setting;

class point {
    public:
        int x,y;
};

class map {
    public:
        map();
        void init();
        bool v[100][100];
        void printinmap(cube *);
        void check(cube *);
        int n,m;
        bool live;
        mutex mtx;
        int score;
        double multi;
        double culculatescore();
} mapping;

class cube {
    public:
        cube()
        {
            srand(time(0)+mapping.score);
            do {
                x=2;
                y=rand()*rand()%Setting.m;
                type1=rand()%7+1;
                type2=rand()%6;
            } while (!cube(type1,type2,x,y).can());
            create();
        };
        cube(int i, int j, int x1, int y1): type1(i), type2(j), x(x1), y(y1)
        {
            create();
        }
        int type1,type2;    //形状, 方向
        /*
            1. * * *


            6. * * *


            3. * * * *

            4. * *
               * *

            5. * *
                 * *

            2.     *
               * * *

            7.   * *
               * *

        */
        int x,y;    //Center Position
        point p[4]; //point
        mutex mtx;
        void changePosition(int);   //1. left, 2. right, 3. down
        void changeShape();
        void create();
        bool can();
        void moveinscreen();
        void printinscreen();
        cube & operator=(const cube & t)
        {
            x=t.x,y=t.y;
            for (int i=0; i<4; i++)
                p[i].x=t.p[i].x,p[i].y=t.p[i].y;
            type1=t.type1;
            type2=t.type2;
            return *this;
        }
};

void set::readSetting()
{
    FILE * fp=fopen("Tetris.set","r");
    if (fp!=NULL) {
        fscanf(fp,"%d%d%d%lf%lf%d",&n,&m,&updateGap,&LargestPoint,&LastPoint,&Verify);
        if (culculateVerify()!=Verify)
            *this=set();
    }
    fclose(fp);
}
void set::showSetting()
{
    cout<<"Here are your settings:\n\tLine:\t\t"<<n<<"\n\tList:\t\t"<<m<<"\n\tUpdateGap:\t"<<updateGap<<"\n\n>";
}
void set::saveSetting()
{
    FILE * fp=fopen("Tetris.set","w+");
    fprintf(fp, "%d %d %d %lf %lf %d",n,m,updateGap,LargestPoint,LastPoint,culculateVerify());
    fclose(fp);
}

void set::changeSetting()
{
    string s="";
    int temp=0;
    while (s!="save") {
        cout<<"Please enter 'line', 'list', 'updateGap' or 'save' to continue...\n\n>";
        cin>>s;
        if (s=="line") {
            cout<<"Please enter an integer from 10 to 30.\n\n>";
            cin>>temp;
            if (temp<10||temp>30)
                cout<<"FIALED IN CHANGESETTING: the integer should in [10, 30]\n\n>";
            else
                n=temp;
        }
        else if (s=="list") {
            cout<<"Please enter an integer from 10 to 50.\n\n>";
            cin>>temp;
            if (temp<10||temp>50)
                cout<<"FIALED IN CHANGESETTING: the integer should in [10, 50]\n\n>";
            else
                m=temp;
        }
        else if (s=="updateGap") {
            cout<<"Please enter an integer from 20 to 400.\n\n>";
            cin>>temp;
            if (temp<20||temp>400)
                cout<<"FIALED IN CHANGESETTING: the integer should in [20, 400]\n\n>";
            else
                updateGap=temp;
        }
        else if (s=="save")
            cout<<"\n\n>";
        else
            cout<<"Command NOT Found! Please try again.\n";
        saveSetting();
    }
}

int set::culculateVerify()
{
    return ((n*m^updateGap)|(unsigned(LargestPoint)&unsigned(LastPoint)))%1127;
}
map::map() {}
void map::init()
{
    n=Setting.n+5;
    m=Setting.m;
    live=1;
    system("cls");
    for (int i=0; i<2*m+2; i++)
        cout<<"#";
    for (int i=0; i<n; i++) {
        gotoxy(hOut1,i,-1);
        cout<<"#";
        gotoxy(hOut1,i,2*m);
        cout<<"#";
    }
    gotoxy(hOut1,n,-1);
    for (int i=0; i<2*m+2; i++)
        cout<<"#";
    for (int i=0; i<100; i++)
        for (int j=0; j<100; j++)
            v[i][j]=0;
    score=0;
    live=1;
    multi=1.23;
}
void map::printinmap(cube * t)
{
    lock_guard<mutex> guard(mtx);
    for (int i=0; i<4; i++)
        v[t->p[i].x][t->p[i].y]=1;
    check(t);
}

void map::check(cube * t)
{
    score++;
    int sum;
    for (int q=0; q<4; q++) {
        sum=0;
        int i=t->p[q].x;
        for (int j=0; j<m; j++)
            sum+=v[i][j];
        if (sum==m) {
            multi*=1.23;
            for (int k=i; k>0; k--)
                for (int l=0; l<m; l++) {
                    v[k][l]=v[k-1][l];
                    gotoxy(hOut1,k,2*l);
                    if (v[k][l])
                        cout<<"\033[47m  \033[0m";
                    else
                        cout<<"\033[0m  ";
                }
            q=0;
        }
    }
    for (int i=0; i<4; i++)
        for (int j=0; j<m; j++)
            if (v[i][j])
                live=0;
}
double map::culculatescore()
{
    return
        score*multi*1000/Setting.n/Setting.m/Setting.updateGap;
}

void cube::changePosition(int t)
{
    lock_guard<mutex> guard(mtx);
    if (!mapping.live)
        return;
    if (t==1) {
        if (cube(type1,type2,x,y-1).can()) {
            moveinscreen();
            *this=cube(type1,type2,x,y-1);
            printinscreen();
        }
    }
    else if (t==2) {
        if (cube(type1,type2,x,y+1).can()) {
            moveinscreen();
            *this=cube(type1,type2,x,y+1);
            printinscreen();
        }
    }
    else if (t==3) {
        if (cube(type1,type2,x+1,y).can()) {
            moveinscreen();
            *this=cube(type1,type2,x+1,y);
            printinscreen();
        }
    }
    if (!cube(type1,type2,x+1,y).can()) {
        for (int i=0; i<4; i++) {
            gotoxy(hOut1,p[i].x,2*p[i].y);
            cout<<"\033[47m  \033[0m";
        }
        mapping.printinmap(this);
        if (mapping.live) {
            *this=cube();
            printinscreen();
        }
    }
}

void cube::changeShape()
{
    lock_guard<mutex> guard(mtx);
    if (cube(type1,type2+1,x,y).can()) {
        moveinscreen();
        *this=cube(type1,type2+1,x,y);
        printinscreen();
    }
}

void cube::create()
{
    if (type1==1) {
        type2%=4;
        p[0].x=x,p[0].y=y;
        if (type2==0) {
            p[1].x=x,p[1].y=y-1;
            p[2].x=x,p[2].y=y+1;
            p[3].x=x+1,p[3].y=y+1;
        }
        else if (type2==1) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x+1,p[3].y=y-1;
        }
        else if (type2==2) {
            p[1].x=x,p[1].y=y-1;
            p[2].x=x,p[2].y=y+1;
            p[3].x=x-1,p[3].y=y-1;
        }
        else if (type2==3) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x-1,p[3].y=y+1;
        }
    }
    else if (type1==2) {
        type2%=4;
        p[0].x=x,p[0].y=y;
        if (type2==0) {
            p[1].x=x,p[1].y=y-1;
            p[2].x=x,p[2].y=y+1;
            p[3].x=x-1,p[3].y=y+1;
        }
        else if (type2==1) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x+1,p[3].y=y+1;
        }
        else if (type2==2) {
            p[1].x=x,p[1].y=y-1;
            p[2].x=x,p[2].y=y+1;
            p[3].x=x+1,p[3].y=y-1;
        }
        else if (type2==3) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x-1,p[3].y=y-1;
        }
    }
    else if (type1==3) {
        type2%=2;
        p[0].x=x,p[0].y=y;
        if (type2==0) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x+2,p[3].y=y;
        }
        else if (type2==1) {
            p[1].x=x,p[1].y=y-1;
            p[2].x=x,p[2].y=y+1;
            p[3].x=x,p[3].y=y+2;
        }
    }
    else if (type1==4) {
        type2%=1;
        p[0].x=x,p[0].y=y;
        p[1].x=x,p[1].y=y+1;
        p[2].x=x+1,p[2].y=y;
        p[3].x=x+1,p[3].y=y+1;
    }
    else if (type1==5) {
        type2%=2;
        p[0].x=x,p[0].y=y;
        if (type2==0) {
            p[1].x=x,p[1].y=y-1;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x+1,p[3].y=y+1;
        }
        else if (type2==1) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x,p[2].y=y-1;
            p[3].x=x+1,p[3].y=y-1;
        }
    }
    else if (type1==6) {
        type2%=4;
        p[0].x=x,p[0].y=y;
        if (type2==0) {
            p[1].x=x,p[1].y=y-1;
            p[2].x=x,p[2].y=y+1;
            p[3].x=x-1,p[3].y=y;
        }
        else if (type2==1) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x,p[3].y=y+1;
        }
        else if (type2==2) {
            p[1].x=x,p[1].y=y-1;
            p[2].x=x,p[2].y=y+1;
            p[3].x=x+1,p[3].y=y;
        }
        else if (type2==3) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x,p[3].y=y-1;
        }
    }
    else if (type1==7) {
        type2%=2;
        p[0].x=x,p[0].y=y;
        if (type2==0) {
            p[1].x=x,p[1].y=y+1;
            p[2].x=x+1,p[2].y=y;
            p[3].x=x+1,p[3].y=y-1;
        }
        else if (type2==1) {
            p[1].x=x-1,p[1].y=y;
            p[2].x=x,p[2].y=y+1;
            p[3].x=x+1,p[3].y=y+1;
        }
    }
}

bool cube::can()
{
    for (int i=0; i<4; i++)
        if (p[i].x<0||p[i].y<0||p[i].x>=mapping.n||p[i].y>=mapping.m)
            return false;
    for (int i=0; i<4; i++)
        if (mapping.v[p[i].x][p[i].y])
            return false;
    return true;
}

void cube::moveinscreen()
{
    for (int i=0; i<4; i++) {
        gotoxy(hOut1,p[i].x,p[i].y*2);
        cout<<"\033[0m  ";
    }
        for (int i=1; i<mapping.n; i++) {
        if (!cube(type1,type2,x+i,y).can()) {
            cube temp(type1,type2,x+i-1,y);
            for (int i=0; i<4; i++) {
                gotoxy(hOut1,temp.p[i].x,temp.p[i].y*2);
                cout<<"\033[0m  ";
            }
            break;
        }
    }
}
void cube::printinscreen()
{
    for (int i=0; i<4; i++) {
        gotoxy(hOut1,p[i].x,p[i].y*2);
        cout<<"\033[41m  \033[0m";
    }
    for (int i=1; i<mapping.n; i++) {
        if (!cube(type1,type2,x+i,y).can()) {
            cube temp(type1,type2,x+i-1,y);
            for (int i=0; i<4; i++) {
                gotoxy(hOut1,temp.p[i].x,temp.p[i].y*2);
                cout<<"\033[44m  \033[0m";
            }
            break;
        }
    }
}


void autodown(cube & p)
{
    while (mapping.live) {
        p.changePosition(3);
        Sleep(Setting.updateGap);
    }
}
void printfailed()
{
    system("cls");
    if (mapping.culculatescore()>Setting.LargestPoint)
        Setting.LargestPoint=mapping.culculatescore();
    Setting.LastPoint=mapping.culculatescore();
    cout<<"Game Over!\nHigest score is:\t"<<setw(17)<<Setting.LargestPoint<<"\nYour score is:\t\t"<<setw(17)<<mapping.culculatescore();
    cout<<"\nPress any key to continue..\n\n>";
    Setting.saveSetting();
    show();
    _getch();
    system("cls");
    cout<<"Tetris v3.0\nCopyright (c) 2020 Purelight.Chan Zee Lok.\nPlease enter \033[1;46m'start'\033[0m to start..\n\n>";
}
void start()
{
    mapping.init();
    hide();
    char ch;
    cube p;
    p.printinscreen();
    thread automove(autodown, ref(p));
    while (mapping.live) {
        ch=_getch();
        switch (ch) {
            case 'a':
                p.changePosition(1);
                break;
            case 'd':
                p.changePosition(2);
                break;
            case 's':
                p.changePosition(3);
                break;
            case ' ':
                p.changeShape();
                break;
            case 75:
                p.changePosition(1);
                break;
            case 77:
                p.changePosition(2);
                break;
            case 80:
                p.changePosition(3);
                break;
            case 'q':
                mapping.live=0;
                break;
            case '/':
                while (cube(p.type1, p.type2, p.x+2, p.y).can())
                    p.changePosition(3);
                break;
            default :
                break;
        }
    }
    automove.join();
    printfailed();
}
void shell()
{
    system("mode con cols=102 lines=37");
    Setting.readSetting();
    string s;
    cout<<"Tetris v3.0\nCopyright (c) 2020 Purelight.Chan Zee Lok.\nPlease enter \033[1;46m'start'\033[0m to start..\n\n>";
    do {
        cin>>s;
        if (s=="start")
            start();
        else if (s=="changeSetting") {
            Setting.changeSetting();
            cout<<"Tetris v3.0\nCopyright (c) 2020 Purelight.Chan Zee Lok.\nPlease enter \033[1;46m'start'\033[0m to start..\n\n>";
        }
        else if (s=="showSetting")
            Setting.showSetting();
        else if (s=="about")
            cout<<"Tetris v3.0\nCopyright (c) 2020 Purelight.Chan Zee Lok.\nPlease enter \033[1;46m'start'\033[0m to start..\n\n>";
        else if (s=="help") {
            cout<<"\nHere are commands you can use:\n>\tstart: To start the game.\n>\tshowSetting: To show Setting.\n";
            cout<<">\tchangeSetting: To change Setting.\n>\thonor: To show your honor.\n>\tabout: To show the info of author.\n>\texit: To exit the game\n\n";
            cout<<"How to control?\n>\tTo use the Function Key 'left' or 'right',\n>\tor space to control the Tetris\n>\t'q': to quit the game.\n>\t'/': to draw the cube to the down.\n\n>";
        }
        else if (s=="honor") {
            cout<<"Here are your honors:\n\tHighest Point:\t"<<Setting.LargestPoint<<"\n\tLatest Point:\t"<<Setting.LastPoint<<"\n\n>";
        }
        else if (s=="exit")
            continue;
        else
            cout<<"NOT such command! type \033[1;46m'help'\033[0m for help!\n\n>";
    } while (s!="exit");
    Setting.saveSetting();
}
int main()
{
    shell();
    return 0;
}
