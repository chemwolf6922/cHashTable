#include <stdio.h>
#include "staticHashTable.h"
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include "crc.h"

int test_data[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};


#define LEN_ARRAY(_array) sizeof(_array)/sizeof(_array[0])

hash_table_item_t items[] = {
    HASH_KEY_VALUE("set_power_on",&test_data[0]),
    HASH_KEY_VALUE("set_power_off",&test_data[1]),
    HASH_KEY_VALUE("reboot",&test_data[2]),
    HASH_KEY_VALUE("restore",&test_data[3]),
    HASH_KEY_VALUE("start_OTA",&test_data[4]),
    HASH_KEY_VALUE("stop_OTA",&test_data[5]),
    HASH_KEY_VALUE("volume_up",&test_data[6]),
    HASH_KEY_VALUE("volume_down",&test_data[7]),
    HASH_KEY_VALUE("factory_test_start",&test_data[8]),
    HASH_KEY_VALUE("factory_test_stop",&test_data[9]),
    HASH_KEY_VALUE("test_wifi",&test_data[10]),
    HASH_KEY_VALUE("test_bluetooth",&test_data[11]),
    HASH_KEY_VALUE("turn_on_LED",&test_data[12]),
    HASH_KEY_VALUE("turn_off_LED",&test_data[13]),
    HASH_KEY_VALUE("report_measurement",&test_data[14]),
    HASH_KEY_VALUE("random_cmd_1",&test_data[15]),
    HASH_KEY_VALUE("random_cmd_2",&test_data[16]),
    HASH_KEY_VALUE("nothing_more",&test_data[17]),
    HASH_KEY_VALUE("hello_world",&test_data[18]),
    HASH_KEY_VALUE("the_last_one",&test_data[19]),
};

void speed_test(int rounds,bool with_cache)
{
    static_hash_table_handle_t hash_table;
    static_hash_table_create(&hash_table,items,LEN_ARRAY(items),with_cache);
    int* p_value = NULL;
    struct timeval start,stop;
    gettimeofday(&start,NULL);
    for(int i=0;i<rounds;i++)
    {
        for(int j = 0;j<LEN_ARRAY(items);j++)
        {
            p_value = static_hash_table_get(hash_table,items[j].key,items[j].key_len);
        }
    }
    gettimeofday(&stop,NULL);
    printf("Time: %.6fs\n",
            ((float)(stop.tv_sec*1000000+stop.tv_usec-start.tv_sec*1000000-start.tv_usec))*0.000001);
    static_hash_table_delete(hash_table);
}

int main(int argc, void** argv)
{
    speed_test(1000000,false);
    speed_test(1000000,true);
    return 0;
}