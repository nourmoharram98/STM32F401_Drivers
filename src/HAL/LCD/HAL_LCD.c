#include"typedefs.h"
#include"Error_states.h"
#include"HAL/LCD/HAL_LCD.h"
#include"MCAL/GPIO/STM32F401cc_MCAL_GPIO.h"
#include"APPLICATION/APP1.h"


/* Macros for CLCD Commands */
#define CLCD_CLEAR_DISPLAY                  0x01
#define CLCD_RETURN_HOME                    0x02
#define CLCD_ENTRY_MODE_SET                 0x06
#define CLCD_DISPLAY_BLINK_CURSOR           0x0f
#define CLCD_DISPLAY                        0x0c
#define CLCD_DISPLAY_CURSOR                 0x0d
#define CLCD_FUNCTION_SET_8BITMODE_TWO_LINES_MODE 	0x38

#define LCD_ENABLE_LOW          0x00
#define LCD_ENABLE_HIGH         0x01

#define MAX_BUFFER_SIZE         10
#define MAX_STRING_SIZE         32

#define END_OF_ROW1             16
#define END_OF_ROW2             33

/********************Location in DDRAM handler for Lines*********************/
#define INDEX_FOR_LINE_1 0X0
#define INDEX_FOR_LINE_2 0X40
#define INDEX_FOR_LINE_3 0X10
#define INDEX_FOR_LINE_4 0X50
#define WRITE_ON_DDRAM_INDEX 0X80


typedef enum
{
    LCD_ReqReady,
    LCD_ReqBusy,
}LCD_UserRequestState;

typedef enum
{
    LCD_NoReq,
    LCD_ReqWrite,
    LCD_ReqClear,
    LCD_ReqSetPos,
}LCD_UserRequestType;


typedef enum
{
    LCD_WriteStart,
    LCD_WriteCharacter,
    LCD_WriteDone
} LCD_WriteState_t;

typedef enum
{
    LCD_SetPosStart,
    LCD_SetPos,
    LCD_SetPosEnd
} LCD_SetPosState_t;
typedef enum
{
    LCD_PowerOn,
    LCD_FunctionSet,
    LCD_DisplayControl,
    LCD_ClearDisplay,
    LCD_EntryModeSet,
    LCD_EndInit
} LCD_InitMode_t;


typedef struct
{
    u8 CurRowPostion;
    u8 CurColPostion;
}LCD_PosCord;

/*an object has been created from this struct*/
struct
{
    const char *UserString[MAX_BUFFER_SIZE]; //buffer
    //const char *UserString; //buffer
    u8 Len[MAX_BUFFER_SIZE];
    //u8 Len;
    LCD_UserRequestState State;
    LCD_UserRequestType Type;
    LCD_PosCord CurrentPos;
} LCD_UserRequest;



/*******************************************************************************
 *                              Variables                                       *
 *******************************************************************************/
extern const HAL_LCD_Configuration_t LCD_Configurations[NumberOfLCDs];
static u32 Global_EnablePinState = LCD_ENABLE_LOW;
static LCD_State_t Global_LCD_State = LCD_Off; //trigger of LCD System (1)
static LCD_InitMode_t Global_LCD_InitMode = LCD_PowerOn; //(3)
static LCD_WriteState_t Global_WriteState = LCD_WriteStart;
//static LCD_PosCord Global_CurrentWritePostion={LCD_DISPLAY_ROW1,LCD_DISPLAY_COL_START};
static u8 Global_BufferCounter=0;
static u8 Global_SetPos_Status=LCD_SetPosStart;

static u8 Cursor_Location=0;
static u8 Overflow_flag=0;
/*******************************************************************************
 *                         Static Function Prototypes		                   *
 *******************************************************************************/
static Sys_enuErrorStates_t LCD_Init_Process(void);
static Sys_enuErrorStates_t LCD_PowerOnProcess(void);
static Sys_enuErrorStates_t LCD_Write_Process(void);
static Sys_enuErrorStates_t LCD_SetPosition_Process(void);
static Sys_enuErrorStates_t LCD_Helper_SetPosition(u8 *PTR_PostionDDRAM);
static Sys_enuErrorStates_t LCD_WriteCommand(char Copy_LCDCommand);
static Sys_enuErrorStates_t LCD_WriteData(char Copy_LCDData);
//static Sys_enuErrorStates_t LCD_CtrlEnablePin(u8 Copy_LCDEnablePinState);
static Sys_enuErrorStates_t LCD_Helper_Clear(void);
static Sys_enuErrorStates_t LCD_OverFlowHandler(void);


