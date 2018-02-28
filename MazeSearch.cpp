#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <bits/stdc++.h>
#include <cmath>
#include <deque>
#include <limits>

#define MAX 100

using namespace std;

struct Cell
{
    bool up;
    bool down;
    bool left;
    bool right;
    bool visited;
    int parentPos;

    int pos;
    float gCost;
    float fCost; //fCost = gCost + hCost
}Maze[MAX][MAX];

class comparator
{
public:
    int operator() (Cell node1,Cell node2)
    {
        return node1.fCost > node2.fCost;
    }
};

stack <int> back_trackX,back_trackY;
queue <int> qX,qY;
priority_queue < Cell,vector<Cell>,comparator > pq;
deque <Cell> node; //double end queue

void InitializeMaze(int nrRows,int nrCols);
void DFS(int r_start,int c_start,int r_end,int c_end,string filename_w);
void BFS(int r_start,int c_start,int r_end,int c_end,string filename_w);
void AZ(int r_start,int c_start,int r_end,int c_end,string filename_w);
float hCost(int r_start,int c_start,int r_end,int c_end);
void AE(int r_start,int c_start,int r_end,int c_end,string filename_w);
void Fringe(int r_start,int c_start,int r_end,int c_end,string filename_w);

int main()
{
    string userInput;
    string filename_r,filename_w;
    int r_start,c_start;
    int r_end,c_end;
    int nrRows,nrCols;
    int totalCells;
    int x,y; //coordinates
    string s; //LRUD

    cout<<"Enter the method you want to use: "; cin>>userInput;

    //input file .txt
    cout<<"Enter the name of input text file: "; cin>>filename_r;
    ifstream infile (filename_r);
    if(infile.is_open())
    {
        infile>>nrRows>>nrCols;
        InitializeMaze(nrRows,nrCols);
        infile>>totalCells;
        for(int i=0;i<totalCells;i++)
        {
            infile>>y>>x>>s;
            if (s[0]=='n') //none
            {
                Maze[y][x].left = false;
                Maze[y][x].right = false;
                Maze[y][x].up = false;
                Maze[y][x].down = false;
            }
            else
            {
                for(int j=0;j<s.size();j++)
                {
                    if (s[j]=='L')
                        Maze[y][x].left = true;
                    if (s[j]=='R')
                        Maze[y][x].right = true;
                    if (s[j]=='U')
                        Maze[y][x].up = true;
                    if (s[j]=='D')
                        Maze[y][x].down = true;
                }
            }
        }
        infile.close();
    }
    else
        cout<<"Unable to read file \n";

    cout<<"Enter the row start: "; cin>>r_start;
    cout<<"Enter the column start: "; cin>>c_start;
    cout<<"Enter the row end: "; cin>>r_end;
    cout<<"Enter the column end: "; cin>>c_end;

    //output file .txt
    cout<<"Enter the name of output text file: "; cin>>filename_w;

    if(userInput=="DFS")
        DFS(r_start,c_start,r_end,c_end,filename_w);
    if(userInput=="BFS")
        BFS(r_start,c_start,r_end,c_end,filename_w);
    if(userInput=="AStarZero" || userInput=="AZ")
        AZ(r_start,c_start,r_end,c_end,filename_w);
    if(userInput=="AStarEuclidean" || userInput=="AE")
        AE(r_start,c_start,r_end,c_end,filename_w);
    if(userInput=="Fringe")
        Fringe(r_start,c_start,r_end,c_end,filename_w);

    return 0;
}

void InitializeMaze(int nrRows,int nrCols)
{
    for(int i=0;i<nrRows;i++)
    {
        for(int j=0;j<nrCols;j++)
        {
            Maze[i][j].up = false;
            Maze[i][j].down = false;
            Maze[i][j].left = false;
            Maze[i][j].right = false;
            Maze[i][j].visited = false;
            Maze[i][j].parentPos = 0;
            Maze[i][j].pos = 0;
            Maze[i][j].gCost = 0;
            Maze[i][j].fCost = 0;
        }
    }
}

