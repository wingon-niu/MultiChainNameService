
// utf8编码

function init_wallet()
{
    if (current_wallet === 'anchor') {
        transport   = new AnchorLinkBrowserTransport();
        anchor_link = new AnchorLink({
            transport,
            chains: [{
                chainId: anchor_chain_id,
                nodeUrl: anchor_node_url
            }]
        });
    }
    else {
    }
}

function my_login()
{
    if (current_user_account != '') {
        if (get_cookie('i18n_lang') === "zh") { alert("提示：您已经在登录状态。"); }
        else                                  { alert("Prompt: You are already logged in."); }
        return;
    }
    if (current_wallet === 'anchor') {
        anchor_link.login(anchor_identifier).then((result) => {
            anchor_session = result.session;
            current_user_account = anchor_session.auth.actor;
            set_login_flag();
        });
    }
    else {
    }
}

function set_login_flag()
{
    let s = $("#login").html() + " " + current_user_account;
    $("#login").html(s);
}

function my_logoff()
{
    if (current_user_account === '') {
        if (get_cookie('i18n_lang') === "zh") { alert("提示：您尚未登录。"); }
        else                                  { alert("Prompt: You are not logged in."); }
        return;
    }
    if (current_wallet === 'anchor') {
        anchor_session.remove();
        current_user_account = '';
        clear_login_flag();
    }
    else {
    }
}

function clear_login_flag()
{
    let s = $("#login").html();
    let words = s.split(' ');
    if (words.length === 3) {
        $("#login").html( words[0] + ' ' + words[1] );
    }
}

function restore_session()
{
    if (current_wallet === 'anchor') {
        anchor_link.restoreSession(anchor_identifier).then((result) => {
            anchor_session = result;
            if (anchor_session) {
                current_user_account = anchor_session.auth.actor;
                set_login_flag();
            }
        })
    }
    else {
    }
}

function create_name_show_modal()
{
    $('#div_create_name').modal({
        relatedTarget: this,
        onCancel: function() {},
        onConfirm: function() {}
    });
}

function check_and_query_fee()
{
    let meta_name = $("#new_name_input").val().trim();

    if (meta_name === '') {
        if (get_cookie('i18n_lang') === "zh") { alert("错误：名称不能为空。"); }
        else                                  { alert("Error: Name can not be empty."); }
        return;
    }

    let level_1_str = '';
    let level_2_str = '';
    let level_3_str = '';
    let my_level    = 0;
    let my_length   = 0;

    // 将名称分拆到各级字符串
    let words = meta_name.split('.');
    if (words.length === 1) {
        level_1_str = words[0];
        my_level = 1;
    }
    else if (words.length === 2) {
        level_1_str = words[1];
        level_2_str = words[0];
        my_level = 2;
    }
    else if (words.length === 3) {
        level_1_str = words[2];
        level_2_str = words[1];
        level_3_str = words[0];
        my_level = 3;
    }
    else {
        if (get_cookie('i18n_lang') === "zh") { alert("错误：最多只支持3级名称。"); }
        else                                  { alert("Error: Only 3 levels are supported at most."); }
        return;
    }

    // 检查各级字符串的首尾是否有空格
    if (my_level === 1) {
        if(level_1_str === '' || level_1_str.trim() === '' || level_1_str != level_1_str.trim()) {
            if (get_cookie('i18n_lang') === "zh") { alert("错误：名称的开头和结尾不能是空格，并且名称不能为空。"); }
            else                                  { alert("Error: The beginning and end of the name can not be spaces, or name can not be empty."); }
            return;
        }
    }
    else if (my_level === 2) {
        if(level_1_str === '' || level_1_str.trim() === '' || level_1_str != level_1_str.trim() || level_2_str === '' || level_2_str.trim() === '' || level_2_str != level_2_str.trim()) {
            if (get_cookie('i18n_lang') === "zh") { alert("错误：名称的开头和结尾不能是空格，并且名称不能为空。"); }
            else                                  { alert("Error: The beginning and end of the name can not be spaces, or name can not be empty."); }
            return;
        }
    }
    else if (my_level === 3) {
        if(level_1_str === '' || level_1_str.trim() === '' || level_1_str != level_1_str.trim() || level_2_str === '' || level_2_str.trim() === '' || level_2_str != level_2_str.trim() || level_3_str === '' || level_3_str.trim() === '' || level_3_str != level_3_str.trim()) {
            if (get_cookie('i18n_lang') === "zh") { alert("错误：名称的开头和结尾不能是空格，并且名称不能为空。"); }
            else                                  { alert("Error: The beginning and end of the name can not be spaces, or name can not be empty."); }
            return;
        }
    }

}

function create_name()
{
    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }
}

//// transfer tokens using a session
//function transfer() {
//    const action = {
//        account: 'eosio.token',
//        name: 'transfer',
//        authorization: [session.auth],
//        data: {
//            from: session.auth.actor,
//            to: 'teamgreymass',
//            quantity: '0.0001 EOS',
//            memo: 'Anchor is the best! Thank you <3'
//        }
//    }
//    session.transact({action}).then((result) => {
//        document.getElementById('log').innerHTML += `Transaction broadcast! ${ result.processed.id }\n`
//    })
//}