/**
 * @brief functions APIs
 * 
 */

void LCD_Runnable(void)
{
    switch(Global_LCD_State)
    {
        case LCD_Init:
            LCD_Init_Process();
            break;
        case LCD_Operation:
            if(LCD_UserRequest.State==LCD_ReqBusy)
            {
                switch(LCD_UserRequest.Type)
                {
                    case LCD_ReqWrite:
                        LCD_Write_Process();
                        break;
                    case LCD_ReqClear:
                        LCD_Helper_Clear();
                    case LCD_ReqSetPos:
                        LCD_SetPosition_Process();
                        break;
                    default:
                        break;
                }
            }
            break;
        case LCD_Off:
            break;
        case LCD_OverFlow:
            LCD_OverFlowHandler();
            break;
        default:
            break;
    }
}
/**
 * Static functions
*/
static Sys_enuErrorStates_t LCD_Init_Process(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    switch(Global_LCD_InitMode)
    {
        case LCD_PowerOn:
            Error_Status=LCD_PowerOnProcess();
            Global_LCD_InitMode=LCD_FunctionSet;
            break;
        case LCD_FunctionSet:
            Error_Status=LCD_WriteCommand(CLCD_FUNCTION_SET_8BITMODE_TWO_LINES_MODE);
            if(Global_EnablePinState==LCD_ENABLE_LOW)
            {
                Global_LCD_InitMode=LCD_DisplayControl;
            }
            break;
        case LCD_DisplayControl:
            Error_Status=LCD_WriteCommand(CLCD_DISPLAY_BLINK_CURSOR);
            if(Global_EnablePinState==LCD_ENABLE_LOW)
            {
                Global_LCD_InitMode=LCD_ClearDisplay;
            }
            break;
        case LCD_ClearDisplay:
            Error_Status=LCD_WriteCommand(CLCD_CLEAR_DISPLAY);
            if(Global_EnablePinState==LCD_ENABLE_LOW)
            {
                Global_LCD_InitMode=LCD_EntryModeSet;
            }
            break;
        case LCD_EntryModeSet:
            Error_Status=LCD_WriteCommand(CLCD_ENTRY_MODE_SET);
            if(Global_EnablePinState==LCD_ENABLE_LOW)
            {
                Global_LCD_InitMode=LCD_EndInit;
            }
            break;
        case LCD_EndInit:
            LCD_UserRequest.State=LCD_ReqReady; //(4)
            LCD_UserRequest.Type=LCD_NoReq;
            Global_LCD_State=LCD_Operation; //(5)
        default:
            Error_Status=INVALID_INPUT_VALUE;
            break;
    }
    return Error_Status;
}


static Sys_enuErrorStates_t LCD_PowerOnProcess(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    GPIO_PinConfigs_t Local_PinConfiguration={0};
    Local_PinConfiguration.Pin_Mode=GPIO_PIN_OUTPUT_PUSHPULL_NP;
    Local_PinConfiguration.Pin_Speed=GPIO_PIN_MEDSPEED;
    for(u8 index_p=0;index_p<LCD_Configurations[LCD_One].config.LCD_PIN_NUM_MODE;index_p++)
    {
        Local_PinConfiguration.Pin_num=LCD_Configurations[LCD_One].pins[index_p].Pin;
        Local_PinConfiguration.Port=LCD_Configurations[LCD_One].pins[index_p].Port;
        GPIO_Init_Pin(&Local_PinConfiguration);    
    }
    return Error_Status;
}