void DFS(int r_start,int c_start,int r_end,int c_end,string filename_w)
{
    if(Maze[r_start][c_start].visited==false)
    {
        back_trackY.push(r_start);
        back_trackX.push(c_start);
        Maze[r_start][c_start].visited = true;
    }

    if( (back_trackY.top()==r_end) && (back_trackX.top()==c_end) )
    {
        ofstream outfile (filename_w);
        if(outfile.is_open())
        {
            outfile<<back_trackX.size()<<endl; //size of back_trackX = back_trackY

            //reverse stack
            stack <int> tmpX,tmpY;
            while( (!back_trackY.empty()) && (!back_trackX.empty()) )
            {
                tmpY.push(back_trackY.top());
                tmpX.push(back_trackX.top());
                back_trackY.pop();
                back_trackX.pop();
            }
            while( (!tmpY.empty()) && (!tmpX.empty()) )
            {
                outfile<<tmpY.top()<<" "<<tmpX.top()<<endl;
                tmpY.pop();
                tmpX.pop();
            }

            outfile.close();
        }
        else
            cout<<"Unable to write file \n";
        return;
    }

    else
    {
        //go up
        if ( (Maze[r_start][c_start].up==false) && (Maze[r_start-1][c_start].visited==false) )
        {
            r_start -= 1;
            DFS(r_start,c_start,r_end,c_end,filename_w);
        }

        //go down
        else if ( (Maze[r_start][c_start].down==false) && (Maze[r_start+1][c_start].visited==false) )
        {
            r_start += 1;
            DFS(r_start,c_start,r_end,c_end,filename_w);
        }

        //go left
        else if ( (Maze[r_start][c_start].left==false) && (Maze[r_start][c_start-1].visited==false) )
        {
            c_start -= 1;
            DFS(r_start,c_start,r_end,c_end,filename_w);
        }

        //go right
        else if ( (Maze[r_start][c_start].right==false) && (Maze[r_start][c_start+1].visited==false) )
        {
            c_start += 1;
            DFS(r_start,c_start,r_end,c_end,filename_w);
        }

        else
        {
            back_trackY.pop();
            back_trackX.pop();
            r_start = back_trackY.top();
            c_start = back_trackX.top();
            DFS(r_start,c_start,r_end,c_end,filename_w);
        }
    }
}

void BFS(int r_start,int c_start,int r_end,int c_end,string filename_w)
{
    int pos_start,pos_cur,posX,posY;
    int node = 1;

    pos_start = r_start*MAX+c_start;
    Maze[r_start][c_start].visited = true;
    qY.push(r_start);
    qX.push(c_start);

    while( (!qY.empty()) && (!qX.empty()) )
    {
        posY = qY.front();
        posX = qX.front();
        pos_cur = posY*MAX+posX; //head node
        //cout<<posY<<" "<<posX<<"\n";

        if( (posY==r_end) && (posX==c_end) && (Maze[posY][posX].visited=true) )
        {
            //create stored array
            int storedY[MAX*MAX];
            int storedX[MAX*MAX];
            storedY[0]=r_end;
            storedX[0]=c_end;

            //find the number of node
            int tmp = pos_cur; //already at the end
            while(tmp!=pos_start)
            {
                tmp = Maze[posY][posX].parentPos;
                posY = tmp/MAX;
                storedY[node] = posY;
                posX = tmp%MAX;
                storedX[node] = posX;
                ++node;
            }

            ofstream outfile (filename_w);
            if(outfile.is_open())
            {
                outfile<<node<<endl;
                for(int i=node-1;i>=0;i--)
                    outfile<<storedY[i]<<" "<<storedX[i]<<endl;
                outfile.close();
            }
            else
                cout<<"Unable to write file \n";

            return;
        }

        else
        {
            qY.pop();
            qX.pop();

            //search up
            if((Maze[posY][posX].up==false) && (Maze[posY-1][posX].visited==false))
            {
                qY.push(posY-1);
                qX.push(posX);
                Maze[posY-1][posX].visited = true;
                Maze[posY-1][posX].parentPos = pos_cur; //neighbor node
            }

            //search down
            if((Maze[posY][posX].down==false) && (Maze[posY+1][posX].visited==false))
            {
                qY.push(posY+1);
                qX.push(posX);
                Maze[posY+1][posX].visited = true;
                Maze[posY+1][posX].parentPos = pos_cur; //neighbor node
            }

            //search left
            if((Maze[posY][posX].left==false) && (Maze[posY][posX-1].visited==false))
            {
                qY.push(posY);
                qX.push(posX-1);
                Maze[posY][posX-1].visited = true;
                Maze[posY][posX-1].parentPos = pos_cur; //neighbor node
            }

            //search right
            if((Maze[posY][posX].right==false) && (Maze[posY][posX+1].visited==false))
            {
                qY.push(posY);
                qX.push(posX+1);
                Maze[posY][posX+1].visited = true;
                Maze[posY][posX+1].parentPos = pos_cur; //neighbor node
            }
        } //end else
    } //end while
}

