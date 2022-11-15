
// utf8编码

const runmode               = "dev";                    // prod or dev

const my_contract           = 'multichainns';

var eos_chain_id            = null;
var eos_network             = null;

if (runmode === "prod") {    // 生产环境
    eos_chain_id     = 'aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906';

    eos_network = {
        blockchain: 'eos',
        protocol: 'https',
        host: 'eos.greymass.com',
        port: 443,
        chainId: eos_chain_id
    }
} else {                     // 开发测试环境
    eos_chain_id     = '8a34ec7df1b8cd06ff4a8abbaa7cc50300823350cadc59ab296cb00d104d2b8f';

    eos_network = {
        blockchain: 'eos',
        protocol: 'https',
        host: '192.168.135.100',
        port: 8899,
        chainId: eos_chain_id
    }
}

const eos_endpoint        = eos_network.protocol     + '://' + eos_network.host     + ':' + eos_network.port;

var current_block_chain   = "eos";
var current_my_contract   = my_contract;
var current_network       = eos_network;
var current_endpoint      = eos_endpoint;
var current_user_account  = "";
var trn_success           = true;   // 最近一次交易的状态
var trn_hash              = "";     // 最近一次成功的交易的交易hash

var doc_scroll_top        = 0;
var current_page          = "name_market"; // 当前页面: name_market/my_names/names_of_my_bidding/system_statistics_info
var item_num_per_page     = 3;             // 每页显示条目数量

