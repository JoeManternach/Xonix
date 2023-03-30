// Name: Joe Manternach
// Description: enhance the Xonix Game
// Date: 27 Mar 2023

#include <SFML/Graphics.hpp>
#include <time.h>
#include <SFML/Audio.hpp>
#include <string>

using namespace sf;

const int TILE_SIZE = 18; //tile size

const int yGRID_VALUE = 25;
const int xGRID_VALUE = 40;
int GRID[yGRID_VALUE][xGRID_VALUE] = {0};

//Name: Enemy
// purpose: to store information for loction of the enemy and also member functions
class Enemy{

public:

    // Constructor for an enemy object
    Enemy(){

        xPosition = 300;
        yPosition = 300;
        xChangeInPosition = 4 - rand() % 8;
        yChangeInPosition = 4 - rand() % 8;
    }

    // Getters for enemy objects sets there locations
    int getX() const { return xPosition; }
    int getYposition() const  { return yPosition; }
    int getXchangeInPosition() const { return xChangeInPosition; }
    int getYchangeInPosition() const { return yChangeInPosition; }

    // Setters for enemy objects
    void setXposition (const int xPositionVal) { xPosition = xPositionVal; }
    void setYposition (const int yPositionVal) { yPosition = yPositionVal; }

    void setXchangeInPosition (const int xChangeInPositionVal) { xChangeInPosition = xChangeInPositionVal; }
    void setYchangeInPosition (const int yChangeInPositionVal) { yChangeInPosition = yChangeInPositionVal; }

    // moves the enemy
    void moveEnemy(){

        // Moves once in the xPosition direction
        xPosition += xChangeInPosition;

        // if it is on a blue tile reverse the direction and then continue moving
        if (GRID[yPosition / TILE_SIZE][xPosition / TILE_SIZE] == 1){

            xChangeInPosition = -xChangeInPosition;
            xPosition += xChangeInPosition;
        }

        // Move once in the yPosition direction
        yPosition += yChangeInPosition;

        // If on a blue tile reverse direction and continue moving
        if (GRID[yPosition / TILE_SIZE][xPosition / TILE_SIZE] == 1){

            yChangeInPosition = -yChangeInPosition;
            yPosition += yChangeInPosition;
        }
    }

private:

    // value of xPosition and yPosition cannot be off of the board
    int xPosition;
    int yPosition;
    int xChangeInPosition;
    int yChangeInPosition;

};

// search for GRID elements around an enemy and prevents the enemy from being enclosed in blue tiles
void drop(int y,int x){

    // If the current tile is empty set it equal to a tile occupied by an enemy
    if (GRID[y][x] == 0){

        GRID[y][x] =- 1;
    }
    // call the drop function on a tile above the previous tile and set that equal to negative one so it doesn't get filled it
    if (GRID[y - 1][x] == 0){

        drop(y-1,x);
    }
    // call the drop function on a tile below the previous tile and set that equal to negative one so it doesn't get filled it
    if (GRID[y + 1][x] == 0){

        drop(y+1,x);
    }
    // call the drop function on a tile to the left of the previous tile and set that equal to negative one so it doesn't get filled it
    if (GRID[y][x - 1] == 0){

        drop(y,x-1);
    }
    // call the drop function on a tile to the right of the previous tile and set that equal to negative one so it doesn't get filled it
    if (GRID[y][x + 1] == 0){

        drop(y,x+1);
    }

}