static Sys_enuErrorStates_t LCD_Write_Process(void)
{

    Sys_enuErrorStates_t Error_Status=NOT_OK;
    static u8 Local_String_Tracker=0;
    static u8 Local_character_tracker=0;
    switch(Global_WriteState)
    {
        case LCD_WriteStart:
            Local_character_tracker=0;
            Local_String_Tracker=0;
            Global_WriteState=LCD_WriteCharacter;
            Error_Status=OK;
            break;
         case LCD_WriteCharacter:
            if (Local_String_Tracker <= Global_BufferCounter)
            {
                if (Local_character_tracker < LCD_UserRequest.Len[Local_String_Tracker])
                {
                    // Get the next character from the current string and send it to LCD_WriteData
                    u8 character = LCD_UserRequest.UserString[Local_String_Tracker][Local_character_tracker];
                    LCD_WriteData(character);
                    // if(Overflow_flag) //line overflow
                    // {
                    //     //do nothing
                    //     APP1_Runnable2();
                    //     Global_WriteState=LCD_WriteDone;
                    // }
                    // else //writing normally
                    // {
                        if(Global_EnablePinState==LCD_ENABLE_LOW)
                        {
                            // Move to the next character in the current string
                            Local_character_tracker++;
                        }
                    // }
                   
                }
                else
                {
                    // Move to the next string in the buffer
                    Local_String_Tracker++;
                    Local_character_tracker = 0;
                }
            }
            else
            {
                Global_WriteState = LCD_WriteDone;
            }
            // if (Local_character_tracker < LCD_UserRequest.Len)
            // {
            //     // Get the next character from the current string and send it to LCD_WriteData
            //     char character = LCD_UserRequest.UserString[Local_character_tracker];
            //     LCD_WriteData(character);
            //     if(Global_EnablePinState==LCD_ENABLE_LOW)
            //     {
            //         // Move to the next character in the current string
            //         Local_character_tracker++;
            //     }
            // }
            // else
            // {
            //     Global_WriteState = LCD_WriteDone;
            //     Local_character_tracker = 0;
            // }
            Error_Status=OK;
            break;
        case LCD_WriteDone:
            // if(Overflow_flag)
            // {
            //     LCD_UserRequest.Type=LCD_ReqSetPos;
            //     LCD_UserRequest.State=LCD_ReqBusy;
            //     switch(Cursor_Location)
            //     {
            //         case END_OF_ROW1:
            //             LCD_UserRequest.CurrentPos.CurRowPostion=LCD_DISPLAY_ROW2;
            //             LCD_UserRequest.CurrentPos.CurColPostion=LCD_DISPLAY_COL_START;
            //             break;
            //         case END_OF_ROW2:
            //             LCD_UserRequest.CurrentPos.CurRowPostion=LCD_DISPLAY_ROW1;
            //             LCD_UserRequest.CurrentPos.CurColPostion=LCD_DISPLAY_COL_START;
            //             Cursor_Location=0;
            //             break;
            //         default:
            //             break;
            //     }
            // }
            // else
            // {
                Global_BufferCounter=0;
                LCD_UserRequest.Type=LCD_NoReq;
                LCD_UserRequest.State=LCD_ReqReady;
                Global_WriteState=LCD_WriteStart;
            // }
            break;
        default:
            break;
    }
    return Error_Status;
}

static Sys_enuErrorStates_t LCD_SetPosition_Process(void)
{
   Sys_enuErrorStates_t Error_Status=NOT_OK;
   static u8 Local_Position_Command=0;
   switch(Global_SetPos_Status)
   {
    case LCD_SetPosStart:
        LCD_Helper_SetPosition(&Local_Position_Command);
        Global_SetPos_Status=LCD_SetPos;
        Error_Status=OK;
        break;
    case LCD_SetPos:
        LCD_WriteCommand(Local_Position_Command);
        if(Global_EnablePinState==LCD_ENABLE_LOW)
        {
            Global_SetPos_Status=LCD_SetPosEnd;
        }
        Error_Status=OK;
        break;
    case LCD_SetPosEnd:
        Local_Position_Command=0;
        Global_SetPos_Status=LCD_SetPosStart;
        if(Overflow_flag)
        {
            Global_LCD_State=LCD_OverFlow;
            // LCD_UserRequest.State=LCD_ReqBusy;
            // LCD_UserRequest.Type=LCD_ReqWrite;
            // Global_WriteState=LCD_WriteCharacter;
            // Global_LCD_State=LCD_Operation;
            // Overflow_flag=0;
            // if(Cursor_Location==END_OF_ROW1)
            // {
            //     Cursor_Location++;
            // }
            // else if(Cursor_Location==END_OF_ROW2)
            // {
            //     Cursor_Location=0;
            // }
        }
        else
        {
            LCD_UserRequest.State=LCD_ReqReady;
            LCD_UserRequest.Type=LCD_NoReq;
        }
        Error_Status=OK;
        break;
    default:
        break;
   }
   return Error_Status;
}

