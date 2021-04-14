#include <iostream>
#include <windows.h>
#include <winuser.h>
#include <sstream>
using namespace std;

//променливи

int const n=13;//рaзмери
int const m=6;//мащаб
int xMe=1, yMe=1;//нашите координати
int x=1, y=1;//бъдещи координати
int xWin=n-2, yWin=n-2;//финал
bool win=false;//спечелихме ли?
int timer = 200;//таймер
int timerWin = 50000;//таймер

//двумерен масив (табличка), който ни е картата

/*
//level 1
int maze [n][n] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,1,0,0,0,1,0,0,0,1},
    {1,1,1,0,0,0,1,0,1,0,1,0,1},
    {1,0,1,0,1,1,0,0,1,0,1,0,1},
    {1,1,0,0,1,0,0,1,0,0,1,0,1},
    {1,0,1,1,1,1,0,0,0,1,0,0,1},
    {1,0,0,0,0,1,0,1,1,0,0,1,1},
    {1,1,0,1,0,0,0,1,0,0,1,0,1},
    {1,1,0,0,1,1,1,0,0,0,0,0,1},
    {1,0,1,0,1,0,0,0,1,1,1,0,1},
    {1,0,0,0,1,0,1,1,0,0,0,1,1},
    {1,1,1,1,0,0,0,0,0,1,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
};
*/

//level 2

int maze [n][n] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,1,0,0,1,1,0,0,1,1},
    {1,0,1,1,0,1,0,0,0,1,0,1,1},
    {1,0,0,0,0,1,0,1,0,0,0,1,1},
    {1,1,0,1,1,1,0,0,1,1,1,0,1},
    {1,0,0,1,0,1,1,0,1,0,0,0,1},
    {1,0,1,0,0,0,0,0,1,0,1,0,1},
    {1,0,0,0,1,0,1,1,0,0,1,0,1},
    {1,1,1,1,0,0,1,0,0,1,0,0,1},
    {1,0,0,0,0,1,0,0,0,1,0,1,1},
    {1,0,1,1,1,0,0,1,0,1,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,1,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1},
};

int WinScreen [n][n] = {
    {0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,1,0,1,0,1,1,1,0,1,0,1,0},
    {0,1,1,1,0,1,0,1,0,1,0,1,0},
    {0,0,1,0,0,1,0,1,0,1,0,1,0},
    {0,0,1,0,0,1,1,1,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,0,0,0,1,0,1,0,1,0,0,1,0},
    {1,0,0,0,1,0,1,0,1,1,0,1,0},
    {1,0,0,0,1,0,1,0,1,0,1,1,0},
    {1,0,1,0,1,0,1,0,1,0,0,1,0},
    {0,1,0,1,0,0,1,0,1,0,0,1,0},
    {0,0,0,0,0,0,0,0,0,0,0,0,0},
};

//готова функция за рисуване
HANDLE hConsoleOutput = ::GetStdHandle(STD_OUTPUT_HANDLE);
COORD screen_buf = {n*m , n*m};
CHAR_INFO blank_screen[n*m * n*m] = {0};


//дефинира цветове
enum COLORS {
    BLACK = 0,
    BLUE = FOREGROUND_BLUE,
    CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN,
    GREEN = FOREGROUND_GREEN,
    RED = FOREGROUND_RED,
    BROWN = FOREGROUND_RED | FOREGROUND_GREEN,
    PURPLE = FOREGROUND_RED | FOREGROUND_BLUE,
    LIGHT_PURPLE = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LIGHT_GREY =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN,

    GREY = 0 | FOREGROUND_INTENSITY,
    LIGHT_BLUE = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    LIGHT_CYAN = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    LIGHT_RED = FOREGROUND_RED | FOREGROUND_INTENSITY,
    YELLOW = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY,
    PINK = FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY,
    WHITE =  FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_INTENSITY
};

//дефинира как да зададем цвят и символ на координати
void draw_char(char ch, int y, int x, COLORS foreground_color, COLORS background_color) {
    CHAR_INFO ch_info;
    ch_info.Char.AsciiChar = ch;
    ch_info.Attributes = foreground_color | (background_color << 4);

    COORD buf_size = {1, 1};
    COORD buf_coord = {0, 0};
    SMALL_RECT screen_pos = {x, y, x+1, y+1};
    ::WriteConsoleOutput(hConsoleOutput, &ch_info, buf_size, buf_coord, &screen_pos);

}

//чистене на екрана
void clear_screen() {
    COORD buf_coord = {0, 0};
    SMALL_RECT screen_pos = {0, 0, screen_buf.X, screen_buf.Y};
    ::WriteConsoleOutput(hConsoleOutput, blank_screen, screen_buf, buf_coord, &screen_pos);
}

