#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <sstream>
#include <stdlib.h>
using namespace std;

class Box
{
    public:
        float box_len;
        void set_values(float);
        void drawBox(float box_len)
        {
             glBegin(GL_QUADS);
             glVertex2f(-box_len / 2, -box_len / 2);
             glVertex2f(box_len / 2, -box_len / 2);
             glVertex2f(box_len / 2, box_len / 2);
             glVertex2f(-box_len / 2, box_len / 2);
             glEnd();
        }
        void drawBox_border(float box_len)
        {
             glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
             glBegin(GL_QUADS);
             glVertex2f(-box_len / 2, -box_len / 2);
             glVertex2f(box_len / 2, -box_len / 2);
             glVertex2f(box_len / 2, box_len / 2);
             glVertex2f(-box_len / 2, box_len / 2);
             glEnd();
             glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        void drawscore(float box_len)
        {
             glBegin(GL_QUADS);
             glVertex2f(-7.2f, -4.0f);
             glVertex2f(-4.2f, -4.0f);
             glVertex2f(-4.2f,-2.0f);
             glVertex2f(-7.2f, -2.0f);
             glEnd();
        }
        void drawscore1(float box_len)
        {
             glBegin(GL_QUADS);
             glVertex2f(-6.8f, -3.8f);
             glVertex2f(-4.5f, -3.8f);
             glVertex2f(-4.5f,-2.2f);
             glVertex2f(-6.8f, -2.2f);
             glEnd();
        }
        void drawvolume(float speed)
        {
             glBegin(GL_QUADS);
             glVertex2f(6.8f, 5.8f);
             glVertex2f(4.5f, 5.8f);
             glVertex2f(4.5f,3.5f);
             glVertex2f(6.8f, 3.5f);
             glEnd();
        }
        void drawvolume1(float speed)
        {
             glBegin(GL_QUADS);
             glVertex2f(4.5f+3*speed, 5.8f);
             glVertex2f(4.5f, 5.8f);
             glVertex2f(4.5f,3.5f);
             glVertex2f(4.5f+3*speed, 3.5f);
             glEnd();
        }

};
class Ball
{
    public:
        float ball_rad;
        float ball_x;
        float ball_y;
        float theta;
        float vel_x;
        float vel_y;
        float mass;
        int color;//1 for white 2 for black
        void set_values(float, float, float,float, float,float,int);
        void drawBall(float ball_rad)
        {
            const float PI = 3.141592653589;
            glBegin(GL_TRIANGLE_FAN);
            for(int i=0 ; i<360 ; i++) {
                glVertex2f(ball_rad * cos(i*PI/180), ball_rad * sin(i*PI/180));
            }
            glEnd();
        }
        void drawBall_border(float ball_rad)
        {
            const float PI = 3.141592653589;
            glBegin(GL_LINE_LOOP);
            for(int i=0 ; i<360 ; i++) {
                glVertex2f(ball_rad * cos(i*PI/180), ball_rad * sin(i*PI/180));
            }
            glEnd();
        }

};

void drawText(const char *text, int length, int x, int y)
{
    glMatrixMode(GL_PROJECTION);
    double *matrix = new double[16];
    glGetDoublev(GL_PROJECTION_MATRIX, matrix);
    glLoadIdentity();
    glOrtho(0,800,0,600,-5,5);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glLoadIdentity();
    glRasterPos2f(x,y);
    for(int i=0;i<length;i++)
    {
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, (int)text[i]);


    }
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixd(matrix);
    glMatrixMode(GL_MODELVIEW);
}

void Ball::set_values(float rad, float x, float y, float velx, float vely, float m,int clr)
{
    ball_rad = rad;
    ball_x = x;
    ball_y = y;
    vel_x = velx;
    vel_y = vely;
    mass = m;
    color = clr;
}

void Box::set_values (float len) {
      box_len = len;
}