static Sys_enuErrorStates_t LCD_Helper_SetPosition(u8 *PTR_PostionDDRAM)
{
   Sys_enuErrorStates_t Error_Status=NOT_OK;
   u8 Local_PositioninDDRAM=0;
    if(PTR_PostionDDRAM==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        switch(LCD_UserRequest.CurrentPos.CurRowPostion)
        {
            case LCD_DISPLAY_ROW1:
                Local_PositioninDDRAM=LCD_UserRequest.CurrentPos.CurColPostion +INDEX_FOR_LINE_1;
                break;
            case LCD_DISPLAY_ROW2:
                Local_PositioninDDRAM=LCD_UserRequest.CurrentPos.CurColPostion +INDEX_FOR_LINE_2;
                break;
            case LCD_DISPLAY_ROW3:
                Local_PositioninDDRAM=LCD_UserRequest.CurrentPos.CurColPostion +INDEX_FOR_LINE_3;
                break;
            case LCD_DISPLAY_ROW4:
                Local_PositioninDDRAM=LCD_UserRequest.CurrentPos.CurColPostion +INDEX_FOR_LINE_4;
                break;
            default:
                Error_Status=INVALID_INPUT_VALUE;
                break;
        }
        *PTR_PostionDDRAM=Local_PositioninDDRAM+WRITE_ON_DDRAM_INDEX;
    }
   return Error_Status;
}

static Sys_enuErrorStates_t LCD_WriteCommand(char Copy_LCDCommand)
{
   Sys_enuErrorStates_t Error_Status=NOT_OK;
   GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[RS_8BIT].Port,LCD_Configurations[LCD_One].pins[RS_8BIT].Pin,GPIO_PIN_STATUS_LOW);
   GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[RW_8BIT].Port,LCD_Configurations[LCD_One].pins[RW_8BIT].Pin,GPIO_PIN_STATUS_LOW);
   if(Global_EnablePinState==LCD_ENABLE_LOW)
   {
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D0].Port,LCD_Configurations[LCD_One].pins[D0].Pin,(Copy_LCDCommand>>D0&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D1].Port,LCD_Configurations[LCD_One].pins[D1].Pin,(Copy_LCDCommand>>D1&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D2].Port,LCD_Configurations[LCD_One].pins[D2].Pin,(Copy_LCDCommand>>D2&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D3].Port,LCD_Configurations[LCD_One].pins[D3].Pin,(Copy_LCDCommand>>D3&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D4].Port,LCD_Configurations[LCD_One].pins[D4].Pin,(Copy_LCDCommand>>D4&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D5].Port,LCD_Configurations[LCD_One].pins[D5].Pin,(Copy_LCDCommand>>D5&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D6].Port,LCD_Configurations[LCD_One].pins[D6].Pin,(Copy_LCDCommand>>D6&1));
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D7].Port,LCD_Configurations[LCD_One].pins[D7].Pin,(Copy_LCDCommand>>D7&1));

        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_PIN_STATUS_HIGH);
        Global_EnablePinState=LCD_ENABLE_HIGH;
        Error_Status=OK;
   }
   else if(Global_EnablePinState==LCD_ENABLE_HIGH)
   {
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_PIN_STATUS_LOW);
        Global_EnablePinState=LCD_ENABLE_LOW;
        Error_Status=OK;
   }
   return Error_Status;
}

