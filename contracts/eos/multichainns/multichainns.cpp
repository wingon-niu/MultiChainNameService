#include "multichainns.hpp"

//

// 初始化全局变量表
ACTION multichainns::initgvarstbl()
{
    require_auth( _self );

    uint64_t id = 1;

    auto itr = _global_vars.find(id);

    eosio::check( itr == _global_vars.end(), "Error: Initialization has already been done." );

    _global_vars.emplace(_self, [&](auto& item){
        item.id                           = get_pri_key(name("globalvars"));

        item.num_of_level_1_name_total    = 0;   // 1级名称的总数量
        item.num_of_level_2_name_total    = 0;   // 2级名称的总数量
        item.num_of_level_3_name_total    = 0;   // 3级名称的总数量

        item.num_of_1_byte_level_1_name   = 0;   // 1个字节的1级名称的数量
        item.num_of_2_bytes_level_1_name  = 0;   // 2个字节的1级名称的数量
        item.num_of_3_bytes_level_1_name  = 0;   // 3个字节的1级名称的数量
        item.num_of_4_bytes_level_1_name  = 0;   // 4个字节的1级名称的数量
        item.num_of_5_bytes_level_1_name  = 0;   // 5个字节的1级名称的数量
        item.num_of_6_bytes_level_1_name  = 0;   // 6个字节的1级名称的数量
        item.num_of_7_bytes_level_1_name  = 0;   // 7个字节的1级名称的数量
        item.num_of_8_bytes_level_1_name  = 0;   // 8个字节的1级名称的数量
        item.num_of_9_bytes_level_1_name  = 0;   // 9个字节的1级名称的数量
        item.num_of_10_bytes_level_1_name = 0;   // 10个字节的1级名称的数量
        item.num_of_11_bytes_level_1_name = 0;   // 11个字节的1级名称的数量
        item.num_of_12_bytes_level_1_name = 0;   // 12个字节的1级名称的数量
        item.num_of_13_bytes_level_1_name = 0;   // 13个字节的1级名称的数量
        item.num_of_14_bytes_level_1_name = 0;   // 14个字节的1级名称的数量
        item.num_of_15_bytes_level_1_name = 0;   // 15个字节的1级名称的数量
        item.num_of_16_bytes_level_1_name = 0;   // 16个字节的1级名称的数量
        item.num_of_17_bytes_level_1_name = 0;   // 17和17以上个字节的1级名称的数量

        item.num_of_1_byte_level_2_name   = 0;   // 1个字节的2级名称的数量
        item.num_of_2_bytes_level_2_name  = 0;   // 2个字节的2级名称的数量
        item.num_of_3_bytes_level_2_name  = 0;   // 3个字节的2级名称的数量
        item.num_of_4_bytes_level_2_name  = 0;   // 4个字节的2级名称的数量
        item.num_of_5_bytes_level_2_name  = 0;   // 5个字节的2级名称的数量
        item.num_of_6_bytes_level_2_name  = 0;   // 6个字节的2级名称的数量
        item.num_of_7_bytes_level_2_name  = 0;   // 7个字节的2级名称的数量
        item.num_of_8_bytes_level_2_name  = 0;   // 8个字节的2级名称的数量
        item.num_of_9_bytes_level_2_name  = 0;   // 9个字节的2级名称的数量
        item.num_of_10_bytes_level_2_name = 0;   // 10个字节的2级名称的数量
        item.num_of_11_bytes_level_2_name = 0;   // 11个字节的2级名称的数量
        item.num_of_12_bytes_level_2_name = 0;   // 12个字节的2级名称的数量
        item.num_of_13_bytes_level_2_name = 0;   // 13个字节的2级名称的数量
        item.num_of_14_bytes_level_2_name = 0;   // 14个字节的2级名称的数量
        item.num_of_15_bytes_level_2_name = 0;   // 15个字节的2级名称的数量
        item.num_of_16_bytes_level_2_name = 0;   // 16个字节的2级名称的数量
        item.num_of_17_bytes_level_2_name = 0;   // 17和17以上个字节的2级名称的数量

        item.num_of_1_byte_level_3_name   = 0;   // 1个字节的3级名称的数量
        item.num_of_2_bytes_level_3_name  = 0;   // 2个字节的3级名称的数量
        item.num_of_3_bytes_level_3_name  = 0;   // 3个字节的3级名称的数量
        item.num_of_4_bytes_level_3_name  = 0;   // 4个字节的3级名称的数量
        item.num_of_5_bytes_level_3_name  = 0;   // 5个字节的3级名称的数量
        item.num_of_6_bytes_level_3_name  = 0;   // 6个字节的3级名称的数量
        item.num_of_7_bytes_level_3_name  = 0;   // 7个字节的3级名称的数量
        item.num_of_8_bytes_level_3_name  = 0;   // 8个字节的3级名称的数量
        item.num_of_9_bytes_level_3_name  = 0;   // 9个字节的3级名称的数量
        item.num_of_10_bytes_level_3_name = 0;   // 10个字节的3级名称的数量
        item.num_of_11_bytes_level_3_name = 0;   // 11个字节的3级名称的数量
        item.num_of_12_bytes_level_3_name = 0;   // 12个字节的3级名称的数量
        item.num_of_13_bytes_level_3_name = 0;   // 13个字节的3级名称的数量
        item.num_of_14_bytes_level_3_name = 0;   // 14个字节的3级名称的数量
        item.num_of_15_bytes_level_3_name = 0;   // 15个字节的3级名称的数量
        item.num_of_16_bytes_level_3_name = 0;   // 16个字节的3级名称的数量
        item.num_of_17_bytes_level_3_name = 0;   // 17和17以上个字节的3级名称的数量
    });
}

