#ifndef StepperMotor_h
#define StepperMotor_h

class StepperMotor {
    private:
        int pp;
        int pd;
        long positon = 0;
        bool direction = false;
        long _time;

    public:
        StepperMotor(int p, int d);
        long getCurrentPosition();
        bool getDirection();    
        void PLSV(long f);    
};

#endif