/*-----with h = 0, then f = g-----*/
void AZ(int r_start,int c_start,int r_end,int c_end,string filename_w)
{
    int posY,posX;
    int pos_start,pos_end;
    pos_start = r_start*MAX+c_start;
    pos_end = r_end*MAX+c_end;

    Maze[r_start][c_start].visited = true;
    Maze[r_start][c_start].pos = pos_start;
    Maze[r_start][c_start].fCost = 0;
    pq.push(Maze[r_start][c_start]);

    while(!pq.empty())
    {
        Cell current = pq.top();
        posY = current.pos/MAX;
        posX = current.pos%MAX;
        //cout<<posY<<" "<<posX<<" "<<current.fCost<<endl;

        if((current.pos == pos_end) && (current.visited==true))
        {
            //reconstruct path
            int node = 1;
            int storedY[MAX*MAX];
            int storedX[MAX*MAX];
            storedY[0]=r_end;
            storedX[0]=c_end;

            int tmp = current.pos;
            while(tmp!=pos_start)
            {
                tmp = Maze[posY][posX].parentPos;
                posY = tmp/MAX;
                storedY[node] = posY;
                posX = tmp%MAX;
                storedX[node] = posX;
                ++node;
            }

            ofstream outfile (filename_w);
            if(outfile.is_open())
            {
                outfile<<node<<endl;
                for(int i=node-1;i>=0;i--)
                    outfile<<storedY[i]<<" "<<storedX[i]<<endl;
                outfile.close();
            }
            else
                cout<<"Unable to write file \n";

            return;
        }

        pq.pop();

        //search up
        if((Maze[posY][posX].up==false) && (Maze[posY-1][posX].visited==false))
        {
            Maze[posY-1][posX].visited = true;
            Maze[posY-1][posX].pos = (posY-1)*MAX+posX;
            Maze[posY-1][posX].parentPos = Maze[posY][posX].pos;
            Maze[posY-1][posX].fCost = Maze[posY][posX].fCost + 1;
            pq.push(Maze[posY-1][posX]);
        }

        //search down
        if((Maze[posY][posX].down==false) && (Maze[posY+1][posX].visited==false))
        {
            Maze[posY+1][posX].visited = true;
            Maze[posY+1][posX].pos = (posY+1)*MAX+posX;
            Maze[posY+1][posX].parentPos = Maze[posY][posX].pos;
            Maze[posY+1][posX].fCost = Maze[posY][posX].fCost + 1;
            pq.push(Maze[posY+1][posX]);
        }

        //search left
        if((Maze[posY][posX].left==false) && (Maze[posY][posX-1].visited==false))
        {
            Maze[posY][posX-1].visited = true;
            Maze[posY][posX-1].pos = posY*MAX+posX-1;
            Maze[posY][posX-1].parentPos = Maze[posY][posX].pos;
            Maze[posY][posX-1].fCost = Maze[posY][posX].fCost + 1;
            pq.push(Maze[posY][posX-1]);
        }

        //search right
        if((Maze[posY][posX].right==false) && (Maze[posY][posX+1].visited==false))
        {
            Maze[posY][posX+1].visited = true;
            Maze[posY][posX+1].pos = posY*MAX+posX+1;
            Maze[posY][posX+1].parentPos = Maze[posY][posX].pos;
            Maze[posY][posX+1].fCost = Maze[posY][posX].fCost + 1;
            pq.push(Maze[posY][posX+1]);
        }
    }
}

float hCost(int r_start,int c_start,int r_end,int c_end)
{
    float dy = abs(r_start-r_end);
    float dx = abs(c_start-c_end);
    return sqrt(dx*dx+dy*dy);
}

