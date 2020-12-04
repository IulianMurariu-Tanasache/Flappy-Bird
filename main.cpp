#include <SFML/Graphics.hpp>
#include<bits/stdc++.h>
#define Log(x) cout<<x<<endl;

using namespace std;
using namespace sf;

struct coord{int x,y;
double s;}a,p[15];//a->coordonatele jucatorului // p-> coordonatele tuburilor si scale-ul lor
int speed,spawnGap,spawnY,score;//viteza,marimea intrarii si inaltimea ei//score
bool rot=false,v[15];//daca sare trebuie rotit si animat//v->ajuta la crearea tuburilor
bool dead=true,start=false;//mort sau incepe jocul

void spawn()//creeaza tuburi
{
    int index;
    for(index=0;index<15;index+=2)
    if(v[index]==false){
    spawnGap = 200+ rand()%80;
    spawnY=rand()%200 + 100;
    p[index].x=590;//tubul apare la dreapta, inafara ecranului
    p[index+1].x=590+26;
    p[index].s=((float)spawnY)/320.00;
    p[index+1].s=((float)(680-spawnY-spawnGap))/320;
    //Log(p[index+1].s);
    v[index]=v[index+1]=true;
    return ;
    }
}

int main()
{
    srand((int)time(0));
    spawn();

    // Create the main window
    RenderWindow app(VideoMode(500, 680), "FlappyBird",Style::Close|Style::Titlebar);
    app.setKeyRepeatEnabled(false);

    //luam texturile si font-ul textului
    Texture birdT,backT,pipeT,baseT,gameoverT,frame2,frame3,frame4;
    birdT.loadFromFile("Frame-1.png");
    frame2.loadFromFile("frame-2.png");
    frame3.loadFromFile("frame-3.png");
    frame4.loadFromFile("frame-4.png");
    backT.loadFromFile("background-day.png");
    pipeT.loadFromFile("pipe-green.png");
    baseT.loadFromFile("base.png");
    gameoverT.loadFromFile("gameover.png");

    //collision-figuri geometrice care reprezinta "rama" jucatorului si a tubului-> folosite in a detecta ciocnirea
    //RectangleShape Cp;
    CircleShape Cp;
    Cp.setOrigin(350*0.07,310*0.07);
    float r=340*0.07;
    Cp.setRadius(r);
    Cp.setPointCount(100);

    RectangleShape Cp1,Cp2;
    //Cp.setSize(Vector2f(705*0.07,600*0.07));
    Cp2.setOrigin(26,0);
    Cp2.setRotation(180);

    Font font;
    font.loadFromFile("font.TTF");

    Sprite player(birdT);Sprite background(backT); Sprite pipe(pipeT);Sprite pipe2(pipeT); Sprite base(baseT); Sprite gameover(gameoverT);

    Text text;
    text.setFont(font);
    text.setPosition(170,600);
    text.setCharacterSize(50);

    //cream fiecare sprite

    player.setOrigin(350,310);
    player.setScale(0.07,0.07);
    background.setScale(1.75,1.33);
    pipe2.setOrigin(26,0);
    base.setScale(1.49,1);
    base.setPosition(0,580);
    gameover.setScale(1.5,1.5);
    gameover.setPosition(100,270);
    a.x=150;
    a.y=250;

	// Start the game loop
    while (app.isOpen())
    {
        app.setFramerateLimit(30+score);//crestem viteza jocului odata cu scorul
        // Process events
        Event event;
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == Event::Closed)
                app.close();
            if(event.type == Event::KeyPressed&&dead==true)//daca se apasa space, jucatorul urca, se roteste si este animat
                if(event.key.code == Keyboard::Space)
                {
                    speed = -10;
                    player.setRotation(-60);
                   rot=true;
                   start=true;
                   player.setTexture(frame3);
                }
        }
        if(start==true){
        speed++;//accelereaza in jos(gravitatie)
        if(speed>-5)
            {rot=false;player.setTexture(frame4);}//daca urca trebuie animat
        if(speed>9)speed=9;//limita la accelerare
        if(speed>-1&&speed<1)player.setTexture(frame2);
        if(speed>=1)player.setTexture(birdT);//animatii
        a.y=a.y+speed;
        if(a.y<24)a.y=23;}//limita in sus sa nu iasa din ecran
        Cp.setPosition(a.x,a.y);
        /*if(player.getGlobalBounds().intersects(base.getGlobalBounds()))
        {
            speed=-1;
        }*/
        player.setPosition(a.x,a.y);
        //Log(a.y);
        if(a.y>590)
            dead=false;//daca atinge pamantul moare
        if(rot==false)
                player.setRotation(10*speed);

         // Clear screen
        app.clear();

        app.draw(background);
        app.draw(base);

        for(int i=0;i<15&&dead==true&&start==true;i=i+2)//miscarea tuburilor
        if(v[i]==true){
            if(p[i].x<-50)
                v[i]=v[i+1]=false;//daca tubul iese din ecran trebuie sters
            else
            {
                p[i].x-=3;
                p[i+1].x-=3;
                pipe.setPosition(p[i].x,680.0-100.0-320.0*p[i].s);
                //Log(680.0-100.0-320.0*p[i].s);
                pipe.setScale(1,p[i].s);
                Cp1.setSize(Vector2f(47.0,320*p[i].s));
                Cp1.setPosition(p[i].x+2,680.0-100.0-320.0*p[i].s);
                app.draw(pipe);
                //app.draw(Cp1);
                pipe2.setPosition(p[i+1].x,320.0*p[i+1].s);
                pipe2.setScale(1,p[i+1].s);
                pipe2.setRotation(180);
                Cp2.setSize(Vector2f(47.0,320*p[i+1].s));
                Cp2.setPosition(p[i+1].x-1,320.0*p[i+1].s);
                app.draw(pipe2);
                //app.draw(Cp2);

                //collision -> daca pasarea se afla la inaltimea tubului de sus sau de jos si il atinge-> moare
                float cp1_r=Cp1.getPosition().x+47.0;
                if((abs(Cp.getPosition().x - Cp2.getPosition().x)<24+r&&Cp.getPosition().y-r<Cp2.getPosition().y)||(((Cp.getPosition().x+r>=Cp1.getPosition().x&&Cp.getPosition().x+r<=Cp1.getPosition().x+47)||(Cp.getPosition().x-r<=Cp1.getPosition().x+47&&Cp.getPosition().x-r>=Cp1.getPosition().x))&&Cp.getPosition().y+r>Cp1.getPosition().y))
                {
                //app.close();
                cout<<"LOL";
                dead=false;
                }
                if(149== p[i].x)//daca am trecut cu succes de mijlocul tubului crestem scorul
                    score++;
                if(p[i].x==350)
                spawn();//crearea unui nou tub
            }
        }
        char s[50];
        char s1[100]="score: ";
        strcpy (s,to_string(score).c_str());
        strcat(s1,s);
        text.setString(s1);

        app.draw(text);

        // Draw the sprite
        app.draw(player);
        //app.draw(Cp);
        if(dead==false)
            app.draw(gameover);
        // Update the window
        app.display();
    }

    return EXIT_SUCCESS;
}
