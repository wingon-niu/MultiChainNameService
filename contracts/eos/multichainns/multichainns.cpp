#include "multichainns.hpp"

//

// 接收用户转账，进行相应处理
[[eosio::on_notify("eosio.token::transfer")]]
void multichainns::deposit(name from, name to, eosio::asset quantity, std::string memo)
{
    if (from == _self) {
        return;
    }
    if (to != _self) {
        return;
    }
    if (!quantity.symbol.is_valid()) {
        return;
    }
    if (!(quantity.symbol == MAIN_SYMBOL)) {
        return;
    }
    if (!quantity.is_valid()) {
        return;
    }
    if (!quantity.is_amount_within_range()) {
        return;
    }
    if (!(quantity.amount > 0)) {
        return;
    }

    if (memo.find("Create meta name: ") == 0) {             // 创建名称
        create_meta_name(from, to, quantity, memo);
    }
    else {
    }
}

// 创建名称
void multichainns::create_meta_name(name from, name to, eosio::asset quantity, std::string memo)
{
    string  flag        = "Create meta name: ";
    string  meta_name   = my_trim(memo.substr(flag.size(), memo.size()-flag.size()));

    eosio::check( meta_name != "", "Error: Bad format of meta name." );
    eosio::check( meta_name.find("\t") == meta_name.npos && meta_name.find("\r") == meta_name.npos && meta_name.find("\n") == meta_name.npos, "Error: Bad format of meta name." );

    auto num_of_dot = get_num_of_dot_in_string(meta_name);

    eosio::check( num_of_dot < 3, "Error: Bad format of meta name." );

    string  level_1_str = "";
    string  level_2_str = "";
    string  level_3_str = "";
    uint8_t my_level    = 0;
    uint8_t my_length   = 0;

    if      (num_of_dot == 0) { my_level = 1; }
    else if (num_of_dot == 1) { my_level = 2; }
    else if (num_of_dot == 2) { my_level = 3; }

    // 将名称分拆到各级字符串
    if (my_level == 1) {
        level_1_str = meta_name;
    }
    else if (my_level == 2) {
        auto i = meta_name.find(".");
        level_1_str = meta_name.substr(i + 1, meta_name.size() - i - 1);
        level_2_str = meta_name.substr(0, i);
    }
    else if (my_level == 3) {
        auto i = meta_name.find(".");
        auto j = meta_name.find(".", i + 1);
        level_1_str = meta_name.substr(j + 1, meta_name.size() - j - 1);
        level_2_str = meta_name.substr(i + 1, j - i - 1);
        level_3_str = meta_name.substr(0, i);
    }

    #ifdef NAME_SERVICE_VERSION_DEV
    print("\n");
    print("\nlevel_1_str: <", level_1_str, ">");
    print("\nlevel_2_str: <", level_2_str, ">");
    print("\nlevel_3_str: <", level_3_str, ">");
    print("\n");
    #endif

    // 检查各级字符串的首尾是否有空格
    if (my_level == 1) {
        eosio::check(         level_1_str  != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_1_str) != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_1_str) == level_1_str, "Error: Bad format of meta name." );
    }
    else if (my_level == 2) {
        eosio::check(         level_1_str  != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_1_str) != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_1_str) == level_1_str, "Error: Bad format of meta name." );

        eosio::check(         level_2_str  != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_2_str) != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_2_str) == level_2_str, "Error: Bad format of meta name." );
    }
    else if (my_level == 3) {
        eosio::check(         level_1_str  != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_1_str) != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_1_str) == level_1_str, "Error: Bad format of meta name." );

        eosio::check(         level_2_str  != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_2_str) != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_2_str) == level_2_str, "Error: Bad format of meta name." );

        eosio::check(         level_3_str  != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_3_str) != "",          "Error: Bad format of meta name." );
        eosio::check( my_trim(level_3_str) == level_3_str, "Error: Bad format of meta name." );
    }

    // 检查各级字符串的长度
    if (my_level == 1) {
        my_length = level_1_str.size();
    }
    else if (my_level == 2) {
        my_length = level_2_str.size();
    }
    else if (my_level == 3) {
        my_length = level_3_str.size();
    }
    auto allowed_num_of_bytes     = get_allowed_num_of_bytes_of_level_x_name(my_level);
    auto max_num_of_bytes_of_name = get_max_num_of_bytes_of_name();
    eosio::check( my_length >= allowed_num_of_bytes,     "Error: Bad format of meta name." );
    eosio::check( my_length <= max_num_of_bytes_of_name, "Error: Bad format of meta name." );

    // 检查自己是否已经存在
    string new_meta_name = "";
    if      (my_level == 1) { new_meta_name = level_1_str; }
    else if (my_level == 2) { new_meta_name = level_2_str + "." + level_1_str; }
    else if (my_level == 3) { new_meta_name = level_3_str + "." + level_2_str + "." + level_1_str; }
    checksum256 new_meta_name_sha_256_hash = sha256(new_meta_name.c_str(), new_meta_name.size());
    eosio::check( exist_in_meta_names(new_meta_name_sha_256_hash) == false, "Error: new meta name already exists." );

}

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

        item.total_transaction_amount             = asset((int64_t)0, MAIN_SYMBOL);  // 总成交金额
        item.total_num_of_records_in_resolv_table = 0;                               // 解析表中的记录的总数
        item.total_share_amount_of_level_1_name   = asset((int64_t)0, MAIN_SYMBOL);  // 与拥有1级名称的用户分享的总金额。
        item.total_share_amount_of_level_2_name   = asset((int64_t)0, MAIN_SYMBOL);  // 与拥有2级名称的用户分享的总金额。

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

