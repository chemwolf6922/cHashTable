#include <stdio.h>
#include "staticHashTable.h"
#include <string.h>
#include "crc.h"

int test_data[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

#define KEY_VALUE(_key,_data) {_key,strlen(_key),_data}

hash_table_item_t items[] = {
    KEY_VALUE("set_power_on",&test_data[0]),
    KEY_VALUE("set_power_off",&test_data[1]),
    KEY_VALUE("reboot",&test_data[2]),
    KEY_VALUE("restore",&test_data[3]),
    KEY_VALUE("start_OTA",&test_data[4]),
    KEY_VALUE("stop_OTA",&test_data[5]),
    KEY_VALUE("volume_up",&test_data[6]),
    KEY_VALUE("volume_down",&test_data[7]),
    KEY_VALUE("factory_test_start",&test_data[8]),
    KEY_VALUE("factory_test_stop",&test_data[9]),
    KEY_VALUE("test_wifi",&test_data[10]),
    KEY_VALUE("test_bluetooth",&test_data[11]),
    KEY_VALUE("turn_on_LED",&test_data[12]),
    KEY_VALUE("turn_off_LED",&test_data[13]),
    KEY_VALUE("report_measurement",&test_data[14]),
    KEY_VALUE("random_cmd_1",&test_data[15]),
    KEY_VALUE("random_cmd_2",&test_data[16]),
    KEY_VALUE("nothing_more",&test_data[17]),
    KEY_VALUE("hello_world",&test_data[18]),
    KEY_VALUE("the_last_one",&test_data[19]),
};

int main(int argc, void** argv)
{
    // char test_str[] = "hahahaha";
    // printf("0x%02x\n",crc_calculate(0b100000000,(uint8_t*)test_str,strlen(test_str)));
    static_hash_table_handle_t hash_table;
    static_hash_table_create(&hash_table,items,20);
    return 0;
}