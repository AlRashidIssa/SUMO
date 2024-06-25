#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784
#define NOTE_A5 880
#define NOTE_B5 988
#define NOTE_C6 1047
#define NOTE_D6 1175

class MAIN
{
public:
    /*
    SUMO ROBOT

    A Class for a robot with motor control, ultrasonic sensor, IR sensors, and a buzzer.

    Attributes:
    -----------
    Pins (motor) -> motor_r, motor_l, motor_en;
    Pins (Ultrasonic) -> TRGI, ECHO;
    Pins (IR) -> F_IR_PIN, B_IR_PIN; // Forward Infrared Pins, Backward Infrared Pins
    Pins (BUZZER) -> BUZZER_PIN

    Methods:
    --------
    setup_pins(int motor_r, int motor_l, int motor_en, int TRGI, int ECHO, int F_IR_PIN, int B_IR_PIN, int BUZZER_PIN):
        Method to initialize pins.

    getDistance(int trigPin, int echoPin):
        Method to get distance from the ultrasonic sensor.

    move_forward():
        Method to move forward.

    move_backward():
        Method to move backward.

    turn_right():
        Method to turn right.

    turn_left():
        Method to turn left.

    stop_robot():
        Method to stop the robot.

    check_IR(int white, int black):
        Method to check IR sensors.

    playSevenDwarfsRingtone():
        Method to play the Seven Dwarfs ringtone on the buzzer.
    */

    // Motor pins
    int motor_r, motor_l, motor_en;
    // Ultrasonic sensor pins
    int TRGI, ECHO;
    // IR sensor pins
    int F_IR_PIN, B_IR_PIN;
    // Buzzer pin
    int BUZZER_PIN;

    void setup_pins(int motor_r, int motor_l, int motor_en, int TRGI, int ECHO, int F_IR_PIN, int B_IR_PIN, int BUZZER_PIN)
    {
        /*
        Method to initialize pins.

        Parameters:
        ----------
        motor_r (int): Pin number for right motor.
        motor_l (int): Pin number for left motor.
        motor_en (int): Pin number for motor enable.
        TRGI (int): Pin number for ultrasonic trigger.
        ECHO (int): Pin number for ultrasonic echo.
        F_IR_PIN (int): Pin number for front IR sensor.
        B_IR_PIN (int): Pin number for back IR sensor.
        BUZZER_PIN (int): Pin number for buzzer.

        Returns:
        -------
        None
        */
        this->motor_r = motor_r;
        this->motor_l = motor_l;
        this->motor_en = motor_en;
        this->TRGI = TRGI;
        this->ECHO = ECHO;
        this->F_IR_PIN = F_IR_PIN;
        this->B_IR_PIN = B_IR_PIN;
        this->BUZZER_PIN = BUZZER_PIN;

        // Initialize pins as outputs and inputs necessary
        pinMode(motor_r, OUTPUT);
        pinMode(motor_l, OUTPUT);
        pinMode(motor_en, OUTPUT);
        pinMode(TRGI, OUTPUT);
        pinMode(ECHO, INPUT);
        pinMode(F_IR_PIN, INPUT);
        pinMode(B_IR_PIN, INPUT);
        pinMode(BUZZER_PIN, OUTPUT);
    }

    long getDistance(int trigPin, int echoPin)
    {
        /*
        Method to get distance from ultrasonic sensor.

        Parameters:
        ----------
        trigPin (int): Pin number for ultrasonic trigger.
        echoPin (int): Pin number for ultrasonic echo.

        Returns:
        -------
        long: Distance measured by the ultrasonic sensor.
        */
        digitalWrite(trigPin, LOW);
        delayMicroseconds(2);
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);

        long duration = pulseIn(echoPin, HIGH);
        long distance = duration * 0.034 / 2;

