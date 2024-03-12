#ifndef     TRAFFIC_LIGHT_APP
#define     TRAFFIC_LIGHT_APP


/**
 * @brief enumeration for the States of traffic light
 * 
 */
typedef enum
{
    Red_State,
    Green_State,
    Yellow_State,
    No_State,
    Number_of_States
}TrafficLight_States_t;

/**
 * @brief runnable to run the applicaiton of traffic light sequence
 * 
 */
void TrafficLightRunnable(void);



#endif