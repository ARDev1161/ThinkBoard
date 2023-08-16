#define HC165_DS 6
#define HC165_CE 7
#define HC165_PL 8
#define HC165_CP 9

struct Button{
  int addr = 0;
  char symbol = '\0';  
};
  
struct Joystick{
    Button center;
    Button up;
    Button down;
    Button left;
    Button right;
};

struct Keyboard{
    Joystick ThumbUp;
    Joystick ThumbDown;
    Joystick Index;
    Joystick Middle;
    Joystick Ring;
    Joystick Little;
} keyboard;