        return distance;
    }

    void move_forward()
    {
        /*
        Method to move forward.

        Returns:
        --------
        None 
        */
        digitalWrite(motor_r, HIGH);
        digitalWrite(motor_l, LOW);
        analogWrite(motor_en, 255); // Full speed
    }

    void move_backward()
    {
        /*
        Method to move backward.

        Returns:
        --------
        None
        */
        digitalWrite(motor_r, LOW);
        digitalWrite(motor_l, HIGH);
        analogWrite(motor_en, 255); // Full speed
    }

    void turn_right()
    {
        /*
        Method to turn right.

        Returns:
        --------
        None
        */
        digitalWrite(motor_r, HIGH);
        digitalWrite(motor_l, HIGH);
        analogWrite(motor_en, 255); // Full speed
    }

    void turn_left()
    {
        /*
        Method to turn left.

        Returns:
        --------
        None
        */
        digitalWrite(motor_r, LOW);
        digitalWrite(motor_l, LOW);
        analogWrite(motor_en, 255); // Full speed
    }

    void stop_robot()
    {
        /*
        Method to stop the robot.

        Returns:
        --------
        None
        */
        digitalWrite(motor_r, LOW);
        digitalWrite(motor_l, LOW);
        analogWrite(motor_en, 0); // Stop
    }

    int check_IR(int white, int black)
    {
        /*
        Method to check IR sensors.

        Parameters:
        ----------
        white (int): The value for the sensor reading to ensure it is not read again.
        black (int): The value for the color to keep in this color range.

        Returns:
        --------
        int: true or false based on sensor readings.
        */
        int front_ir = digitalRead(F_IR_PIN);
        int back_ir = digitalRead(B_IR_PIN);

        if (front_ir >= white)
        {
            return 1;
        }
        if (back_ir >= white)
        {
            return 2;
        }

        return 0;
    }

    void playSevenDwarfsRingtone()
    {
        /*
        Method to play the Seven Dwarfs ringtone on the buzzer.

        Attributes:
            notes (array):
                Notes for the Seven Dwarfs ringtone
            durations  (array):
                Durations for the Seven Dwarfs ringtone
        */

        // Define notes and durations for the Seven Dwarfs ringtone
        int notes[] = {NOTE_D5, NOTE_E5, NOTE_F5, NOTE_G5, NOTE_A5, NOTE_B5, NOTE_C6, NOTE_D6};
        int durations[] = {4, 4, 4, 4, 4, 4, 4, 4};

        // Play each note in sequence
        for (int i = 0; i < 8; i++)
        {
            tone(BUZZER_PIN, notes[i]);
            delay(500 / durations[i]);
            noTone(BUZZER_PIN);
            delay(50);
        }
    }
};

class STRATEGER : public MAIN
{
public:
    /*
    A Class have all strategies used in the robot.

    Methods (Strategies):
    ---------------------
    keep_in_circle():
        Method to keep the robot moving within the black circle.
    */
    void keep_in_circle()
    {
        int white = 1;
        int black = 0;

        while (true)
        {
            int ir_read = check_IR(white, black);

            if (ir_read == 1)
            {
                move_forward();
            }
            else
            {
                stop_robot();
                turn_left();
                delay(500); // short duration
                stop_robot();
            }

            if (ir_read == 1) // 1 -> the ir Forword
            {
                move_backward();
                delay(500); // Move backward for a short duration
                stop_robot();
                turn_right();
                delay(500); // Turn right for a short duration
                stop_robot();
            }
        }
    }
};

// Define your pin configurations here
#define MOTOR_R 3
#define MOTOR_L 5
#define MOTOR_EN A4
#define TRGI 7
#define ECHO 8
#define IR_F A0
#define IR_B A1
#define BUZZER_PIN A5

// Create an object of STRATEGER class
STRATEGER strateger;

void setup()
{
    Serial.begin(9600);

    strateger.setup_pins(MOTOR_R, MOTOR_L, MOTOR_EN, TRGI, ECHO, IR_F, IR_B, BUZZER_PIN);
    strateger.playSevenDwarfsRingtone();
}

void loop()
{
    strateger.keep_in_circle();
}