int main(){

    // Seed for the random number function
    srand(time(0));

    // Loading in texture for the homepage background
    Texture homeScreenBackgroundTexture;
    homeScreenBackgroundTexture.loadFromFile("images/homeScreenBackground.jpg");
    if (!homeScreenBackgroundTexture.loadFromFile("images/homeScreenBackground.jpg")){

        return EXIT_FAILURE;
    }

    // Loading in Fonts for the score and also text on the homepage
    Font gameFont;

    gameFont.loadFromFile("fonts/gameFont.ttf");

    if (!gameFont.loadFromFile("fonts/gameFont.ttf")){

        return EXIT_FAILURE;
    }

    // Creating homescreen sprite
    Sprite homeScreenBackgroundSprite(homeScreenBackgroundTexture);

    // Window that the game will be played in
	RenderWindow gameWindow(VideoMode(xGRID_VALUE * TILE_SIZE, yGRID_VALUE * TILE_SIZE), "Xonix gameOn!");
	gameWindow.setFramerateLimit(60);

    // setting the position of the homescreen sprite
    homeScreenBackgroundSprite.setPosition(0,0);

    // homescreen variables
    bool homeScreenOpen = true;
    Event homeScreenEvent;

    // variables to track movement of the mouse wheel
    float yMouseWheelMovement = 0;
    float yChangeMouseWheelMovement = 0;

    // Homescreen loop
    while(homeScreenOpen){

        // setting the homescreen message
        Text titleText;
        titleText.setFont(gameFont);
        titleText.setString("Welcome to Xonix");
        titleText.setCharacterSize(30);
        titleText.setFillColor(Color (10000,0,0,1000));
        titleText.setPosition(200,50);

        // setting the instructions on the homescreen
        Text instructions;
        instructions.setFont(gameFont);
        instructions.setString("\tThe goal of Xonix is to fill in all the\n\ttiles without running into any enemies\n\tyou can use the arrow keys to move\n\n\n\n\n\t\t\tTo start the game scroll up!\n");
        instructions.setCharacterSize(18);
        instructions.setFillColor(Color (10000,0,0,1000));
        instructions.setPosition(120,120);

        // Open window and go to the homescreen
        while (gameWindow.pollEvent(homeScreenEvent)) {

            // If the user closes the gameWindow, end the game
            if (homeScreenEvent.type == Event::Closed) {

                // if the user presses the close button... end the loop and close the window
                gameWindow.close();
                homeScreenOpen = false;
            }

            // if the mouse wheel is moved enter
            if (homeScreenEvent.type == Event::MouseWheelMoved){

                // move the homescreen sprite sliding the homescreen of the window
                yChangeMouseWheelMovement = -70;
                yMouseWheelMovement += yChangeMouseWheelMovement;
                homeScreenBackgroundSprite.setPosition(0,yMouseWheelMovement);

                // when the home screen is fully off the screen end the loop and start the game
                const Vector2<float> OFF_SCREEN = {0,-450};
                if (homeScreenBackgroundSprite.getPosition().y <= OFF_SCREEN.y){

                    homeScreenOpen = false;
                }

            }
        }

        // draw everything to the screen
        gameWindow.clear();
        gameWindow.draw(homeScreenBackgroundSprite);
        gameWindow.draw(titleText);
        gameWindow.draw(instructions);
        gameWindow.display();
    }

    // Variables to represent the images in the game tileTexture = the titles, gameOverTexture = game over message, enemyTexture = the enemy
	Texture tileTexture;
    Texture gameOverTexture;
    Texture enemyTexture;

    // loading textures: titles, gameover, and enemy
	tileTexture.loadFromFile("images/tiles.png");
    gameOverTexture.loadFromFile("images/gameover.png");
    enemyTexture.loadFromFile("images/enemy.png");

    // Makes sure the files opened correctly
    if (!tileTexture.loadFromFile("images/tiles.png")){

        return EXIT_FAILURE;
    }
    if (!gameOverTexture.loadFromFile("images/gameover.png")){

        return EXIT_FAILURE;
    }
    if (!enemyTexture.loadFromFile("images/enemy.png")){

        return EXIT_FAILURE;
    }


    // Loading movement sound to the buffer and checking to make sure it loaded properly
    SoundBuffer buffer;
    buffer.loadFromFile("sounds/movementSound.wav");
    if (!buffer.loadFromFile("sounds/movementSound.wav")){

        return EXIT_FAILURE;
    }

    // Giving the movement sound a name movementSound.play() to play my sound
    Sound movementSound;
    movementSound.setBuffer(buffer);

    // creating a text for the score
    Text scoreText;
    scoreText.setFont(gameFont);


    // Initializing the member varibales and also sets the position for the enemy
	Sprite tileSprite(tileTexture);
    Sprite gameOverSprite(gameOverTexture);
    Sprite enemySprite(enemyTexture);

    // seting the position for the game over sprite
	gameOverSprite.setPosition(100, 100);
	enemySprite.setOrigin(20, 20);

    // Creates enemyArray variable to set how many enemies there will be, also creates an enemy object to represent each enemy
	int numberOfEnemies = 4;

    Enemy enemyArray[10];

    // Initialize key game information, location, timer, and delay
	bool gameOn = true;

	int xPOSITION = 0;
    int yPOSITION = 0;
    int xPOSITION_CHANGE = 0;
    int yPOSITION_CHANGE = 0;

    float timer = 0;
    float delay = 0.07;

    Clock clock;



    // Here we are using global constants to modify the GRID, making the blue border
	for (int yVal = 0; yVal < yGRID_VALUE; yVal++){

        for (int xVal = 0; xVal < xGRID_VALUE; xVal++){

            if (yVal == 0 || xVal == 0 || yVal == yGRID_VALUE - 1 || xVal == xGRID_VALUE - 1){
                GRID[yVal][xVal] = 1;
            }
        }
    }

    // gameOn loop stays open for as long as the gameWindow is open
    while (gameWindow.isOpen()){

        // getting the time that has past in seconds
        float time = clock.getElapsedTime().asSeconds();

        // restart the clock
        clock.restart();
        timer += time;

        // Creates an event userEvent with the purpose of tracking events by the user
        Event userEvent;

        // Loop will be ran when there is an event made by the user.
        while (gameWindow.pollEvent(userEvent)){

            // If the user closes the gameWindow, end the game
            if (userEvent.type == Event::Closed){

                gameWindow.close();
            }

            // if the user presses the esc key it resets the GRID to zero and places the user at enemyArray specific point
            if (userEvent.type == Event::KeyPressed){

                // if the user presses the escape key reset the grid
                if (userEvent.key.code == Keyboard::Escape){

                    for (int yVal = 1; yVal < yGRID_VALUE - 1; yVal++) {

                        for (int xVal = 1; xVal < xGRID_VALUE - 1; xVal++){

                            GRID[yVal][xVal] = 0;
                        }
                    }

                    // put the player here
                    xPOSITION = 10; yPOSITION = 0;
                    gameOn = true;
                }
            }
        }

        // Tracks the users movement of the player
        // Move left
        if (Keyboard::isKeyPressed(Keyboard::Left)){

            xPOSITION_CHANGE = -1;
            yPOSITION_CHANGE = 0;

            // play the sound after the delay has been overcome
            if (timer > delay){

                movementSound.play();
            }
        }

        // Move right
        if (Keyboard::isKeyPressed(Keyboard::Right)){

            xPOSITION_CHANGE = 1;
            yPOSITION_CHANGE = 0;

            // play the sound after the delay has been overcome
            if (timer > delay){

                movementSound.play();
            }
        }

        // Move up
        if (Keyboard::isKeyPressed(Keyboard::Up)){

            xPOSITION_CHANGE = 0;
            yPOSITION_CHANGE = -1;

            // play the sound after the delay has been overcome
            if (timer > delay){

                movementSound.play();
            }
        }

        // Move down
        if (Keyboard::isKeyPressed(Keyboard::Down)){

            xPOSITION_CHANGE = 0;
            yPOSITION_CHANGE = 1;

            // play the sound after the delay has been overcome
            if (timer > delay){

                movementSound.play();
            }
        }

        // If game is over end the loop
        if (gameOn) {

            // if the timer value has been acheived moveEnemy the player
            if (timer > delay) {

                xPOSITION += xPOSITION_CHANGE;
                yPOSITION += yPOSITION_CHANGE;

                // Keeps the player inside the boundry on the xPosition axis
                if (xPOSITION < 0) {

                    xPOSITION = 0;
                }
                if (xPOSITION > xGRID_VALUE - 1) {

                    xPOSITION = xGRID_VALUE - 1;
                }

                // Keeps player inside the boundry on the Y axis
                if (yPOSITION < 0) {

                    yPOSITION = 0;
                }
                if (yPOSITION > yGRID_VALUE - 1) {

                    yPOSITION = yGRID_VALUE - 1;
                }

                // Ends the game if the player runs over their trail because it is already green
                if (GRID[yPOSITION][xPOSITION] == 2) {

                    gameOn = false;
                }

                // if the spot is empty make it green
                if (GRID[yPOSITION][xPOSITION] == 0) {

                    GRID[yPOSITION][xPOSITION] = 2;

                    if (timer > delay) {

                        movementSound.play();
                    }
                }

                // reset the timer
                timer = 0;
            }

            // Moves the enemies
            for (int i = 0; i < numberOfEnemies; i++) {

                enemyArray[i].moveEnemy();
            }

            int gameScore = 0;

            // if the player is on a blue tile
            if (GRID[yPOSITION][xPOSITION] == 1) {

                // set movement to 0
                xPOSITION_CHANGE = 0;
                yPOSITION_CHANGE = 0;

                // if you trap an enemy inside of enemyArray new box do not fill in where the enemy is
                for (int i = 0; i < numberOfEnemies; i++) {

                    drop(enemyArray[i].getYposition() / TILE_SIZE, enemyArray[i].getX() / TILE_SIZE);
                }

                // Goes over every tile of the GRID and if its value is -1 set equal to 0(dont fill it in) otherwise fill it in
                for (int yVal = 0; yVal < yGRID_VALUE; yVal++) {

                    for (int xVal = 0; xVal < xGRID_VALUE; xVal++) {

                        if (GRID[yVal][xVal] == -1) {

                            GRID[yVal][xVal] = 0;
                        } else {

                            GRID[yVal][xVal] = 1;

                            gameScore += 1;

                        }
                    }
                }


            }

            // If an enemy touches the players trail (green tile) end the game
            for (int enemy = 0; enemy < numberOfEnemies; enemy++) {

                if (GRID[enemyArray[enemy].getYposition() / TILE_SIZE][enemyArray[enemy].getX() / TILE_SIZE] == 2) {

                    gameOn = false;
                }
            }

            // Erases the whole gameWindow
            gameWindow.clear();

            // Goes over every tile and if it is 0 do not do anything, if it is 1 make it blue, if it is two make it green the information to the buffer
            for (int yVal = 0; yVal < yGRID_VALUE; yVal++) {

                for (int xVal = 0; xVal < xGRID_VALUE; xVal++) {

                    if (GRID[yVal][xVal] != 0) {


                        if (GRID[yVal][xVal] == 1) {

                            tileSprite.setTextureRect(IntRect(0, 0, TILE_SIZE, TILE_SIZE));
                        }
                        if (GRID[yVal][xVal] == 2) {

                            tileSprite.setTextureRect(IntRect(54, 0, TILE_SIZE, TILE_SIZE));
                        }

                        tileSprite.setPosition(xVal * TILE_SIZE, yVal * TILE_SIZE);
                        gameWindow.draw(tileSprite);
                    }
                }
            }
            // Sets the player position to red
            tileSprite.setTextureRect(IntRect(36, 0, TILE_SIZE, TILE_SIZE));
            tileSprite.setPosition(xPOSITION * TILE_SIZE, yPOSITION * TILE_SIZE);
            gameWindow.draw(tileSprite);

            // draw the score to the screen subtracting off the 126 border titles
            if (gameScore != 0) {
                scoreText.setString(std::to_string(gameScore - 126));
                scoreText.setCharacterSize(30);
                scoreText.setPosition(360, 10);
                gameWindow.draw(scoreText);
            }


            // Makes the enemy rotate
            enemySprite.rotate(10);

            // where the enemy is paint enemyArray picture of them
            for (int enemy = 0; enemy < numberOfEnemies; enemy++) {

                enemySprite.setPosition(enemyArray[enemy].getX(), enemyArray[enemy].getYposition());
                gameWindow.draw(enemySprite);
            }

            // If the game over load the game over message
            if (!gameOn) {

                gameWindow.draw(gameOverSprite);
            }

            // Display to the gameWindow the changes
            gameWindow.display();
        }
    }

    return 0;
}