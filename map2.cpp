#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <random>
#include <climits>
#include <unistd.h>
using namespace std;

class Tiles
{
private:
    /* data */
public:
    unsigned short id;
    bool up , down , left , right;
    sf::Sprite sprite;
    Tiles(int id , bool up , bool down , bool left , bool right) {
        this->id = id;
        this->up = up;
        this->down = down;
        this->left = left;
        this->right = right;
    }
    ~Tiles() {}
};

class Cell
{
private:
    /* data */
public:
    int row;
    int col;
    Cell(/* args */) {}
    ~Cell() {}
};

class WFC
{
private:
    /* data */
public:
    vector<vector<vector<Tiles>>> grid;
    vector<Tiles> patterns;
    Cell cell;
    int size;
    void initGrid(){
        grid.resize(size);
        for(int i = 0 ; i < size ; i++){
            // Resize each inner vector to match the size of the patterns vector
            grid[i].resize(size, patterns);
        }
    }


    WFC(int s , vector<Tiles> patterns) {
        
        this->size = s;
        this->patterns = patterns;
        //cout<<this->patterns.size()<<endl;
        //grid.assign
        this->initGrid();
        // for(int i = 0 ; i < size ; i++){
        //     for(int j = 0 ; j < size ; j++)
        //         cout<<"Size at "<<i<<" "<<j<<" is "<<grid[i][j].size()<<endl;
        // }
        this->collapseGrid();
    }

    bool isGridFullCollapsed(){
        for(int i = 0 ; i < size ; i++){
            for(int j = 0 ; j < size ; j ++){
                if(grid[i][j].size() > 1)
                    return false;
            }
        }
        return true;
    }

    void cellWithLowestEntropy(){
        //cout<<"REACHED LOWERING";
        int min = INT_MAX;
        for(int i = 0 ; i < size ; i++){
            for(int j = 0 ; j < size ; j++){
                if(!grid[i][j].empty() && grid[i][j].size() > 1 && min > grid[i][j].size()){
                    //cout<<"Grid size at "<<i<<" "<<j<<" is "<<grid[i][j].size()<<endl;
                    
                    min = grid[i][j].size();
                    cell.row = i;
                    cell.col = j;
                }
            }
        }
        //cout<<"Cell  "<<cell.row<<" "<<cell.col<<endl;
        //cout<<"LOWEST CHOSEN"<<endl;
    }
    
    void collapseGrid(){
        while (!isGridFullCollapsed())
        {
            /* code */
            cellWithLowestEntropy();
            
            //sleep(5);
            collapseCell();
            //cout<<"Collapsed"<<" Cell  "<<cell.row<<" "<<cell.col<<endl;
            lowerNeighboursEntropy();
            //cout<<"Entropy lowered"<<endl;
        }
        
    }

    void lowerNeighboursEntropy(){
        if(cell.row > 0 && !grid[cell.row - 1][cell.col].empty())
        for(int i =0 ; i < grid[cell.row-1][cell.col].size(); i++){
            if(grid[cell.row][cell.col][0].up != grid[cell.row-1][cell.col][i].down){
                grid[cell.row - 1][cell.col].erase(grid[cell.row - 1][cell.col].begin() + i);
                // Decrement the index after erasing an element
                i--;
            }
        }
        if(cell.row < size - 1 && !grid[cell.row + 1][cell.col].empty())
        for(int i =0 ; i < grid[cell.row+1][cell.col].size(); i++){
            if(grid[cell.row][cell.col][0].down != grid[cell.row+1][cell.col][i].up){
                grid[cell.row + 1][cell.col].erase(grid[cell.row + 1][cell.col].begin() + i);
                // Decrement the index after erasing an element
                i--;
            }
        }
        if(cell.col > 0 && !grid[cell.row][cell.col - 1].empty())
        for(int i =0 ; i < grid[cell.row][cell.col - 1].size(); i++){
            if(grid[cell.row][cell.col][0].left != grid[cell.row][cell.col - 1][i].right){
                grid[cell.row][cell.col - 1].erase(grid[cell.row][cell.col - 1].begin() + i);
                // Decrement the index after erasing an element
                i--;
            }
        }
        if(cell.col < size - 1 && !grid[cell.row][cell.col + 1].empty())
        for(int i =0 ; i < grid[cell.row][cell.col + 1].size(); i++){
            if(grid[cell.row][cell.col][0].right != grid[cell.row][cell.col + 1][i].left){
                grid[cell.row][cell.col + 1].erase(grid[cell.row][cell.col + 1].begin() + i);
                // Decrement the index after erasing an element
                i--;
            }
        }
    }
    void collapseCell(){
        
        int random = rand() % grid[cell.row][cell.col].size();
        cout<<grid[cell.row][cell.col].size()<<" "<<random<<endl;
        Tiles tile = grid[cell.row][cell.col][random];
        vector<Tiles> tile_vec;
        tile_vec.push_back(tile);
        grid[cell.row][cell.col] = tile_vec;
    }
    ~WFC() {}
};