//задаване на фон
void set_background() {
    for (int i = 0; i < screen_buf.X; i++) {
        for (int j = 0; j < screen_buf.Y; j++) {
             draw_char(' ', j, i, WHITE, WHITE);
        }
    }
}


int main(){

//задаване на гранизи за конзолата
SMALL_RECT rect = {0, 0, screen_buf.X-1, screen_buf.Y-1};

    SetConsoleScreenBufferSize(hConsoleOutput, screen_buf);
    SetConsoleWindowInfo(hConsoleOutput, TRUE, &rect);
    SetConsoleTitle("MAZE");

//задава целия масив да е с бели квадратчета
set_background();

//оцветява в мащаб за мястото на победа
for(int k=0; k<m; k++){
    for(int l=0; l<m; l++){
        maze [xWin*m + k][yWin*m +l]=-1;
        draw_char(' ', xWin*m + k, yWin*m +l, RED, LIGHT_GREEN);
    }
}

//начално рисуване на лабиринта
for(int i=0; i<n; i++){
    //обхождане на редовете
    for(int j=0; j<n; j++){
        //обхождане на колоните
        if (maze [i][j]==1){
            //на мястото на всяка 1 слага стена
            for(int k=0; k<m; k++){
                for(int l=0; l<m; l++){
                    draw_char('x', i*m+k, j*m+l, RED, BLUE);
                }
            }
        }

    }
}

//движение(докато не спечелим)
while(!win){

//при движение нагоре
if(GetAsyncKeyState('A')){
       y=y-1;
    //ако бъдещите координати са прани се мести на новата позиция
    if(maze[x][y]==0){
            //старата ми позиция се изчиства
            maze[xMe][yMe]=0;
            for(int k=0; k<m; k++){
                for(int l=0; l<m; l++){
                    draw_char(' ', xMe*m+k, yMe*m+l, RED, WHITE);
                }
            }
        xMe=x;
        yMe=y;
    //ако бъдещите координати са запълнени си освавяме на същото място
    }else{
        x=xMe;
        y=yMe;
    }
}else if(GetAsyncKeyState('S')){
       x=x+1;
           //ако бъдещите координати са прани се мести на новата позиция
    if(maze[x][y]==0){
            //старата ми позиция се изчиства
            maze[xMe][yMe]=0;
            for(int k=0; k<m; k++){
                for(int l=0; l<m; l++){
                    draw_char(' ', xMe*m+k, yMe*m+l, RED, WHITE);
                }
            }
        xMe=x;
        yMe=y;
    //ако бъдещите координати са запълнени си освавяме на същото място
    }else{
        x=xMe;
        y=yMe;
    }
}else if(GetAsyncKeyState('W')){
       x=x-1;
           //ако бъдещите координати са прани се мести на новата позиция
    if(maze[x][y]==0){
            //старата ми позиция се изчиства
            maze[xMe][yMe]=0;
            for(int k=0; k<m; k++){
                for(int l=0; l<m; l++){
                    draw_char(' ', xMe*m+k, yMe*m+l, RED, WHITE);
                }
            }
        xMe=x;
        yMe=y;
    //ако бъдещите координати са запълнени си освавяме на същото място
    }else{
        x=xMe;
        y=yMe;
    }
}else if(GetAsyncKeyState('D')){
       y=y+1;
    //ако бъдещите координати са прани се мести на новата позиция
    if(maze[x][y]==0){
            //старата ми позиция се изчиства
            maze[xMe][yMe]=0;
            for(int k=0; k<m; k++){
                for(int l=0; l<m; l++){
                    draw_char(' ', xMe*m+k, yMe*m+l, RED, WHITE);
                }
            }
        xMe=x;
        yMe=y;
    //ако бъдещите координати са запълнени си освавяме на същото място
    }else{
        x=xMe;
        y=yMe;
    }
}

//оцветяваме новата си позиция
maze [xMe][yMe]=2;
for(int k=0; k<m; k++){
                for(int l=0; l<m; l++){
                    draw_char(' ', xMe*m+k, yMe*m+l, RED, RED);
                }
            }

//чакаме няколко милисекунди
Sleep(timer);

 //победихме ли????
if(xMe==xWin && yMe==yWin){
    win=true;
    clear_screen();
    for ( int i=0; i<n; i++){
        for (int j=0; j<n; j++){
            if (WinScreen [i][j]==1){
                for(int k=0; k<m; k++){
                    for(int l=0; l<m; l++){
                        draw_char(' ', i*m+k, j*m+l, RED, BLUE);
                    }
                }
            }else {
                for(int k=0; k<m; k++){
                    for(int l=0; l<m; l++){
                        draw_char(' ', i*m+k, j*m+l, RED, WHITE);
                }
            }
        }
        }
    }
 //   cout << endl << "YOU WON!!!!!!"<< endl;
    Sleep(timerWin);
}

}


return 0;
}