// 累加总成交金额
void multichainns::add_total_transaction_amount(const asset& quantity)
{
    uint64_t id = 1;
    auto itr = _global_vars.find(id);
    if( itr != _global_vars.end() ) {
        _global_vars.modify( itr, _self, [&]( auto& item ) { item.total_transaction_amount += quantity; });
    }
}

// 解析表中的记录的总数加1
void multichainns::add_total_num_of_records_in_resolv_table()
{
    uint64_t id = 1;
    auto itr = _global_vars.find(id);
    if( itr != _global_vars.end() ) {
        _global_vars.modify( itr, _self, [&]( auto& item ) { item.total_num_of_records_in_resolv_table += 1; });
    }
}

// 解析表中的记录的总数减1
void multichainns::sub_total_num_of_records_in_resolv_table()
{
    uint64_t id = 1;
    auto itr = _global_vars.find(id);
    if( itr != _global_vars.end() ) {
        _global_vars.modify( itr, _self, [&]( auto& item ) { item.total_num_of_records_in_resolv_table -= 1; });
    }
}

// 累加 与拥有1级名称的用户分享的总金额
void multichainns::add_total_share_amount_of_level_1_name(const asset& quantity)
{
    uint64_t id = 1;
    auto itr = _global_vars.find(id);
    if( itr != _global_vars.end() ) {
        _global_vars.modify( itr, _self, [&]( auto& item ) { item.total_share_amount_of_level_1_name += quantity; });
    }
}

// 累加 与拥有2级名称的用户分享的总金额
void multichainns::add_total_share_amount_of_level_2_name(const asset& quantity)
{
    uint64_t id = 1;
    auto itr = _global_vars.find(id);
    if( itr != _global_vars.end() ) {
        _global_vars.modify( itr, _self, [&]( auto& item ) { item.total_share_amount_of_level_2_name += quantity; });
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

    add_total_transaction_amount(asset((int64_t)10010001, MAIN_SYMBOL));
    add_total_num_of_records_in_resolv_table();
    add_total_share_amount_of_level_1_name(asset((int64_t)20020002, MAIN_SYMBOL));
    add_total_share_amount_of_level_2_name(asset((int64_t)30030003, MAIN_SYMBOL));

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

        item.fee_of_transaction_amount_percentage         = 0.03;            // 交易名称的时候，收取的交易费用所占成交金额的比例。
        item.fee_of_level_1_name_share_percentage         = 0.01;            // 创建、交易2级和3级名称的时候，所属上级1级名称的拥有者可以分享的费用的比例。
        item.fee_of_level_2_name_share_percentage         = 0.02;            // 创建、交易3级名称的时候，所属上级2级名称的拥有者可以分享的费用的比例。
        item.fee_of_level_1_name_share_quantity           = asset((int64_t)0, MAIN_SYMBOL);  // 为2级和3级名称新增解析记录的时候，所属上级1级名称的拥有者可以分享的费用的金额。
        item.fee_of_level_2_name_share_quantity           = asset((int64_t)0, MAIN_SYMBOL);  // 为3级名称新增解析记录的时候，所属上级2级名称的拥有者可以分享的费用的金额。

        item.max_num_of_repeated_hashes_in_resolves_table = 10;              // 在解析表中，一个相同的 sha256 hash 字符串允许出现的最大次数，默认为12次。
                                                                             // 例如，很多人把自己的BTC地址解析为中本聪的BTC地址，那么这个地址在整个解析表中最多出现12次。

        item.max_num_of_bytes_of_name     = 10;                              // 名称的最大长度，单位是字节。

        item.fee_of_one_resolv_record     = asset((int64_t)0, MAIN_SYMBOL);  // 每条解析记录的收费

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

// 设置x级名称允许的字节数量为y和y以上
ACTION multichainns::setallowedxy(const uint32_t x, const uint8_t y)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        if      ( x == 1) { item.allowed_num_of_bytes_of_level_1_name = y; }
        else if ( x == 2) { item.allowed_num_of_bytes_of_level_2_name = y; }
        else if ( x == 3) { item.allowed_num_of_bytes_of_level_3_name = y; }
    });
}