// x级名称的总数量加1
void multichainns::add_num_of_level_x_name_total(const uint32_t x)
{
    uint64_t id = 1;
    auto itr = _global_vars.find(id);
    if( itr != _global_vars.end() ) {
        if      (x == 1) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_level_1_name_total += 1; }); }
        else if (x == 2) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_level_2_name_total += 1; }); }
        else if (x == 3) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_level_3_name_total += 1; }); }
    }
}

// y个字节的x级名称的数量加1
void multichainns::add_num_of_y_bytes_level_x_name(const uint32_t x, const uint32_t y)
{
    uint64_t id = 1;
    auto itr = _global_vars.find(id);
    if( itr != _global_vars.end() ) {
        if      (x == 1) {
            if      (y == 1)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_1_byte_level_1_name   += 1; }); }
            else if (y == 2)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_2_bytes_level_1_name  += 1; }); }
            else if (y == 3)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_3_bytes_level_1_name  += 1; }); }
            else if (y == 4)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_4_bytes_level_1_name  += 1; }); }
            else if (y == 5)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_5_bytes_level_1_name  += 1; }); }
            else if (y == 6)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_6_bytes_level_1_name  += 1; }); }
            else if (y == 7)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_7_bytes_level_1_name  += 1; }); }
            else if (y == 8)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_8_bytes_level_1_name  += 1; }); }
            else if (y == 9)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_9_bytes_level_1_name  += 1; }); }
            else if (y == 10) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_10_bytes_level_1_name += 1; }); }
            else if (y == 11) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_11_bytes_level_1_name += 1; }); }
            else if (y == 12) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_12_bytes_level_1_name += 1; }); }
            else if (y == 13) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_13_bytes_level_1_name += 1; }); }
            else if (y == 14) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_14_bytes_level_1_name += 1; }); }
            else if (y == 15) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_15_bytes_level_1_name += 1; }); }
            else if (y == 16) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_16_bytes_level_1_name += 1; }); }
            else if (y == 17) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_17_bytes_level_1_name += 1; }); }
        }
        else if (x == 2) {
            if      (y == 1)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_1_byte_level_2_name   += 1; }); }
            else if (y == 2)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_2_bytes_level_2_name  += 1; }); }
            else if (y == 3)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_3_bytes_level_2_name  += 1; }); }
            else if (y == 4)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_4_bytes_level_2_name  += 1; }); }
            else if (y == 5)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_5_bytes_level_2_name  += 1; }); }
            else if (y == 6)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_6_bytes_level_2_name  += 1; }); }
            else if (y == 7)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_7_bytes_level_2_name  += 1; }); }
            else if (y == 8)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_8_bytes_level_2_name  += 1; }); }
            else if (y == 9)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_9_bytes_level_2_name  += 1; }); }
            else if (y == 10) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_10_bytes_level_2_name += 1; }); }
            else if (y == 11) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_11_bytes_level_2_name += 1; }); }
            else if (y == 12) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_12_bytes_level_2_name += 1; }); }
            else if (y == 13) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_13_bytes_level_2_name += 1; }); }
            else if (y == 14) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_14_bytes_level_2_name += 1; }); }
            else if (y == 15) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_15_bytes_level_2_name += 1; }); }
            else if (y == 16) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_16_bytes_level_2_name += 1; }); }
            else if (y == 17) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_17_bytes_level_2_name += 1; }); }
        }
        else if (x == 3) {
            if      (y == 1)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_1_byte_level_3_name   += 1; }); }
            else if (y == 2)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_2_bytes_level_3_name  += 1; }); }
            else if (y == 3)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_3_bytes_level_3_name  += 1; }); }
            else if (y == 4)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_4_bytes_level_3_name  += 1; }); }
            else if (y == 5)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_5_bytes_level_3_name  += 1; }); }
            else if (y == 6)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_6_bytes_level_3_name  += 1; }); }
            else if (y == 7)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_7_bytes_level_3_name  += 1; }); }
            else if (y == 8)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_8_bytes_level_3_name  += 1; }); }
            else if (y == 9)  { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_9_bytes_level_3_name  += 1; }); }
            else if (y == 10) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_10_bytes_level_3_name += 1; }); }
            else if (y == 11) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_11_bytes_level_3_name += 1; }); }
            else if (y == 12) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_12_bytes_level_3_name += 1; }); }
            else if (y == 13) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_13_bytes_level_3_name += 1; }); }
            else if (y == 14) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_14_bytes_level_3_name += 1; }); }
            else if (y == 15) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_15_bytes_level_3_name += 1; }); }
            else if (y == 16) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_16_bytes_level_3_name += 1; }); }
            else if (y == 17) { _global_vars.modify( itr, _self, [&]( auto& item ) { item.num_of_17_bytes_level_3_name += 1; }); }
        }
    }
}

