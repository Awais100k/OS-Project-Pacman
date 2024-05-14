#include "SFML/Graphics.hpp"
#include "SFML/Audio.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

const int mapSize = 27;
int mapArray[mapSize][mapSize];
float dt;


//Load map Textures
void loadMapTextures(sf::Texture *&texts,int size){
    texts[0].loadFromFile("sprites/sky.png");
    texts[1].loadFromFile("sprites/path.png");
    // texts[2].loadFromFile("sprites/up_left_o.png");
    // texts[3].loadFromFile("sprites/up_mid_o.png");
    // texts[4].loadFromFile("sprites/up_right_o.png");
    // texts[5].loadFromFile("sprites/mid_left_o.png");
    // texts[6].loadFromFile("sprites/mid_right_o.png");
    // texts[7].loadFromFile("sprites/down_left_o.png");
    // texts[8].loadFromFile("sprites/down_mid_o.png");
    // texts[9].loadFromFile("sprites/down_right_o.png");
    // texts[10].loadFromFile("sprites/up_left.png");
    // texts[11].loadFromFile("sprites/up_mid.png");
    // texts[12].loadFromFile("sprites/up_right.png");
    // texts[13].loadFromFile("sprites/mid_left.png");
    // texts[14].loadFromFile("sprites/mid_right.png");
    // texts[15].loadFromFile("sprites/down_left.png");
    // texts[16].loadFromFile("sprites/down_mid.png");
    // texts[17].loadFromFile("sprites/down_right.png");
}





int main(){
   

    //SFML VARIABLES
    sf::RenderWindow* window;
	sf::Event sfEvent;
	sf::Clock dtClock;
    
    //Init Window
    window = new sf::RenderWindow(sf::VideoMode(576,576), "Pacman");
    window->setFramerateLimit(120);

    //Map textures;
    int noTextures = 2;
    sf::Texture *mapText = new sf::Texture[noTextures];
    loadMapTextures(mapText,noTextures);
    


    sf::Sprite *mapSprites = new sf::Sprite[noTextures];    
    for (int i = 0; i < noTextures; i++)
    {
        mapSprites[i].setTexture(mapText[i]);
    }
    

    //Loading the map from the text file
    std::ifstream ifs("maps/map1.txt");
    if (ifs.is_open()) {
        //int mapArray[mapSize][mapSize];
        char a, b; // Use two characters to read the number
        for (int i = 0; i < mapSize; i++) {
            for (int j = 0; j < mapSize; j++) {
                ifs.get(a); // Read the first digit
                if(a == '\n' ||  a == ' ')
                    j--;
                // else if (b == ' ') // If there's a space after the second digit
                //     mapArray[i][j] = (a - '0') * 10; // Convert first digit to integer
                else {
                    ifs.get(b); // Read the second digit
                    mapArray[i][j] = (a - '0') * 10 + (b - '0'); // Convert both digits to integer
                }
            }
        }
    }
    
    
    
    
    
    
    
    
    ifs.close();
    for (int i = 0; i < mapSize; i++)
        {
            for (int j = 0; j < mapSize; j++)
            {
                //mapArray[i][j] = 2;
                cout<<mapArray[i][j]<<" ";
            }
            cout<<endl;
        }
    //Main Loop

    while (window->isOpen()) {


        ////////////////////////////////Update//////////////////////////////////////////

        dt = dtClock.restart().asSeconds();
        while (window->pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed)
                window->close();
            if(sfEvent.type == sf::Event::KeyPressed){
                if(sfEvent.key.code == sf::Keyboard::Escape);
                    window->close();
            }
        }  

        



        //////////////////////////////Render///////////////////////////////////////////

        window->clear();

        for(int i = 0 ; i < mapSize ; i++){
            for(int j = 0 ; j < mapSize ; j++){
                if(mapArray[i][j] == 0){
                    mapSprites[0].setPosition(j*16,i*16);
                    window->draw(mapSprites[0]);
                }
                else if(mapArray[i][j] == 1){
                    mapSprites[1].setPosition(j*16,i*16);
                    window->draw(mapSprites[1]);
                }
                // else if(mapArray[i][j] == 2){
                //     mapSprites[2].setPosition(j*16,i*16);
                //     window->draw(mapSprites[2]);
                // }
                // else if(mapArray[i][j] == 3){
                //     mapSprites[3].setPosition(j*16,i*16);
                //     window->draw(mapSprites[3]);
                // }
                // else if(mapArray[i][j] == 4){
                //     mapSprites[4].setPosition(j*16,i*16);
                //     window->draw(mapSprites[4]);
                // }
                // else if(mapArray[i][j] == 5){
                //     mapSprites[5].setPosition(j*16,i*16);
                //     window->draw(mapSprites[5]);
                // }
                // else if(mapArray[i][j] == 6){
                //     mapSprites[6].setPosition(j*16,i*16);
                //     window->draw(mapSprites[6]);
                // }
                // else if(mapArray[i][j] == 7){
                //     mapSprites[7].setPosition(j*16,i*16);
                //     window->draw(mapSprites[7]);
                // }
                // else if(mapArray[i][j] == 8){
                //     mapSprites[8].setPosition(j*16,i*16);
                //     window->draw(mapSprites[8]);
                // }
                // else if(mapArray[i][j] == 9){
                //     mapSprites[9].setPosition(j*16,i*16);
                //     window->draw(mapSprites[9]);
                // }
                // else if(mapArray[i][j] == 10){
                //     mapSprites[10].setPosition(j*16,i*16);
                //     window->draw(mapSprites[10]);
                // }
                // else if(mapArray[i][j] == 11){
                //     mapSprites[11].setPosition(j*16,i*16);
                //     window->draw(mapSprites[11]);
                // }
                // else if(mapArray[i][j] == 12){
                //     mapSprites[12].setPosition(j*16,i*16);
                //     window->draw(mapSprites[12]);
                // }
                // else if(mapArray[i][j] == 13){
                //     mapSprites[13].setPosition(j*16,i*16);
                //     window->draw(mapSprites[13]);
                // }
                // else if(mapArray[i][j] == 14){
                //     mapSprites[14].setPosition(j*16,i*16);
                //     window->draw(mapSprites[14]);
                // }
                // else if(mapArray[i][j] == 15){
                //     mapSprites[15].setPosition(j*16,i*16);
                //     window->draw(mapSprites[15]);
                // }
                // else if(mapArray[i][j] == 16){
                //     mapSprites[16].setPosition(j*16,i*16);
                //     window->draw(mapSprites[16]);
                // }
                // else if(mapArray[i][j] == 17){
                //     mapSprites[17].setPosition(j*16,i*16);
                //     window->draw(mapSprites[17]);
                // }


        //cout<<map.grid[i][j][0].id<<endl;
            }
        } 


        //window->draw(circle);





        window->display();

    }


}
