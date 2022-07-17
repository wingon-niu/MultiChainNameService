#include <eosio/eosio.hpp>
#include <eosio/time.hpp>
#include <eosio/system.hpp>
#include <eosio/asset.hpp>
#include <eosio/contract.hpp>
#include <eosio/dispatcher.hpp>
#include <eosio/print.hpp>
#include <utility>
#include <vector>
#include <string>

#define  NAME_SERVICE_VERSION_DEV              // NAME_SERVICE_VERSION_DEV             or NAME_SERVICE_VERSION_PROD
#define  NAME_SERVICE_WITH_CLEAR_FUNCTION_YES  // NAME_SERVICE_WITH_CLEAR_FUNCTION_YES or NAME_SERVICE_WITH_CLEAR_FUNCTION_NO

#ifdef   NAME_SERVICE_VERSION_DEV
    #define  NAME_SERVICE_MAIN_TOKEN     "SYS"
#else
    #define  NAME_SERVICE_MAIN_TOKEN     "EOS"
#endif

#define  MAIN_SYMBOL         symbol(symbol_code(NAME_SERVICE_MAIN_TOKEN), 4)
#define  ZERO_ASSET          asset((int64_t)0, MAIN_SYMBOL)                       // 0 EOS

using namespace eosio;
using namespace std;

// MultiChainNameService
// 多链名称服务和唯一身份多链认证系统