#ifdef NAME_SERVICE_VERSION_DEV
// 将全局变量表中的所有计数类变量加1，测试时使用，上线时去掉。
ACTION multichainns::addallgvars()
{
    require_auth( _self );

    add_num_of_level_x_name_total(1);
    add_num_of_level_x_name_total(2);
    add_num_of_level_x_name_total(3);

    add_num_of_y_bytes_level_x_name(1, 1);
    add_num_of_y_bytes_level_x_name(1, 2);
    add_num_of_y_bytes_level_x_name(1, 3);
    add_num_of_y_bytes_level_x_name(1, 4);
    add_num_of_y_bytes_level_x_name(1, 5);
    add_num_of_y_bytes_level_x_name(1, 6);
    add_num_of_y_bytes_level_x_name(1, 7);
    add_num_of_y_bytes_level_x_name(1, 8);
    add_num_of_y_bytes_level_x_name(1, 9);
    add_num_of_y_bytes_level_x_name(1, 10);
    add_num_of_y_bytes_level_x_name(1, 11);
    add_num_of_y_bytes_level_x_name(1, 12);
    add_num_of_y_bytes_level_x_name(1, 13);
    add_num_of_y_bytes_level_x_name(1, 14);
    add_num_of_y_bytes_level_x_name(1, 15);
    add_num_of_y_bytes_level_x_name(1, 16);
    add_num_of_y_bytes_level_x_name(1, 17);

    add_num_of_y_bytes_level_x_name(2, 1);
    add_num_of_y_bytes_level_x_name(2, 2);
    add_num_of_y_bytes_level_x_name(2, 3);
    add_num_of_y_bytes_level_x_name(2, 4);
    add_num_of_y_bytes_level_x_name(2, 5);
    add_num_of_y_bytes_level_x_name(2, 6);
    add_num_of_y_bytes_level_x_name(2, 7);
    add_num_of_y_bytes_level_x_name(2, 8);
    add_num_of_y_bytes_level_x_name(2, 9);
    add_num_of_y_bytes_level_x_name(2, 10);
    add_num_of_y_bytes_level_x_name(2, 11);
    add_num_of_y_bytes_level_x_name(2, 12);
    add_num_of_y_bytes_level_x_name(2, 13);
    add_num_of_y_bytes_level_x_name(2, 14);
    add_num_of_y_bytes_level_x_name(2, 15);
    add_num_of_y_bytes_level_x_name(2, 16);
    add_num_of_y_bytes_level_x_name(2, 17);

    add_num_of_y_bytes_level_x_name(3, 1);
    add_num_of_y_bytes_level_x_name(3, 2);
    add_num_of_y_bytes_level_x_name(3, 3);
    add_num_of_y_bytes_level_x_name(3, 4);
    add_num_of_y_bytes_level_x_name(3, 5);
    add_num_of_y_bytes_level_x_name(3, 6);
    add_num_of_y_bytes_level_x_name(3, 7);
    add_num_of_y_bytes_level_x_name(3, 8);
    add_num_of_y_bytes_level_x_name(3, 9);
    add_num_of_y_bytes_level_x_name(3, 10);
    add_num_of_y_bytes_level_x_name(3, 11);
    add_num_of_y_bytes_level_x_name(3, 12);
    add_num_of_y_bytes_level_x_name(3, 13);
    add_num_of_y_bytes_level_x_name(3, 14);
    add_num_of_y_bytes_level_x_name(3, 15);
    add_num_of_y_bytes_level_x_name(3, 16);
    add_num_of_y_bytes_level_x_name(3, 17);
}
#endif

