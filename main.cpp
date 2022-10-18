
#include <SFML/Graphics.hpp>//include the library for textures etc.
#include <cmath>
#include <string>
#include <ctime>
#include <SFML/Audio.hpp>
#include <iostream>
#include <math.h>
#include <chrono>
#include <thread>

using namespace sf;
using namespace std;

const int num = 9; //checkpoints
int points[num][2] = {300, 610,
                      1270, 430,
                      1380, 2380,
                      1900, 2460,
                      1970, 1700,
                      2560, 1680,
                      2450, 3150,
                      500, 3300,
                      300,1900};
class Car{
public:
// Name of function: car
// Description: initialize the member variables
// Inputs:none
// Outputs:none
    Car(){
        m_speed =0;
        m_angle=0;
        m_n=0;
        m_background_height=3648;
        m_background_width=2880;
    }
// Name of function: move
// Description:changes a cars x and y position according to the speed and angle of said car
// Inputs:none
// Outputs:none
    void move(){
    //change cars x and y position
        m_x += sin(m_angle)* m_speed;
        m_y -= cos(m_angle)* m_speed;
    }
// Name of function: findTarget
// Description: determines the non-user controlled car's turning direction based off the checkpoint and current position
// Inputs:none
// Outputs:none
    void findTarget(){
        float tx = points[m_n][0];
        float ty = points[m_n][1];
        float beta = m_angle - atan2(tx - m_x,-ty + m_y);// beta is the angle between the difference in position between the checkpoint and the current postion of the car.
        if(sin(beta)<0){ // if beta is between 180 and 360 degrees
            m_angle += 0.005 * m_speed;//then car turns right
        }
        else{
            m_angle -= 0.005 * m_speed;//otherwise car turns left
        }
        if((m_x-tx)*(m_x-tx)+(m_y-ty)*(m_y-ty)<25*25){
            m_n = (m_n+1)% num;//change to the next checkpoint
        }

    }
    float get_m_x() const{return m_x;};
    void set_m_x(float x)
    {
        if(x > m_background_width)//if position is greater than the width set = to width
        {
            m_x = m_background_width;
        }
        else if(x < 0)//if position is less than width set equal to width
        {
            m_x = 0;
        }
        else
        {
            m_x = x;
        }

    }
    float get_m_y() const{return m_y;};
    void set_m_y(float y)
    {
        if(y > m_background_height)//if at edge set equal to edge
        {
            m_y = m_background_height;
        }
        else if(y < 0)//if less than the texture set equal to border
        {
            m_y = 50;
        }
        else
        {
            m_y = y;
        }

    }
    float get_m_speed() const{return m_speed;};
    void set_m_speed(float speed){m_speed = speed;};
    float get_m_angle() const{return m_angle;};
    void set_m_angle(float angle){m_angle = angle;};
    float get_m_n() const{return m_n;};
    void set_m_n(float n){m_n = n;};
    void set_m_background_width(float background_width){m_background_width = abs(background_width);};
    void set_m_background_height(float background_height){m_background_height = abs(background_height);};
    float get_m_background_width() const{return m_background_width;};
    float get_m_background_height() const{return m_background_height;};


private:
    float m_x;
    float m_y, m_speed, m_angle;
    int m_n;
    float m_background_width,m_background_height;
};


