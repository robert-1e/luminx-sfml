#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>

class Player
{
public:
    const int sideLength = 20;

    // 0 for in host, 1 for entering host, 2 for leaving host, 3 for moving
    int spiritState = 0;
    float spiritSize = 0.7f;

    //                  up     down   left   right  jump
    bool movement[5] = {false, false, false, false, false};

    // Frames left to jump - jumpframes-- every frame
    int jumpFrames = 4;

    // Arbitrary constant
    const float speed = 1.5f;

    sf::Vector2f pos;
    sf::Vector2f velocity = sf::Vector2f(0, 0);

    Player(sf::Vector2f startPos)
    {
        pos = startPos;
    }

    void draw(sf::RenderWindow& window) // <- shoutout chatgpt for telling me to add the & which fixed shit for some reason
    {
        if (spiritState <= 1)
        {
            float inRadius = sideLength / 2;

            sf::ConvexShape body;
            body.setFillColor(sf::Color::Black);
            body.setPointCount(4);

            body.setPoint(0, {pos.x - inRadius + velocity.x, pos.y - inRadius});
            body.setPoint(1, {pos.x + inRadius + velocity.x, pos.y - inRadius});
            body.setPoint(2, {pos.x + inRadius, pos.y + inRadius});
            body.setPoint(3, {pos.x - inRadius, pos.y + inRadius});

            inRadius *= 0.7;

            sf::ConvexShape spirit;
            spirit.setFillColor(sf::Color::White);
            spirit.setPointCount(4);

            spirit.setPoint(0, {pos.x - inRadius + velocity.x, pos.y - inRadius});
            spirit.setPoint(1, {pos.x + inRadius + velocity.x, pos.y - inRadius});
            spirit.setPoint(2, {pos.x + inRadius, pos.y + inRadius});
            spirit.setPoint(3, {pos.x - inRadius, pos.y + inRadius});

            window.draw(body);
            window.draw(spirit);
        }
    }
};

int main()
{
    // Create the main window      Yes thank you templatecode comments :D
    sf::RenderWindow window(sf::VideoMode({800, 600}), "Luminx");

    /* Template code for later use (100% not gonna use this)
    // Load a sprite to display
    const sf::Texture texture("../assets/eg-img.jpeg");
    sf::Sprite sprite(texture);
    */

    Player player = Player(sf::Vector2f(40, 40));

    while (window.isOpen())
    {
        // EVENTS!!!!!!!!!!!
        while (const std::optional event = window.pollEvent())
        {
            //      is '->is<T_>' meant to be typeof?
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();

        /* That pesky template code again
        // Draw the sprite
        window.draw(sprite);
        */

        // blank lines for screenshot
        // ...
        // ...
        // ...
        // ...
        // ...
        // ...
        // ...
        // ...

        player.draw(window);

        window.display();
    }
}