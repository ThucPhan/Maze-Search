#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <stack>

#define MAX 1000

using namespace std;

struct Cell
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool visited;
}Maze[MAX][MAX];

void InitializeMaze(int nrRows,int nrCols);
void GenerateMaze(int nrRows,int nrCols);
void WriteFile(int nrRows,int nrCols,string filename);

int main()
{
    int nrRows, nrCols;
    string filename;

    cout<<"Enter the number of rows: "; cin>>nrRows;
    cout<<"Enter the number of columns: "; cin>>nrCols;

    InitializeMaze(nrRows,nrCols);
    GenerateMaze(nrRows,nrCols);

    cout<<"Enter output file name: "; cin>>filename;
    WriteFile(nrRows,nrCols,filename);

    return 0;
}

void InitializeMaze(int nrRows,int nrCols)
{
    for(int i=0;i<nrRows+2;i++)
    {
        for(int j=0;j<nrCols+2;j++)
        {
            Maze[i][j].up = true;
            Maze[i][j].down = true;
            Maze[i][j].left = true;
            Maze[i][j].right = true;
            Maze[i][j].visited = false;
        }
    }

    for(int j=0;j<nrCols+2;j++)
    {
        Maze[0][j].visited = true;
        Maze[nrRows+1][j].visited = true;
    }

    for(int i=0;i<nrRows+2;i++)
    {
        Maze[i][0].visited = true;
        Maze[i][nrCols+1].visited = true;
    }
}

void GenerateMaze(int nrRows,int nrCols)
{
    srand(time(NULL));
    int direction = 0;
    int randomX = rand()%nrCols +1;
    int randomY = rand()%nrRows +1;
    int visitedCells = 1;
    int totalCells = nrRows*nrCols;
    stack <int> back_trackX,back_trackY;

    Maze[randomY][randomX].visited = true;

    while(visitedCells<totalCells)
    {
        if( ((Maze[randomY-1][randomX].visited==false) && (Maze[randomY][randomX].up==true) && (Maze[randomY-1][randomX].down==true)) ||
            ((Maze[randomY+1][randomX].visited==false) && (Maze[randomY][randomX].down==true) && (Maze[randomY+1][randomX].up==true)) ||
            ((Maze[randomY][randomX-1].visited==false) && (Maze[randomY][randomX].left==true) && (Maze[randomY][randomX-1].right==true)) ||
            ((Maze[randomY][randomX+1].visited==false) && (Maze[randomY][randomX].right==true) && (Maze[randomY][randomX+1].left==true)) )
        {
            direction = rand()%4 +1; //4 directions

            //go up
            if((direction==1))
            {
                if(Maze[randomY-1][randomX].visited==false)
                {
                    Maze[randomY][randomX].up = false;
                    Maze[randomY-1][randomX].down = false;
                    Maze[randomY-1][randomX].visited = true;
                    back_trackX.push(randomX);
                    back_trackY.push(randomY);
                    visitedCells++;
                    randomY -= 1;
                }
                else
                    continue;
            }

            //go down
            else if ((direction==2))
            {
                if(Maze[randomY+1][randomX].visited==false)
                {
                    Maze[randomY][randomX].down = false;
                    Maze[randomY+1][randomX].up = false;
                    Maze[randomY+1][randomX].visited = true;
                    back_trackX.push(randomX);
                    back_trackY.push(randomY);
                    visitedCells++;
                    randomY += 1;
                }
                else
                    continue;
            }

            //go left
            else if ((direction==3))
            {
                if(Maze[randomY][randomX-1].visited==false)
                {
                    Maze[randomY][randomX].left = false;
                    Maze[randomY][randomX-1].right = false;
                    Maze[randomY][randomX-1].visited = true;
                    back_trackX.push(randomX);
                    back_trackY.push(randomY);
                    visitedCells++;
                    randomX -= 1;
                }
                else
                    continue;
            }

            //go right
            else if ((direction==4))
            {
                if(Maze[randomY][randomX+1].visited==false)
                {
                    Maze[randomY][randomX].right = false;
                    Maze[randomY][randomX+1].left = false;
                    Maze[randomY][randomX+1].visited = true;
                    back_trackX.push(randomX);
                    back_trackY.push(randomY);
                    visitedCells++;
                    randomX += 1;
                }
                else
                    continue;
            }
        }
        else
        {
            randomX = back_trackX.top();
            randomY = back_trackY.top();
            back_trackX.pop();
            back_trackY.pop();
        }
    }
}

void WriteFile(int nrRows,int nrCols,string filename)
{
    int totalCells = nrRows*nrCols;
    ofstream file (filename);
    if(file.is_open())
    {
        file<<nrRows<<" "<<nrCols<<endl;
        file<<totalCells<<endl;
        for (int i=1;i<nrRows+1;i++)
        {
            for (int j=1;j<nrCols+1;j++)
            {
                file<<i-1<<" "<<j-1<<" ";
                if(Maze[i][j].left==true)
                    file<<"L";
                if(Maze[i][j].right==true)
                    file<<"R";
                if(Maze[i][j].up==true)
                    file<<"U";
                if(Maze[i][j].down==true)
                    file<<"D";
                if(Maze[i][j].left==false &&
                   Maze[i][j].right==false &&
                   Maze[i][j].up==false &&
                   Maze[i][j].down==false)
                    file<<"none";
                file<<endl;
            }
        }
        file.close();
    }
    else
        cout<<"Unable to write file\n";
}
