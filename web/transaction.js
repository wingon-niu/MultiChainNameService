
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