// 设置：交易名称的时候，收取的交易费用所占成交金额的比例。
ACTION multichainns::settxnpercnt(const float percentage)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        item.fee_of_transaction_amount_percentage = percentage;
    });
}

// 设置：创建、交易2级和3级名称的时候，所属上级1级名称的拥有者可以分享的费用的比例。
ACTION multichainns::set1namepert(const float percentage)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        item.fee_of_level_1_name_share_percentage = percentage;
    });
}

// 设置：创建、交易3级名称的时候，所属上级2级名称的拥有者可以分享的费用的比例。
ACTION multichainns::set2namepert(const float percentage)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        item.fee_of_level_2_name_share_percentage = percentage;
    });
}

// 设置：为2级和3级名称新增解析记录的时候，所属上级1级名称的拥有者可以分享的费用的金额。
ACTION multichainns::set1nameqtt(const asset& quantity)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        item.fee_of_level_1_name_share_quantity = quantity;
    });
}

// 设置：为3级名称新增解析记录的时候，所属上级2级名称的拥有者可以分享的费用的金额。
ACTION multichainns::set2nameqtt(const asset& quantity)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        item.fee_of_level_2_name_share_quantity = quantity;
    });
}

// 设置同一个hash字符串在解析表中出现的最大次数
ACTION multichainns::setmaxnumorh(const uint8_t max_num_of_repeated_hashes_in_resolves_table)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        item.max_num_of_repeated_hashes_in_resolves_table = max_num_of_repeated_hashes_in_resolves_table;
    });
}

// 设置名称的最大长度，单位是字节。
ACTION multichainns::setmnlength(const uint8_t n)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        item.max_num_of_bytes_of_name = n;
    });
}

// 设置每条解析记录的收费
ACTION multichainns::setfeeof1rr(const asset& quantity)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        item.fee_of_one_resolv_record = quantity;
    });
}

// 设置y个字节的x级名称的收费
ACTION multichainns::setfeeofxy(const uint32_t x, const uint32_t y, const asset& quantity)
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    set_fee_of_y_bytes_level_x_name(x, y, quantity);
}

// 设置y个字节的x级名称的收费
void multichainns::set_fee_of_y_bytes_level_x_name(const uint32_t x, const uint32_t y, const asset& quantity)
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);

    _global_parameters.modify( itr, _self, [&]( auto& item ) {
        if      (x == 1) {
            if      (y == 1)  { item.fee_of_1_byte_level_1_name    = quantity; }
            else if (y == 2)  { item.fee_of_2_bytes_level_1_name   = quantity; }
            else if (y == 3)  { item.fee_of_3_bytes_level_1_name   = quantity; }
            else if (y == 4)  { item.fee_of_4_bytes_level_1_name   = quantity; }
            else if (y == 5)  { item.fee_of_5_bytes_level_1_name   = quantity; }
            else if (y == 6)  { item.fee_of_6_bytes_level_1_name   = quantity; }
            else if (y == 7)  { item.fee_of_7_bytes_level_1_name   = quantity; }
            else if (y == 8)  { item.fee_of_8_bytes_level_1_name   = quantity; }
            else if (y == 9)  { item.fee_of_9_bytes_level_1_name   = quantity; }
            else if (y == 10) { item.fee_of_10_bytes_level_1_name  = quantity; }
            else if (y == 11) { item.fee_of_11_bytes_level_1_name  = quantity; }
            else if (y == 12) { item.fee_of_12_bytes_level_1_name  = quantity; }
            else if (y == 13) { item.fee_of_13_bytes_level_1_name  = quantity; }
            else if (y == 14) { item.fee_of_14_bytes_level_1_name  = quantity; }
            else if (y == 15) { item.fee_of_15_bytes_level_1_name  = quantity; }
            else if (y == 16) { item.fee_of_16_bytes_level_1_name  = quantity; }
            else if (y == 17) { item.fee_of_17_bytes_level_1_name  = quantity; }
        }
        else if (x == 2) {
            if      (y == 1)  { item.fee_of_1_byte_level_2_name    = quantity; }
            else if (y == 2)  { item.fee_of_2_bytes_level_2_name   = quantity; }
            else if (y == 3)  { item.fee_of_3_bytes_level_2_name   = quantity; }
            else if (y == 4)  { item.fee_of_4_bytes_level_2_name   = quantity; }
            else if (y == 5)  { item.fee_of_5_bytes_level_2_name   = quantity; }
            else if (y == 6)  { item.fee_of_6_bytes_level_2_name   = quantity; }
            else if (y == 7)  { item.fee_of_7_bytes_level_2_name   = quantity; }
            else if (y == 8)  { item.fee_of_8_bytes_level_2_name   = quantity; }
            else if (y == 9)  { item.fee_of_9_bytes_level_2_name   = quantity; }
            else if (y == 10) { item.fee_of_10_bytes_level_2_name  = quantity; }
            else if (y == 11) { item.fee_of_11_bytes_level_2_name  = quantity; }
            else if (y == 12) { item.fee_of_12_bytes_level_2_name  = quantity; }
            else if (y == 13) { item.fee_of_13_bytes_level_2_name  = quantity; }
            else if (y == 14) { item.fee_of_14_bytes_level_2_name  = quantity; }
            else if (y == 15) { item.fee_of_15_bytes_level_2_name  = quantity; }
            else if (y == 16) { item.fee_of_16_bytes_level_2_name  = quantity; }
            else if (y == 17) { item.fee_of_17_bytes_level_2_name  = quantity; }
        }
        else if (x == 3) {
            if      (y == 1)  { item.fee_of_1_byte_level_3_name    = quantity; }
            else if (y == 2)  { item.fee_of_2_bytes_level_3_name   = quantity; }
            else if (y == 3)  { item.fee_of_3_bytes_level_3_name   = quantity; }
            else if (y == 4)  { item.fee_of_4_bytes_level_3_name   = quantity; }
            else if (y == 5)  { item.fee_of_5_bytes_level_3_name   = quantity; }
            else if (y == 6)  { item.fee_of_6_bytes_level_3_name   = quantity; }
            else if (y == 7)  { item.fee_of_7_bytes_level_3_name   = quantity; }
            else if (y == 8)  { item.fee_of_8_bytes_level_3_name   = quantity; }
            else if (y == 9)  { item.fee_of_9_bytes_level_3_name   = quantity; }
            else if (y == 10) { item.fee_of_10_bytes_level_3_name  = quantity; }
            else if (y == 11) { item.fee_of_11_bytes_level_3_name  = quantity; }
            else if (y == 12) { item.fee_of_12_bytes_level_3_name  = quantity; }
            else if (y == 13) { item.fee_of_13_bytes_level_3_name  = quantity; }
            else if (y == 14) { item.fee_of_14_bytes_level_3_name  = quantity; }
            else if (y == 15) { item.fee_of_15_bytes_level_3_name  = quantity; }
            else if (y == 16) { item.fee_of_16_bytes_level_3_name  = quantity; }
            else if (y == 17) { item.fee_of_17_bytes_level_3_name  = quantity; }
        }
    });
}

