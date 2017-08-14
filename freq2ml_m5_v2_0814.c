#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uint8_t int
#define uint16_t int
#define uint32_t int
#define int16_t int
#define int8_t int

#define Sound_Over_Level        (200)
#define Sample_Num              500
#define FILE_NAME    "100ml.txt"

int16_t water_data[Sample_Num] = {0}; 

static uint16_t start;
static uint16_t end;
uint8_t check_freq_amount;
uint16_t check_freq[2];
uint8_t check_freq_index;
int8_t check_freq_offset;

uint8_t valid_freq_amount;
uint16_t valid_freq[20];
uint8_t last_valid_freq;
uint8_t check_freq_flag;
uint8_t valid_type;
uint8_t last_valid_index1;
uint8_t last_valid_index2;

int water_ml_global;

int16_t water_data[Sample_Num];
const char crlf[2] = {0x0D, 0x0A};

void getSampleData(int16_t *);
int freq2WaterML();

int main(void)
{
    getSampleData(water_data);
    water_ml_global = freq2WaterML();
    printf("--------> %d ml\n", water_ml_global);
    return 0;
}

void getSampleData(int16_t *data)
{
    FILE *article;
    article = fopen(FILE_NAME,"r");
    char *s;
    char buffer[8192];
	int num = 0, i = 0; 
    fread(buffer,8192,1,article);

    s = strtok(buffer,crlf);
	if (s != NULL)
	    data[num++] = atoi(s);
	
    while(s!=NULL)
    {
        s = strtok(NULL,"\n");
		
		if (s == NULL)
		    break; 
		else
		    data[num++] = atoi(s);
    }
#if 0 	
    for(i = 0; i < num; i++)
	    printf("%d\n", data[i]);
#endif		
}

