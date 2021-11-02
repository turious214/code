void robotAutoMotorMove(struct Robot * robot, int front_left_sensor, int front_right_sensor, int left_sensor, int right_sensor) {
    int defaultDirection = 1;

    if ((front_left_sensor == 0) && (front_right_sensor == 0)) {
        if (robot->currentSpeed<5)
            robot->direction = UP;
    }
    
    if (front_left_sensor - front_right_sensor > 0) {
        robot->direction = RIGHT;
    }
    else if (front_right_sensor - front_left_sensor > 0) {
        robot->direction = LEFT;
    }

    if ((front_left_sensor > 0) && (front_right_sensor > 0)) {
        robot->direction = DOWN;
        robot->direction = DOWN;
        if (left_sensor >0) {
            robot->direction = RIGHT;
        }
        else if (right_sensor >0) {
            robot->direction = LEFT;
        }
        else if (left_sensor <1 || right_sensor <1) {
            if (front_left_sensor - front_right_sensor > 0) {
                robot->direction = RIGHT;
            }
            else if (front_right_sensor - front_left_sensor > 0) {
                robot->direction = LEFT;
            }
            else if (front_right_sensor == front_left_sensor) {
                robot->direction = LEFT;
            }
        }
    } 
    
    if (left_sensor - right_sensor > 3 ) {
        robot->direction = RIGHT;
    }
    else if (right_sensor - left_sensor > 3 ) {
        robot->direction = LEFT;
    }
}