// 初始化全局参数表
ACTION multichainns::initgprmstbl()
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr == _global_parameters.end(), "Error: Initialization has already been done." );

    _global_parameters.emplace(_self, [&](auto& item){
        item.id                                           = get_pri_key(name("globalparams"));

        item.allowed_num_of_bytes_of_level_1_name         = 20;              // 允许的1级名称的字节数量，允许创建长度等于或者大于此项设置的名称。
        item.allowed_num_of_bytes_of_level_2_name         = 20;              // 允许的2级名称的字节数量，允许创建长度等于或者大于此项设置的名称。
        item.allowed_num_of_bytes_of_level_3_name         = 20;              // 允许的3级名称的字节数量，允许创建长度等于或者大于此项设置的名称。

        item.max_num_of_repeated_hashes_in_resolves_table = 10;              // 在解析表中，一个相同的 sha256 hash 字符串允许出现的最大次数，默认为12次。
                                                                             // 例如，很多人把自己的BTC地址解析为中本聪的BTC地址，那么这个地址在整个解析表中最多出现12次。

        item.fee_of_1_byte_level_1_name   = asset((int64_t)0, MAIN_SYMBOL);  // 1个字节的1级名称的收费
        item.fee_of_2_bytes_level_1_name  = asset((int64_t)0, MAIN_SYMBOL);  // 2个字节的1级名称的收费
        item.fee_of_3_bytes_level_1_name  = asset((int64_t)0, MAIN_SYMBOL);  // 3个字节的1级名称的收费
        item.fee_of_4_bytes_level_1_name  = asset((int64_t)0, MAIN_SYMBOL);  // 4个字节的1级名称的收费
        item.fee_of_5_bytes_level_1_name  = asset((int64_t)0, MAIN_SYMBOL);  // 5个字节的1级名称的收费
        item.fee_of_6_bytes_level_1_name  = asset((int64_t)0, MAIN_SYMBOL);  // 6个字节的1级名称的收费
        item.fee_of_7_bytes_level_1_name  = asset((int64_t)0, MAIN_SYMBOL);  // 7个字节的1级名称的收费
        item.fee_of_8_bytes_level_1_name  = asset((int64_t)0, MAIN_SYMBOL);  // 8个字节的1级名称的收费
        item.fee_of_9_bytes_level_1_name  = asset((int64_t)0, MAIN_SYMBOL);  // 9个字节的1级名称的收费
        item.fee_of_10_bytes_level_1_name = asset((int64_t)0, MAIN_SYMBOL);  // 10个字节的1级名称的收费
        item.fee_of_11_bytes_level_1_name = asset((int64_t)0, MAIN_SYMBOL);  // 11个字节的1级名称的收费
        item.fee_of_12_bytes_level_1_name = asset((int64_t)0, MAIN_SYMBOL);  // 12个字节的1级名称的收费
        item.fee_of_13_bytes_level_1_name = asset((int64_t)0, MAIN_SYMBOL);  // 13个字节的1级名称的收费
        item.fee_of_14_bytes_level_1_name = asset((int64_t)0, MAIN_SYMBOL);  // 14个字节的1级名称的收费
        item.fee_of_15_bytes_level_1_name = asset((int64_t)0, MAIN_SYMBOL);  // 15个字节的1级名称的收费
        item.fee_of_16_bytes_level_1_name = asset((int64_t)0, MAIN_SYMBOL);  // 16个字节的1级名称的收费
        item.fee_of_17_bytes_level_1_name = asset((int64_t)0, MAIN_SYMBOL);  // 17和17以上个字节的1级名称的收费

        item.fee_of_1_byte_level_2_name   = asset((int64_t)0, MAIN_SYMBOL);  // 1个字节的2级名称的收费
        item.fee_of_2_bytes_level_2_name  = asset((int64_t)0, MAIN_SYMBOL);  // 2个字节的2级名称的收费
        item.fee_of_3_bytes_level_2_name  = asset((int64_t)0, MAIN_SYMBOL);  // 3个字节的2级名称的收费
        item.fee_of_4_bytes_level_2_name  = asset((int64_t)0, MAIN_SYMBOL);  // 4个字节的2级名称的收费
        item.fee_of_5_bytes_level_2_name  = asset((int64_t)0, MAIN_SYMBOL);  // 5个字节的2级名称的收费
        item.fee_of_6_bytes_level_2_name  = asset((int64_t)0, MAIN_SYMBOL);  // 6个字节的2级名称的收费
        item.fee_of_7_bytes_level_2_name  = asset((int64_t)0, MAIN_SYMBOL);  // 7个字节的2级名称的收费
        item.fee_of_8_bytes_level_2_name  = asset((int64_t)0, MAIN_SYMBOL);  // 8个字节的2级名称的收费
        item.fee_of_9_bytes_level_2_name  = asset((int64_t)0, MAIN_SYMBOL);  // 9个字节的2级名称的收费
        item.fee_of_10_bytes_level_2_name = asset((int64_t)0, MAIN_SYMBOL);  // 10个字节的2级名称的收费
        item.fee_of_11_bytes_level_2_name = asset((int64_t)0, MAIN_SYMBOL);  // 11个字节的2级名称的收费
        item.fee_of_12_bytes_level_2_name = asset((int64_t)0, MAIN_SYMBOL);  // 12个字节的2级名称的收费
        item.fee_of_13_bytes_level_2_name = asset((int64_t)0, MAIN_SYMBOL);  // 13个字节的2级名称的收费
        item.fee_of_14_bytes_level_2_name = asset((int64_t)0, MAIN_SYMBOL);  // 14个字节的2级名称的收费
        item.fee_of_15_bytes_level_2_name = asset((int64_t)0, MAIN_SYMBOL);  // 15个字节的2级名称的收费
        item.fee_of_16_bytes_level_2_name = asset((int64_t)0, MAIN_SYMBOL);  // 16个字节的2级名称的收费
        item.fee_of_17_bytes_level_2_name = asset((int64_t)0, MAIN_SYMBOL);  // 17和17以上个字节的2级名称的收费

        item.fee_of_1_byte_level_3_name   = asset((int64_t)0, MAIN_SYMBOL);  // 1个字节的3级名称的收费
        item.fee_of_2_bytes_level_3_name  = asset((int64_t)0, MAIN_SYMBOL);  // 2个字节的3级名称的收费
        item.fee_of_3_bytes_level_3_name  = asset((int64_t)0, MAIN_SYMBOL);  // 3个字节的3级名称的收费
        item.fee_of_4_bytes_level_3_name  = asset((int64_t)0, MAIN_SYMBOL);  // 4个字节的3级名称的收费
        item.fee_of_5_bytes_level_3_name  = asset((int64_t)0, MAIN_SYMBOL);  // 5个字节的3级名称的收费
        item.fee_of_6_bytes_level_3_name  = asset((int64_t)0, MAIN_SYMBOL);  // 6个字节的3级名称的收费
        item.fee_of_7_bytes_level_3_name  = asset((int64_t)0, MAIN_SYMBOL);  // 7个字节的3级名称的收费
        item.fee_of_8_bytes_level_3_name  = asset((int64_t)0, MAIN_SYMBOL);  // 8个字节的3级名称的收费
        item.fee_of_9_bytes_level_3_name  = asset((int64_t)0, MAIN_SYMBOL);  // 9个字节的3级名称的收费
        item.fee_of_10_bytes_level_3_name = asset((int64_t)0, MAIN_SYMBOL);  // 10个字节的3级名称的收费
        item.fee_of_11_bytes_level_3_name = asset((int64_t)0, MAIN_SYMBOL);  // 11个字节的3级名称的收费
        item.fee_of_12_bytes_level_3_name = asset((int64_t)0, MAIN_SYMBOL);  // 12个字节的3级名称的收费
        item.fee_of_13_bytes_level_3_name = asset((int64_t)0, MAIN_SYMBOL);  // 13个字节的3级名称的收费
        item.fee_of_14_bytes_level_3_name = asset((int64_t)0, MAIN_SYMBOL);  // 14个字节的3级名称的收费
        item.fee_of_15_bytes_level_3_name = asset((int64_t)0, MAIN_SYMBOL);  // 15个字节的3级名称的收费
        item.fee_of_16_bytes_level_3_name = asset((int64_t)0, MAIN_SYMBOL);  // 16个字节的3级名称的收费
        item.fee_of_17_bytes_level_3_name = asset((int64_t)0, MAIN_SYMBOL);  // 17和17以上个字节的3级名称的收费
    });
}

