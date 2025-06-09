// Code i copeid (template) iz here: https://www.sfml-dev.org/documentation/3.0.0/
// its the "short eggsample" part
// it threw like 3 errors ,how tf is that simple @sfml-devs

#include <cmath>

// #include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
using namespace sf;

// For debugging purposes only !1!!1!!!!! deletz me for releasez or smth thatrs rpolly a good idea !!!11!
#include <iostream>
using namespace std;

class Player
{
private:
    /**
     * @returns A vector of the new grid position
     */
    Vector2i snapToGrid()
    {
        velocity = Vector2f();

        int col = floor(pos.x / sideLength);
        int row = floor(pos.y / sideLength);

        pos.x = sideLength * (col + 0.5f);
        pos.y = sideLength * (row + 0.5f);

        return Vector2i(col, row);
    }

public:
    const int sideLength = 20;
    // Arbitrary constant - can't equal 1 (/0 error)
    const float shrinkSpeed = 1.4f;

    // 0 for in host, 1 for entering host, 2 for leaving host, 3 for moving
    uint8_t spiritState = 0;
    float spiritSize = 0.7f;

    //                  up     down   left   right  jump
    bool moveKeys[5] = {false, false, false, false, false};

    // Frames left to jump
    uint8_t jumpFrames = 4;

    // Arbitrary constant
    const float speed = 1.5f;

    Vector2f pos;
    Vector2f velocity = Vector2f();

    Player(Vector2f startPos)
    {
        pos = startPos;
    }

    // omg i finally understnadnd the & symbol this is insane :0
    void draw(RenderWindow &window) // <- shoutout chatgpt for telling me to add the '&' which fixed shit for some reason
    {
        if (spiritState <= 1)
        {
            float inRadius = sideLength / 2;

            ConvexShape body;
            body.setFillColor(Color::Black);
            body.setPointCount(4);

            body.setPoint(0, {pos.x - inRadius + velocity.x, pos.y - inRadius});
            body.setPoint(1, {pos.x + inRadius + velocity.x, pos.y - inRadius});
            body.setPoint(2, {pos.x + inRadius, pos.y + inRadius});
            body.setPoint(3, {pos.x - inRadius, pos.y + inRadius});

            inRadius *= 0.7;

            window.draw(body);

            ConvexShape spirit;
            spirit.setFillColor(Color::White);
            spirit.setPointCount(4);

            spirit.setPoint(0, {pos.x - inRadius + velocity.x, pos.y - inRadius});
            spirit.setPoint(1, {pos.x + inRadius + velocity.x, pos.y - inRadius});
            spirit.setPoint(2, {pos.x + inRadius, pos.y + inRadius});
            spirit.setPoint(3, {pos.x - inRadius, pos.y + inRadius});

            window.draw(spirit);
        }
        else
        {
            float inRadius = sideLength * spiritSize / 2;

            ConvexShape spirit;
            spirit.setFillColor(Color::White);
            spirit.setPointCount(4);

            spirit.setPoint(0, {pos.x - inRadius, pos.y - inRadius});
            spirit.setPoint(1, {pos.x + inRadius, pos.y - inRadius});
            spirit.setPoint(2, {pos.x + inRadius, pos.y + inRadius});
            spirit.setPoint(3, {pos.x - inRadius, pos.y + inRadius});

            window.draw(spirit);
        }
    }

    template <size_t rows, size_t cols>
    void update(int (&blocks)[rows][cols], Vector2u size)
    {
        // TODO: make this work properly ( = dT / 16)
        const float dTMult = 1;

        if (spiritState == 1)
        {
            Vector2i res = snapToGrid();

            blocks[res.y][res.x] = 0;

            spiritState = 0;
        }
        else if (spiritState == 2)
        {
            Vector2i res = snapToGrid();

            blocks[res.y][res.x] = 1;

            spiritState = 3;
        }

        if (spiritState == 0)
        {
            // TODO: Add dT to changing spirit size
            spiritSize = spiritSize > 0.65f ? 0.7f : (spiritSize + 0.7f * (shrinkSpeed - 1)) / shrinkSpeed;
            // spiritSize += 0.7f * (shrinkSpeed - 1);
            // spiritSize /= shrinkSpeed;

            velocity.y += (sideLength / 30) * dTMult;

            if ((0 < jumpFrames) && (moveKeys[0] || moveKeys[4]))
            {
                velocity.y = -10;
                jumpFrames = 0;
            }

            if (moveKeys[2] && !moveKeys[3])
            {
                velocity.x -= speed * dTMult;
            }
            else if (!moveKeys[2] && moveKeys[3])
            {
                velocity.x += speed * dTMult;
            }

            // Limits the speed with stuff idk
            velocity.x *= 0.8;

            // Collisiony shit
            Vector2f nextPos({pos.x + velocity.x * dTMult, pos.y + velocity.y * dTMult});

            float inRadius = sideLength / 2;

            // Border collisions
            if (nextPos.x - inRadius < 0)
            {
                nextPos.x = inRadius;
                velocity.x = velocity.x < 0 ? 0 : velocity.x;
            }
            else if (size.x < nextPos.x + inRadius)
            {
                nextPos.x = size.x - inRadius;
                velocity.x = velocity.x > 0 ? 0 : velocity.x;
            }

            if (nextPos.y - inRadius < 0)
            {
                nextPos.y = inRadius;
                velocity.y = velocity.y < 0 ? 0 : velocity.x;
            }
            else if (size.y < nextPos.y + inRadius)
            {
                nextPos.y = size.y - inRadius;
                velocity.y = velocity.y > 0 ? 0 : velocity.x;
                jumpFrames = 4;
            }
            else if (0 < jumpFrames)
            {
                jumpFrames--;
            }

            // Block collisions
            // TODO: Block collisions
            /* code */

            pos = nextPos;
        }
        else if (spiritState == 3)
        {
            // TODO: Add dT to changing spirit size
        }
    }
};

