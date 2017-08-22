#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define DEBUG 1
#define FILE_NAME               "water_data.txt"
#define SOUND_OVER_LEVEL        200
#define SAMPLE_NUM              501

typedef struct
{
    uint16_t freq;
    uint16_t level;
}VALID_FREQ;

void read_rawData_file(char *file_name, int16_t *raw_data, int16_t *raw_data_amount);
void calc_valid_freq(int16_t *raw_data, VALID_FREQ *valid_freq, int8_t *valid_freq_amount);
int8_t check_harmonic(VALID_FREQ *valid_freq, int8_t valid_freq_amount, \
	                  int8_t *harmonic_index, int8_t *harmonic_amount);
int16_t calc_meet_freq(int8_t result, VALID_FREQ *valid_freq, int8_t valid_freq_amount, \
	                   int8_t *harmonic_index, int8_t harmonic_amount);

int8_t meas_harmonic_again(VALID_FREQ *valid_freq, int8_t *harmonic_index, int8_t *harmonic_amount);
int16_t map_freq2ml(int16_t freq);

const char crlf[2] = {0x0D, 0x0A};

int main(int argc, char* argv[])
{
	int i;
	int8_t result;
	int16_t water_ml;
    int8_t harmonic_index[5] = {0}, harmonic_amount, valid_freq_amount=0;
	int16_t raw_data_amount, meet_freq;
    int16_t raw_data[501] = {0};
    VALID_FREQ valid_freq[30] = {0};
    char *file_name;

    file_name = argv[1];
    read_rawData_file(file_name, raw_data, &raw_data_amount);

    calc_valid_freq(raw_data, valid_freq, &valid_freq_amount);

    for( i=0; i < valid_freq_amount; i++ )
    {
        valid_freq[i].freq = 1000 + valid_freq[i].freq * 10;
    }

#if DEBUG  
    for(i = 0; i < valid_freq_amount; i++)
	    printf("%d\n", valid_freq[i].freq);
#endif    

    result = check_harmonic(valid_freq, valid_freq_amount, harmonic_index, &harmonic_amount);

#if DEBUG    
    printf("result: %d\n", result);
#endif    

    meet_freq = calc_meet_freq(result, valid_freq, valid_freq_amount, harmonic_index, harmonic_amount);

    printf("meet_freq: %dHz\n", meet_freq);
    
    //result = meas_harmonic_again(valid_freq, harmonic_index, &harmonic_amount);
    water_ml = map_freq2ml(meet_freq);
     
    printf("water_level: %dml\n", water_ml);

    return 0;
}

