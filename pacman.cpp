#include "SFML/Graphics.hpp"
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

const int mapSize = 27;
int mapArray[mapSize][mapSize];
float dt;
float powerPallet = 0;

int score = 0;


// Load map textures
void loadMapTextures(sf::Texture *&texts, int size) {
    texts[0].loadFromFile("sprites/map/0.png");
    texts[1].loadFromFile("sprites/map/1.png");
    texts[14].loadFromFile("sprites/map/14.png");
    texts[15].loadFromFile("sprites/map/15.png");
}

bool canMove(int x, int y) {
    // if (x < 0 || x >= mapSize || y < 0 || y >= mapSize)
    //     return false;
    return (mapArray[y][x] == 1) || (mapArray[y][x] == 14) || (mapArray[y][x] == 15);
}
void CollectPallets(int x , int y){
    if(mapArray[y][x] == 14){
        score++;
        mapArray[y][x] = 1;
    }
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

void movePlayer(Sprite &playerSprite, Keyboard::Key keyPressed) {
     int newX = playerSprite.getPosition().x / 16;
    int newY = playerSprite.getPosition().y / 16;
    switch (keyPressed) {
        case Keyboard::Up:
            newY--;
            break;
        case Keyboard::Down:
            newY++;
            break;
        case Keyboard::Left:
            newX--;
            break;
        case Keyboard::Right:
            newX++;
            break;
    }
    if (canMove(newX, newY)) {
        
        playerSprite.setPosition(newX * 16, newY * 16);
        CollectPallets(newX, newY);
    } else {
        if((playerSprite.getPosition().x / 16) == 26 && (newX == 27)){
                playerSprite.setPosition(0, newY * 16);
        }
        else if((playerSprite.getPosition().x / 16) == 0 && (newX == -1)){
            playerSprite.setPosition(26 * 16,newY * 16);
        }
    }
    cout<<newX << " "<<newY<<endl;
    cout<<"Position : "<<playerSprite.getPosition().x / 16 <<" "<<playerSprite.getPosition().y /16<<endl;
}




int main() {
    sf::RenderWindow window(sf::VideoMode(576, 800), "Pacman");
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
    for (int i = 0; i < mapSize; i++) {
        for (int j = 0; j < mapSize; j++) {
            if(mapArray[i][j] < 10)
                cout <<"0"<< mapArray[i][j] << " ";
            else
                cout << mapArray[i][j] << " ";
        }
        cout << endl;
    }

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
    Sprite pacmanSprite;
    pacmanSprite.setTexture(pacmanTexture);
    pacmanSprite.setPosition(400, 100);
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

                movePlayer(pacmanSprite, sfEvent.key.code);
            }
        }

        // Update ghosts
        updateGhosts(ghostSprites, ghostDirections, upTextures, downTextures, leftTextures, rightTextures, dt);
        
        // Check collisions with ghosts
        for (size_t i = 0; i < ghostSprites.size(); ++i) {
            if (pacmanSprite.getGlobalBounds().intersects(ghostSprites[i].getGlobalBounds())) {
                lives--;
                pacmanSprite.setPosition(400, 100);
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

        window.draw(pacmanSprite);

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