// 设置所有的x级名称y个字节的收费，按照代码中写好的固定值
ACTION multichainns::setallfee()
{
    require_auth( _self );

    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    set_fee_of_y_bytes_level_x_name(1, 1,  asset((int64_t)1080000, MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 2,  asset((int64_t)720000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 3,  asset((int64_t)360000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 4,  asset((int64_t)240000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 5,  asset((int64_t)231000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 6,  asset((int64_t)240000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 7,  asset((int64_t)231000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 8,  asset((int64_t)222000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 9,  asset((int64_t)240000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 10, asset((int64_t)231000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 11, asset((int64_t)222000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 12, asset((int64_t)240000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 13, asset((int64_t)231000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 14, asset((int64_t)222000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 15, asset((int64_t)240000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 16, asset((int64_t)231000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(1, 17, asset((int64_t)222000,  MAIN_SYMBOL));

    set_fee_of_y_bytes_level_x_name(2, 1,  asset((int64_t)190000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 2,  asset((int64_t)180000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 3,  asset((int64_t)170000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 4,  asset((int64_t)160000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 5,  asset((int64_t)150000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 6,  asset((int64_t)190000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 7,  asset((int64_t)140000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 8,  asset((int64_t)130000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 9,  asset((int64_t)190000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 10, asset((int64_t)120000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 11, asset((int64_t)110000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 12, asset((int64_t)190000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 13, asset((int64_t)104000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 14, asset((int64_t)103000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 15, asset((int64_t)190000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 16, asset((int64_t)102000,  MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(2, 17, asset((int64_t)101000,  MAIN_SYMBOL));

    set_fee_of_y_bytes_level_x_name(3, 1,  asset((int64_t)90000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 2,  asset((int64_t)81000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 3,  asset((int64_t)72000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 4,  asset((int64_t)60000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 5,  asset((int64_t)51000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 6,  asset((int64_t)90000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 7,  asset((int64_t)42000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 8,  asset((int64_t)30000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 9,  asset((int64_t)90000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 10, asset((int64_t)21000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 11, asset((int64_t)12000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 12, asset((int64_t)60000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 13, asset((int64_t) 9000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 14, asset((int64_t) 8100,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 15, asset((int64_t)60000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 16, asset((int64_t) 6000,   MAIN_SYMBOL));
    set_fee_of_y_bytes_level_x_name(3, 17, asset((int64_t) 5100,   MAIN_SYMBOL));
}

// 获取：交易名称的时候，收取的交易费用所占成交金额的比例。
float multichainns::get_fee_of_transaction_amount_percentage()
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    return itr->fee_of_transaction_amount_percentage;
}

// 获取：创建、交易2级和3级名称的时候，所属上级1级名称的拥有者可以分享的费用的比例。
float multichainns::get_fee_of_level_1_name_share_percentage()
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    return itr->fee_of_level_1_name_share_percentage;
}

// 获取：创建、交易3级名称的时候，所属上级2级名称的拥有者可以分享的费用的比例。
float multichainns::get_fee_of_level_2_name_share_percentage()
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    return itr->fee_of_level_2_name_share_percentage;
}

// 获取：为2级和3级名称新增解析记录的时候，所属上级1级名称的拥有者可以分享的费用的金额。
asset multichainns::get_fee_of_level_1_name_share_quantity()
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    return itr->fee_of_level_1_name_share_quantity;
}

// 获取：为3级名称新增解析记录的时候，所属上级2级名称的拥有者可以分享的费用的金额。
asset multichainns::get_fee_of_level_2_name_share_quantity()
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    return itr->fee_of_level_2_name_share_quantity;
}

// 获取解析表中的hash字符串的最大重复次数
uint8_t multichainns::get_max_num_of_repeated_hashes_in_resolves_table()
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    return itr->max_num_of_repeated_hashes_in_resolves_table;
}

// 获取名称的最大长度，单位是字节。
uint8_t multichainns::get_max_num_of_bytes_of_name()
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    return itr->max_num_of_bytes_of_name;
}

// 获取每条解析记录的收费
asset multichainns::get_fee_of_one_resolv_record()
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    return itr->fee_of_one_resolv_record;
}

// 获取x级名称允许的字节数量
uint8_t multichainns::get_allowed_num_of_bytes_of_level_x_name(const uint32_t x)
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    if      (x == 1) { return itr->allowed_num_of_bytes_of_level_1_name; }
    else if (x == 2) { return itr->allowed_num_of_bytes_of_level_2_name; }
    else if (x == 3) { return itr->allowed_num_of_bytes_of_level_3_name; }
    else             { return 255; }
}

// 获取y个字节的x级名称的收费
asset multichainns::get_fee_of_y_bytes_level_x_name(const uint32_t x, const uint32_t y)
{
    uint64_t id = 1;
    auto itr = _global_parameters.find(id);
    eosio::check( itr != _global_parameters.end(), "Error: There is no record in global parameters table." );

    if      (x == 1) {
        if      (y == 1)  { return itr->fee_of_1_byte_level_1_name;   }
        else if (y == 2)  { return itr->fee_of_2_bytes_level_1_name;  }
        else if (y == 3)  { return itr->fee_of_3_bytes_level_1_name;  }
        else if (y == 4)  { return itr->fee_of_4_bytes_level_1_name;  }
        else if (y == 5)  { return itr->fee_of_5_bytes_level_1_name;  }
        else if (y == 6)  { return itr->fee_of_6_bytes_level_1_name;  }
        else if (y == 7)  { return itr->fee_of_7_bytes_level_1_name;  }
        else if (y == 8)  { return itr->fee_of_8_bytes_level_1_name;  }
        else if (y == 9)  { return itr->fee_of_9_bytes_level_1_name;  }
        else if (y == 10) { return itr->fee_of_10_bytes_level_1_name; }
        else if (y == 11) { return itr->fee_of_11_bytes_level_1_name; }
        else if (y == 12) { return itr->fee_of_12_bytes_level_1_name; }
        else if (y == 13) { return itr->fee_of_13_bytes_level_1_name; }
        else if (y == 14) { return itr->fee_of_14_bytes_level_1_name; }
        else if (y == 15) { return itr->fee_of_15_bytes_level_1_name; }
        else if (y == 16) { return itr->fee_of_16_bytes_level_1_name; }
        else if (y == 17) { return itr->fee_of_17_bytes_level_1_name; }
        else              { return asset((int64_t)1000000000000, MAIN_SYMBOL); }
    }
    else if (x == 2) {
        if      (y == 1)  { return itr->fee_of_1_byte_level_2_name;   }
        else if (y == 2)  { return itr->fee_of_2_bytes_level_2_name;  }
        else if (y == 3)  { return itr->fee_of_3_bytes_level_2_name;  }
        else if (y == 4)  { return itr->fee_of_4_bytes_level_2_name;  }
        else if (y == 5)  { return itr->fee_of_5_bytes_level_2_name;  }
        else if (y == 6)  { return itr->fee_of_6_bytes_level_2_name;  }
        else if (y == 7)  { return itr->fee_of_7_bytes_level_2_name;  }
        else if (y == 8)  { return itr->fee_of_8_bytes_level_2_name;  }
        else if (y == 9)  { return itr->fee_of_9_bytes_level_2_name;  }
        else if (y == 10) { return itr->fee_of_10_bytes_level_2_name; }
        else if (y == 11) { return itr->fee_of_11_bytes_level_2_name; }
        else if (y == 12) { return itr->fee_of_12_bytes_level_2_name; }
        else if (y == 13) { return itr->fee_of_13_bytes_level_2_name; }
        else if (y == 14) { return itr->fee_of_14_bytes_level_2_name; }
        else if (y == 15) { return itr->fee_of_15_bytes_level_2_name; }
        else if (y == 16) { return itr->fee_of_16_bytes_level_2_name; }
        else if (y == 17) { return itr->fee_of_17_bytes_level_2_name; }
        else              { return asset((int64_t)1000000000000, MAIN_SYMBOL); }
    }
    else if (x == 3) {
        if      (y == 1)  { return itr->fee_of_1_byte_level_3_name;   }
        else if (y == 2)  { return itr->fee_of_2_bytes_level_3_name;  }
        else if (y == 3)  { return itr->fee_of_3_bytes_level_3_name;  }
        else if (y == 4)  { return itr->fee_of_4_bytes_level_3_name;  }
        else if (y == 5)  { return itr->fee_of_5_bytes_level_3_name;  }
        else if (y == 6)  { return itr->fee_of_6_bytes_level_3_name;  }
        else if (y == 7)  { return itr->fee_of_7_bytes_level_3_name;  }
        else if (y == 8)  { return itr->fee_of_8_bytes_level_3_name;  }
        else if (y == 9)  { return itr->fee_of_9_bytes_level_3_name;  }
        else if (y == 10) { return itr->fee_of_10_bytes_level_3_name; }
        else if (y == 11) { return itr->fee_of_11_bytes_level_3_name; }
        else if (y == 12) { return itr->fee_of_12_bytes_level_3_name; }
        else if (y == 13) { return itr->fee_of_13_bytes_level_3_name; }
        else if (y == 14) { return itr->fee_of_14_bytes_level_3_name; }
        else if (y == 15) { return itr->fee_of_15_bytes_level_3_name; }
        else if (y == 16) { return itr->fee_of_16_bytes_level_3_name; }
        else if (y == 17) { return itr->fee_of_17_bytes_level_3_name; }
        else              { return asset((int64_t)1000000000000, MAIN_SYMBOL); }
    }
    else {
        return asset((int64_t)1000000000000, MAIN_SYMBOL);
    }
}

#ifdef NAME_SERVICE_VERSION_DEV
// 打印全局参数表中的所有参数，测试时使用，上线时去掉。
ACTION multichainns::printallgpms()
{
    require_auth( _self );

    print("allowed_num_of_bytes_of_level_1_name: ", get_allowed_num_of_bytes_of_level_x_name(1), "\n");
    print("allowed_num_of_bytes_of_level_2_name: ", get_allowed_num_of_bytes_of_level_x_name(2), "\n");
    print("allowed_num_of_bytes_of_level_3_name: ", get_allowed_num_of_bytes_of_level_x_name(3), "\n\n");

    print("fee_of_transaction_amount_percentage: ", get_fee_of_transaction_amount_percentage(), "\n");
    print("fee_of_level_1_name_share_percentage: ", get_fee_of_level_1_name_share_percentage(), "\n");
    print("fee_of_level_2_name_share_percentage: ", get_fee_of_level_2_name_share_percentage(), "\n");
    print("fee_of_level_1_name_share_quantity: ",   get_fee_of_level_1_name_share_quantity(),   "\n");
    print("fee_of_level_2_name_share_quantity: ",   get_fee_of_level_2_name_share_quantity(),   "\n\n");

    print("max_num_of_repeated_hashes_in_resolves_table: ", get_max_num_of_repeated_hashes_in_resolves_table(), "\n\n");

    print("max_num_of_bytes_of_name: ", get_max_num_of_bytes_of_name(), "\n\n");

    print("fee_of_one_resolv_record: ", get_fee_of_one_resolv_record(), "\n\n");

    print("fee_of_1_byte_level_1_name:   ", get_fee_of_y_bytes_level_x_name(1, 1),  "\n");
    print("fee_of_2_bytes_level_1_name:  ", get_fee_of_y_bytes_level_x_name(1, 2),  "\n");
    print("fee_of_3_bytes_level_1_name:  ", get_fee_of_y_bytes_level_x_name(1, 3),  "\n");
    print("fee_of_4_bytes_level_1_name:  ", get_fee_of_y_bytes_level_x_name(1, 4),  "\n");
    print("fee_of_5_bytes_level_1_name:  ", get_fee_of_y_bytes_level_x_name(1, 5),  "\n");
    print("fee_of_6_bytes_level_1_name:  ", get_fee_of_y_bytes_level_x_name(1, 6),  "\n");
    print("fee_of_7_bytes_level_1_name:  ", get_fee_of_y_bytes_level_x_name(1, 7),  "\n");
    print("fee_of_8_bytes_level_1_name:  ", get_fee_of_y_bytes_level_x_name(1, 8),  "\n");
    print("fee_of_9_bytes_level_1_name:  ", get_fee_of_y_bytes_level_x_name(1, 9),  "\n");
    print("fee_of_10_bytes_level_1_name: ", get_fee_of_y_bytes_level_x_name(1, 10), "\n");
    print("fee_of_11_bytes_level_1_name: ", get_fee_of_y_bytes_level_x_name(1, 11), "\n");
    print("fee_of_12_bytes_level_1_name: ", get_fee_of_y_bytes_level_x_name(1, 12), "\n");
    print("fee_of_13_bytes_level_1_name: ", get_fee_of_y_bytes_level_x_name(1, 13), "\n");
    print("fee_of_14_bytes_level_1_name: ", get_fee_of_y_bytes_level_x_name(1, 14), "\n");
    print("fee_of_15_bytes_level_1_name: ", get_fee_of_y_bytes_level_x_name(1, 15), "\n");
    print("fee_of_16_bytes_level_1_name: ", get_fee_of_y_bytes_level_x_name(1, 16), "\n");
    print("fee_of_17_bytes_level_1_name: ", get_fee_of_y_bytes_level_x_name(1, 17), "\n\n");

    print("fee_of_1_byte_level_2_name:   ", get_fee_of_y_bytes_level_x_name(2, 1),  "\n");
    print("fee_of_2_bytes_level_2_name:  ", get_fee_of_y_bytes_level_x_name(2, 2),  "\n");
    print("fee_of_3_bytes_level_2_name:  ", get_fee_of_y_bytes_level_x_name(2, 3),  "\n");
    print("fee_of_4_bytes_level_2_name:  ", get_fee_of_y_bytes_level_x_name(2, 4),  "\n");
    print("fee_of_5_bytes_level_2_name:  ", get_fee_of_y_bytes_level_x_name(2, 5),  "\n");
    print("fee_of_6_bytes_level_2_name:  ", get_fee_of_y_bytes_level_x_name(2, 6),  "\n");
    print("fee_of_7_bytes_level_2_name:  ", get_fee_of_y_bytes_level_x_name(2, 7),  "\n");
    print("fee_of_8_bytes_level_2_name:  ", get_fee_of_y_bytes_level_x_name(2, 8),  "\n");
    print("fee_of_9_bytes_level_2_name:  ", get_fee_of_y_bytes_level_x_name(2, 9),  "\n");
    print("fee_of_10_bytes_level_2_name: ", get_fee_of_y_bytes_level_x_name(2, 10), "\n");
    print("fee_of_11_bytes_level_2_name: ", get_fee_of_y_bytes_level_x_name(2, 11), "\n");
    print("fee_of_12_bytes_level_2_name: ", get_fee_of_y_bytes_level_x_name(2, 12), "\n");
    print("fee_of_13_bytes_level_2_name: ", get_fee_of_y_bytes_level_x_name(2, 13), "\n");
    print("fee_of_14_bytes_level_2_name: ", get_fee_of_y_bytes_level_x_name(2, 14), "\n");
    print("fee_of_15_bytes_level_2_name: ", get_fee_of_y_bytes_level_x_name(2, 15), "\n");
    print("fee_of_16_bytes_level_2_name: ", get_fee_of_y_bytes_level_x_name(2, 16), "\n");
    print("fee_of_17_bytes_level_2_name: ", get_fee_of_y_bytes_level_x_name(2, 17), "\n\n");

    print("fee_of_1_byte_level_3_name:   ", get_fee_of_y_bytes_level_x_name(3, 1),  "\n");
    print("fee_of_2_bytes_level_3_name:  ", get_fee_of_y_bytes_level_x_name(3, 2),  "\n");
    print("fee_of_3_bytes_level_3_name:  ", get_fee_of_y_bytes_level_x_name(3, 3),  "\n");
    print("fee_of_4_bytes_level_3_name:  ", get_fee_of_y_bytes_level_x_name(3, 4),  "\n");
    print("fee_of_5_bytes_level_3_name:  ", get_fee_of_y_bytes_level_x_name(3, 5),  "\n");
    print("fee_of_6_bytes_level_3_name:  ", get_fee_of_y_bytes_level_x_name(3, 6),  "\n");
    print("fee_of_7_bytes_level_3_name:  ", get_fee_of_y_bytes_level_x_name(3, 7),  "\n");
    print("fee_of_8_bytes_level_3_name:  ", get_fee_of_y_bytes_level_x_name(3, 8),  "\n");
    print("fee_of_9_bytes_level_3_name:  ", get_fee_of_y_bytes_level_x_name(3, 9),  "\n");
    print("fee_of_10_bytes_level_3_name: ", get_fee_of_y_bytes_level_x_name(3, 10), "\n");
    print("fee_of_11_bytes_level_3_name: ", get_fee_of_y_bytes_level_x_name(3, 11), "\n");
    print("fee_of_12_bytes_level_3_name: ", get_fee_of_y_bytes_level_x_name(3, 12), "\n");
    print("fee_of_13_bytes_level_3_name: ", get_fee_of_y_bytes_level_x_name(3, 13), "\n");
    print("fee_of_14_bytes_level_3_name: ", get_fee_of_y_bytes_level_x_name(3, 14), "\n");
    print("fee_of_15_bytes_level_3_name: ", get_fee_of_y_bytes_level_x_name(3, 15), "\n");
    print("fee_of_16_bytes_level_3_name: ", get_fee_of_y_bytes_level_x_name(3, 16), "\n");
    print("fee_of_17_bytes_level_3_name: ", get_fee_of_y_bytes_level_x_name(3, 17), "\n\n");
}
#endif

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
    else if (table_name == "metanames") {
        keysForDeletion.clear();
        for (auto& item : _meta_names) {
            keysForDeletion.push_back(item.id64);
        }
        for (uint64_t key : keysForDeletion) {
            auto itr = _meta_names.find(key);
            if (itr != _meta_names.end()) {
                _meta_names.erase(itr);
            }
        }
    }
    else if (table_name == "resolves") {
        keysForDeletion.clear();
        for (auto& item : _resolves) {
            keysForDeletion.push_back(item.id);
        }
        for (uint64_t key : keysForDeletion) {
            auto itr = _resolves.find(key);
            if (itr != _resolves.end()) {
                _resolves.erase(itr);
            }
        }
    }
}
#endif

// 检查名称表中是否存在指定sha256 hash对应的名称
bool multichainns::exist_in_meta_names(const checksum256& hash_of_name)
{
    auto index = _meta_names.get_index<name("bynamehash")>();
    auto itr = index.lower_bound(hash_of_name);
    if (itr == index.end()) {
        return false;
    }
    else {
        if (itr->meta_name_sha256_hash == hash_of_name) { return true;  }
        else                                            { return false; }
    }
}

// 去掉字符串首尾的空格、Tab、回车、换行
string multichainns::my_trim(const string& str_src)
{
    if (str_src == "") {
        return "";
    }
    string s = str_src;
    s.erase(0, s.find_first_not_of(" \t\r\n"));
    s.erase(s.find_last_not_of(" \t\r\n") + 1);
    return s;
}

// 获取字符串中含有.的数量
uint8_t multichainns::get_num_of_dot_in_string(const string& str_src)
{
    uint8_t r = 0;
    auto i = str_src.find(".");
    auto j = str_src.find(".");
    j = 0;
    for (auto k = 0; k < 256; k++) {
        i = str_src.find(".", j);
        if (i == str_src.npos) {
            break;
        }
        else {
            r++;
            j = i + 1;
        }
    }
    return r;
}

#ifdef NAME_SERVICE_VERSION_DEV
// 做一些测试使用，测试时使用，上线时去掉。
ACTION multichainns::test()
{
    require_auth( _self );

    print("\n\n");
    print("\n<>: <",                                    my_trim(""),                               ">\n");
    print("\n< >: <",                                   my_trim(" "),                              ">\n");
    print("\n<  >: <",                                  my_trim("  "),                             ">\n");
    print("\n<   >: <",                                 my_trim("   "),                            ">\n");
    print("\n< \\t\\r\\n>: <",                          my_trim(" \t\r\n"),                        ">\n");
    print("\n< \\t\\r\\n abc def>: <",                  my_trim(" \t\r\n abc def"),                ">\n");
    print("\n< \\t\\r\\n  abc def>: <",                 my_trim(" \t\r\n  abc def"),               ">\n");
    print("\n< \\t\\r\\n   abc def>: <",                my_trim(" \t\r\n   abc def"),              ">\n");
    print("\n<abc def \\t\\r\\n >: <",                  my_trim("abc def \t\r\n "),                ">\n");
    print("\n<abc def  \\t\\r\\n >: <",                 my_trim("abc def  \t\r\n "),               ">\n");
    print("\n<abc def   \\t\\r\\n >: <",                my_trim("abc def   \t\r\n "),              ">\n");
    print("\n< \\t\\r\\n abc def \\t\\r\\n >: <",       my_trim(" \t\r\n abc def \t\r\n "),        ">\n");
    print("\n< \\t\\r\\n  abc def  \\t\\r\\n >: <",     my_trim(" \t\r\n  abc def  \t\r\n "),      ">\n");
    print("\n< \\t\\r\\n   abc def   \\t\\r\\n >: <",   my_trim(" \t\r\n   abc def   \t\r\n "),    ">\n");
    print("##############################\n");
    print("\nabc : ",     get_num_of_dot_in_string("abc"),     "\n");
    print("\nabc. : ",    get_num_of_dot_in_string("abc."),    "\n");
    print("\n.abc : ",    get_num_of_dot_in_string(".abc"),    "\n");
    print("\n.abc. : ",   get_num_of_dot_in_string(".abc."),   "\n");
    print("\n.abc.d : ",  get_num_of_dot_in_string(".abc.d"),  "\n");
    print("\ne.abc. : ",  get_num_of_dot_in_string("e.abc."),  "\n");
    print("\ne.abc.d : ", get_num_of_dot_in_string("e.abc.d"), "\n");
    print("\n... : ",     get_num_of_dot_in_string("..."),     "\n");
    print("\n.a.a. : ",   get_num_of_dot_in_string(".a.a."),   "\n");
    print("\n\n");
}
#endif
