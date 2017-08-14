#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define uint8_t int
#define uint16_t int
#define uint32_t int
#define int16_t int
#define int8_t int

#define Sound_Over_Level        (200)
#define Sample_Num              501
#define FILE_NAME               "water_data.txt"

typedef struct
{
    uint16_t freq;
    uint16_t level;
}VALID_FREQ;

int16_t water_data[Sample_Num] = {0}; 

static uint16_t start;
static uint16_t end;
uint8_t check_freq_amount;
uint16_t check_freq[2];
uint8_t check_freq_index;
int8_t check_freq_offset;

uint8_t valid_freq_amount;
//uint16_t valid_freq[20];
uint8_t last_valid_freq;
uint8_t check_freq_flag;
uint8_t valid_type;
uint8_t last_valid_index1;
uint8_t last_valid_index2;

int water_ml_global;

VALID_FREQ valid_freq[30];
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
#if 1 	
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

    for(i= 0; i < 501; i++)
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
                        while(water_data[max_level_freq - index1] > 900)
                        {
                            if( max_level_freq - index1 <= 6 ) break;

                            ++index1;
                        }
                        index1--;

                        while(water_data[max_level_freq + index2] > 900)
                        {
                            if( max_level_freq + index2 >= Sample_Num - 6 ) break;

                            ++index2;
                        }
                        index2--;
                    }

                    if( max_level_freq - index1 > 6 && max_level_freq + index2 < Sample_Num - 6 )
                    {
                        for(j=1 ; j< 6; j++)
                        {
                            if( water_data[max_level_freq - index1 - j] < water_data[max_level_freq - index1] * 6 / 10 ||
                                water_data[max_level_freq + index2 + j] < water_data[max_level_freq + index2] * 6 / 10 )
                            {
                                if( full_flag == 0 )
                                {
                                    valid_freq[valid_freq_amount].freq = max_level_freq;
                                    valid_freq[valid_freq_amount].level = max_level;
                                }
                                else
                                {
                                    valid_freq[valid_freq_amount].freq = first_full_freq; // + (last_full_freq - first_full_freq)/4;
                                    valid_freq[valid_freq_amount].level = 1023;
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
        min_level[0] = valid_freq[i].freq * 17;
        
        if( valid_freq[i].freq < 2200)
        {
            max_level[0] = valid_freq[i].freq * 19;
        }
        else
        {
            max_level[0] = valid_freq[i].freq * 21;
        }

        max_level[1] = valid_freq[i].freq * 27;
        min_level[1] = valid_freq[i].freq * 25;
        
        max_level[2] = valid_freq[i].freq * 14;
        min_level[2] = valid_freq[i].freq * 16;

        for( j = last_valid_index2; j < valid_freq_amount; j++ )
        {
            for( k=0; k<3; k++)
            {
                if( valid_freq[j].freq * 10 < max_level[k] && valid_freq[j].freq * 10 > min_level[k] )
                {
                    if( k != 2 || ( valid_freq[i].freq > 2000))
                    {
                        check_freq_amount = 2;
                        check_freq[0] = i; //valid_freq[i].freq;
                        check_freq[1] = j; //valid_freq[j].freq;
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

    uint32_t freq_index_for_ML[49]=
    {
        1170, 1190, 1210, 1230, 1250, 
        1270, 1290, 1310, 1340, 1360, 
        1390, 1420, 1450, 1480, 1510, 
        1540, 1570, 1610, 1650, 1690, 
        1730, 1770, 1810, 1860, 1910, 
        1960, 2010, 2060, 2120, 2170, 
        2230, 2300, 2360, 2430, 2500, 
        2580, 2670, 2760, 2860, 2970, 
        3100, 3230, 3390, 3570, 3820, 
        4110, 4490, 5090, 5980
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
    
                if( meet_freq >= freq_index_for_ML[48] )
                {
                    water_ml = 480;
                    //printf("1 %d\n", water_ml);
                    water_ml_global = water_ml;
                }
                else
                {
                    for( i=0; i< 49; i++)
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
                meet_freq  = valid_freq[last_valid_index1].freq * 10 / 18;
            }
            else
            {
                meet_freq  = valid_freq[last_valid_index1].freq;
            }
        }

        if( valid_type == 1 )
        {
            for( i=last_valid_index1; i < valid_freq_amount; i++ )
            {
                if( valid_freq[i].freq > 2800 )
                {
                    meet_freq  = valid_freq[i].freq;
                    last_valid_index1 = i+1;
                    check_freq_amount = 1;
                    check_freq[0] = i; //valid_freq[i].freq;
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
        valid_freq[i].freq = 1000 + valid_freq[i].freq * 10;
        printf("harmonic[%d] = %d, %f\n",i , valid_freq[i].freq, ((float)valid_freq[i].freq/(float)valid_freq[0].freq));
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
