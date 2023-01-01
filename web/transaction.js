
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
    // 查询各级名称允许的长度
    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            const resp = await rpc.get_table_rows({
                json:  true,
                code:  current_my_contract,
                scope: current_my_contract,
                table: 'globalparams',
                index_position: 1,
                key_type: 'i64',
                lower_bound: 1,
                upper_bound: 2,
                limit: 1,
                reverse: false,
                show_payer: false
            });
            let len = resp.rows.length;
            if (len === 1) {
                $("#allowed_num_of_bytes_of_level_1_name_value_span").html(resp.rows[0].allowed_num_of_bytes_of_level_1_name);
                $("#allowed_num_of_bytes_of_level_2_name_value_span").html(resp.rows[0].allowed_num_of_bytes_of_level_2_name);
                $("#allowed_num_of_bytes_of_level_3_name_value_span").html(resp.rows[0].allowed_num_of_bytes_of_level_3_name);
                $("#max_num_of_bytes_of_name_value_span").html(            resp.rows[0].max_num_of_bytes_of_name);
            }
            // 完成
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();

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

    // 计算本级名称的长度，单位：字节
    if (my_level === 1) {
        my_length = get_length_of_bytes_of_utf8_str(level_1_str);
    }
    else if (my_level === 2) {
        my_length = get_length_of_bytes_of_utf8_str(level_2_str);
    }
    else if (my_level === 3) {
        my_length = get_length_of_bytes_of_utf8_str(level_3_str);
    }
    let name_length = 0;
    if (my_length < 17) { name_length = my_length; }
    else                { name_length = 17; }
    if (get_cookie('i18n_lang') === "zh") { $("#name_belong_to_value_span").html(my_level + "级名称, 长度为" + name_length + "字节。（注：超过17个字节的当作17个字节处理。UTF8编码，一个汉字为3个字节。）"); }
    else                                  { $("#name_belong_to_value_span").html("Level=" + my_level + ", Length=" + name_length + "bytes. (Note: If there are more than 17 bytes, they will be treated as 17 bytes.)"); }

    // 根据名称的级别与长度查询费用
    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            const resp = await rpc.get_table_rows({
                json:  true,
                code:  current_my_contract,
                scope: current_my_contract,
                table: 'globalparams',
                index_position: 1,
                key_type: 'i64',
                lower_bound: 1,
                upper_bound: 2,
                limit: 1,
                reverse: false,
                show_payer: false
            });
            let len = resp.rows.length;
            if (len === 1) {
                if (my_level === 1) {
                    if      (name_length ===  1) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_1_byte_level_1_name);   }
                    else if (name_length ===  2) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_2_bytes_level_1_name);  }
                    else if (name_length ===  3) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_3_bytes_level_1_name);  }
                    else if (name_length ===  4) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_4_bytes_level_1_name);  }
                    else if (name_length ===  5) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_5_bytes_level_1_name);  }
                    else if (name_length ===  6) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_6_bytes_level_1_name);  }
                    else if (name_length ===  7) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_7_bytes_level_1_name);  }
                    else if (name_length ===  8) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_8_bytes_level_1_name);  }
                    else if (name_length ===  9) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_9_bytes_level_1_name);  }
                    else if (name_length === 10) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_10_bytes_level_1_name); }
                    else if (name_length === 11) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_11_bytes_level_1_name); }
                    else if (name_length === 12) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_12_bytes_level_1_name); }
                    else if (name_length === 13) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_13_bytes_level_1_name); }
                    else if (name_length === 14) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_14_bytes_level_1_name); }
                    else if (name_length === 15) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_15_bytes_level_1_name); }
                    else if (name_length === 16) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_16_bytes_level_1_name); }
                    else if (name_length === 17) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_17_bytes_level_1_name); }
                }
                else if (my_level === 2) {
                    if      (name_length ===  1) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_1_byte_level_2_name);   }
                    else if (name_length ===  2) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_2_bytes_level_2_name);  }
                    else if (name_length ===  3) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_3_bytes_level_2_name);  }
                    else if (name_length ===  4) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_4_bytes_level_2_name);  }
                    else if (name_length ===  5) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_5_bytes_level_2_name);  }
                    else if (name_length ===  6) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_6_bytes_level_2_name);  }
                    else if (name_length ===  7) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_7_bytes_level_2_name);  }
                    else if (name_length ===  8) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_8_bytes_level_2_name);  }
                    else if (name_length ===  9) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_9_bytes_level_2_name);  }
                    else if (name_length === 10) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_10_bytes_level_2_name); }
                    else if (name_length === 11) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_11_bytes_level_2_name); }
                    else if (name_length === 12) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_12_bytes_level_2_name); }
                    else if (name_length === 13) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_13_bytes_level_2_name); }
                    else if (name_length === 14) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_14_bytes_level_2_name); }
                    else if (name_length === 15) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_15_bytes_level_2_name); }
                    else if (name_length === 16) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_16_bytes_level_2_name); }
                    else if (name_length === 17) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_17_bytes_level_2_name); }
                }
                else if (my_level === 3) {
                    if      (name_length ===  1) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_1_byte_level_3_name);   }
                    else if (name_length ===  2) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_2_bytes_level_3_name);  }
                    else if (name_length ===  3) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_3_bytes_level_3_name);  }
                    else if (name_length ===  4) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_4_bytes_level_3_name);  }
                    else if (name_length ===  5) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_5_bytes_level_3_name);  }
                    else if (name_length ===  6) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_6_bytes_level_3_name);  }
                    else if (name_length ===  7) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_7_bytes_level_3_name);  }
                    else if (name_length ===  8) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_8_bytes_level_3_name);  }
                    else if (name_length ===  9) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_9_bytes_level_3_name);  }
                    else if (name_length === 10) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_10_bytes_level_3_name); }
                    else if (name_length === 11) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_11_bytes_level_3_name); }
                    else if (name_length === 12) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_12_bytes_level_3_name); }
                    else if (name_length === 13) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_13_bytes_level_3_name); }
                    else if (name_length === 14) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_14_bytes_level_3_name); }
                    else if (name_length === 15) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_15_bytes_level_3_name); }
                    else if (name_length === 16) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_16_bytes_level_3_name); }
                    else if (name_length === 17) { $("#new_name_fee_value_span").html(resp.rows[0].fee_of_17_bytes_level_3_name); }
                }
            }
            // 完成
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();
}

