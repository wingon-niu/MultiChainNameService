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
