#ifndef StepperMotor_h
#define StepperMotor_h

class StepperMotor {
    private:
        int pp;
        int pd;
        long position = 0;
        long increment = 0;
        bool exe_complete_flag = false;
        bool pulse_output_flag = false;
        bool direction = false;
        long t_us;
        bool p_state = false;

    public:
        StepperMotor(int p, int d);
        long getCurrentPosition();
        bool getDirection();  
        bool getExeCompleteFlag(); 
        void PLSV(long f);    
        void DRVI(long p, long f);
};

#endif