void read_rawData_file(char* file_name, int16_t *data, int16_t *num_data)
{
    FILE *article;
    article = fopen(file_name,"r");
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

void calc_valid_freq(int16_t *raw_data, VALID_FREQ *valid_freq, int8_t *valid_freq_amount)
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
            if( raw_data[i] > SOUND_OVER_LEVEL )
            {
            	//printf("%d %d\n", i, raw_data[i]);
                meet_point = 1;
                first_meet_freq = i;
                //last_meet_freq = i;
                full_flag = 0;
                first_full_freq = 0;
                last_full_freq = 0;
                max_level = raw_data[i];
                max_level_freq = i;
            }
        }
        else
        {
            if( raw_data[i] > SOUND_OVER_LEVEL )
            {
                if( full_flag == 0 )
                {
                    if(raw_data[i] > max_level)
                    {
                        max_level = raw_data[i];
                        max_level_freq = i;

	                    if(raw_data[i] == 1023)
	                    {
	                        full_flag = 1;
	                        first_full_freq = i;
	                    }
                    }
                }
                else if( full_flag == 1)
                {
                    if(raw_data[i] < 1023)
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

                if( last_full_freq - first_full_freq > 3 )
                {
                    valid_freq[*valid_freq_amount].freq = first_full_freq + (last_full_freq - first_full_freq)/2;
                    valid_freq[*valid_freq_amount].level = max_level;
                    
                    (*valid_freq_amount)++;
                }
                else if( (i - first_meet_freq) > 2 )
                {
                    uint8_t index1= 1, index2=1;

                    if( max_level > 900 )
                    {
                        while(raw_data[max_level_freq - index1] > 900)
                        {
                            if( max_level_freq - index1 <= 6 ) break;

                            ++index1;
                        }
                        index1--;

                        while(raw_data[max_level_freq + index2] > 900)
                        {
                            if( max_level_freq + index2 >= SAMPLE_NUM - 6 ) break;

                            ++index2;
                        }
                        index2--;
                    }

                    if( max_level_freq - index1 > 6 && max_level_freq + index2 < SAMPLE_NUM - 6 )
                    {
                        for(j=1 ; j< 6; j++)
                        {
                            if( raw_data[max_level_freq - index1 - j] < raw_data[max_level_freq - index1] / 2 ||
                                raw_data[max_level_freq + index2 + j] < raw_data[max_level_freq + index2] / 2 )
                            {
                                if( full_flag == 0 )
                                {
                                    valid_freq[*valid_freq_amount].freq = max_level_freq;
                                    //valid_freq[*valid_freq_amount].level = max_level;
                                }
                                else
                                {
                                    valid_freq[*valid_freq_amount].freq = first_full_freq; // + (last_full_freq - first_full_freq)/4;
                                    //valid_freq[*valid_freq_amount].level = 1023;
                                }
                                valid_freq[*valid_freq_amount].level = max_level;
                                (*valid_freq_amount)++;

                                break;
                            }
                        }
                    }
                }
            }
        }
    }
}
int8_t check_harmonic(VALID_FREQ *valid_freq, int8_t valid_freq_amount, int8_t *harmonic_index, int8_t *harmonic_amount)
{    
	uint8_t i,j,k;
    uint32_t max_level[3], min_level[3];

    for( i=harmonic_index[0]; i < valid_freq_amount - 1; i++ )
    {
        min_level[0] = valid_freq[i].freq * 17;
        
        if( valid_freq[i].freq < 2000)
        {
            max_level[0] = valid_freq[i].freq * 19;
        }
        else
        {
            max_level[0] = valid_freq[i].freq * 21;
        }

        max_level[1] = valid_freq[i].freq * 27;
        min_level[1] = valid_freq[i].freq * 25;
        
        max_level[2] = valid_freq[i].freq * 16;
        min_level[2] = valid_freq[i].freq * 14;

        for( j = harmonic_index[1]; j < valid_freq_amount; j++ )
        {
            for( k=0; k<3; k++)
            {
                if( valid_freq[j].freq * 10 < max_level[k] && valid_freq[j].freq * 10 > min_level[k] )
                {
                    if( k != 2 || ( valid_freq[i].freq > 2000))
                    {
                        //The first harmonic was found
                        //check_freq_amount = 2;
                        //check_freq[0] = i; //valid_freq[i].freq;
                        //check_freq[1] = j; //valid_freq[j].freq;
                        if ( k != 2 )
                        {
							harmonic_index[0] = i;
							harmonic_index[1] = j + 1;
                        }
                        else
                        {
							harmonic_index[1] = i;
							harmonic_index[2] = j + 1;
                        }
                        return k;
                    }
                }
            }
        }

        harmonic_index[1] = i + 2;//No harmonic, set the index to valid_freq_amount
    }
    
    return 3;
}

int16_t calc_meet_freq(int8_t result, VALID_FREQ *valid_freq, int8_t valid_freq_amount, \
	                   int8_t *harmonic_index, int8_t harmonic_amount)
{
	int16_t i;
	int8_t valid_type;
	int16_t meet_freq;

    if(result == 3)
    {
        valid_type = 1;
        harmonic_index[0] = 0;
    }
    else if( result == 2)
    {
        meet_freq  = valid_freq[harmonic_index[1]].freq * 10 / 18;
    }
    else
    {
        meet_freq  = valid_freq[harmonic_index[0]].freq;
    }

    if( valid_type == 1 )
    {
        for( i=harmonic_index[0]; i < valid_freq_amount; i++ )
        {
            if( valid_freq[i].freq > 2800 )
            {
                meet_freq  = valid_freq[i].freq;
                harmonic_index[0] = i+1;
                //check_freq_amount = 1;
                //check_freq[0] = i; //valid_freq[i].freq;
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
            return 0;
        }
    }   

    //Initial
    //check_freq_offset = -50;
    //check_freq_index = 0;
    
    //*****Write send_freq command*****
    //sound_Run(valid_freq[check_freq[check_freq_index]].freq + check_freq_offset);

    //check_freq_flag = 1;	

    return meet_freq;
}

int8_t meas_harmonic_again(VALID_FREQ *valid_freq, int8_t *harmonic_index, int8_t *harmonic_amount)
{return 1;}

int16_t map_freq2ml(int16_t freq)
{    
	int i;
    uint16_t water_ml;

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

    if( freq >= freq_index_for_ML[48] )
    {
        water_ml = 480;
    }
    else
    {
        for( i=0; i< 49; i++)
        {
            if( freq < freq_index_for_ML[i] )
            {
                if( i==0 )
                {
                    water_ml = 0;
                }
                else
                {
                    water_ml = (i-1) * 10 + ( freq - freq_index_for_ML[i-1] ) * 10 / 
                               (freq_index_for_ML[i] - freq_index_for_ML[i-1]); 
                }

                break;
            }
        }
    }

    return water_ml;
}