function create_name()
{
    let fee = $("#new_name_fee_value_span").html();
    if (fee.trim() === '') {
        if (get_cookie('i18n_lang') === "zh") { alert("提示：请先查询所需费用。"); }
        else                                  { alert("Prompt: Please query the required fee first."); }
        return;
    }

    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }

    // 发送交易
    if (current_wallet === 'anchor') {
        (async () => {
            try {
                const action = {
                    account:       'eosio.token',
                    name:          'transfer',
                    authorization: [anchor_session.auth],
                    data: {
                        from:     anchor_session.auth.actor,
                        to:       current_my_contract,
                        quantity: fee,
                        memo:     'Create meta name: ' + $("#new_name_input").val().trim()
                    }
                };
                let result = await anchor_session.transact({action});
                alert("OK");
            } catch (e) {
            }
        })();
    }
    else {
    }
}

function make_sale_order(id, name_base64)
{
    $("#my_names_dropdown_" + id).dropdown('close');

    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }

    let meta_name = CryptoJS.enc.Base64.parse(name_base64).toString(CryptoJS.enc.Utf8);
    $("#make_sale_order_name_input").val(meta_name);

    $('#div_make_sale_order').modal({
        relatedTarget: this,
        onCancel: function() {},
        onConfirm: function() {}
    });
}

function do_make_sale_order()
{
    // 发送交易
    if (current_wallet === 'anchor') {
        (async () => {
            try {
                const action = {
                    account:       current_my_contract,
                    name:          'makesellord',
                    authorization: [anchor_session.auth],
                    data: {
                        user:      anchor_session.auth.actor,
                        meta_name: $("#make_sale_order_name_input").val(),
                        quantity:  $("#make_sale_order_price_input").val()
                    }
                };
                let result = await anchor_session.transact({action});
                alert("OK");
            } catch (e) {
            }
        })();
    }
    else {
    }
}

function cancel_sale_order(id, name_base64)
{
    $("#my_names_dropdown_" + id).dropdown('close');

    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }

    let meta_name = CryptoJS.enc.Base64.parse(name_base64).toString(CryptoJS.enc.Utf8);

    // 发送交易
    if (current_wallet === 'anchor') {
        (async () => {
            try {
                const action = {
                    account:       current_my_contract,
                    name:          'cancelsellod',
                    authorization: [anchor_session.auth],
                    data: {
                        user:      anchor_session.auth.actor,
                        meta_name: meta_name
                    }
                };
                let result = await anchor_session.transact({action});
                alert("OK");
            } catch (e) {
            }
        })();
    }
    else {
    }
}

