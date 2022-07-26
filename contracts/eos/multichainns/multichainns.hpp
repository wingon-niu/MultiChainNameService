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
        _meta_names                     (get_self(), get_self().value),
        _global_vars                    (get_self(), get_self().value),
        _global_parameters              (get_self(), get_self().value),
        _pri_keys                       (get_self(), get_self().value){};

    // 初始化全局变量表
    ACTION initgvarstbl();

    // 初始化全局参数表
    ACTION initgprmstbl();

    // 设置x级名称允许的字节数量为y和y以上
    ACTION setallowedxy(const uint32_t x, const uint8_t y);

    // 设置同一个hash字符串在解析表中出现的最大次数
    ACTION setmaxnumorh(const uint8_t max_num_of_repeated_hashes_in_resolves_table);

    // 设置名称的最大长度，单位是字节。
    ACTION setmnlength(const uint8_t n);

    // 设置每条解析记录的收费
    ACTION setfeeof1rr(const asset& quantity);

    // 设置y个字节的x级名称的收费
    ACTION setfeeofxy(const uint32_t x, const uint32_t y, const asset& quantity);

    // 设置所有的x级名称y个字节的收费，按照代码中写好的固定值
    ACTION setallfee();

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

#ifdef NAME_SERVICE_VERSION_DEV
    // 打印全局参数表中的所有参数，测试时使用，上线时去掉。
    ACTION printallgpms();
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

    // 累加总成交金额
    void add_total_transaction_amount(const asset& quantity);

    // 解析表中的记录的总数加1
    void add_total_num_of_records_in_resolv_table();

    // 解析表中的记录的总数减1
    void sub_total_num_of_records_in_resolv_table();

    // y个字节的x级名称的数量加1
    void add_num_of_y_bytes_level_x_name(const uint32_t x, const uint32_t y);

    // 设置y个字节的x级名称的收费
    void set_fee_of_y_bytes_level_x_name(const uint32_t x, const uint32_t y, const asset& quantity);

    // 获取解析表中的hash字符串的最大重复次数
    uint8_t get_max_num_of_repeated_hashes_in_resolves_table();

    // 获取名称的最大长度，单位是字节。
    uint8_t get_max_num_of_bytes_of_name();

    // 获取每条解析记录的收费
    asset get_fee_of_one_resolv_record();

    // 获取x级名称允许的字节数量
    uint8_t get_allowed_num_of_bytes_of_level_x_name(const uint32_t x);

    // 获取y个字节的x级名称的收费
    asset get_fee_of_y_bytes_level_x_name(const uint32_t x, const uint32_t y);

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    // 名称表
    TABLE st_meta_names {
        uint64_t     id64;
        uint32_t     id32;
        name         owner;
        string       meta_name;
        checksum256  meta_name_sha256_hash;
        uint8_t      language;               // 0: 没有设定      1: 中文      2: 英文
        uint32_t     id32_of_upper_level;    // 上级名称的id32。如果是0，表示这是一个1级名称。
        uint8_t      level;                  // 本名称的级别
        uint8_t      length;                 // 本名称的长度，单位是字节。只包括自身字符串的长度，不包括上、下级字符串与“.”在内。
        uint8_t      category;               // 1: 公共的              2: 私有的
        uint8_t      status;                 // 0: 不在挂单出售中      1: 挂单出售中
        asset        selling_price;          // 挂单出售的价格，允许的最高价格为： 429496.7295 EOS
        name         active_buyer;           // 主动求购者。创建时此项设置为_self，用于区分是否有真正的主动求购者。
        asset        active_purchase_price;  // 主动求购价格
        string       avatar;                 // JSON字符串，内含头像的首hash等内容。
        string       description;            // 描述
        uint32_t     creation_time;          // 创建时间
        uint32_t     expiration_time;        // 到期时间。在创建时间的1024年之后到期。
        string       spare1;                 // 备用1
        string       spare2;                 // 备用2
        string       spare3;                 // 备用3

        uint64_t  primary_key()                const { return id64; }

        uint128_t by_language_id32upper_selflevel_length_status_sellingpriceasc_selfid32asc() const {
            uint32_t sp32 = static_cast<uint32_t>(static_cast<uint64_t>(selling_price.amount));
            return (uint128_t{language}<<120) + (uint128_t{id32_of_upper_level}<<88) + (uint128_t{level}<<80) + (uint128_t{length}<<72) + (uint128_t{status}<<64) + (uint128_t{sp32}<<32) + (uint128_t{id32});
        }

        uint128_t by_language_id32upper_selflevel_length_status_sellingpriceasc_selfid32desc() const {
            uint32_t sp32 = static_cast<uint32_t>(static_cast<uint64_t>(selling_price.amount));
            return (uint128_t{language}<<120) + (uint128_t{id32_of_upper_level}<<88) + (uint128_t{level}<<80) + (uint128_t{length}<<72) + (uint128_t{status}<<64) + (uint128_t{sp32}<<32) + (uint128_t{~id32});
        }

        uint128_t by_language_id32upper_selflevel_length_status_sellingpricedesc_selfid32asc() const {
            uint32_t sp32 = static_cast<uint32_t>(static_cast<uint64_t>(selling_price.amount));
            return (uint128_t{language}<<120) + (uint128_t{id32_of_upper_level}<<88) + (uint128_t{level}<<80) + (uint128_t{length}<<72) + (uint128_t{status}<<64) + (uint128_t{~sp32}<<32) + (uint128_t{id32});
        }

        uint128_t by_language_id32upper_selflevel_length_status_sellingpricedesc_selfid32desc() const {
            uint32_t sp32 = static_cast<uint32_t>(static_cast<uint64_t>(selling_price.amount));
            return (uint128_t{language}<<120) + (uint128_t{id32_of_upper_level}<<88) + (uint128_t{level}<<80) + (uint128_t{length}<<72) + (uint128_t{status}<<64) + (uint128_t{~sp32}<<32) + (uint128_t{~id32});
        }

        checksum256 by_meta_name_sha256_hash() const {
            return meta_name_sha256_hash;
        }

        uint128_t by_owner_id32asc() const {
            return (uint128_t{owner.value}<<64) + uint128_t{id32};
        }

        uint128_t by_owner_id32desc() const {
            return (uint128_t{owner.value}<<64) + uint128_t{~id32};
        }

        uint128_t by_activebuyer_id32asc() const {
            return (uint128_t{active_buyer.value}<<64) + uint128_t{id32};
        }

        uint128_t by_activebuyer_id32desc() const {
            return (uint128_t{active_buyer.value}<<64) + uint128_t{~id32};
        }
    };
    typedef eosio::multi_index<
        "metanames"_n, st_meta_names,
        indexed_by< "byascasc"_n,     const_mem_fun<st_meta_names, uint128_t,   &st_meta_names::by_language_id32upper_selflevel_length_status_sellingpriceasc_selfid32asc> >,
        indexed_by< "byascdesc"_n,    const_mem_fun<st_meta_names, uint128_t,   &st_meta_names::by_language_id32upper_selflevel_length_status_sellingpriceasc_selfid32desc> >,
        indexed_by< "bydescasc"_n,    const_mem_fun<st_meta_names, uint128_t,   &st_meta_names::by_language_id32upper_selflevel_length_status_sellingpricedesc_selfid32asc> >,
        indexed_by< "bydescdesc"_n,   const_mem_fun<st_meta_names, uint128_t,   &st_meta_names::by_language_id32upper_selflevel_length_status_sellingpricedesc_selfid32desc> >,
        indexed_by< "bynamehash"_n,   const_mem_fun<st_meta_names, checksum256, &st_meta_names::by_meta_name_sha256_hash> >,
        indexed_by< "byowneridasc"_n, const_mem_fun<st_meta_names, uint128_t,   &st_meta_names::by_owner_id32asc> >,
        indexed_by< "byowniddesc"_n,  const_mem_fun<st_meta_names, uint128_t,   &st_meta_names::by_owner_id32desc> >,
        indexed_by< "bybuyeridasc"_n, const_mem_fun<st_meta_names, uint128_t,   &st_meta_names::by_activebuyer_id32asc> >,
        indexed_by< "bybuyeriddes"_n, const_mem_fun<st_meta_names, uint128_t,   &st_meta_names::by_activebuyer_id32desc> >
    > tb_meta_names;

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

        asset        total_transaction_amount;                  // 总成交金额
        uint32_t     total_num_of_records_in_resolv_table;      // 解析表中的记录的总数

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

        uint8_t   max_num_of_bytes_of_name;      // 名称的最大长度，单位是字节。
                                                 // 例如设置为6，则允许创建的最长的1级名称为：abcdef。
                                                 // 允许创建的最长的2级名称为：abcdef.abcdef。
                                                 // 允许创建的最长的3级名称为：abcdef.abcdef.abcdef。

        asset     fee_of_one_resolv_record;      // 每条解析记录的收费

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

    tb_meta_names                     _meta_names;
    tb_global_vars                    _global_vars;
    tb_global_parameters              _global_parameters;
    tb_pri_keys                       _pri_keys;
};