int main(){
    srand(time(0));
    //std::cout<<"Hello\n";
    vector<Tiles> patterns;
    Tiles tile0(0,0,0,0,0);//blank
    Tiles tile1(1,1,0,1,1);//up
    Tiles tile2(2,0,1,1,1);//down
    Tiles tile3(3,1,1,1,0);//left
    Tiles tile4(4,1,1,0,1);//right
    Tiles tile5(5,1,1,0,0);//vertival
    Tiles tile6(6,0,0,1,1);//horizontal;
    Tiles tile7(7,1,0,1,0);//up_left
    Tiles tile8(8,1,0,0,1);//up_right
    Tiles tile9(9,0,1,1,0);//down_left
    Tiles tile10(10,0,1,0,1);//down_right
    Tiles tile11(7,1,1,1,1);//cross
    patterns.push_back(tile0);
    patterns.push_back(tile1);
    patterns.push_back(tile2);
    patterns.push_back(tile3);
    patterns.push_back(tile4);
    patterns.push_back(tile5);
    patterns.push_back(tile6);
    patterns.push_back(tile7);
    patterns.push_back(tile8);
    patterns.push_back(tile9);
    patterns.push_back(tile10);
    //patterns.push_back(tile11);

    WFC map(9,patterns);

    //cout<<endl<<endl;
    // for(int i = 0 ; i < map.size ; i++){
    //     for(int j = 0 ; j < map.size ; j++){
    //         //if(map.grid[i][j].empty())
    //         if(map.grid[i][j][0].id < 10)
    //         std::cout<<"0"<<map.grid[i][j][0].id << " ";
    //         else
    //         std::cout<<map.grid[i][j][0].id << " ";
    //     }
    //     std::cout<<std::endl;
    // }


    sf::Texture tex[11];

    tex[0].loadFromFile("res2/blank.png");
    tex[1].loadFromFile("res2/up.png");
    tex[2].loadFromFile("res2/down.png");
    tex[3].loadFromFile("res2/left.png");
    tex[4].loadFromFile("res2/right.png");
    tex[5].loadFromFile("res2/vertical.png");
    tex[6].loadFromFile("res2/horizontal.png");
    tex[7].loadFromFile("res2/up_left.png");
    tex[8].loadFromFile("res2/up_right.png");
    tex[9].loadFromFile("res2/down_left.png");
    tex[10].loadFromFile("res2/down_right.png");
    //tex[11].loadFromFile("res2/cross.png");


    float f = 2.5;

    for(int i = 0 ; i < map.size ; i++){
        for(int j = 0 ; j < map.size ; j++){
            if(!map.grid[i][j].empty()){
            map.grid[i][j][0].sprite.setTexture(tex[map.grid[i][j][0].id]);
            map.grid[i][j][0].sprite.setScale(sf::Vector2f(f,f));
            map.grid[i][j][0].sprite.setPosition(i * 32 * f , j * 32 * f);
            }
            //cout<<"Pos "<<map.grid[i][j][0].sprite.getPosition().x<<" "<<map.grid[i][j][0].sprite.getPosition().y<<endl;
        }
    }
    cout<<"All ok"<<endl;

    sf::RenderWindow window(sf::VideoMode(720, 720), "SFML works!");


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
       
        for(int i = 0 ; i < map.size ; i++){
            for(int j = 0 ; j < map.size ; j++){
                if(!map.grid[i][j].empty()){
                window.draw(map.grid[i][j][0].sprite);
                cout<<"Drawing at "<<i<<" "<<j<<endl;
                }
        //cout<<map.grid[i][j][0].id<<endl;
            }
        } 
        //window.draw(map.grid[2][0][0].sprite);
       // window.draw(firsttry_coord_map.shapes[0][0]);

        window.display();
    }





    

}