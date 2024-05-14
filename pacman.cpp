#include "SFML/Graphics.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

using namespace std;
using namespace sf;

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
    Player(Vector2i pos,Vector2i target_pos, int dir , int target_dir , float speed , Texture& texture){
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
                sprite.move(0 , speed * -dt);
            }
            break;
        case 2:
            if(canMove(pos.x , pos.y + 1)){
                if(sprite.getPosition().y / 16 > pos.y + 1)
                    pos.y++;
                sprite.move(0 , speed * dt);
            }
            break;
        case 3:
            if(canMove(pos.x - 1 , pos.y)){
                if(sprite.getPosition().x / 16 < pos.x - 1)
                    pos.x--;
                sprite.move(speed * -dt , 0);
            }
            break;
        case 4:
            if(canMove(pos.x + 1 , pos.y)){
                if(sprite.getPosition().x / 16 > pos.x + 1)
                    pos.x++;
                sprite.move(speed * dt , 0);
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



// Load map textures
void loadMapTextures(sf::Texture *&texts, int size) {
    texts[0].loadFromFile("sprites/map/0.png");
    texts[1].loadFromFile("sprites/map/1.png");
    texts[14].loadFromFile("sprites/map/14.png");
    texts[15].loadFromFile("sprites/map/15.png");
}




void updateGhosts(vector<Sprite> &ghostSprites, vector<int> &ghostDirections, 
                  vector<Texture> &upTextures, vector<Texture> &downTextures, 
                  vector<Texture> &leftTextures, vector<Texture> &rightTextures, float dt) {
    static float elapsed_time = 0.0f;
    elapsed_time += dt;

    if (elapsed_time >= 0.2f) { 
        elapsed_time = 0.0f; 

        for (size_t i = 0; i < ghostSprites.size(); ++i) {
            int newX = ghostSprites[i].getPosition().x / 16;
            int newY = ghostSprites[i].getPosition().y / 16;

            int nextX = newX;
            int nextY = newY;

            switch (ghostDirections[i]) {
                case 0: // up
                    nextY--;
                    break;
                case 1: // down
                    nextY++;
                    break;
                case 2: // left
                    nextX--;
                    break;
                case 3: // right
                    nextX++;
                    break;
            }

            if (canMove(nextX, nextY)) {
                ghostSprites[i].setPosition(nextX * 16, nextY * 16);
                newX = nextX;
                newY = nextY;
            } else {
                vector<int> validDirections;
                for (int j = 0; j < 4; j++) {
                    int dX = (j == 2) ? -1 : (j == 3) ? 1 : 0;
                    int dY = (j == 0) ? -1 : (j == 1) ? 1 : 0;
                    if (canMove(newX + dX, newY + dY)) {
                        validDirections.push_back(j);
                    }
                }
                if (!validDirections.empty()) {
                    ghostDirections[i] = validDirections[rand() % validDirections.size()];
                }
            }

            // Update ghost's texture based on its direction
            switch (ghostDirections[i]) {
                case 0:
                    ghostSprites[i].setTexture(upTextures[i]);
                    break;
                case 1:
                    ghostSprites[i].setTexture(downTextures[i]);
                    break;
                case 2:
                    ghostSprites[i].setTexture(leftTextures[i]);
                    break;
                case 3:
                    ghostSprites[i].setTexture(rightTextures[i]);
                    break;
            }
        }
    }
}


// void movePlayer(Sprite &playerSprite, Keyboard::Key direction) {
//     int newX = playerSprite.getPosition().x / 16;
//     int newY = playerSprite.getPosition().y / 16;
//     switch (direction) {
//         case Keyboard::Up:
//             if (canMove(newX, newY - 1)){
//                 playerSprite.move(0, -16);
//                 CollectPallets(newX , newY - 1);
//             }
//             break;
//         case Keyboard::Down:
//             if (canMove(newX, newY + 1)){
//                 playerSprite.move(0, 16);
//                 CollectPallets(newX , newY + 1);
//             }
//             break;
//         case Keyboard::Left:
//             if (newX <= 0){
//                 playerSprite.move(26*16,0);
//                 CollectPallets(26 , newY);
//             }
//             else if (canMove(newX - 1, newY)){
//                 playerSprite.move(-16, 0);
//                 CollectPallets(newX - 1, newY);
//             }
//             break;
//         case Keyboard::Right:
//             if (newX >= 26){
//                 playerSprite.move(-26*16,0);
//                 CollectPallets(0, newY);
//             }
//             else if (canMove(newX + 1, newY)){
//                 playerSprite.move(16, 0);
//                 CollectPallets(newX + 1, newY);
//             }
//             break;
//     }
// }

// void movePlayer(Sprite &playerSprite, Keyboard::Key keyPressed) {
//     int newX = playerSprite.getPosition().x / 16;
//     int newY = playerSprite.getPosition().y / 16;
//     switch (keyPressed) {
//         case Keyboard::Up:
//             newY--;
//             break;
//         case Keyboard::Down:
//             newY++;
//             break;
//         case Keyboard::Left:
//             newX--;
//             break;
//         case Keyboard::Right:
//             newX++;
//             break;
//     }
//     if (canMove(newX, newY)) {
//         playerSprite.setPosition(newX * 16, newY * 16);
//         CollectPallets(newX, newY);
//     } else {
//         if((playerSprite.getPosition().x / 16) == 26 && (newX == 27)){
//                 playerSprite.setPosition(0, newY * 16);
//         }
//         else if((playerSprite.getPosition().x / 16) == 0 && (newX == -1)){
//             playerSprite.setPosition(26 * 16,newY * 16);
//         }
//     }
//     cout<<newX << " "<<newY<<endl;
//     cout<<"Position : "<<playerSprite.getPosition().x / 16 <<" "<<playerSprite.getPosition().y /16<<endl;
// }




int main() {
    sf::RenderWindow window(sf::VideoMode(576, 432), "Pacman");
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
    const int numGhosts = 4;
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
    Player pacman(Vector2i(13,23),Vector2i(0,0),0,0,50.f,pacmanTexture);



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
        updateGhosts(ghostSprites, ghostDirections, upTextures, downTextures, leftTextures, rightTextures, dt);
        pacman.playerMove();
        // Check collisions with ghosts
        for (size_t i = 0; i < ghostSprites.size(); ++i) {
            if (pacman.sprite.getGlobalBounds().intersects(ghostSprites[i].getGlobalBounds())) {
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
            window.draw(ghostSprites[i]);
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
