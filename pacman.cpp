#include "SFML/Graphics.hpp"
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include <queue>

using namespace std;
using namespace sf;


RenderWindow window(sf::VideoMode(576, 432), "Pacman");


const int mapSize = 27;
int mapArray[mapSize][mapSize];
float dt;
float powerPallet = 0;
int score = 0;



bool canMove(int x, int y) {
    return (mapArray[y][x] == 1) || (mapArray[y][x] == 14) || (mapArray[y][x] == 15);
}

struct Player{
    Vector2i pos;
    Vector2i targetPos;
    int dir;
    int targetDir;
    float speed;
    Sprite sprite;
    float plTimer;
    Clock plClock;
    Player(){}
    void initPlayer(Vector2i pos,Vector2i target_pos, int dir , int target_dir , float speed , Texture& texture){
        this->pos = pos;
        this->dir = dir;
        this->targetPos = target_pos;
        this->targetDir = target_dir;
        this->speed = speed;
        this->sprite.setTexture(texture);
        this->sprite.setPosition(pos.x * 16,pos.y * 16);
    }
    void playerMove(){
        if(mapArray[pos.y][pos.x] == 14){
            score++;
            mapArray[pos.y][pos.x] = 1;
        }
        if(pos.x == 26 && dir == 4){
            pos.x = 0;
            sprite.setPosition(0,sprite.getPosition().y);
            return;
        }
        else if(pos.x == 0 && dir == 3){
            pos.x = 26;
            sprite.setPosition(26 * 16 , sprite.getPosition().y);
            return;
        }
        if(canMove(pos.x+targetPos.x,pos.y+targetPos.y)){
            dir = targetDir;
        }

        switch (dir)
        {
        case 1:
            if(canMove(pos.x , pos.y - 1)){
                if(sprite.getPosition().y / 16 < pos.y - 1)
                    pos.y--;
                sprite.move(0 , speed * -plTimer);
            }
            break;
        case 2:
            if(canMove(pos.x , pos.y + 1)){
                if(sprite.getPosition().y / 16 > pos.y + 1)
                    pos.y++;
                sprite.move(0 , speed * plTimer);
            }
            break;
        case 3:
            if(canMove(pos.x - 1 , pos.y)){
                if(sprite.getPosition().x / 16 < pos.x - 1)
                    pos.x--;
                sprite.move(speed * -plTimer , 0);
            }
            break;
        case 4:
            if(canMove(pos.x + 1 , pos.y)){
                if(sprite.getPosition().x / 16 > pos.x + 1)
                    pos.x++;
                sprite.move(speed * plTimer , 0);
            }
            break;
        default:
            break;
        }
    }

    void setTarget(Keyboard::Key key){
        if(Keyboard::isKeyPressed(Keyboard::W)){
            targetPos.x = 0;
            targetPos.y = -1;
            targetDir = 1; 
        }
        if(Keyboard::isKeyPressed(Keyboard::S)){
            targetPos.x = 0;
            targetPos.y = +1;
            targetDir = 2; 
        }
        if(Keyboard::isKeyPressed(Keyboard::A)){
            targetPos.x = -1;
            targetPos.y = 0;
            targetDir = 3; 
        }
        if(Keyboard::isKeyPressed(Keyboard::D)){
            targetPos.x = 1;
            targetPos.y = 0;
            targetDir = 4; 
        }
    }
    void killPlayer(){
        sprite.setPosition(13 * 16,23 * 16);
        targetDir = 0;
        targetPos.x = 0;
        targetPos.y = 0;
        pos.x = 13;
        pos.y = 23;
        dir = 0;
    }
};

struct Ghosts{
    Vector2i pos;
    Vector2i targetPos;
    int dir ;
    int targetDir;
    float speed;
    Sprite sprite;
    float gtimer;
    Clock gtClock;
    Ghosts(){dir = 0 ; targetDir = 0; targetPos.x= 0 ; targetPos.y = 0;}

