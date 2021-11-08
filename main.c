#include "stdio.h"
#include "stdlib.h"
#include "sdl.h"
#include "SDL2_gfxPrimitives.h"
#include "time.h"

#include "formulas.h"
#include "wall.h"
#include "robot.h"
#include "math.h"

int done = 0;


int main(int argc, char *argv[]) {
    SDL_Window *window;
    SDL_Renderer *renderer;

    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        return 1;
    }

    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    window = SDL_CreateWindow("Robot Maze", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, OVERALL_WINDOW_WIDTH, OVERALL_WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, 0);

    struct Robot robot;
    struct Wall_collection *head = NULL;
    int front_left_sensor, front_right_sensor=0;
    int left_sensor, right_sensor=0;
    clock_t start_time, end_time;
    int msec;

    // SETUP MAZE
    // You can create your own maze here. line of code is adding a wall.
    // You describe position of top left corner of wall (x, y), then width and height going down/to right
    // Relative positions are used (OVERALL_WINDOW_WIDTH and OVERALL_WINDOW_HEIGHT)
    // But you can use absolute positions. 10 is used as the width, but you can change this.
    insertAndSetFirstWall(&head, 1, 1, 730, 644, 5);
    insertAndSetFirstWall(&head, 2, 1, 1, 5, 640);
    insertAndSetFirstWall(&head, 3, 1, 640, 230, 5);
    insertAndSetFirstWall(&head, 4, 320, 640, 5, 90);
    insertAndSetFirstWall(&head, 5, 320, 640, 230, 5);  // modi
    insertAndSetFirstWall(&head, 6, 550, 640, 5, 90);
    insertAndSetFirstWall(&head, 7, 640, 100, 5, 630);
    insertAndSetFirstWall(&head, 8, 6, 1, 645, 5);
    insertAndSetFirstWall(&head, 9, 150, 415, 5, 120);  // modi
    insertAndSetFirstWall(&head, 10, 95, 530, 355, 5);
    insertAndSetFirstWall(&head, 11, 260, 310, 5, 220); // modi
    insertAndSetFirstWall(&head, 12, 445, 415, 5,130);
    insertAndSetFirstWall(&head, 13, 450, 415, 90,5);
    insertAndSetFirstWall(&head, 14, 540, 415, 5,120);
    insertAndSetFirstWall(&head, 15, 540, 530, 100,5);
    insertAndSetFirstWall(&head, 16, 1, 310, 210,5);
    insertAndSetFirstWall(&head, 17, 206, 200, 5,280);  // modi
    insertAndSetFirstWall(&head, 18, 260, 310, 190, 5);  // modi
    insertAndSetFirstWall(&head, 19, 445, 200, 5,115);
    insertAndSetFirstWall(&head, 20, 540, 100, 5,210);
    insertAndSetFirstWall(&head, 21, 540, 310, 100,5);
    insertAndSetFirstWall(&head, 22, 105, 100, 5,100);
    insertAndSetFirstWall(&head, 23, 105, 200, 205,5);
    insertAndSetFirstWall(&head, 24, 215, 6, 5,100);
    insertAndSetFirstWall(&head, 25, 215, 106, 230,5);
    setup_robot(&robot);
    updateAllWalls(head, renderer);

    SDL_Event event;
    while(!done){
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
        SDL_RenderClear(renderer);

        //Move robot based on user input commands/auto commands
        if (robot.auto_mode == 1)
            robotAutoMotorMove(&robot, front_left_sensor, front_right_sensor, left_sensor, right_sensor);
        robotMotorMove(&robot);

        //Check if robot reaches endpoint. and check sensor values
        if (checkRobotReachedEnd(&robot, 640-10-220, 480, 100, 10)){ //Maze 8
            end_time = clock();
            msec = (end_time-start_time) * 1000 / CLOCKS_PER_SEC;
            robotSuccess(&robot, msec);
        }
        else if(checkRobotHitWalls(&robot, head))
            robotCrash(&robot);
        //Otherwise compute sensor information
        else {
            front_left_sensor = checkRobotSensorFrontLeftAllWalls(&robot, head);
            if (front_left_sensor>0)
                printf("Getting close on the left. Score = %d\n", front_left_sensor);

            front_right_sensor = checkRobotSensorFrontRightAllWalls(&robot, head);
            if (front_right_sensor>0)
                printf("Getting close on the right. Score = %d\n", front_right_sensor);

            left_sensor = checkRobotSensorLeftAllWalls(&robot, head);
            if (left_sensor>0)
                printf("Getting close on the add left. Score = %d\n", left_sensor);

            right_sensor = checkRobotSensorRightAllWalls(&robot, head);
            if (right_sensor>0)
                printf("Getting close on the add right. Score = %d\n", right_sensor);
        }
        robotUpdate(renderer, &robot);
        updateAllWalls(head, renderer);

        // Check for user input
        SDL_RenderPresent(renderer);
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                done = 1;
            }
            const Uint8 *state = SDL_GetKeyboardState(NULL);
            if(state[SDL_SCANCODE_UP] && robot.direction != DOWN){
                robot.direction = UP;
            }
            if(state[SDL_SCANCODE_DOWN] && robot.direction != UP){
                robot.direction = DOWN;
            }
            if(state[SDL_SCANCODE_LEFT] && robot.direction != RIGHT){
                robot.direction = LEFT;
            }
            if(state[SDL_SCANCODE_RIGHT] && robot.direction != LEFT){
                robot.direction = RIGHT;
            }
            if(state[SDL_SCANCODE_SPACE]){
                setup_robot(&robot);
            }
            if(state[SDL_SCANCODE_RETURN]){
                robot.auto_mode = 1;
                start_time = clock();
            }
        }

        SDL_Delay(120);
    }
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    printf("DEAD\n");
}
