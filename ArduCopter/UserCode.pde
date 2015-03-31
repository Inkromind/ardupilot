/// -*- tab-width: 4; Mode: C++; c-basic-offset: 4; indent-tabs-mode: nil -*-

#include <AMW_Facade.h>
#include <AC_SafetyControl.h>

#ifdef USERHOOK_INIT
void userhook_init()
{
    // put your initialisation code here
    // this will be called once at start-up
    AC_SafetyControl::getInstance()->init();
}
#endif

#ifdef USERHOOK_FASTLOOP
void userhook_FastLoop()
{
    // put your 100Hz code here
    AMW_Facade::run100Hz();
    AC_SafetyControl::getInstance()->run();
}
#endif

#ifdef USERHOOK_50HZLOOP
void userhook_50Hz()
{
    AMW_Facade::run50Hz();
}
#endif

#ifdef USERHOOK_MEDIUMLOOP
void userhook_MediumLoop()
{
    // put your 10Hz code here
    AMW_Facade::run10Hz();
}
#endif

#ifdef USERHOOK_SLOWLOOP
void userhook_SlowLoop()
{
    // put your 3.3Hz code here
}
#endif

#ifdef USERHOOK_SUPERSLOWLOOP
void userhook_SuperSlowLoop()
{
//    ap.CH8_flag = read_3pos_switch(g.rc_8.radio_in);
//    if (ap.CH8_flag == AUX_SWITCH_HIGH) {
//        AMW_Facade::initPlanner();
//        if (control_mode != MAD) {
//            set_mode(MAD);
//        }
//    }
    AMW_Facade::run1Hz();
    // put your 1Hz code here
}
#endif
