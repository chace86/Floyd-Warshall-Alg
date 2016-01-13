// Chace Anderson
// Assignment 8
// CSC 300
// 12/11/15

#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <vector>
#include <string>

using namespace std;

class Graph
{
    int sCount;
    int rCount;
    int **adjmatrix;
    int **next;
    vector<string> key;

    public:
        // Assignment 6
        Graph( int routes, int stations );
        ~Graph( void );
        void addRoute( int from, int to, int weight );
        bool isRoute( int from, int to );
        void printmatrix( void );
        // Assignment 7
        void calcShortestRoutes( void );
        int shortestRoute( string src, string dst );
        void addStationID( int stations, string names );
        int stationNumber( string names );
        // Assignment 8
        void printPath( string src, string dst );
};

Graph::Graph( int routes, int stations )
{
    int i,j;
    sCount = stations;
    rCount = routes;

    adjmatrix = new int*[sCount+1];
    for( i = 0; i < sCount+1; i++ )
        adjmatrix[i] = new int [sCount+1];

    for( i = 0; i < sCount+1; i++ ) // populate with 99, AKA infinity
        for( j=0; j < sCount+1; j++ )
            adjmatrix[i][j] = 99;

    for( i=1; i <= sCount; i++ ) // input vertices into array
    {
        adjmatrix[0][i] = i-1;
        adjmatrix[i][0] = i-1;
    }

    //Assignment 7
    key = vector<string>(sCount);

    // Assignment 8
    next = new int*[sCount+1];
    for( i = 0; i < sCount+1; i++ )
        next[i] = new int [sCount+1];

    for( i = 0; i < sCount+1; i++ ) // populate with -1, AKA NULL
        for( j = 0; j < sCount+1; j++ )
            next[i][j] = -1;

    for( i = 1; i <= sCount; i++ ) // input vertices into array
    {
        next[0][i] = i-1;
        next[i][0] = i-1;
    }
}

Graph::~Graph( void )
{
    for( int i = 0; i < sCount+1; i++ )
    {
        delete adjmatrix[i];
        adjmatrix[i] = NULL;
    }

    delete adjmatrix;
    adjmatrix = NULL;

    for( int i = 0; i < sCount+1; i++ )
    {
        delete next[i];
        next[i] = NULL;
    }

    delete next;
    next = NULL;
}

void Graph::addRoute( int from, int to, int weight )
{
    adjmatrix[from+1][to+1] = weight;
    // Assignment 8
    next[from+1][to+1] = to;
}

bool Graph::isRoute( int from, int to )
{
    bool isPath = false;
    if( from == to )
        return isPath;

    int i;
    queue<int> vertices;
    bool *visited = new bool[sCount];
    for( i = 0; i < sCount; i++ )
        visited[i] = false;

    visited[from] = true;
    vertices.push(from);

    while( !vertices.empty() && isPath == false )
    {
        int current = vertices.front();
        vertices.pop();
        for( i = 1; i < sCount+1; i++)
        {
            if( visited[i-1] == false )
            {
                if( adjmatrix[current+1][i] != 99 && i-1 == to )
                {
                    isPath = true;
                    break;
                }

                if( adjmatrix[current+1][i] != 99 && visited[i-1] == false )
                {
                    vertices.push(i-1);
                    visited[i-1] = true;
                }
            }
        }
    }

    delete[] visited;
    visited = NULL;

    return isPath;
}

void Graph::printmatrix( void )
{
    int i, j;

    cout << endl;
    cout << "Stations: " << sCount << endl;
    cout << "Routes: " << rCount << endl;
    cout << endl;
    cout << "KEY" << endl;
    for( i = 0; i < sCount; i++ )
        cout << key[i] << " = " << i << endl;

    cout << endl;
    for( i = 0; i < sCount+1; i++ )
    {
        for( j = 0; j < sCount+1; j++ )
            cout << setw(2) << adjmatrix[i][j] << " ";
        cout << endl;
    }
}

// Assignment 7
void Graph::calcShortestRoutes( void )
{
    int i, j, k;
    // FLOYD-WARSHALL ALGORITHM WITH PATH RECONSTRUCTION
    for( k = 1; k < sCount+1; k++ )
        for( i = 1; i < sCount+1; i++ )
            for( j = 1; j < sCount+1; j++ )
            {
                if( adjmatrix[i][j] > adjmatrix[i][k] + adjmatrix[k][j] )
                {
                    adjmatrix[i][j] = adjmatrix[i][k] + adjmatrix[k][j];
                    next[i][j] = next[i][k];
                }
            }
}

int Graph::shortestRoute( string src, string dst )
{
    return adjmatrix[stationNumber(src)+1][stationNumber(dst)+1];
}

void Graph::addStationID( int stations, string names )
{
    key[stations] = names;
}

int Graph::stationNumber( string names )
{
    for( int i = 0; i < sCount; i++)
        if( key[i].compare(names) == 0 )
            return i;

    return -1;
}
// Assignment 8
void Graph::printPath( string src, string dst )
{
    if( isRoute( stationNumber(src), stationNumber(dst) ) == true )
    {
        int from = stationNumber(src);
        int to = stationNumber(dst);
        queue<int>path;
        path.push(from);

        while( from != to )
        {
            from = next[from+1][to+1];
            path.push(from);
        }

        cout << "Your route is ";
        while( path.size() !=1 )
        {
            cout << key[path.front()] << "->";
            path.pop();
        }
        cout << key[path.front()] << endl;
        path.pop();
    }
}


/*============ MAIN FUNCTION ============*/
int main( int argc, char **argv )
{

    if( argc != 3)
    {
        cout << "ERROR: Must run on Linux." << endl;
        cout << "Requires input.txt and stationmappings.txt files" << endl;
        cout << "cmd line: ./a.out input.txt stationmappings.txt" << endl;
    }

    else
    {
        string line, temp, leaving, traveling;
        int x, y, z, option = 1;

        ifstream myfile( argv[1] );

        getline( myfile, line );
        stringstream ss(line);
        ss >> x;
        ss >> y;
        int vertex = x;
        Graph myGraph(y,x);

        while( getline( myfile, line ) )
        {
            stringstream ss(line);
            ss >> x;
            ss >> y;
            ss >> z;
            myGraph.addRoute( x, y, z );
        }
        myfile.close();

        ifstream myfile2( argv[2] );
        while( getline( myfile2, line ) )
        {
            stringstream ss(line);
            ss >> x;
            ss >> temp;
            myGraph.addStationID( x, temp );
        }
        myfile2.close();

        myGraph.calcShortestRoutes();

        while(1)
        {
            if( option == 0 )
                break;

            else if( option == 1)
            {
                myGraph.printmatrix();
                cout << endl;
                cout << "-- Menu --" << endl;
                cout << "    " << "0 => Exit" << endl;
                cout << "    " << "1 => Print Menu" << endl;
                cout << "    " << "2 => Check if path exists" << endl;
            }

            else if( option == 2 )
            {
                cout << "From what station are you leaving? ";
                cin >> leaving;
                cout << "To what station are you traveling? ";
                cin >> traveling;

                if( myGraph.isRoute( myGraph.stationNumber(leaving), myGraph.stationNumber(traveling) ) == true )
                {
                    cout << "There is a route between " << leaving << " and " << traveling
                    << " with a minimum distance of " << myGraph.shortestRoute( leaving, traveling ) << " miles." << endl;
                    myGraph.printPath( leaving, traveling );
                }

                else
                    cout << "There is no route between " << leaving << " and " << traveling << "." << endl;
            }

            else
                cout << "Invalid input!" << endl;

            cin >> option;

        }
    }

    return 0;
}