    void moveGhost(){
        //cout<<" Tar: "<<targetPos.x<<" "<<targetPos.y<<endl;
        //cout<<" Pos: "<<pos.x <<" "<<pos.y<<endl;
        //Decreasing difficulty
        // int random = rand() % 6;
        // if(random < 4){
            if(pos.y > targetPos.y){
                dir = 1;
            }
            else if(pos.y < targetPos.y){
                dir = 2;
            }
            else if(pos.x > targetPos.x){
                dir = 3;
            }
            else if(pos.x < targetPos.x){
                dir = 4;
            }
        //}
        // else{
        //     int randomDir = rand() % 5;
        //     dir = randomDir;
        // }
        //cout<<"dir"<<dir<<endl;
        switch (dir)
        {
        case 1:
            if(canMove(pos.x , pos.y - 1)){
                if(sprite.getPosition().y / 16 < pos.y - 1)
                    pos.y--;
                sprite.move(0 , speed * -gtimer);
            }
            break;
        case 2:
            if(canMove(pos.x , pos.y + 1)){
                if(sprite.getPosition().y / 16 > pos.y + 1)
                    pos.y++;
                sprite.move(0 , speed * gtimer);
            }
            break;
        case 3:
            if(canMove(pos.x - 1 , pos.y)){
                if(sprite.getPosition().x / 16 < pos.x - 1)
                    pos.x--;
                sprite.move(speed * -gtimer , 0);
            }
            break;
        case 4:
            if(canMove(pos.x + 1 , pos.y)){
                if(sprite.getPosition().x / 16 > pos.x + 1)
                    pos.x++;
                sprite.move(speed * gtimer , 0);
            }
            break;
        default:
            break;
        }
    }
    
};
const int numGhosts = 4;
Ghosts ghosts[numGhosts];
Player pacman;



// Function to find the shortest path using Breadth-First Search (BFS)
Vector2i findShortestPath(Vector2i start , Vector2i end){
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    bool visited[mapSize][mapSize] = {false};
    queue<Vector2i>q;
    Vector2i parent[mapSize][mapSize];
    Vector2i child;
    q.push(start);
    visited[start.x][start.y] = true;
     while (!q.empty()) {
        Vector2i curr = q.front();
        q.pop();

        if (curr == end) {
            
            while (!(curr.x == start.x && curr.y == start.y)) {
                child = curr;
                curr = {parent[curr.x][curr.y].x, parent[curr.x][curr.y].y};
            }
           
            return child;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];
            if (canMove(nx, ny) && !visited[nx][ny]) {
                visited[nx][ny] = true;
                parent[nx][ny] = {curr.x, curr.y};
                q.push({nx, ny});
            }
        }
    }
    return start;
    
}
//Thread function to handle the ghost movement
void *ghostMovement(void *arg) {
    int ghostIndex = *((int*)arg);
    
    while (window.isOpen()) {
        ghosts[ghostIndex].gtimer = ghosts[ghostIndex].gtClock.restart().asSeconds();
        //cout<<dtGhost<<endl;
        // Calculate the shortest path to the player's position using BFS
        Vector2i ghostTarget = findShortestPath(ghosts[ghostIndex].pos, pacman.pos);
        ghosts[ghostIndex].targetPos = ghostTarget;

        ghosts[ghostIndex].moveGhost();
        //sleep(milliseconds(15));
    }
    return NULL;
}
//Thread function to handle the player movement
void *playerManagement(void *arg){
    while (window.isOpen())
    {
        pacman.plTimer = pacman.plClock.restart().asSeconds();
        pacman.playerMove();
    }
    
}

// Load map textures
void loadMapTextures(sf::Texture *&texts, int size) {
    texts[0].loadFromFile("sprites/map/0.png");
    texts[1].loadFromFile("sprites/map/1.png");
    texts[14].loadFromFile("sprites/map/14.png");
    texts[15].loadFromFile("sprites/map/15.png");
}





