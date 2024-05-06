#include "GraphicsSettings.cpp"



using namespace std;


class Game
{
private:
    sf::RenderWindow* window;
	sf::Event sfEvent;
    GraphicSettings gfxsettings;
    sf::CircleShape circle;
	sf::Clock dtClock;
	float dt;

	//std::stack<State*> states;

	//std::map<std::string, int> supportedKeys;

	//float gridSize;
public:
    Game(/* args */) {
        this->initVariable();
        this->initGraphicsSettings();
        this->initWindow();
    }
    virtual ~Game() {}


    void initVariable() {
        this->window = nullptr;
        this->dt = 0.f;
        this->gridSize = 100.f;
        this->circle.setRadius(10.f);
        this->circle.setFillColor(sf::Color::Red);
        this->circle.setPosition(50.f,50.f);
    }
    void initGraphicsSettings() {
        this->gfxsettings.loadFromFile("Config/graphics.ini");
    }
    void initWindow() {
        
        if(this->gfxsettings.fullscreen)
            this->window = new sf::RenderWindow(this->gfxsettings.resolution, this->gfxsettings.title , sf::Style::Fullscreen , this->gfxsettings.contextSettings);
        else
            this->window = new sf::RenderWindow(this->gfxsettings.resolution, this->gfxsettings.title, sf::Style::Titlebar | sf::Style::Close, this->gfxsettings.contextSettings);
            
        this->window->setFramerateLimit(this->gfxsettings.frameRateLimit);
        this->window->setVerticalSyncEnabled(this->gfxsettings.vsync);
    }

    void updateSFMLEvent() {
        while (window->pollEvent(sfEvent)) {
            if (sfEvent.type == sf::Event::Closed)
                window->close();
        }
    }

    void updateDt() {
        dt = this->dtClock.restart().asSeconds();
    }

    void update() {
        updateSFMLEvent();  
    }

    void render() {
        window->clear();

        window->draw(circle);    

        window->display();
    }

    void run() {
        while (this->window->isOpen()) {
            updateDt();
            update();
            render();
        }
    }

};