void drawScene();
void update(int value);
void update1(int value);
void friction(int value);
void decrease_score(int value);
void drawTriangle();
void initRendering();
void handleResize(int w, int h);
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x,int y);
void Myinitialize();
// Global Variables
float speed = 0.01f;
int flag=0;
int aim=2;
const float PI = 3.141592653589;
bool mouserightdown = false;
float g = 10.0f;
float mu = 0.000092f;  
int mousex, mousey; 
float alpha = 0.8f;
int score = 30;
Box outerbox,innerbox,innerbox1,scoreboard,scoreboard1,volume;
Ball pocket_left1,pocket_left2,pocket_right1,pocket_right2,border1,border2,border3,border4,border5,border6,border7,border8,border9,border10,border11,border12,striker,center_border;
Ball array[12];
int main(int argc, char **argv) {

    // Initialize GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    int w = glutGet(GLUT_SCREEN_WIDTH);
    int h = glutGet(GLUT_SCREEN_HEIGHT);
    int windowWidth = w * 2 / 3;
    int windowHeight = h * 2 / 3;

    glutInitWindowSize(windowWidth, windowHeight);
    glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

    glutCreateWindow("Carom game");  // Setup the window
    initRendering();

    // Register callbacks
    Myinitialize();
    glutDisplayFunc(drawScene);
    glutIdleFunc(drawScene);
    glutKeyboardFunc(handleKeypress1);
    glutSpecialFunc(handleKeypress2);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(handleResize);
    glutTimerFunc(10, update, 0);
    glutTimerFunc(1, update1, 0);
    glutTimerFunc(1, friction, 0);
    glutTimerFunc(1000, decrease_score , 0);

    glutMainLoop();
    return 0;
}
void Myinitialize()
{
    outerbox.set_values(4.0f);
    innerbox.set_values(7.0f);
    innerbox1.set_values(7.2f);
    pocket_left1.set_values(0.15f,-1.635f,1.635f,0.0f,0.0f,0.0f,0);
    pocket_left2.set_values(0.15f,-1.63f,-1.635f,0.0f,0.0f,0.0f,0);
    pocket_right1.set_values(0.15f,1.63f,1.635f,0.0f,0.0f,0.0f,0);
    pocket_right2.set_values(0.15f,1.63f,-1.635f,0.0f,0.0f,0.0f,0);
    border1.set_values(0.11f,-1.0f,1.14f,0.0f,0.0f,0.0f,0);
    border2.set_values(0.11f,-1.0f,-1.14f,0.0f,0.0f,0.0f,0);
    border3.set_values(0.11f,1.0f,1.14f,0.0f,0.0f,0.0f,0);
    border4.set_values(0.11f,1.0f,-1.14f,0.0f,0.0f,0.0f,0);
    border5.set_values(0.11f,-1.2f,1.0f,0.0f,0.0f,0.0f,0);
    border6.set_values(0.11f,-1.2f,-1.0f,0.0f,0.0f,0.0f,0);
    border7.set_values(0.11f,1.2f,1.0f,0.0f,0.0f,0.0f,0);
    border8.set_values(0.11f,1.2f,-1.0f,0.0f,0.0f,0.0f,0);
    border9.set_values(0.13f,-0.8f,0.7f,0.0f,0.0f,0.0f,0);
    border10.set_values(0.13f,-0.8f,-0.7f,0.0f,0.0f,0.0f,0);
    border11.set_values(0.13f,0.8f,0.7f,0.0f,0.0f,0.0f,0);
    border12.set_values(0.13f,0.8f,-0.7f,0.0f,0.0f,0.0f,0);
    striker.set_values(0.11f,0.1f,-1.14f,0.0f,0.0f,2.0f,0);
    striker.theta =0.0f;
    center_border.set_values(0.4f,0.0f,0.1f,0.0f,0.0f,0.0f,0);
    array[1].set_values(0.09f,0.0f,0.1f,0.0f,0.0f,1.0f,3);
    array[2].set_values(0.09f,0.25f,0.30f,0.0f,0.0f,1.0f,1);
    array[3].set_values(0.09f,0.26f,-0.05f,0.0f,0.0f,1.0f,1);
    array[4].set_values(0.09f,-0.1f,0.40f,0.0f,0.0f,1.0f,1);
    array[5].set_values(0.09f,-0.05f,-0.2f,0.0f,0.0f,1.0f,1);
    array[6].set_values(0.09f,-0.3f,0.08f,0.0f,0.0f,1.0f,1);
    array[7].set_values(0.09f,0.09f,0.41f,0.0f,0.0f,1.0f,2);
    array[8].set_values(0.09f,0.14f,-0.19f,0.0f,0.0f,1.0f,2);
    array[9].set_values(0.09f,-0.25f,0.26f,0.0f,0.0f,1.0f,2);
    array[10].set_values(0.09f,0.32f,0.13f,0.0f,0.0f,1.0f,2);
    array[11].set_values(0.09f,-0.23f,-0.1f,0.0f,0.0f,1.0f,2);


}
// Function to draw objects on the screen
void drawScene() {
    if (mouserightdown)
    {
        int w = glutGet(GLUT_SCREEN_WIDTH);
        // Convert mouse position to OpenGL's coordinate system
       striker.ball_x = double(mousex)/w - 0.4f;
       striker.ball_y = -1.14f;
    }
   
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glColor3f(0.5f, 0.0f, 0.0f);
    outerbox.drawBox(outerbox.box_len);
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    innerbox1.drawBox(innerbox1.box_len);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glColor3f(1.0f, 0.7f, 0.4f);
    innerbox.drawBox(innerbox.box_len);
    glPopMatrix();
    if(striker.theta!=0 && striker.ball_y==-1.14f)
    {
    glPushMatrix();
    glColor3f(0.7f, 0.0f, 0.0f);
    glBegin(GL_LINES);
    glVertex2f(striker.ball_x,striker.ball_y);
    glVertex2f(striker.ball_x-0.5*sin(striker.theta*PI/180),striker.ball_y+0.5*cos(striker.theta*PI/180));
    glEnd();
    glPopMatrix();
    }   
    if(striker.ball_y == -1.14f)
    {
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    volume.drawvolume(speed);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glColor3f(1.0f, 0.6f, 0.2f);
    volume.drawvolume1(speed);
    glPopMatrix();
    }
    glPushMatrix();
    glTranslatef(pocket_left1.ball_x, pocket_left1.ball_y, 0.0f);
    glColor3f(0.52f, 0.3411f, 0.137f);
    pocket_left1.drawBall(pocket_left1.ball_rad);
    glColor3f(0.0f, 0.0f, 0.0f);
    pocket_left1.drawBall_border(pocket_left1.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(pocket_left2.ball_x, pocket_left2.ball_y, 0.0f);
    glColor3f(0.52f, 0.3411f, 0.137f);
    pocket_left1.drawBall(pocket_left2.ball_rad);
    glColor3f(0.0f, 0.0f, 0.0f);
    pocket_left2.drawBall_border(pocket_left2.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(pocket_right1.ball_x, pocket_right1.ball_y, 0.0f);
    glColor3f(0.52f, 0.3411f, 0.137f);
    pocket_right1.drawBall(pocket_right1.ball_rad);
    glColor3f(0.0f, 0.0f, 0.0f);
    pocket_right1.drawBall_border(pocket_right1.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(pocket_right2.ball_x, pocket_right2.ball_y, 0.0f);
    glColor3f(0.52f, 0.3411f, 0.137f);
    pocket_right2.drawBall(pocket_right2.ball_rad);
    glColor3f(0.0f, 0.0f, 0.0f);
    pocket_right2.drawBall_border(pocket_right2.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border1.ball_x, border1.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    border1.drawBall(border1.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glLineWidth(2.0f);
    glTranslatef(border2.ball_x, border2.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    border2.drawBall(border2.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(1.0f,1.25f);
    glVertex2f(-1.0f,1.25f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(1.0f,1.03f);
    glVertex2f(-1.0f,1.03f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(1.0f,-1.25f);
    glVertex2f(-1.0f,-1.25f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(1.0f,-1.03f);
    glVertex2f(-1.0f,-1.03f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border3.ball_x, border3.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    border3.drawBall(border3.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border4.ball_x, border4.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    border4.drawBall(border4.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border5.ball_x, border5.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    border5.drawBall(border5.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border6.ball_x, border6.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    border6.drawBall(border6.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border7.ball_x, border7.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    border7.drawBall(border7.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border8.ball_x, border8.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    border8.drawBall(border8.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border9.ball_x, border9.ball_y, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    border9.drawBall_border(border9.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border10.ball_x, border10.ball_y, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    border10.drawBall_border(border10.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border11.ball_x, border11.ball_y, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    border11.drawBall_border(border11.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(border12.ball_x, border12.ball_y, 0.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    border12.drawBall_border(border12.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(1.31f,1.0f);
    glVertex2f(1.31f,-1.0f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(1.089999f,1.0f);
    glVertex2f(1.089999f,-1.0f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.31f,1.0f);
    glVertex2f(-1.31f,-1.0f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.089999f,1.0f);
    glVertex2f(-1.089999f,-1.0f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.73f,0.6f);
    glVertex2f(1.3f,1.3f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.73f,0.6f);
    glVertex2f(-1.3f,1.3f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(-0.73f,-0.6f);
    glVertex2f(-1.3f,-1.3f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glColor3f(0.0f,0.0f,0.0f);
    glBegin(GL_LINES);
    glVertex2f(0.73f,-0.6f);
    glVertex2f(1.3f,-1.3f);
    glEnd();
    glPopMatrix();
    glPushMatrix();
    glLineWidth(2.0f);
    glTranslatef(center_border.ball_x, center_border.ball_y, 0.0f);
    glColor3f(0.7f, 0.0f, 0.0f);
    center_border.drawBall_border(center_border.ball_rad);
    glPopMatrix();
    glPushMatrix();
    glLineWidth(1.0f);
    glTranslatef(striker.ball_x, striker.ball_y, 0.0f);
    glRotatef(striker.theta, 0.0f, 0.0f, 1.0f);
    glColor3f(0.0f, 0.0f, 1.0f);
    striker.drawBall(striker.ball_rad);
    glColor3f(0.0f, 0.0f, 0.0f);
    striker.drawBall_border(striker.ball_rad);
    glPopMatrix();
    for(int i=1;i<=11;i++)
    {
    if( ((array[i].ball_x+1.635f)*(array[i].ball_x+1.635f)+ (array[i].ball_y -1.635f)*(array[i].ball_y -1.635f))>=0.0225f && ((array[i].ball_x+1.63f)*(array[i].ball_x+1.63f)+ (array[i].ball_y +1.635f)*(array[i].ball_y +1.635f))>=0.0225f && ((array[i].ball_x-1.63f)*(array[i].ball_x-1.63f)+ (array[i].ball_y -1.635f)*(array[i].ball_y -1.635f))>=0.0225f && ((array[i].ball_x-1.63f)*(array[i].ball_x-1.63f)+ (array[i].ball_y +1.635f)*(array[i].ball_y +1.635f))>=0.0225f)
    {
    glPushMatrix();
    glTranslatef(array[i].ball_x, array[i].ball_y, 0.0f);
    if(array[i].color == 1)
    glColor3f(0.0f, 0.0f, 0.0f);
    else if(array[i].color == 2)
    glColor3f(1.0f, 1.0f, 1.0f);
    else if(array[i].color == 3)
    glColor3f(0.8f, 0.0f, 0.4f);
    array[i].drawBall(array[i].ball_rad);
    glColor3f(0.0f, 0.0f, 0.0f);
    array[i].drawBall_border(array[i].ball_rad);
    glPopMatrix();
    }
    }
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glColor3f(0.0f, 0.0f, 0.0f);
    scoreboard.drawscore(0.0f);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(0.0f, 0.0f, -5.0f);
    glColor3f(1.0f, 0.6f, 0.2f);
    scoreboard1.drawscore1(0.0f);
    glPopMatrix();
    ostringstream temp;
    temp<<score;
    std::string text;
    text="Current Score: " + temp.str();
    glColor3f(0.0f,0.0f, 0.0f);
    drawText(text.data(), text.size(), 45,100);
    if(flag==0)
    {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -5.0f);
        glColor3f(0.5f, 0.0f, 0.0f);
        outerbox.drawBox(7.5f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -5.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        innerbox1.drawBox(10.0f);
        glPopMatrix();
        glPopMatrix();
        glColor3f(0.0f,0.0f,1.0f);
        
       std::string text;
       text="Enter w for being on the white side and b for being on the black"; 
       glColor3f(0.0f,0.0f, 1.0f);
       drawText(text.data(), text.size(), 220,500);
    }
    if(flag==2)
    {
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -5.0f);
        glColor3f(0.5f, 0.0f, 0.0f);
        outerbox.drawBox(7.5f);
        glPushMatrix();
        glTranslatef(0.0f, 0.0f, -5.0f);
        glColor3f(1.0f, 1.0f, 1.0f);
        innerbox1.drawBox(10.0f);
        glPopMatrix();
        glPopMatrix();
        glColor3f(0.0f,0.0f,1.0f);
        
       std::string text;
       text="Instructions for the game :- Press Enter to continue :) All the Best (y)" ; 
       glColor3f(0.0f,0.0f, 1.0f);
       drawText(text.data(), text.size(), 220,500);
       text="-> Use the right arrow and left arrow key to move the striker to the right and the left respectively." ; 
       glColor3f(0.0f,0.0f, 1.0f);
       drawText(text.data(), text.size(), 220,480);
       text="-> Use the Up arrow key and the Down key to increase and decrease the speed respectively." ; 
       glColor3f(0.0f,0.0f, 1.0f);
       drawText(text.data(), text.size(), 220,460);
       text="-> Use the a and c keys to rotate the striker anti-clockwise and clockwise for hitting." ; 
       glColor3f(0.0f,0.0f, 1.0f);
       drawText(text.data(), text.size(), 220,440);
       text="-> Initiate the strike by pressing the Spacebar." ;  
       glColor3f(0.0f,0.0f, 1.0f);
       drawText(text.data(), text.size(), 220,420);
       text="Use the tab key to bring the striker back to initial position." ;  
       glColor3f(0.0f,0.0f, 1.0f);
       drawText(text.data(), text.size(), 220,400);
        text="-> Release the right mouse to initiate the strike." ;  
       glColor3f(0.0f,0.0f, 1.0f);
       drawText(text.data(), text.size(), 220,380);
      






    }
    glPopMatrix();
    glutSwapBuffers();
}
void friction(int value)
{
    for(int i=1;i<=11;i++)
    {
        float phi = atan2(array[i].vel_y,array[i].vel_x);
        float temp=0.00002f;
        if(array[i].vel_x!=0)
        array[i].vel_x-=temp*cos(phi);
        if(array[i].vel_y!=0)
        array[i].vel_y-=temp*sin(phi);
        if(abs(array[i].vel_x) < 0.0001f && abs(array[i].vel_y) < 0.0001f)
        {
            array[i].vel_x=0.0f;
            array[i].vel_y=0.0f;
        }

    }

    float phi = atan2(striker.vel_y,striker.vel_x);
        float temp=0.00002f;
        if(striker.vel_x!=0)
        striker.vel_x-=temp*cos(phi);
        if(striker.vel_y!=0)
        striker.vel_y-=temp*sin(phi);
        glutPostRedisplay();
        glutTimerFunc(1, friction, 0);

        if(abs(striker.vel_x) < 0.0001f && abs(striker.vel_y) < 0.0001f)
        {
            striker.vel_x = 0.0f;
            striker.vel_y = 0.0f;
        }


}

void update1(int value){
    if(striker.ball_x + striker.ball_rad > (outerbox.box_len-0.5f)/2 || striker.ball_x - striker.ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        striker.vel_x *= -alpha*1;
        striker.vel_y *= alpha;
    }
    if(striker.ball_y + striker.ball_rad > (outerbox.box_len-0.5f)/2 || striker.ball_y - striker.ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        striker.vel_y *= -alpha*1;
        striker.vel_x *= alpha;
    }
    
    striker.ball_x += striker.vel_x ;
    striker.ball_y += striker.vel_y ;

    for(int i=1;i<=11;i++)
    {
        if(array[i].ball_x + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_x - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_x *= -alpha*1;
        array[i].vel_y *= alpha;
    }
    if(array[i].ball_y + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_y - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_y *= -alpha*1;
        array[i].vel_x *= alpha;
    }
    array[i].ball_x += array[i].vel_x ;
    array[i].ball_y += array[i].vel_y ;
    }

    glutPostRedisplay();
    glutTimerFunc(1, update1, 0);

}

void update(int value) {

    if( ((striker.ball_x+1.635f)*(striker.ball_x+1.635f)+ (striker.ball_y -1.635f)*(striker.ball_y -1.635f))<0.0225f)
        {
            striker.vel_x = 0.0f;
            striker.vel_y = 0.0f;
            striker.ball_x=0.1f;
            striker.ball_y=-1.14f;
            score-=5;
            //0.15f,-1.635f,1.635f
        }
        else if(((striker.ball_x+1.63f)*(striker.ball_x+1.63f)+ (striker.ball_y +1.635f)*(striker.ball_y +1.635f))<0.0225f)
        {
            striker.vel_x = 0.0f;
            striker.vel_y = 0.0f;
            striker.ball_x=0.1f;
            striker.ball_y=-1.14f;
            score-=5;
            //0.15f,-1.63f,-1.635f
        }
       else if(((striker.ball_x-1.63f)*(striker.ball_x-1.63f)+ (striker.ball_y -1.635f)*(striker.ball_y -1.635f))<0.0225f)
        {
            striker.vel_x = 0.0f;
            striker.vel_y = 0.0f;
            striker.ball_x=0.1f;
            striker.ball_y=-1.14f;
            score-=5;
        }
        else if(((striker.ball_x-1.63f)*(striker.ball_x-1.63f)+ (striker.ball_y +1.635f)*(striker.ball_y +1.635f))<0.0225f)
        {
            striker.vel_x = 0.0f;
            striker.vel_y = 0.0f;
             striker.ball_x=0.1f;
            striker.ball_y=-1.14f;
            score-=5;
        }

    for(int i=1;i<=11;i++)
    {
        if( ((array[i].ball_x+1.635f)*(array[i].ball_x+1.635f)+ (array[i].ball_y -1.635f)*(array[i].ball_y -1.635f))<0.0225f)
        {
            array[i].vel_x = 0.0f;
            array[i].vel_y = 0.0f;
            array[i].ball_x=10.635f;
            array[i].ball_y=10.635f;
            if(i==1)
                score+=50;
            if(array[i].color==aim)
            {
                score+=10;

            }
            else
            {
                score-=5;
            }
            //0.15f,-1.635f,1.635f
        }
        else if(((array[i].ball_x+1.63f)*(array[i].ball_x+1.63f)+ (array[i].ball_y +1.635f)*(array[i].ball_y +1.635f))<0.0225f)
        {
            array[i].vel_x = 0.0f;
            array[i].vel_y = 0.0f;
             array[i].ball_x=10.63f;
            array[i].ball_y=10.635f;
            if(i==1)
                score+=50;
            if(array[i].color==aim)
            {
                score+=10;

            }
            else
            {
                score-=5;
            }
            //0.15f,-1.63f,-1.635f
        }
       else if(((array[i].ball_x-1.63f)*(array[i].ball_x-1.63f)+ (array[i].ball_y -1.635f)*(array[i].ball_y -1.635f))<0.0225f)
        {
            array[i].vel_x = 0.0f;
            array[i].vel_y = 0.0f;
             array[i].ball_x=10.63f;
            array[i].ball_y=10.635f;
            if(i==1)
                score+=50;
            if(array[i].color==aim)
            {
                score+=10;

            }
            else
            {
                score-=5;
            }
        }
        else if(((array[i].ball_x-1.63f)*(array[i].ball_x-1.63f)+ (array[i].ball_y +1.635f)*(array[i].ball_y +1.635f))<0.0225f)
        {
            array[i].vel_x = 0.0f;
            array[i].vel_y = 0.0f;
             array[i].ball_x=10.63f;
            array[i].ball_y=-10.635f;
            if(i==1)
                score+=50;
            if(array[i].color==aim)
            {
                score+=10;

            }
            else
            {
                score-=5;
            }
        }

        
    }

    for(int i=1;i<=11;i++)
    {
        array[i].ball_x+=array[i].vel_x;
        array[i].ball_y+=array[i].vel_y;
       
    if(array[i].ball_x + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_x - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_x *= -alpha*1;
        array[i].vel_y *= alpha;
        array[i].ball_x+=array[i].vel_x;
        array[i].ball_y+=array[i].vel_y;
    }
    if(array[i].ball_y + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_y - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_y *= -alpha*1;
        array[i].vel_x *= alpha;
        array[i].ball_x+=array[i].vel_x;
        array[i].ball_y+=array[i].vel_y;
    }

     if(striker.ball_x + striker.ball_rad > (outerbox.box_len-0.5f)/2 || striker.ball_x - striker.ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        striker.vel_x *= -alpha*1;
        striker.vel_y *= alpha;
        striker.ball_x+=striker.vel_x;
        striker.ball_y+=striker.vel_y;
    }
    if(striker.ball_y + striker.ball_rad > (outerbox.box_len-0.5f)/2 || striker.ball_y - striker.ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        striker.vel_y *= -alpha*1;
        striker.vel_x *= alpha;
        striker.ball_x+=striker.vel_x;
        striker.ball_y+=striker.vel_y;
    }
        //array[i].ball_x+=array[i].vel_x;
        //array[i].ball_y+=array[i].vel_y;

        float phi;
        if((striker.ball_y-array[i].ball_y)*(striker.ball_y-array[i].ball_y) + (striker.ball_x-array[i].ball_x)*(striker.ball_x-array[i].ball_x) <= (striker.ball_rad+array[i].ball_rad)*(striker.ball_rad+array[i].ball_rad))
        {
            //system("mpg321 1.mp3");
            if(striker.ball_x>array[i].ball_x && striker.ball_y>array[i].ball_y)
               phi =atan((array[i].ball_y-striker.ball_y)/(array[i].ball_x-striker.ball_x));
            else  if(striker.ball_x<array[i].ball_x && striker.ball_y < array[i].ball_y)
                phi =atan((array[i].ball_y-striker.ball_y)/(array[i].ball_x-striker.ball_x));
            else  if(striker.ball_x<array[i].ball_x && striker.ball_y > array[i].ball_y)
                phi =atan2((array[i].ball_y-striker.ball_y),(array[i].ball_x-striker.ball_x));
            else  if(striker.ball_x>array[i].ball_x && striker.ball_y < array[i].ball_y)
                phi =atan2((array[i].ball_y-striker.ball_y),(array[i].ball_x-striker.ball_x));
        
        
        striker.vel_x=(striker.vel_x*cos(phi)*cos(phi) + striker.vel_y*sin(phi)*cos(phi)+2*array[i].vel_x*cos(phi)*cos(phi)+2*array[i].vel_y*sin(phi)*cos(phi))/3 - striker.vel_y*sin(phi)*cos(phi) + striker.vel_x*sin(phi)*(sin(phi))  ;
        striker.vel_y=(striker.vel_x*cos(phi)*sin(phi) + striker.vel_y*sin(phi)*sin(phi)+2*array[i].vel_x*cos(phi)*sin(phi)+2*array[i].vel_y*sin(phi)*sin(phi))/3 + striker.vel_y*cos(phi)*cos(phi) - striker.vel_x*sin(phi)*(cos(phi))  ;
        array[i].vel_x=(-1*(array[i].vel_x*cos(phi)*cos(phi) + array[i].vel_y*sin(phi)*cos(phi))+4*striker.vel_x*cos(phi)*cos(phi)+4*striker.vel_y*sin(phi)*cos(phi))/3 - array[i].vel_y*sin(phi)*cos(phi) + array[i].vel_x*sin(phi)*(sin(phi))  ;
        array[i].vel_y=(-1*(array[i].vel_x*cos(phi)*sin(phi) + array[i].vel_y*sin(phi)*sin(phi))+4*striker.vel_x*cos(phi)*sin(phi)+4*striker.vel_y*sin(phi)*sin(phi))/3 + array[i].vel_y*cos(phi)*cos(phi) - array[i].vel_x*sin(phi)*(cos(phi))  ;
        float d = sqrt((striker.ball_y-array[i].ball_y)*(striker.ball_y-array[i].ball_y) + (striker.ball_x-array[i].ball_x)*(striker.ball_x-array[i].ball_x));
         float im1 = 0.5f;
         float im2 = 1.0f;
        striker.ball_x+=(im1/(im1+im2))*(striker.ball_x-array[i].ball_x)*((striker.ball_rad+array[i].ball_rad-d)/d);
         striker.ball_y+=(im1/(im1+im2))*(striker.ball_y-array[i].ball_y)*((striker.ball_rad+array[i].ball_rad-d)/d);
         array[i].ball_x-=(im2/(im1+im2))*(striker.ball_x-array[i].ball_x)*((striker.ball_rad+array[i].ball_rad-d)/d);
         array[i].ball_y-=(im2/(im1+im2))*(striker.ball_y-array[i].ball_y)*((striker.ball_rad+array[i].ball_rad-d)/d);
        striker.ball_x+=striker.vel_x;
        striker.ball_y+=striker.vel_y;
         if(array[i].ball_x + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_x - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_x *= -alpha*1;
        array[i].vel_y *= alpha;
        array[i].ball_x+=array[i].vel_x;
        array[i].ball_y+=array[i].vel_y;
    }
    if(array[i].ball_y + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_y - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_y *= -alpha*1;
        array[i].vel_x *= alpha;
        array[i].ball_x+=array[i].vel_x;
        array[i].ball_y+=array[i].vel_y;
    }
    //system("mpg321 1.mp3");
       
      }
      

      }
      for(int i=1;i<=11;i++)
      {
      array[i].ball_x+=array[i].vel_x;
      array[i].ball_y+=array[i].vel_y;

      if(array[i].ball_x + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_x - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_x *= -alpha*1;
        array[i].vel_y *= alpha;
        array[i].ball_x+=array[i].vel_x;
      array[i].ball_y+=array[i].vel_y;
    }
    if(array[i].ball_y + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_y - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_y *= -alpha*1;
        array[i].vel_x *= alpha;
        array[i].ball_x+=array[i].vel_x;
      array[i].ball_y+=array[i].vel_y;
    }

        for(int j=i+1;j<=11;j++)
        {
            float phi;

            if((array[i].ball_y-array[j].ball_y)*(array[i].ball_y-array[j].ball_y) + (array[i].ball_x-array[j].ball_x)*(array[i].ball_x-array[j].ball_x) <= (array[i].ball_rad+array[j].ball_rad)*(array[i].ball_rad+array[j].ball_rad))
        {
            //system("mpg321 1.mp3");

            if(array[i].ball_x>array[j].ball_x && array[i].ball_y>array[j].ball_y)
               phi =atan((array[j].ball_y-array[i].ball_y)/(array[j].ball_x-array[i].ball_x));
            else  if(array[i].ball_x<array[j].ball_x && array[i].ball_y < array[j].ball_y)
                phi =atan((array[j].ball_y-array[i].ball_y)/(array[j].ball_x-array[i].ball_x));
                 else  if(array[i].ball_x<array[j].ball_x && array[i].ball_y > array[j].ball_y)
                phi =atan2((array[j].ball_y-array[i].ball_y),(array[j].ball_x-array[i].ball_x));
                 else  if(array[i].ball_x>array[j].ball_x && array[i].ball_y < array[j].ball_y)
                phi =atan2((array[j].ball_y-array[i].ball_y),(array[j].ball_x-array[i].ball_x));
            
            
            array[i].vel_x = array[j].vel_x*cos(phi)*cos(phi) + array[j].vel_y*sin(phi)*cos(phi) - array[i].vel_y*cos(phi)*sin(phi) + array[i].vel_x*sin(phi)*sin(phi);
            array[i].vel_y = array[j].vel_x*cos(phi)*sin(phi) + array[j].vel_y*sin(phi)*sin(phi) + array[i].vel_y*cos(phi)*cos(phi) - array[i].vel_x*sin(phi)*cos(phi);
            array[j].vel_x = array[i].vel_x*cos(phi)*cos(phi) + array[i].vel_y*sin(phi)*cos(phi) - array[j].vel_y*cos(phi)*sin(phi) + array[j].vel_x*sin(phi)*sin(phi);
            array[j].vel_y = array[i].vel_x*cos(phi)*sin(phi) + array[i].vel_y*sin(phi)*sin(phi) + array[j].vel_y*cos(phi)*cos(phi) - array[j].vel_x*sin(phi)*cos(phi);


        float d = sqrt((array[i].ball_y-array[j].ball_y)*(array[i].ball_y-array[j].ball_y) + (array[i].ball_x-array[j].ball_x)*(array[i].ball_x-array[j].ball_x));
         float im1 = 1.0f;
         float im2 = 1.0f;
        array[i].ball_x+=(im1/(im1+im2))*(array[i].ball_x-array[j].ball_x)*((array[i].ball_rad+array[j].ball_rad-d)/d);
        array[i].ball_y+=(im1/(im1+im2))*(array[i].ball_y-array[j].ball_y)*((array[i].ball_rad+array[j].ball_rad-d)/d);
         array[j].ball_x-=(im2/(im1+im2))*(array[i].ball_x-array[j].ball_x)*((array[i].ball_rad+array[j].ball_rad-d)/d);
         array[j].ball_y-=(im2/(im1+im2))*(array[i].ball_y-array[j].ball_y)*((array[i].ball_rad+array[j].ball_rad-d)/d);
        
        
        
       
      }

      if(array[i].ball_x + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_x - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_x *= -alpha*1;
        array[i].vel_y *= alpha;
        array[i].ball_x+=array[i].vel_x;
        array[i].ball_y+=array[i].vel_y;
    }
    if(array[i].ball_y + array[i].ball_rad > (outerbox.box_len-0.5f)/2 || array[i].ball_y - array[i].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[i].vel_y *= -alpha*1;
        array[i].vel_x *= alpha;
        array[i].ball_x+=array[i].vel_x;
        array[i].ball_y+=array[i].vel_y;
    }
    if(array[j].ball_x + array[j].ball_rad > (outerbox.box_len-0.5f)/2 || array[j].ball_x - array[j].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[j].vel_x *= -alpha*1;
        array[j].vel_y *= alpha;
        array[j].ball_x+=array[j].vel_x;
        array[j].ball_y+=array[j].vel_y;
    }
    if(array[j].ball_y + array[j].ball_rad > (outerbox.box_len-0.5f)/2 || array[j].ball_y - array[j].ball_rad < -(outerbox.box_len-0.5f)/2)
    {
        array[j].vel_y *= -alpha*1;
        array[j].vel_x *= alpha;
        array[j].ball_x+=array[j].vel_x;
        array[j].ball_y+=array[j].vel_y;
    }

 
        }
      }
      glutPostRedisplay();
    glutTimerFunc(10, update, 0);
}

void decrease_score(int value) {
    if(flag==1)
    score -=1;
    glutTimerFunc(3000, decrease_score, 0);
}


// Initializing some openGL 3D rendering options
void initRendering() {

    glEnable(GL_DEPTH_TEST);        // Enable objects to be drawn ahead/behind one another
    glEnable(GL_COLOR_MATERIAL);    // Enable coloring
    glClearColor(0.9f, 0.8f, 1.0f, 1.0f);   // Setting a background color
}

// Function called when the window is resized
void handleResize(int w, int h) {

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (float)w / (float)h, 0.1f, 200.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void handleKeypress1(unsigned char key, int x, int y) {

    if (key == 27) {
        exit(0);     // escape key is pressed
    }
    if (key == 97)
    {
        striker.theta +=10;
    }
    if (key ==99)
    {
        striker.theta -=10;
    }
    if (key == 32)
    {
        striker.vel_x = speed * cos((striker.theta+90.0f)*PI/180);
        striker.vel_y = speed * sin((striker.theta+90.0f)*PI/180);
        //cout << striker.vel_x << ' ' << striker.vel_y;
    }
    if(key==9)
    {
        striker.ball_x=0.1f;
        striker.ball_y=-1.14f;
        striker.vel_x=0.0f;
        striker.vel_y=0.0f;
    }
    if(key==98)
    {
        flag=2;
        aim=1;

    }
    if(key==119)
    {
        flag=2;
        aim=2;

    }
    if(key==13)
        flag=1;
}

void handleKeypress2(int key, int x, int y) {

    if (key == GLUT_KEY_LEFT)
        striker.ball_x -= 0.1;
    if (key == GLUT_KEY_RIGHT)
        striker.ball_x += 0.1;
    if (key == GLUT_KEY_UP)
    {

        speed+= 0.001f;
    }
    if (key == GLUT_KEY_DOWN)
    {
        speed -= 0.001f;
        
    }
}

void mouse(int button, int state, int x, int y)
{
     if (state == GLUT_DOWN)
    {
        if (button == GLUT_LEFT_BUTTON)
        {
             int w = glutGet(GLUT_SCREEN_WIDTH);
             int h = glutGet(GLUT_SCREEN_HEIGHT);
             double x1 = double(x)/w;
             double y1 = double(y)/h;
             double opp = y1-striker.ball_y;
             double adj = x1-striker.ball_x;
             striker.theta = atan(opp/adj);
             if(striker.theta < 1.40f)
             {
                striker.theta-=10.0f;
             }
             else
             {
                striker.theta+=10.0f;
             }

            
        }
        
    }
    // Save the left button state
    if (button == GLUT_RIGHT_BUTTON)
    {
        mouserightdown = (state == GLUT_DOWN);
        glutPostRedisplay();  // Left button has changed; array[1]isplay!
    }
     if (state == GLUT_UP)
    {
        if (button == GLUT_LEFT_BUTTON)
            {
                 striker.vel_x = speed * cos((striker.theta+90.0f)*PI/180);
                 striker.vel_y = speed * sin((striker.theta+90.0f)*PI/180);

            }
    }

    // Save the mouse position
    mousex = x;
    mousey = y;
}

// motion
// The GLUT motion function
void motion(int x, int y)
{
    // We only do anything if the left button is down
    if (mouserightdown)
    {

        glutPostRedisplay();
    }
    // Save the mouse position
    mousex = x;
    mousey = y;
}