function direct_deal_purchase_order(id, name_base64, buyer, purchase_price)
{
    $("#my_names_dropdown_" + id).dropdown('close');

    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }

    let meta_name = CryptoJS.enc.Base64.parse(name_base64).toString(CryptoJS.enc.Utf8);

    // 发送交易
    if (current_wallet === 'anchor') {
        (async () => {
            try {
                const action = {
                    account:       current_my_contract,
                    name:          'directdealpo',
                    authorization: [anchor_session.auth],
                    data: {
                        user:      anchor_session.auth.actor,
                        buyer:     buyer,
                        meta_name: meta_name,
                        quantity:  purchase_price
                    }
                };
                let result = await anchor_session.transact({action});
                alert("OK");
            } catch (e) {
            }
        })();
    }
    else {
    }
}

function manage_resolution_records(id, name_base64)
{
    $("#my_names_dropdown_" + id).dropdown('close');

    let meta_name = CryptoJS.enc.Base64.parse(name_base64).toString(CryptoJS.enc.Utf8);
    $("#manage_resolution_records_name_input").val(meta_name);
    $("#manage_resolution_records_fee_value_span").html("");
    $("#manage_resolution_records_target_select").html("");

    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            var resp;
            let lower_bd = '1';
            let len      = 0;
            let i        = 0;
            let more     = true;
            // 查询解析目标的列表，并生成下拉选择框
            while (more) {
                resp = await rpc.get_table_rows({
                    json:  true,
                    code:  current_my_contract,
                    scope: current_my_contract,
                    table: 'rslvtargets',
                    index_position: 2,
                    key_type: 'i64',
                    lower_bound: lower_bd,
                    limit: 20,
                    reverse: false,
                    show_payer: false
                });
                len = resp.rows.length;
                for (i = 0; i < len; i++) {
                    $("#manage_resolution_records_target_select").append('<option value="' + resp.rows[i].target + '">' + resp.rows[i].target + '</option>');
                }
                more = resp.more;
                if (more) {
                    lower_bd = resp.next_key;
                }
            }
            // 查询并显示已有的解析记录
            $("#div_list_of_resolv_records").html("");
            let results     = '';
            let lower_bound = new BigNumber( id );
            lower_bound     = lower_bound.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
            lower_bound     = lower_bound.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
            lower_bd        = lower_bound.toFixed();
            len      = 0;
            i        = 0;
            more     = true;
            while (more) {
                resp = await rpc.get_table_rows({
                    json:  true,
                    code:  current_my_contract,
                    scope: current_my_contract,
                    table: 'resolves',
                    index_position: 2,
                    key_type: 'i128',
                    lower_bound: lower_bd,
                    limit: 2,
                    reverse: false,
                    show_payer: false
                });
                len = resp.rows.length;
                for (i = 0; i < len; i++) {
                    if (resp.rows[i].id32_of_meta_name === id) {
                        results = results + '<tr>';
                        results = results + '<td style="vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + resp.rows[i].target_object  + '</td>';
                        results = results + '<td style="vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + resp.rows[i].target_content + '</td>';
                        results = results + '<td style="vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;"><a href="##" onclick="user_remove_one_resolv_record(\'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\', \'' + resp.rows[i].target_object + '\');">' + $("#remove_one_resolv_record").html() + '</a></td>';
                        results = results + '</tr>';
                    }
                    else {
                        break;
                    }
                }
                more = resp.more;
                if (more) {
                    lower_bd = resp.next_key;
                }
            }
            // 完成
            if (results === '') {
            }
            else {
                results = '<table width="100%" border="1">' + results + '</table>';
            }
            $("#div_list_of_resolv_records").html(results);
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();

    $('#div_manage_resolution_records').modal({
        relatedTarget: this,
        onCancel: function() {},
        onConfirm: function() {}
    });
}

function do_manage_resolution_records_query_fee()
{
    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            const resp = await rpc.get_table_rows({
                json:  true,
                code:  current_my_contract,
                scope: current_my_contract,
                table: 'globalparams',
                index_position: 1,
                key_type: 'i64',
                lower_bound: 1,
                upper_bound: 2,
                limit: 1,
                reverse: false,
                show_payer: false
            });
            let len = resp.rows.length;
            if (len === 1) {
                $("#manage_resolution_records_fee_value_span").html(resp.rows[0].fee_of_one_resolv_record);
            }
            // 完成
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();
}