static Sys_enuErrorStates_t LCD_WriteData(char Copy_LCDData)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
  
    if(Cursor_Location==END_OF_ROW1 || Cursor_Location==END_OF_ROW2)
    {
        Overflow_flag=1;
        Global_LCD_State=LCD_OverFlow;
    }
    else
    {           
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[RS_8BIT].Port,LCD_Configurations[LCD_One].pins[RS_8BIT].Pin,GPIO_PIN_STATUS_HIGH);
        GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[RW_8BIT].Port,LCD_Configurations[LCD_One].pins[RW_8BIT].Pin,GPIO_PIN_STATUS_LOW);
        if(Global_EnablePinState==LCD_ENABLE_LOW)
        {
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_PIN_STATUS_LOW);

            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D0].Port,LCD_Configurations[LCD_One].pins[D0].Pin,(Copy_LCDData>>D0&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D1].Port,LCD_Configurations[LCD_One].pins[D1].Pin,(Copy_LCDData>>D1&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D2].Port,LCD_Configurations[LCD_One].pins[D2].Pin,(Copy_LCDData>>D2&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D3].Port,LCD_Configurations[LCD_One].pins[D3].Pin,(Copy_LCDData>>D3&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D4].Port,LCD_Configurations[LCD_One].pins[D4].Pin,(Copy_LCDData>>D4&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D5].Port,LCD_Configurations[LCD_One].pins[D5].Pin,(Copy_LCDData>>D5&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D6].Port,LCD_Configurations[LCD_One].pins[D6].Pin,(Copy_LCDData>>D6&1));
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D7].Port,LCD_Configurations[LCD_One].pins[D7].Pin,(Copy_LCDData>>D7&1));

            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_PIN_STATUS_HIGH);
            Global_EnablePinState=LCD_ENABLE_HIGH;

            Error_Status=OK;
        }
        else if(Global_EnablePinState==LCD_ENABLE_HIGH)
        {
            Cursor_Location++;
            GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_PIN_STATUS_LOW);
            Global_EnablePinState=LCD_ENABLE_LOW;
            Error_Status=OK;
        }
    }
    // if(Global_EnablePinState==LCD_ENABLE_LOW)
    // {
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_PIN_STATUS_LOW);

    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D0].Port,LCD_Configurations[LCD_One].pins[D0].Pin,(Copy_LCDData>>D0&1));
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D1].Port,LCD_Configurations[LCD_One].pins[D1].Pin,(Copy_LCDData>>D1&1));
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D2].Port,LCD_Configurations[LCD_One].pins[D2].Pin,(Copy_LCDData>>D2&1));
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D3].Port,LCD_Configurations[LCD_One].pins[D3].Pin,(Copy_LCDData>>D3&1));
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D4].Port,LCD_Configurations[LCD_One].pins[D4].Pin,(Copy_LCDData>>D4&1));
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D5].Port,LCD_Configurations[LCD_One].pins[D5].Pin,(Copy_LCDData>>D5&1));
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D6].Port,LCD_Configurations[LCD_One].pins[D6].Pin,(Copy_LCDData>>D6&1));
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[D7].Port,LCD_Configurations[LCD_One].pins[D7].Pin,(Copy_LCDData>>D7&1));

    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_PIN_STATUS_HIGH);
    //         Global_EnablePinState=LCD_ENABLE_HIGH;

    //         Error_Status=OK;
    // }
    // else if(Global_EnablePinState==LCD_ENABLE_HIGH)
    // {
    //         GPIO_Set_PinValue(LCD_Configurations[LCD_One].pins[EN_8BIT].Port,LCD_Configurations[LCD_One].pins[EN_8BIT].Pin,GPIO_PIN_STATUS_LOW);
    //         Global_EnablePinState=LCD_ENABLE_LOW;
    //         Cursor_Location++;
    //         Error_Status=OK;
    // }
   return Error_Status; 
}
/*
static Sys_enuErrorStates_t LCD_CtrlEnablePin(u8 Copy_LCDEnablePinState)
{
   Sys_enuErrorStates_t Error_Status=NOT_OK;

    return Error_Status;
}
*/
static Sys_enuErrorStates_t LCD_Helper_Clear(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    LCD_WriteCommand(CLCD_CLEAR_DISPLAY);
    if(Global_EnablePinState==LCD_ENABLE_LOW)
    {
        LCD_UserRequest.Type=LCD_NoReq;
        LCD_UserRequest.State=LCD_ReqReady;
        Error_Status=OK;
    }
    else
    {
        Error_Status=OK;
    }
    return Error_Status;
}
/**
 * user functions
*/
Sys_enuErrorStates_t LCD_InitAsync(void)
{
    Sys_enuErrorStates_t Error_Status=OK;
    Global_LCD_State=LCD_Init; //(2)
    return Error_Status;
}
Sys_enuErrorStates_t LCD_ClearScreenAsync(void)
{
    Sys_enuErrorStates_t Error_Status=OK;
    if(Global_LCD_State==LCD_Operation && LCD_UserRequest.State==LCD_ReqReady)
    {
        LCD_UserRequest.State=LCD_ReqBusy;
        LCD_UserRequest.Type=LCD_ReqClear;
    }
    else
    {
        Error_Status=NOT_OK;
    }
    return Error_Status;
}
Sys_enuErrorStates_t LCD_SetCursorPosAsync(u8 Copy_LCDPosx,u8 Copy_LCDPosy)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(Global_LCD_State==LCD_Operation && LCD_UserRequest.State==LCD_ReqReady)
    {
        if(Copy_LCDPosx>=LCD_NUM_OF_ROWS || Copy_LCDPosy >= LCD_NUM_OF_COLS)
        {
            Error_Status=INVALID_INPUT_VALUE;
        }
        else
        {
            LCD_UserRequest.State=LCD_ReqBusy;
            LCD_UserRequest.Type=LCD_ReqSetPos;
            LCD_UserRequest.CurrentPos.CurRowPostion=Copy_LCDPosx;
            LCD_UserRequest.CurrentPos.CurColPostion=Copy_LCDPosy;
            Error_Status=OK;
        }
       
    }
    else
    {
        Error_Status=NOT_OK;
    }    
    return Error_Status;
}
Sys_enuErrorStates_t LCD_WriteStringAsync(const char *ptrToString,u8 String_size)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    if(ptrToString == NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else if(String_size >= MAX_STRING_SIZE)
    {
        Error_Status=INVALID_INPUT_VALUE;
    }
    //else if(Global_LCD_State==LCD_Operation && LCD_UserRequest.State==LCD_ReqReady)
    else if(Global_LCD_State==LCD_Operation && Global_BufferCounter<MAX_BUFFER_SIZE)
    {
        LCD_UserRequest.State=LCD_ReqBusy;
        LCD_UserRequest.Type=LCD_ReqWrite;
       // LCD_UserRequest.UserString=ptrToString;
        //LCD_UserRequest.Len=String_size;
        LCD_UserRequest.UserString[Global_BufferCounter]=ptrToString;
        LCD_UserRequest.Len[Global_BufferCounter]=String_size;
        Global_BufferCounter++;
        Error_Status=OK;
    }
    return Error_Status;
}
Sys_enuErrorStates_t LCD_GetStatus(u32* PtrToLCDstatus)
{
    Sys_enuErrorStates_t Error_Status=OK;
    if(PtrToLCDstatus==NULL_POINTER)
    {
        Error_Status=NULL_POINTER_ERROR;
    }
    else
    {
        *PtrToLCDstatus=Global_LCD_State;
    }
    return Error_Status;
}
Sys_enuErrorStates_t LCD_WriteSetCB(LCD_CBFUNC_t PtrTofunc)
{
    Sys_enuErrorStates_t Error_Status=OK;
    
    return Error_Status;
}
Sys_enuErrorStates_t LCD_ClearSetCB(LCD_CBFUNC_t PtrTofunc)
{
    Sys_enuErrorStates_t Error_Status=OK;
    
    return Error_Status;
}
Sys_enuErrorStates_t LCD_SetCursorSetCB(LCD_CBFUNC_t PtrTofunc)
{
    Sys_enuErrorStates_t Error_Status=OK;
    
    return Error_Status;
}static Sys_enuErrorStates_t LCD_OverFlowHandler(void)
{
    Sys_enuErrorStates_t Error_Status=NOT_OK;
    static u8 stage_counter = 0;

    if (Overflow_flag)
    {
        switch (stage_counter)
        {
            case 0:
                // First stage of overflow handling
                Global_LCD_State = LCD_Operation;
                LCD_UserRequest.Type = LCD_ReqSetPos;
                LCD_UserRequest.State = LCD_ReqBusy;
                switch (Cursor_Location)
                {
                    case END_OF_ROW1:
                        LCD_UserRequest.CurrentPos.CurRowPostion = LCD_DISPLAY_ROW2;
                        LCD_UserRequest.CurrentPos.CurColPostion = LCD_DISPLAY_COL_START;
                        break;
                    case END_OF_ROW2:
                        LCD_UserRequest.CurrentPos.CurRowPostion = LCD_DISPLAY_ROW1;
                        LCD_UserRequest.CurrentPos.CurColPostion = LCD_DISPLAY_COL_START;
                        Cursor_Location = 0;
                        break;
                    default:
                        break;
                }
                stage_counter++;
                break;
            case 1:
                // Second stage of overflow handling
                Global_LCD_State = LCD_Operation;
                LCD_UserRequest.State = LCD_ReqBusy;
                LCD_UserRequest.Type = LCD_ReqWrite;
                Global_WriteState = LCD_WriteCharacter;
                Overflow_flag = 0;
                if (Cursor_Location == END_OF_ROW1)
                {
                    Cursor_Location++;
                }
                else if (Cursor_Location == END_OF_ROW2)
                {
                    Cursor_Location = 0;
                }
                stage_counter = 0; // Reset stage counter
                break;
            default:
                break;
        }
        return OK; // Return OK if overflow handling is in progress
    }
    else
    {
        return NOT_OK; // Return NOT_OK if no overflow condition
    }
}