uint16_t calc_vaild_freq()
{
    uint8_t meet_point = 0;
    uint16_t first_meet_freq;
    //uint16_t last_meet_freq;
    uint8_t  full_flag;
    uint16_t first_full_freq;
    uint16_t last_full_freq;
    uint16_t max_level;
    uint16_t max_level_freq;
    int i, j;

    for(i= 0; i < 500; i++)
    {
        if( meet_point == 0 )
        {
            if( water_data[i] > Sound_Over_Level )
            {
                meet_point = 1;
                first_meet_freq = i;
                //last_meet_freq = i;
                full_flag = 0;
                first_full_freq = 0;
                last_full_freq = 0;
                max_level = water_data[i];
                max_level_freq = i;
            }
        }
        else
        {
            if( water_data[i] > Sound_Over_Level )
            {
                if( full_flag == 0 )
                {
                    if(water_data[i] == 1023)
                    {
                        full_flag = 1;
                        first_full_freq = i;
                    }
                    else if(water_data[i] > max_level)
                    {
                        max_level = water_data[i];
                        max_level_freq = i;
                    }
                }
                else if( full_flag == 1)
                {
                    if(water_data[i] < 1023)
                    {
                        full_flag = 2;
                        last_full_freq = i - 1;
                    }
                }
            }
            else
            {
                //last_meet_freq = i - 1;

                //start = i;

                meet_point = 0;

                if( (i - first_meet_freq) > 2 )
                {
                    uint8_t index1= 0, index2=0;

                    if( max_level > 900 )
                    {
                        while(water_data[max_level_freq - ++index1] > 900)
                        {
                            if( max_level_freq - index1 <= 6 ) break;
                        }
                        index1--;

                        while(water_data[max_level_freq + ++index2] > 900)
                        {
                            if( max_level_freq + index2 >= Sample_Num - 6 ) break;
                        }
                        index2--;
                    }

                    if( max_level_freq - index1 > 6 && max_level_freq + index2 < Sample_Num - 6 )
                    {
                        for(j=1 ; j< 6; j++)
                        {
                            if( water_data[max_level_freq - index1 - j] < water_data[max_level_freq - index1] / 2 ||
                                water_data[max_level_freq + index2 + j] < water_data[max_level_freq + index2] / 2 )
                            {
                                if( full_flag == 0 )
                                {
                                    valid_freq[valid_freq_amount] = max_level_freq;
                                }
                                else
                                {
                                    valid_freq[valid_freq_amount] = (first_full_freq + last_full_freq)/2;
                                }

                                valid_freq_amount++;

                                break;
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

uint8_t check_harmonic()
{
    uint8_t i,j,k;
    uint32_t max_level[3], min_level[3];

    for( i=last_valid_index1; i < valid_freq_amount - 1; i++ )
    {
        min_level[0] = valid_freq[i] * 17;
        
        if( valid_freq[i] < 2200)
        {
            max_level[0] = valid_freq[i] * 19;
        }
        else
        {
            max_level[0] = valid_freq[i] * 21;
        }

        if( valid_freq[i] < 2200)
        {
            max_level[1] = valid_freq[i] * 27;
            min_level[1] = valid_freq[i] * 25;
        }
        else
        {
            max_level[1] = valid_freq[i] * 28;
            min_level[1] = valid_freq[i] * 26;
        }
        
        min_level[2] = valid_freq[i] * 14;
        max_level[2] = valid_freq[i] * 16;

        for( j = last_valid_index2; j < valid_freq_amount; j++ )
        {
            for( k=0; k<3; k++)
            {
                if( valid_freq[j] * 10 < max_level[k] && valid_freq[j] * 10 > min_level[k] )
                {
                    if( k != 2 || ( valid_freq[i] > 2000 && valid_freq[i] < 3600))
                    {
                        check_freq_amount = 2;
                        check_freq[0] = valid_freq[i];
                        check_freq[1] = valid_freq[j];
                        last_valid_index1 = i;
                        last_valid_index2 = j + 1;
                        return k;
                    }
                }
            }
        }

        last_valid_index2 = i + 2;
    }
    
    return 3;
}

void calc_water_ml()
{
    uint16_t water_ml;
    uint16_t i;
    uint8_t result;
    static uint32_t meet_freq;
    
    //Brandon
    uint32_t freq_index_for_ML[47]=
    {
        1170, 1190, 1205, 1230, 1260, 
        1290, 1310, 1340, 1360, 1400, 
        1430, 1460, 1490, 1520, 1540, 
        1590, 1610, 1650, 1690, 1720, 
        1750, 1810, 1840, 1880, 1930, 
        1980, 2020, 2070, 2110, 2170, 
        2210, 2280, 2325, 2400, 2450, 
        2540, 2640, 2730, 2820, 2970, 
        3150, 3310, 3550, 3880, 4310, 
        5040, 5730
    };

    //s_calc_water_tid = 0; //Brandon

    if( check_freq_flag == 1 )
    {
        //if( adc_Mic() > 200 ) //Brandon
        {
            if(++check_freq_index < check_freq_amount)
            {
                check_freq_offset = -50;
                //sound_Run(check_freq[check_freq_index] + check_freq_offset);
                //s_calc_water_tid = app_easy_timer(10, calc_water_ml);
                calc_water_ml();
            }
            else
            {
                //adc_Temp(); //Brandon 
                
                //meet_freq = meet_freq * 57045 /(54600 + 100 * SysInfo.show_temp); //Brandon
    
                if( meet_freq >= freq_index_for_ML[46] )
                {
                    water_ml = 470;
                    //printf("1 %d\n", water_ml);
                    water_ml_global = water_ml;
                }
                else
                {
                    for( i=0; i< 47; i++)
                    {
                        if( meet_freq < freq_index_for_ML[i] )
                        {
                            if( i==0 )
                            {
                                water_ml = 0;
                                //printf("2 %d\n", water_ml);
                                water_ml_global = water_ml;
                            }
                            else
                            {
                                water_ml = (i-1) * 10 + (freq_index_for_ML[i] - meet_freq ) * 10 / 
                                           (freq_index_for_ML[i] - freq_index_for_ML[i-1]);
                                //printf("3 %d\n", water_ml); 
                                water_ml_global = water_ml;
                            }
    
                            break;
                        }
                    }
                }
    
                //calc_drink_ml(water_ml); //Brandon
#ifdef DISPLAY_ML
                //show_water_level(water_ml); //Brandon
#else
                //show_keep_water(); //Brandon
#endif
                return;
            }
        }
/*
        else
        {
            if( check_freq_offset < 50 )
            {
                check_freq_offset += 10;
                //sound_Run(check_freq[check_freq_index] + check_freq_offset);
                //s_calc_water_tid = app_easy_timer(1, calc_water_ml); //Brandon
                calc_water_ml();
            }
            else
            {
                if(++check_freq_index < check_freq_amount)
                {
                    check_freq_offset = -50;
                    //sound_Run(check_freq[check_freq_index] + check_freq_offset);
                    //s_calc_water_tid = app_easy_timer(10, calc_water_ml); //Brandon
                    calc_water_ml();
                }
                else
                {
                    check_freq_flag = 0;
                }
            }
        }
*/
    }

    if( check_freq_flag == 0 )
    {
        if( valid_type == 0 )
        {
            result = check_harmonic();

            if(result == 3)
            {
                valid_type = 1;
                last_valid_index1 = 0;
            }
            else if( result == 2)
            {
                meet_freq  = valid_freq[last_valid_index1] * 10 / 18;
            }
            else
            {
                meet_freq  = valid_freq[last_valid_index1];
            }
        }

        if( valid_type == 1 )
        {
            for( i=last_valid_index1; i < valid_freq_amount; i++ )
            {
                if( valid_freq[i] > 2000 )
                {
                    meet_freq  = valid_freq[i];
                    last_valid_index1 = i+1;
                    check_freq_amount = 1;
                    check_freq[0] = valid_freq[i];
                    break;
                }
            }

            if( i == valid_freq_amount)
            {
                //calc_drink_ml(485); //Brandon
#ifdef DISPLAY_ML
                //show_water_level(485); //Brandon
#else
                //show_keep_water(); //Brandon
#endif
                return;
            }
        }

        check_freq_offset = -50;
        check_freq_index = 0;
        
        //sound_Run(check_freq[check_freq_index] + check_freq_offset); //Brandon

        check_freq_flag = 1;

//Brandon add start
        if (check_freq_flag == 1)
        {
            calc_water_ml();
        }
//Brandon add end

        //s_calc_water_tid = app_easy_timer(10, calc_water_ml);
    }
}

int freq2WaterML()//
{
    uint16_t i;

    valid_freq_amount = 0;

    calc_vaild_freq();

    for( i=0; i < valid_freq_amount; i++ )
    {
        valid_freq[i] = 1000 + valid_freq[i] * 10;
        printf("harmonic[%d] = %d, %f\n",i , valid_freq[i], ((float)valid_freq[i]/(float)valid_freq[0]));
    }
 //Brandon
    if( valid_freq_amount == 0 )
    {
        //calc_drink_ml(485);
#ifdef DISPLAY_ML
        //show_water_level(485);
#else
        //show_keep_water();
#endif
        water_ml_global = 485;
    }
    else
    {
        check_freq_flag = 0;
        valid_type = 0;
        last_valid_index1 = 0;
        last_valid_index2 = 1;
        calc_water_ml();
    }
    return water_ml_global;
}