function do_manage_resolution_records_insert_or_update_one_resolv_record()
{
    let fee = $("#manage_resolution_records_fee_value_span").html();
    if (fee.trim() === '') {
        if (get_cookie('i18n_lang') === "zh") { alert("提示：请先查询所需费用。"); }
        else                                  { alert("Prompt: Please query the required fee first."); }
        return;
    }

    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }

    // 发送交易
    if (current_wallet === 'anchor') {
        (async () => {
            try {
                const action = {
                    account:       'eosio.token',
                    name:          'transfer',
                    authorization: [anchor_session.auth],
                    data: {
                        from:     anchor_session.auth.actor,
                        to:       current_my_contract,
                        quantity: fee,
                        memo:     'Insert or update one resolv record: <meta_name_begin_flag>' + $("#manage_resolution_records_name_input").val() + '<meta_name_end_flag> ' + $("#manage_resolution_records_target_select").val() + ' ' + $("#manage_resolution_records_content_input").val();
                    }
                };
                let result = await anchor_session.transact({action});
                alert("OK");
            } catch (e) {
            }
        })();
    }
    else {
    }
}

function user_remove_one_resolv_record(name_base64, target_object)
{
}

function direct_buy(id, name_base64, owner, selling_price)
{
    $("#names_of_market_dropdown_" + id).dropdown('close');

    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }

    let meta_name = CryptoJS.enc.Base64.parse(name_base64).toString(CryptoJS.enc.Utf8);

    // 发送交易
    if (current_wallet === 'anchor') {
        (async () => {
            try {
                const action = {
                    account:       'eosio.token',
                    name:          'transfer',
                    authorization: [anchor_session.auth],
                    data: {
                        from:     anchor_session.auth.actor,
                        to:       current_my_contract,
                        quantity: selling_price,
                        memo:     'Direct buy meta name: ' + owner + ' ' + meta_name
                    }
                };
                let result = await anchor_session.transact({action});
                alert("OK");
            } catch (e) {
            }
        })();
    }
    else {
    }
}

function make_purchase_order(id, name_base64)
{
    $("#names_of_market_dropdown_" + id).dropdown('close');

    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }

    let meta_name = CryptoJS.enc.Base64.parse(name_base64).toString(CryptoJS.enc.Utf8);
    $("#make_purchase_order_name_input").val(meta_name);

    $('#div_make_purchase_order').modal({
        relatedTarget: this,
        onCancel: function() {},
        onConfirm: function() {}
    });
}

function do_make_purchase_order()
{
    // 发送交易
    if (current_wallet === 'anchor') {
        (async () => {
            try {
                const action = {
                    account:       'eosio.token',
                    name:          'transfer',
                    authorization: [anchor_session.auth],
                    data: {
                        from:     anchor_session.auth.actor,
                        to:       current_my_contract,
                        quantity: $("#make_purchase_order_price_input").val(),
                        memo:     'Actively place purchase order for meta name: ' + $("#make_purchase_order_name_input").val()
                    }
                };
                let result = await anchor_session.transact({action});
                alert("OK");
            } catch (e) {
            }
        })();
    }
    else {
    }
}

function create_sub_name(id, name_base64)
{
    $("#names_of_market_dropdown_" + id).dropdown('close');

    let meta_name = CryptoJS.enc.Base64.parse(name_base64).toString(CryptoJS.enc.Utf8);

    $("#new_name_input").val('.' + meta_name);
    $("#new_name_fee_value_span").html("");
    $("#name_belong_to_value_span").html("");
    create_name_show_modal();
}

function cancel_purchase_order(id, name_base64, purchase_price)
{
    $("#names_of_my_bidding_dropdown_" + id).dropdown('close');

    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }

    let meta_name = CryptoJS.enc.Base64.parse(name_base64).toString(CryptoJS.enc.Utf8);

    // 发送交易
    if (current_wallet === 'anchor') {
        (async () => {
            try {
                const action = {
                    account:       current_my_contract,
                    name:          'cancelactpo',
                    authorization: [anchor_session.auth],
                    data: {
                        user:      anchor_session.auth.actor,
                        meta_name: meta_name,
                        quantity:  purchase_price
                    }
                };
                let result = await anchor_session.transact({action});
                alert("OK");
            } catch (e) {
            }
        })();
    }
    else {
    }
}