CONTRACT multichainns : public eosio::contract {

public:

    using contract::contract;

    multichainns(name self, name first_receiver, datastream<const char*> ds) : contract(self, first_receiver, ds),
        _global_vars                    (get_self(), get_self().value),
        _global_parameters              (get_self(), get_self().value),
        _pri_keys                       (get_self(), get_self().value){};

    // 初始化全局变量表
    ACTION initgvarstbl();

    // 初始化全局参数表
    ACTION initgprmstbl();

    // 设置x级名称允许的字节数量为y和y以上
    ACTION setallowedxy(const uint32_t x, const uint8_t y);

    // // 为用户新增转账信息
    // ACTION addaccount(const name& user, const asset& quantity);

    // // 用户注册
    // ACTION userregist(const name& user, const string& user_name, const string& user_family_name, const string& gender, const string& birthday, const string& description);

#ifdef NAME_SERVICE_WITH_CLEAR_FUNCTION_YES
    // 清除指定的 multi_index 中的所有数据，测试时使用，上线时去掉
    ACTION cleardata(const string& table_name);
#endif

#ifdef NAME_SERVICE_VERSION_DEV
    // 将全局变量表中的所有计数类变量加1，测试时使用，上线时去掉。
    ACTION addallgvars();
#endif

private:

    // 返回当前时间戳
    uint32_t now() const {
        return current_time_point().sec_since_epoch();
    };

    // 获取某个表的主键
    uint64_t get_pri_key(const name& table_name);

    // x级名称的总数量加1
    void add_num_of_level_x_name_total(const uint32_t x);

    // y个字节的x级名称的数量加1
    void add_num_of_y_bytes_level_x_name(const uint32_t x, const uint32_t y);

    ////////////////////////////////////////////////////////////////////////////////////////////////////

//    // 文章
//    TABLE st_article {
//        name         user;
//        uint64_t     article_id;
//        string       article_hash;        // 文章的内容的数据的首hash
//        uint64_t     num_of_trns;         // 发送文章进行的转账交易次数
//        string       content_sha3_hash;   // 文章内容的Sha3 Hash字符串，64个字节
//        uint8_t      category;            // 1=现实笔记；2=梦想笔记
//        uint8_t      type;                // 1=微文；    2=长文      （区别在于长文可以有标题，微文没有标题。长文与微文都没有长度限制。）
//        uint8_t      storage_location;    // 1=EOS；     2=ETH；     3=BSC；    5=BTC；                    （文章内容数据存储在哪条链上）
//        uint64_t     forward_article_id;  // 转发的文章的id，0表示没有转发
//        uint32_t     forwarded_times;     // 被转发的次数
//        uint32_t     replied_times;       // 被回复的次数
//        uint32_t     num_of_liked;        // 被点赞的次数
//        uint32_t     last_replied_time;
//        uint32_t     post_time;
//
//        uint64_t  primary_key()                const { return article_id; }
//        uint128_t by_user_category_post_time() const {
//            return (uint128_t{user.value}<<64) + (uint128_t{category}<<32) + uint128_t{~post_time};
//        }
//        uint128_t by_category_article()        const {
//            return (uint128_t{category}<<64) + uint128_t{~article_id};
//        }
//        uint128_t by_forward_article()         const {
//            return (uint128_t{forward_article_id}<<64) + uint128_t{~article_id};
//        }
//        uint128_t by_user_article()            const {
//            return (uint128_t{user.value}<<64) + uint128_t{~article_id};
//        }
//        uint128_t by_category_last_replied_time_article() const {
//            return (uint128_t{category}<<96) + (uint128_t{~last_replied_time}<<64) + uint128_t{~article_id};
//        }
//        uint128_t by_user_last_replied_time_post_time() const {
//            return (uint128_t{user.value}<<64) + (uint128_t{~last_replied_time}<<32) + uint128_t{~post_time};
//        }
//    };
//    typedef eosio::multi_index<
//        "articles"_n, st_article,
//        indexed_by< "byusrcatpost"_n, const_mem_fun<st_article, uint128_t, &st_article::by_user_category_post_time> >,
//        indexed_by< "bycatarticle"_n, const_mem_fun<st_article, uint128_t, &st_article::by_category_article> >,
//        indexed_by< "byforwardart"_n, const_mem_fun<st_article, uint128_t, &st_article::by_forward_article> >,
//        indexed_by< "byusrarticle"_n, const_mem_fun<st_article, uint128_t, &st_article::by_user_article> >,
//        indexed_by< "byclrtimeart"_n, const_mem_fun<st_article, uint128_t, &st_article::by_category_last_replied_time_article> >,
//        indexed_by< "byulrteptime"_n, const_mem_fun<st_article, uint128_t, &st_article::by_user_last_replied_time_post_time> >
//    > tb_articles;

    // 全局变量
    TABLE st_global_vars {
        uint64_t     id;

        uint32_t     num_of_level_1_name_total;     // 1级名称的总数量
        uint32_t     num_of_level_2_name_total;     // 2级名称的总数量
        uint32_t     num_of_level_3_name_total;     // 3级名称的总数量

        uint32_t     num_of_1_byte_level_1_name;    // 1个字节的1级名称的数量
        uint32_t     num_of_2_bytes_level_1_name;   // 2个字节的1级名称的数量
        uint32_t     num_of_3_bytes_level_1_name;   // 3个字节的1级名称的数量
        uint32_t     num_of_4_bytes_level_1_name;   // 4个字节的1级名称的数量
        uint32_t     num_of_5_bytes_level_1_name;   // 5个字节的1级名称的数量
        uint32_t     num_of_6_bytes_level_1_name;   // 6个字节的1级名称的数量
        uint32_t     num_of_7_bytes_level_1_name;   // 7个字节的1级名称的数量
        uint32_t     num_of_8_bytes_level_1_name;   // 8个字节的1级名称的数量
        uint32_t     num_of_9_bytes_level_1_name;   // 9个字节的1级名称的数量
        uint32_t     num_of_10_bytes_level_1_name;  // 10个字节的1级名称的数量
        uint32_t     num_of_11_bytes_level_1_name;  // 11个字节的1级名称的数量
        uint32_t     num_of_12_bytes_level_1_name;  // 12个字节的1级名称的数量
        uint32_t     num_of_13_bytes_level_1_name;  // 13个字节的1级名称的数量
        uint32_t     num_of_14_bytes_level_1_name;  // 14个字节的1级名称的数量
        uint32_t     num_of_15_bytes_level_1_name;  // 15个字节的1级名称的数量
        uint32_t     num_of_16_bytes_level_1_name;  // 16个字节的1级名称的数量
        uint32_t     num_of_17_bytes_level_1_name;  // 17和17以上个字节的1级名称的数量

        uint32_t     num_of_1_byte_level_2_name;    // 1个字节的2级名称的数量
        uint32_t     num_of_2_bytes_level_2_name;   // 2个字节的2级名称的数量
        uint32_t     num_of_3_bytes_level_2_name;   // 3个字节的2级名称的数量
        uint32_t     num_of_4_bytes_level_2_name;   // 4个字节的2级名称的数量
        uint32_t     num_of_5_bytes_level_2_name;   // 5个字节的2级名称的数量
        uint32_t     num_of_6_bytes_level_2_name;   // 6个字节的2级名称的数量
        uint32_t     num_of_7_bytes_level_2_name;   // 7个字节的2级名称的数量
        uint32_t     num_of_8_bytes_level_2_name;   // 8个字节的2级名称的数量
        uint32_t     num_of_9_bytes_level_2_name;   // 9个字节的2级名称的数量
        uint32_t     num_of_10_bytes_level_2_name;  // 10个字节的2级名称的数量
        uint32_t     num_of_11_bytes_level_2_name;  // 11个字节的2级名称的数量
        uint32_t     num_of_12_bytes_level_2_name;  // 12个字节的2级名称的数量
        uint32_t     num_of_13_bytes_level_2_name;  // 13个字节的2级名称的数量
        uint32_t     num_of_14_bytes_level_2_name;  // 14个字节的2级名称的数量
        uint32_t     num_of_15_bytes_level_2_name;  // 15个字节的2级名称的数量
        uint32_t     num_of_16_bytes_level_2_name;  // 16个字节的2级名称的数量
        uint32_t     num_of_17_bytes_level_2_name;  // 17和17以上个字节的2级名称的数量

        uint32_t     num_of_1_byte_level_3_name;    // 1个字节的3级名称的数量
        uint32_t     num_of_2_bytes_level_3_name;   // 2个字节的3级名称的数量
        uint32_t     num_of_3_bytes_level_3_name;   // 3个字节的3级名称的数量
        uint32_t     num_of_4_bytes_level_3_name;   // 4个字节的3级名称的数量
        uint32_t     num_of_5_bytes_level_3_name;   // 5个字节的3级名称的数量
        uint32_t     num_of_6_bytes_level_3_name;   // 6个字节的3级名称的数量
        uint32_t     num_of_7_bytes_level_3_name;   // 7个字节的3级名称的数量
        uint32_t     num_of_8_bytes_level_3_name;   // 8个字节的3级名称的数量
        uint32_t     num_of_9_bytes_level_3_name;   // 9个字节的3级名称的数量
        uint32_t     num_of_10_bytes_level_3_name;  // 10个字节的3级名称的数量
        uint32_t     num_of_11_bytes_level_3_name;  // 11个字节的3级名称的数量
        uint32_t     num_of_12_bytes_level_3_name;  // 12个字节的3级名称的数量
        uint32_t     num_of_13_bytes_level_3_name;  // 13个字节的3级名称的数量
        uint32_t     num_of_14_bytes_level_3_name;  // 14个字节的3级名称的数量
        uint32_t     num_of_15_bytes_level_3_name;  // 15个字节的3级名称的数量
        uint32_t     num_of_16_bytes_level_3_name;  // 16个字节的3级名称的数量
        uint32_t     num_of_17_bytes_level_3_name;  // 17和17以上个字节的3级名称的数量

        uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index<"globalvars"_n, st_global_vars> tb_global_vars;

    // 全局参数
    TABLE st_global_parameters {
        uint64_t  id;

        uint8_t   allowed_num_of_bytes_of_level_1_name;          // 允许的1级名称的字节数量，允许创建长度等于或者大于此项设置的名称。
        uint8_t   allowed_num_of_bytes_of_level_2_name;          // 允许的2级名称的字节数量，允许创建长度等于或者大于此项设置的名称。
        uint8_t   allowed_num_of_bytes_of_level_3_name;          // 允许的3级名称的字节数量，允许创建长度等于或者大于此项设置的名称。

        uint8_t   max_num_of_repeated_hashes_in_resolves_table;  // 在解析表中，一个相同的 sha256 hash 字符串允许出现的最大次数，默认为12次。
                                                                 // 例如，很多人把自己的BTC地址解析为中本聪的BTC地址，那么这个地址在整个解析表中最多出现12次。

        asset     fee_of_1_byte_level_1_name;    // 1个字节的1级名称的收费
        asset     fee_of_2_bytes_level_1_name;   // 2个字节的1级名称的收费
        asset     fee_of_3_bytes_level_1_name;   // 3个字节的1级名称的收费
        asset     fee_of_4_bytes_level_1_name;   // 4个字节的1级名称的收费
        asset     fee_of_5_bytes_level_1_name;   // 5个字节的1级名称的收费
        asset     fee_of_6_bytes_level_1_name;   // 6个字节的1级名称的收费
        asset     fee_of_7_bytes_level_1_name;   // 7个字节的1级名称的收费
        asset     fee_of_8_bytes_level_1_name;   // 8个字节的1级名称的收费
        asset     fee_of_9_bytes_level_1_name;   // 9个字节的1级名称的收费
        asset     fee_of_10_bytes_level_1_name;  // 10个字节的1级名称的收费
        asset     fee_of_11_bytes_level_1_name;  // 11个字节的1级名称的收费
        asset     fee_of_12_bytes_level_1_name;  // 12个字节的1级名称的收费
        asset     fee_of_13_bytes_level_1_name;  // 13个字节的1级名称的收费
        asset     fee_of_14_bytes_level_1_name;  // 14个字节的1级名称的收费
        asset     fee_of_15_bytes_level_1_name;  // 15个字节的1级名称的收费
        asset     fee_of_16_bytes_level_1_name;  // 16个字节的1级名称的收费
        asset     fee_of_17_bytes_level_1_name;  // 17和17以上个字节的1级名称的收费

        asset     fee_of_1_byte_level_2_name;    // 1个字节的2级名称的收费
        asset     fee_of_2_bytes_level_2_name;   // 2个字节的2级名称的收费
        asset     fee_of_3_bytes_level_2_name;   // 3个字节的2级名称的收费
        asset     fee_of_4_bytes_level_2_name;   // 4个字节的2级名称的收费
        asset     fee_of_5_bytes_level_2_name;   // 5个字节的2级名称的收费
        asset     fee_of_6_bytes_level_2_name;   // 6个字节的2级名称的收费
        asset     fee_of_7_bytes_level_2_name;   // 7个字节的2级名称的收费
        asset     fee_of_8_bytes_level_2_name;   // 8个字节的2级名称的收费
        asset     fee_of_9_bytes_level_2_name;   // 9个字节的2级名称的收费
        asset     fee_of_10_bytes_level_2_name;  // 10个字节的2级名称的收费
        asset     fee_of_11_bytes_level_2_name;  // 11个字节的2级名称的收费
        asset     fee_of_12_bytes_level_2_name;  // 12个字节的2级名称的收费
        asset     fee_of_13_bytes_level_2_name;  // 13个字节的2级名称的收费
        asset     fee_of_14_bytes_level_2_name;  // 14个字节的2级名称的收费
        asset     fee_of_15_bytes_level_2_name;  // 15个字节的2级名称的收费
        asset     fee_of_16_bytes_level_2_name;  // 16个字节的2级名称的收费
        asset     fee_of_17_bytes_level_2_name;  // 17和17以上个字节的2级名称的收费

        asset     fee_of_1_byte_level_3_name;    // 1个字节的3级名称的收费
        asset     fee_of_2_bytes_level_3_name;   // 2个字节的3级名称的收费
        asset     fee_of_3_bytes_level_3_name;   // 3个字节的3级名称的收费
        asset     fee_of_4_bytes_level_3_name;   // 4个字节的3级名称的收费
        asset     fee_of_5_bytes_level_3_name;   // 5个字节的3级名称的收费
        asset     fee_of_6_bytes_level_3_name;   // 6个字节的3级名称的收费
        asset     fee_of_7_bytes_level_3_name;   // 7个字节的3级名称的收费
        asset     fee_of_8_bytes_level_3_name;   // 8个字节的3级名称的收费
        asset     fee_of_9_bytes_level_3_name;   // 9个字节的3级名称的收费
        asset     fee_of_10_bytes_level_3_name;  // 10个字节的3级名称的收费
        asset     fee_of_11_bytes_level_3_name;  // 11个字节的3级名称的收费
        asset     fee_of_12_bytes_level_3_name;  // 12个字节的3级名称的收费
        asset     fee_of_13_bytes_level_3_name;  // 13个字节的3级名称的收费
        asset     fee_of_14_bytes_level_3_name;  // 14个字节的3级名称的收费
        asset     fee_of_15_bytes_level_3_name;  // 15个字节的3级名称的收费
        asset     fee_of_16_bytes_level_3_name;  // 16个字节的3级名称的收费
        asset     fee_of_17_bytes_level_3_name;  // 17和17以上个字节的3级名称的收费

        uint64_t primary_key() const { return id; }
    };
    typedef eosio::multi_index<"globalparams"_n, st_global_parameters> tb_global_parameters;

    // 保存各个表的主键的表
    TABLE st_pri_keys {
        name      table_name;
        uint64_t  pri_key;

        uint64_t  primary_key()  const { return table_name.value; }
    };
    typedef eosio::multi_index<"prikeys"_n, st_pri_keys> tb_pri_keys;

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    tb_global_vars                    _global_vars;
    tb_global_parameters              _global_parameters;
    tb_pri_keys                       _pri_keys;
};