int main() {
    srand(time(0));
    window.setFramerateLimit(120);

    // Map textures
    int noTextures = 18;
    sf::Texture *mapText = new sf::Texture[noTextures];
    loadMapTextures(mapText, noTextures);

    sf::Sprite *mapSprites = new sf::Sprite[noTextures];
    for (int i = 0; i < noTextures; i++) {
        mapSprites[i].setTexture(mapText[i]);
    }

    std::ifstream ifs("maps/map1.txt");
    if (ifs.is_open()) {
        char a, b;
        for (int i = 0; i < mapSize; i++) {
            for (int j = 0; j < mapSize; j++) {
                ifs.get(a);
                if (a == '\n' || a == ' ')
                    j--;
                else {
                    ifs.get(b);
                    mapArray[i][j] = (a - '0') * 10 + (b - '0');
                }
            }
        }
    }
    ifs.close();

    // Print map array
    // for (int i = 0; i < mapSize; i++) {
    //     for (int j = 0; j < mapSize; j++) {
    //         if(mapArray[i][j] < 10)
    //             cout <<"0"<< mapArray[i][j] << " ";
    //         else
    //             cout << mapArray[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    // Create ghosts
    
    pthread_t tid[numGhosts];
    Texture ghostTexture;
    ghostTexture.loadFromFile("sprites/str.png");
    for(int i = 0 ; i< numGhosts ; i++){
        int* ghostIndex = new int(i);
        ghosts[i].pos.y = 13;
        ghosts[i].pos.x = 13+i;
        ghosts[i].sprite.setTexture(ghostTexture);
        ghosts[i].sprite.setPosition(ghosts[i].pos.x * 16 , ghosts[i].pos.y * 16);
        //ghosts[i].sprite.setPosition(11*16 , 23*16);
        ghosts[i].speed = 50.f;
        int arg[1];
        arg[0] = i;
        pthread_create(&tid[i], NULL, ghostMovement, (void*)ghostIndex);
    }
    
    vector<Sprite> ghostSprites(numGhosts);
    vector<Texture> upTextures(numGhosts), downTextures(numGhosts), 
                    leftTextures(numGhosts), rightTextures(numGhosts);
    vector<int> ghostDirections(numGhosts, 0); // 0: up, 1: down, 2: left, 3: right

    for (int i = 0; i < numGhosts; ++i) {
        upTextures[i].loadFromFile("sprites/str.png");
        downTextures[i].loadFromFile("sprites/str.png");
        leftTextures[i].loadFromFile("sprites/str.png");
        rightTextures[i].loadFromFile("sprites/str.png");
        ghostSprites[i].setTexture(upTextures[i]);
    }

    ghostSprites[0].setPosition(((mapSize/2) + 1 )* 16, ((mapSize/2) + 1)* 16);
    ghostSprites[1].setPosition(100 , 400);
    ghostSprites[2].setPosition(100 , 99);
    ghostSprites[3].setPosition(100 , 99);


    // Player (Pacman) setup
    Texture pacmanTexture;
    pacmanTexture.loadFromFile("sprites/player.png");
    // Sprite pacmanSprite;
    // pacmanSprite.setTexture(pacmanTexture);
    // pacmanSprite.setPosition(400, 100);
    pacman.initPlayer(Vector2i(13,23),Vector2i(0,0),0,0,50.f,pacmanTexture);

    pthread_t playerThreadID;
    pthread_create(&playerThreadID , NULL , playerManagement , NULL);


    int lives = 3;


    Font font;
    font.loadFromFile("sprites/Arial.ttf");
    Text livesText;
    Text scoreText;
    livesText.setFont(font);
    livesText.setCharacterSize(24);
    livesText.setFillColor(Color::White);
    livesText.setPosition(450, 10);

    scoreText.setFont(font);
    scoreText.setCharacterSize(24);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(450, 40);


    // Main loop
    sf::Clock dtClock;
    while (window.isOpen() && lives>=0) {
        //cout<<"x:"<<pacmanSprite.getPosition().x<<"  y:"<<pacmanSprite.getPosition().y;

        dt = dtClock.restart().asSeconds();

        sf::Event sfEvent;
        while (window.pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed)
                window.close();
            if (sfEvent.type == sf::Event::KeyPressed) {
                if (sfEvent.key.code == sf::Keyboard::Escape)
                    window.close();
                

                pacman.setTarget(sfEvent.key.code);
                //movePlayer(pacmanSprite, sfEvent.key.code);
            }
        }

        // Update ghosts
        //updateGhosts(ghostSprites, ghostDirections, upTextures, downTextures, leftTextures, rightTextures, dt);
        //pacman.playerMove();
        
        // Check collisions with ghosts
        for (size_t i = 0; i < numGhosts; ++i) {
            if (pacman.sprite.getGlobalBounds().intersects(ghosts[i].sprite.getGlobalBounds())) {
                lives--;
                pacman.killPlayer();
            }
        }



        /////////////////////////////////Render////////////////////////////

        window.clear();

        for (int i = 0; i < mapSize; i++) {
            for (int j = 0; j < mapSize; j++) {
                if(mapArray[i][j] == 14){
                    mapSprites[1].setPosition(j*16,i*16);
                    window.draw(mapSprites[1]);
                }
                else if(mapArray[i][j] == 15){
                    mapSprites[1].setPosition(j*16,i*16);
                    window.draw(mapSprites[1]);
                }
                mapSprites[mapArray[i][j]].setPosition(j*16,i*16);
                window.draw(mapSprites[mapArray[i][j]]);
            }
        }

        window.draw(pacman.sprite);

        for (int i = 0; i < numGhosts; ++i) {
            window.draw(ghosts[i].sprite);
        }

        // Draw lives
        
        livesText.setString("Lives: " + to_string(lives));
        scoreText.setString("Score: " + to_string(score));
        window.draw(livesText);
        window.draw(scoreText);

        window.display();
    }

    return 0;
}