int main()
{
    // Create the main window      Yes thank you templatecode comments :D
    RenderWindow window(VideoMode({800, 600}), "Luminx", Style::Default);

    /* Template code for later use (100% not gonna use this)
    // Load a sprite to display
    const Texture texture("../assets/eg-img.jpeg");
    Sprite sprite(texture);
    */

    int blocks[30][60] = {
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

    Player player = Player(Vector2f(40, 40));

    while (window.isOpen())
    {
        // TODO: figure out transitions to/from different "pages"
        // also figure out the "pages" themselves

        // EVENTS!!!!!!!!!!! (the loop is still just unmodified template code)
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<Event::Closed>())
            {
                window.close();
            }
            else if (const auto *resized = event->getIf<Event::Resized>())
            {
                window.setView(View(FloatRect({0.f, 0.f}, Vector2f(resized->size))));
            }
            else if (const auto *keyPressed = event->getIf<Event::KeyPressed>())
            {
                switch (keyPressed->scancode)
                {
                case Keyboard::Scancode::W:
                    player.moveKeys[0] = true;
                    break;

                case Keyboard::Scancode::S:
                    player.moveKeys[1] = true;
                    break;

                case Keyboard::Scancode::A:
                    player.moveKeys[2] = true;
                    break;

                case Keyboard::Scancode::D:
                    player.moveKeys[3] = true;
                    break;

                case Keyboard::Scancode::Space:
                    player.moveKeys[4] = true;
                    break;

                default:
                    cout << "Uncaught keypress" << endl;
                    break;
                }
            }
            else if (const auto *keyReleased = event->getIf<Event::KeyReleased>())
            {
                switch (keyReleased->scancode)
                {
                case Keyboard::Scancode::W:
                    player.moveKeys[0] = false;
                    break;

                case Keyboard::Scancode::S:
                    player.moveKeys[1] = false;
                    break;

                case Keyboard::Scancode::A:
                    player.moveKeys[2] = false;
                    break;

                case Keyboard::Scancode::D:
                    player.moveKeys[3] = false;
                    break;

                case Keyboard::Scancode::Space:
                    player.moveKeys[4] = false;
                    break;

                default:
                    cout << "Uncaught keyrelease" << endl;
                    break;
                }
            }
        }

        window.clear(Color(50, 50, 50));

        /* That pesky template code again
        // Draw the sprite
        window.draw(sprite);
        */

        // Updating the stuff
        player.update(blocks, window.getSize());

        // drawugng stuff
        size_t xLength = sizeof(blocks[0]) / sizeof(blocks[0][0]);
        size_t yLength = sizeof(blocks) / (sizeof(blocks[0]));

        for (size_t row = 0; row < sizeof(blocks) / sizeof(blocks[0]); row++)
        {
            for (size_t col = 0; col < sizeof(blocks[0]) / sizeof(blocks[0][0]); col++)
            {
                switch (blocks[row][col])
                {
                case 0:
                    // do nothing :D
                    break;
                case 1:
                {
                    RectangleShape block({20.f, 20.f});
                    block.setFillColor(Color::Black);

                    block.setPosition({20.f * col, 20.f * row});

                    window.draw(block);
                    break;
                }
                default:
                    // TODO: add the row and col to this error message
                    throw "Unknown block type at ";
                    break;
                }
            }
        }

        player.draw(window);

        window.display();
    }

    return 0;
}