// 获取某个表的主键
uint64_t multichainns::get_pri_key(const name& table_name)
{
    uint64_t key = 1;

    auto itr = _pri_keys.find(table_name.value);

    if( itr == _pri_keys.end() ) {
        _pri_keys.emplace(_self, [&](auto& item){
            item.table_name = table_name;
            item.pri_key    = key;
        });
    }
    else {
        key = itr->pri_key + 1;
        _pri_keys.modify( itr, _self, [&]( auto& item ) {
            item.pri_key    = key;
        });
    }

    return key;
}

#ifdef NAME_SERVICE_WITH_CLEAR_FUNCTION_YES
// 清除指定的 multi_index 中的所有数据，测试时使用，上线时去掉
ACTION multichainns::cleardata(const string& table_name)
{
    require_auth( _self );

    std::vector<uint64_t> keysForDeletion;
    print("\nclear all data of <", table_name.c_str(), ">.\n");

    if (table_name == "globalvars") {
        keysForDeletion.clear();
        for (auto& item : _global_vars) {
            keysForDeletion.push_back(item.id);
        }
        for (uint64_t key : keysForDeletion) {
            auto itr = _global_vars.find(key);
            if (itr != _global_vars.end()) {
                _global_vars.erase(itr);
            }
        }
    }
    else if (table_name == "globalparams") {
        keysForDeletion.clear();
        for (auto& item : _global_parameters) {
            keysForDeletion.push_back(item.id);
        }
        for (uint64_t key : keysForDeletion) {
            auto itr = _global_parameters.find(key);
            if (itr != _global_parameters.end()) {
                _global_parameters.erase(itr);
            }
        }
    }
    else if (table_name == "prikeys") {
        keysForDeletion.clear();
        for (auto& item : _pri_keys) {
            keysForDeletion.push_back(item.table_name.value);
        }
        for (uint64_t key : keysForDeletion) {
            auto itr = _pri_keys.find(key);
            if (itr != _pri_keys.end()) {
                _pri_keys.erase(itr);
            }
        }
    }
}
#endif