/*-----f = g + h-----*/
void AE(int r_start,int c_start,int r_end,int c_end,string filename_w)
{
    int posY,posX;
    int pos_start,pos_end;
    pos_start = r_start*MAX+c_start;
    pos_end = r_end*MAX+c_end;

    Maze[r_start][c_start].visited = true;
    Maze[r_start][c_start].pos = pos_start;
    Maze[r_start][c_start].gCost = 0;
    Maze[r_start][c_start].fCost = Maze[r_start][c_start].gCost + hCost(r_start,c_start,r_end,c_end);
    pq.push(Maze[r_start][c_start]);

    while(!pq.empty())
    {
        Cell current = pq.top();
        posY = current.pos/MAX;
        posX = current.pos%MAX;
        //cout<<posY<<" "<<posX<<" "<<current.fCost<<endl;

        if((current.pos == pos_end) && (current.visited==true))
        {
            //reconstruct path
            int node = 1;
            int storedY[MAX*MAX];
            int storedX[MAX*MAX];
            storedY[0]=r_end;
            storedX[0]=c_end;

            int tmp = current.pos;
            while(tmp!=pos_start)
            {
                tmp = Maze[posY][posX].parentPos;
                posY = tmp/MAX;
                storedY[node] = posY;
                posX = tmp%MAX;
                storedX[node] = posX;
                ++node;
            }

            ofstream outfile (filename_w);
            if(outfile.is_open())
            {
                outfile<<node<<endl;
                for(int i=node-1;i>=0;i--)
                    outfile<<storedY[i]<<" "<<storedX[i]<<endl;
                outfile.close();
            }
            else
                cout<<"Unable to write file \n";

            return;
        }

        pq.pop();

        //search up
        if((Maze[posY][posX].up==false) && (Maze[posY-1][posX].visited==false))
        {
            Maze[posY-1][posX].visited = true;
            Maze[posY-1][posX].pos = (posY-1)*MAX+posX;
            Maze[posY-1][posX].parentPos = Maze[posY][posX].pos;
            Maze[posY-1][posX].gCost = Maze[posY][posX].gCost + 1;
            Maze[posY-1][posX].fCost = Maze[posY-1][posX].gCost + hCost(posY-1,posX,r_end,c_end);
            pq.push(Maze[posY-1][posX]);
        }

        //search down
        if((Maze[posY][posX].down==false) && (Maze[posY+1][posX].visited==false))
        {
            Maze[posY+1][posX].visited = true;
            Maze[posY+1][posX].pos = (posY+1)*MAX+posX;
            Maze[posY+1][posX].parentPos = Maze[posY][posX].pos;
            Maze[posY+1][posX].gCost = Maze[posY][posX].gCost + 1;
            Maze[posY+1][posX].fCost = Maze[posY+1][posX].gCost + hCost(posY+1,posX,r_end,c_end);
            pq.push(Maze[posY+1][posX]);
        }

        //search left
        if((Maze[posY][posX].left==false) && (Maze[posY][posX-1].visited==false))
        {
            Maze[posY][posX-1].visited = true;
            Maze[posY][posX-1].pos = posY*MAX+posX-1;
            Maze[posY][posX-1].parentPos = Maze[posY][posX].pos;
            Maze[posY][posX-1].gCost = Maze[posY][posX].gCost + 1;
            Maze[posY][posX-1].fCost = Maze[posY][posX-1].gCost + hCost(posY,posX-1,r_end,c_end);
            pq.push(Maze[posY][posX-1]);
        }

        //search right
        if((Maze[posY][posX].right==false) && (Maze[posY][posX+1].visited==false))
        {
            Maze[posY][posX+1].visited = true;
            Maze[posY][posX+1].pos = posY*MAX+posX+1;
            Maze[posY][posX+1].parentPos = Maze[posY][posX].pos;
            Maze[posY][posX+1].gCost = Maze[posY][posX].gCost + 1;
            Maze[posY][posX+1].fCost = Maze[posY][posX+1].gCost + hCost(posY,posX+1,r_end,c_end);
            pq.push(Maze[posY][posX+1]);
        }
    }
}

