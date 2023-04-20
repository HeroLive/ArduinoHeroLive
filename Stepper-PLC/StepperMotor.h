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
        double interval = 0;
        bool p_state = false;
        bool d_state = false;

    public:
        StepperMotor(int p, int d);
        long getCurrentPosition();
        bool getDirection();  
        bool getExeCompleteFlag(); 
        void PLSV(long f); //f: speed    
        void DRVI(long p, long f); //p: pulse, f: speed
        void DRVA(long p, long f); //p: pulse, f: speed
        void ZRN(long fr, long fc, int ns); //fr: Zero Return Speed, fc: Creep Speed, ns: Near Point Signal 
};

#endif