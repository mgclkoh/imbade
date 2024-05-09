#ifndef MY_INPUT_H
#define MY_INPUT_H

// 버튼의 떨림을 방지하기 위해서 일정시간 대기한다
#ifndef DEBOUNCE_MIL
#define DEBOUNCE_MIL 30
#endif

// pressesCnt()를 위한 대기 시간이다 
#ifndef CNT_DELAY_MIL
#define CNT_DELAY_MIL 600
#endif

#ifndef MY_INPUT_DELAY_MIL
#define MY_INPUT_DELAY_MIL 3
#endif

// onLongPressed(),longReleased()에서 일정시간 대기한다 
#ifndef LONG_DELAY_MIL
#define LONG_DELAY_MIL 1500
#endif

#define RUN_STATUS_ON_RELEASED 0b11
#define RUN_STATUS_ON_PRESSED  0b00
#define RUN_STATUS_RELEASED    0b01
#define RUN_STATUS_PRESSED     0b10
#define RUN_STATUS_ON_DEBOUNCE -1

#define STATUS_ON_RELEASED           0
#define STATUS_ON_RELEASED_FIRST     1
#define STATUS_PRESSED               2
#define STATUS_ON_PRESSED_FIRST      3
#define STATUS_ON_PRESSED            4
#define STATUS_ON_LONG_PRESSED       5
#define STATUS_ON_LONG_PRESSED_FIRST 6
#define STATUS_RELEASED              7
#define STATUS_RELEASED_FIRST        8
#define STATUS_LONG_RELEASED         9
#define STATUS_CLICK                 10
#define STATUS_COUNT                 11

class MyInput {

  public:
    // constructor에는 void나 돌려 받을 변수형이 없다.
    // closeVal : 접점이 연결될 때의 핀 값 (LOW 또는 HIGH)      
    // pinMode:INPUT_PULLUP 또는 INPUT
    MyInput(int pinNo, int closeVal, int pinMode); 
    MyInput(int pinNo); 
    MyInput(); 

    void setPin(int pinNo, int closeVal, int pinMode); 
    void setPin(int pinNo);

    // loop()의 첫 머리에 둔다.
    // ISR에 두면 안된다.
    int run();
    
    // 접점이 연결될 때 참을 돌려준다. 
    bool pressed(); 

    // 접점이 떨어질 때 참을 돌려준다  
    // 세 함수가 같다  
    bool released();
    bool shortClick();   
    
    // 접점이 계속 떨어져 있을 때 참을 돌려준다.
    // 접점이 계속 떨어져 있으면 반복적으로 참을 돌려준다 
    bool onReleased();    
    // 접점이 계속 연결되어 있을 때 참을 돌려준다.
    // 접점이 계속 연결되어 있으면 반복적으로 참을 돌려준다    
    bool onPressed();

    // 접점이 계속 떨어져 있을 때 한 번만 참을 돌려준다.
    bool onReleasedFirst();   
    // 접점이 계속 연결되어 있을 때 한 번만 참을 돌려준다.
    bool onPressedFirst();
    
    // onLongPressed() 앞에는 반드시 onPressed() 참이 먼저 온다. 
    // onLongPressed()는 반복적으로 참이 온다. 
    // 일정 시간(LONG_DELAY_MIL) 이상 누르고 있을 때
    // 참을 돌려준다.
    bool onLongPressed(); 

    // onLongPressed() 앞에는 반드시 onPressed() 참이 먼저 온다. 
    // 일정 시간(LONG_DELAY_MIL) 이상 누르고 있을 때
    // 한번만 참을 돌려준다.
    // 두 함수가 같다.
    bool onLongPressedFirst();
    bool longClick(); 
    bool longPressed();  // depreciated,사용중지 예정

    // 일정 시간(LONG_DELAY_MIL) 이상 누르고 있다가 
    // 뗄 때 참을 돌려준다.
    bool longReleased();
    
    // 클릭 수를 세다가 일정 시간(CNT_DELAY_MIL) 이후
    // 새로운 클릭이 없으면 그 수를 알려준다.
    int count(); 

    // 아래는 click()은 pressed(),released(),
    // longReleased()와 함께 쓰면 안된다.
    // click()은 그 메쏘드를 수반하기 때문이다.
    
    // 지정한 수만큼 클릭했을 때 참을 돌려준다.    
    boolean click(int clickCnt);
    // 한 번 클릭했을 때 참을 돌려준다.
    boolean click();
    
    //다음에 오는 released()를 무력화한다 
    void avoidReleased();

    //클릭수,길게 누름 등의 기준 시간 설정
    //기본값은 15000 
    void setLongDelayMil(unsigned long longDelayMil);

    int status();   
    
    //핀을 읽은 값 
    int read();
    
  private:
    void init(int INbtnPin, int INpinMode, int INonVal);
    int btnPin;
    int btnCloseVal;
    int btnOpenVal;
    
    int pinVal;
    int PRpinVal;   
    int pinStatus; 
    int PRpinStatus; 

    int RTNstatus = STATUS_RELEASED; 
    
    int pressedCnt = 0;
    int TMPpressedCnt = 0;
    int onLongPressedCnt = 0;
    unsigned long thisMil;
    unsigned long pressedMil = 0;
    
    boolean useShortLongReleased = false;
    boolean useShortLongPressed = false;

    boolean SWvoidReleased = false; 

    unsigned long debounceStartMil = 0;
    boolean  debounceStarted = false;

    unsigned long longDelayMil = LONG_DELAY_MIL;

    int PRpinStatusDebounce = -1;
    int pinStatusDebounce   = -1;

    int onLongClick = false;

};

#endif