int main() {
    auto window_width =VideoMode::getDesktopMode().width;
    auto window_height =VideoMode::getDesktopMode().height;
    sf::Font time_font;;
    time_font.loadFromFile("fonts/ka1.ttf");
    if (!time_font.loadFromFile("fonts/ka1.ttf"))//load in fonts//karmatic arcade font byvic fieger//https://www.dafont.com/karmatic-arcade.font//license, free for personal use
    {
        return EXIT_FAILURE;
    }
    reset:
    RenderWindow menu(VideoMode(window_width,window_height),"Main Menu!");//render main menu
    menu.clear(Color::Transparent);
    Text menu_txt;//set text
    menu_txt.setFont(time_font);
    menu_txt.setCharacterSize(60);
    menu_txt.setString("Press ENTER to Start Game! \n \n \t \t \t     ESC to Exit");
    menu_txt.move(380,500);
    menu_txt.setFillColor(Color::Black);
    menu_txt.setOutlineColor(Color::Red);
    menu_txt.setOutlineThickness(10);
    menu.draw(menu_txt);//draw text
    Text menu_hdr;//set text
    menu_hdr.setFont(time_font);
    menu_hdr.setCharacterSize(100);
    menu_hdr.setString("RACING!!!");
    menu_hdr.setStyle(Text::Bold);
    menu_hdr.setStyle(Text::Italic);
    menu_hdr.setFillColor(Color::Red);
    menu_hdr.setOutlineColor(Color::Black);
    menu_hdr.setOutlineThickness(20);
    menu_hdr.move(670,250);
    menu.draw(menu_hdr);//draw text
    menu.display();//display
    bool checker_1 =false;
    while(true&&checker_1==false){//waiting loop for inputs (menu)

        if(Keyboard::isKeyPressed(sf::Keyboard::Enter))
        {
            menu.close();//close menu
            checker_1=true;
        }
        if(Keyboard::isKeyPressed((sf::Keyboard::Escape)))
        {
            return 0;//end program
        }
    }
    RenderWindow app(VideoMode(window_width, window_height), "Car Racing Game!");
    app.setFramerateLimit(60);
    Texture t1, t2;//initialize textures
    t1.loadFromFile("images/background.png");//open background texture
    if (!t1.loadFromFile("images/background.png")) {//check if background texture is open
        return EXIT_FAILURE;
    }
    t2.loadFromFile("images/car.png");//open car texture
    if (!t2.loadFromFile("images/car.png")) {//check to see if car texture is open
        return EXIT_FAILURE;
    }
    t1.setSmooth(true);//smoothes textures
    t2.setSmooth(true);//smoothes textures

    sf::Music music;
    if (!music.openFromFile("sounds/background_music1.ogg"))//extreme action by bensound//https://www.bensound.com/royalty-free-music/track/extreme-action//license free for use
    {
        return EXIT_FAILURE; // error
    }



    Sprite sBackground(t1),sCar(t2);//set background and car textures//sCar(t2)
    const int N = 5;//number of cars

    sBackground.scale(2, 2);//scale the background in size by a factor of 2

    sCar.setOrigin(22, 22);//center the car to ist true center
    float R = 22;


    Car car[N];//create an array of 5 car objects
    for (int i = 0; i < N; i++) {
        car[i].set_m_x(300 + i * 50);//iterate thru different starting x values for each car
        car[i].set_m_y(1700 + i * 80);//iterate thru different starting y values for each car
        car[i].set_m_speed(7 + i);//set speeds for each car
        car[i].set_m_background_height(3648);
        car[i].set_m_background_width(2880);
    }
    // define speed, maxspeed acceleration deceleration and the turning speed.
    float speed = 0, angle = 0;
    float maxSpeed = 18.0;
    float acc = 0.2, dec = 0.3;
    float turnSpeed = 0.08;

    int offsetX = 0, offsetY = 0;//define offsetx and offsety

    Clock clock;//set clock
    float timer = 0;//time is set to 0
    music.setLoop(true);//put in game loop so that when R is pressed the music starts over.
    music.play();//play music
    music.stop();
    int i = 0;//checkpoint counter
    bool checker= true;//checker variable
    int numframes=0;//frame counter
    bool checker_2 = false;//chekcer variables to replace breaks
    bool checker_3 = true;
    bool checker_4 = true;
    while (app.isOpen() && checker_2 == false && checker_4 == true) {
        if(Keyboard::isKeyPressed(sf::Keyboard::Escape))//if esc is pressed exit
        {
            checker_2 = true;
        }
        if(numframes==0)
        {
            chrono::seconds inter(1);//set interval to 1 second
            string three_two_one = "3210";
            Text starting_text;//initialize text
            starting_text.setCharacterSize(150);
            starting_text.setFont(time_font);
            starting_text.setFillColor(Color::White);
            FloatRect rectangle_1 = starting_text.getGlobalBounds();
            starting_text.move((window_width/2)-(window_width/25),(window_height/2)-(window_height/6));
            cout << window_height << endl;
            cout << window_width << endl;
            for(int i = 0; i < three_two_one.size(); i++)//iterate
            {

                app.clear(Color(255,0,0,0.0));//clear to red
                    if(three_two_one[i]=='0')
                    {
                        starting_text.setString("GO!");//when three_two_one is == 0 display GO!
                    }
                    else
                    {
                        starting_text.setString(three_two_one[i]);//set the string to one of the values in three_two_one
                    }
                    app.draw(starting_text);//draw text
                    app.display();//display text
                    this_thread::sleep_for(inter);//wait one second(inter)
            }
        }
        if(checker== true){//check if end of race is not true
            float time = clock.getElapsedTime().asSeconds();//contuine iterating thru time
            clock.restart();
            timer+=time;
        }

        Event e;//define event e
        while (app.pollEvent(e)) {//constantly pulling for events
            if (e.type == Event::Closed) {//check if the event is Event::Closed
                app.close();//close app
            }
        }

        bool Up = 0, Right = 0, Down = 0, Left = 0;//initalize up down right and left booleans
        if (Keyboard::isKeyPressed(Keyboard::Up)) {//check if keyboard::up is pressed
            Up = 1;//set up to 1
        }
        if (Keyboard::isKeyPressed(Keyboard::Right)) {//check if keyboard::right is pressed
            Right = 1;//set right to 1
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {//check if keyboard::down is pressed
            Down = 1;//set down to 1
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {//check if keyboard::left is pressed
            Left = 1;//set left to 1
        }

        //car movement
        if (Up && speed < maxSpeed) {//check if up is pressed and if speed isnt max speed
            if (speed < 0) {//if the car is moving backwards
                speed += dec;//increse the speed by the dec value
            } else {//if the car is at rest or is moving forward
                speed += acc;//increase the car by the acc value
            }
        }

        if (Down && speed > -maxSpeed) {//check if down is pressed and the speed is greater than the max backwards speed
            if (speed > 0) {//if the car is moving forward
                speed -= dec;//decrease the speed by the dec value
            } else {//if the car is at rest or moving backwards
                speed -= acc;//decrease speed by the acc value
            }
        }

        if (!Up && !Down) {//if up or down is not pressed
            if (speed - dec > 0){ //if the car is moving forward
                speed -= dec;//deccelerate the car by the dec value
            }
            else if (speed + dec < 0) {//if the car is mobing backwards
            speed += dec;//deccelerate the car by the dec value
            }
            else {//if the speed is in the range -0.3 to 0.3 (-dec,dec)
            speed = 0;//set speed to 0
            }
        }


        if (Right && speed != 0) {//car is moving and right is 1
            angle += turnSpeed * speed / maxSpeed;//add to angle ex: turn right(direct relationship with speed)
        }
        if (Left && speed != 0) {//car is moving and left is 1
            angle -= turnSpeed * speed / maxSpeed;//subtract to angle ex: turn left(direct relationship with speed)
        }
        //set calculated speed and angle for user car
        car[0].set_m_speed(speed);
        car[0].set_m_angle(angle);

        for (int i = 0; i < N; i++) {
            car[i].move();//call move car member function for each car in the game
        }
        for (int i = 1; i < N; i++) {
            car[i].findTarget();//call find target for each non-user car in the game
        }

        //collision
        for (int i = 0; i < N; i++) { // iterate thru every 2 car combination see if they are colliding
            for (int j = 0; j < N; j++) {
                int dx = 0, dy = 0;//initialize dx and dy
                bool breaker = true ;//breaker variable set to true
                while ((dx * dx + dy * dy <  4 * R * R) && breaker == true ) {
                    car[i].set_m_x(car[i].get_m_x() + dx / 10.0);//adjust car i's x position away from car j in the positive direction
                    car[i].set_m_y(car[i].get_m_y() + dy / 10.0);//adjust car i's y position away from car j in the positive direction
                    car[j].set_m_x(car[j].get_m_x()- dx / 10.0);//adjust car j's x position away from car i in the negative direction
                    car[j].set_m_y(car[j].get_m_y()- dy / 10.0);//adjust car j's y position away from car i in the negative direction
                    dx = car[i].get_m_x() - car[j].get_m_x();//define dx as difference in x position of car j and car i
                    dy = car[i].get_m_y() - car[j].get_m_y();//define dy as difference in y position of car j and car i
                    if (!dx && !dy) {
                        breaker = false;//break out of while loop
                    }
                }
            }
        }


        app.clear(Color::Transparent);//clear the window so the drawable objects can be displayed

        if (car[0].get_m_x() > 950) {//keeps car x position center of frame
            offsetX = car[0].get_m_x() - (window_width/2);
        }
        if (car[0].get_m_y() > 500) {//keeps car y position center of frame
            offsetY = car[0].get_m_y() - ((window_height/2)-(window_height/12));
        }


        sBackground.setPosition(-offsetX, -offsetY);//set background so center the car with calculated offset
        app.draw(sBackground);//draw background sprite

        Color colors[10] = {Color::Red, Color::Green, Color::Magenta, Color::Blue, Color::White};//array of avaliable colors

        for (int i = 0; i < N; i++) {
            sCar.setPosition(car[i].get_m_x() - offsetX, car[i].get_m_y() - offsetY);//set position of each car object
            sCar.setRotation(car[i].get_m_angle() * 180 / 3.141593);//initialize car object rotation
            sCar.setColor(colors[i]);//initialize colors for each car object
            app.draw(sCar);//draw car object to screen
        }
        sf::Text text_time;//initialize timer text

        text_time.setFont(time_font);
        text_time.setString(std::to_string(timer-4));//print timer to screen minus 4 for the countdown timer
        text_time.setCharacterSize(60);
        text_time.setFillColor(sf::Color::Black);
        text_time.setOutlineThickness(6.4);
        text_time.setOutlineColor(sf::Color::White);
        text_time.setStyle(sf::Text::Bold);
        text_time.move(50,50);//move around
        app.draw(text_time);//draw timer to screen every iteration

        if(i==9)
        {
            float final_time = timer;
            checker= false;
            sf::Text r_text;//final text
            r_text.setFont(time_font);
            r_text.setString("Press the R key to Reset Race! \n \t \t \t \tTime: " + to_string(final_time-4));//display final time as a string
            r_text.setCharacterSize(60);
            r_text.setFillColor(sf::Color::Black);
            r_text.setStyle(sf::Text::Bold);
            r_text.setOutlineColor(sf::Color::White);
            r_text.setOutlineThickness(10);
            r_text.move((window_width/2)-(window_width/3), (window_height/2)-(window_height/7));
            app.draw(r_text);//draw final text to screen
            music.stop();//stop music at end of game
            //play new sound.

            if(Keyboard::isKeyPressed(Keyboard::Escape))
            {
                app.close();//close app if esc is pressed
            }
        }
        checker_3 = true;
        while(i<=num && checker_3 == true)
        {
            CircleShape circleShape(50);//initialize circle for checkpoint
            circleShape.setOrigin(25,25);
            circleShape.setPosition(-offsetX+points[i][0],-offsetY+ points[i][1]);//set position of cirle to the checkpoint
            circleShape.setFillColor(Color::Transparent);
            circleShape.setOutlineThickness(15);
            circleShape.setOutlineColor(Color::Yellow);
            app.draw(circleShape);//draw circle
            if(((car[0].get_m_x() > points[i][0] - 100) && (car[0].get_m_x() < (points[i][0]+100))) && ((car[0].get_m_y() > points[i][1] - 100) && (car[0].get_m_y() <(points[i][1]+100))))//check if collided with circle
            {
                i++;//go to the next checkpoint
            }
            checker_3 = false;//break
        }

        app.display();//display all objects to the window
        numframes++;//count frames each iteration
    }

    return 0;
}
