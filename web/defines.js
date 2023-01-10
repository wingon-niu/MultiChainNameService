
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

var doc_scroll_top        = 0;
var current_page          = "name_market"; // 当前页面: name_market/my_names/names_of_my_bidding/system_statistics_info
var item_num_per_page     = 20;            // 每页显示条目数量

var current_wallet        = 'anchor';      // 默认使用 Anchor 钱包

// 以下 Anchor 相关

const anchor_identifier = my_contract;
var   transport;
var   anchor_link;
var   anchor_chain_id = '';
var   anchor_node_url = '';
var   anchor_session;

if (runmode === "prod") {    // 生产环境
    anchor_chain_id = 'aca376f206b8fc25a6ed44dbdc66547c36c6c33e3a119ffbeaef943642f0e906';
    anchor_node_url = 'https://eos.greymass.com:443';
}
else {                       // 开发测试环境
    anchor_chain_id = '8a34ec7df1b8cd06ff4a8abbaa7cc50300823350cadc59ab296cb00d104d2b8f';
    anchor_node_url = 'https://192.168.135.100:8899';
}
