#include "multichainns.hpp"

//

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