void Fringe(int r_start,int c_start,int r_end,int c_end,string filename_w)
{
    int posY,posX;
    int pos_start,pos_end;
    pos_start = r_start*MAX+c_start;
    pos_end = r_end*MAX+c_end;

    Maze[r_start][c_start].visited = true;
    Maze[r_start][c_start].pos = pos_start;
    Maze[r_start][c_start].fCost = Maze[r_start][c_start].gCost + hCost(r_start,c_start,r_end,c_end);
    float flimit = Maze[r_start][c_start].fCost;
    node.push_back(Maze[r_start][c_start]);
    int found = 0;

    while( (!found) && (!node.empty()) )
    {
        float fmin = numeric_limits<float>::infinity();
        for(Cell n:node)
        {
            posY = n.pos/MAX;
            posX = n.pos%MAX;
            n.fCost = n.gCost + hCost(posY,posX,r_end,c_end);

            if(n.fCost>flimit)
            {
                fmin = min(n.fCost,fmin);
                continue;
            }
            if(n.pos==pos_end)
            {
                found = 1;

                //reconstruct path
                int node = 1;
                int storedY[MAX*MAX];
                int storedX[MAX*MAX];
                storedY[0]=r_end;
                storedX[0]=c_end;

                int tmp = n.pos;
                while(tmp!=pos_start)
                {
                    tmp = Maze[posY][posX].parentPos;
                    posY = tmp/MAX;
                    storedY[node] = posY;
                    posX = tmp%MAX;
                    storedX[node] = posX;
                    ++node;
                }

                ofstream outfile (filename_w);
                if(outfile.is_open())
                {
                    outfile<<node<<endl;
                    for(int i=node-1;i>=0;i--)
                        outfile<<storedY[i]<<" "<<storedX[i]<<endl;
                    outfile.close();
                }
                else
                    cout<<"Unable to write file \n";

                break;
            }

            //search up
            if((Maze[posY][posX].up==false) && (Maze[posY-1][posX].visited==false))
            {
                Maze[posY-1][posX].visited = true;
                Maze[posY-1][posX].pos = (posY-1)*MAX+posX;
                Maze[posY-1][posX].parentPos = Maze[posY][posX].pos;
                Maze[posY-1][posX].gCost = Maze[posY][posX].gCost + 1;
                Maze[posY-1][posX].fCost = Maze[posY-1][posX].gCost + hCost(posY-1,posX,r_end,c_end);
                node.push_back(Maze[posY-1][posX]);
            }

            //search down
            if((Maze[posY][posX].down==false) && (Maze[posY+1][posX].visited==false))
            {
                Maze[posY+1][posX].visited = true;
                Maze[posY+1][posX].pos = (posY+1)*MAX+posX;
                Maze[posY+1][posX].parentPos = Maze[posY][posX].pos;
                Maze[posY+1][posX].gCost = Maze[posY][posX].gCost + 1;
                Maze[posY+1][posX].fCost = Maze[posY+1][posX].gCost + hCost(posY+1,posX,r_end,c_end);
                node.push_back(Maze[posY+1][posX]);
            }

            //search left
            if((Maze[posY][posX].left==false) && (Maze[posY][posX-1].visited==false))
            {
                Maze[posY][posX-1].visited = true;
                Maze[posY][posX-1].pos = posY*MAX+posX-1;
                Maze[posY][posX-1].parentPos = Maze[posY][posX].pos;
                Maze[posY][posX-1].gCost = Maze[posY][posX].gCost + 1;
                Maze[posY][posX-1].fCost = Maze[posY][posX-1].gCost + hCost(posY,posX-1,r_end,c_end);
                node.push_back(Maze[posY][posX-1]);
            }

            //search right
            if((Maze[posY][posX].right==false) && (Maze[posY][posX+1].visited==false))
            {
                Maze[posY][posX+1].visited = true;
                Maze[posY][posX+1].pos = posY*MAX+posX+1;
                Maze[posY][posX+1].parentPos = Maze[posY][posX].pos;
                Maze[posY][posX+1].gCost = Maze[posY][posX].gCost + 1;
                Maze[posY][posX+1].fCost = Maze[posY][posX+1].gCost + hCost(posY,posX+1,r_end,c_end);
                node.push_back(Maze[posY][posX+1]);
            }

            node.pop_front();
            flimit = fmin;
        }//end for
    }//end while
}
