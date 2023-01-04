
// utf8编码

function get_names_of_market(id32_of_upper_level, level)
{
    let index_position = 2;
    let key_type       = 'i128';

    let lower_bd  = new BigNumber( id32_of_upper_level );
    let upper_bd  = new BigNumber( lower_bd.plus(1) );

    lower_bd      = lower_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
    lower_bd      = lower_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
    lower_bd      = lower_bd.multipliedBy(16777216);   // 16777216   = 2的24次方，相当于左移24位。

    upper_bd      = upper_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
    upper_bd      = upper_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
    upper_bd      = upper_bd.multipliedBy(16777216);   // 16777216   = 2的24次方，相当于左移24位。

    do_get_names_of_market(id32_of_upper_level, level, index_position, key_type, lower_bd.toFixed(), upper_bd.toFixed());
}

function do_get_names_of_market(id32_of_upper_level, level, index_position, key_type, lower_bound, upper_bound)
{
    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            const resp = await rpc.get_table_rows({
                json:  true,
                code:  current_my_contract,
                scope: current_my_contract,
                table: 'metanames',
                index_position: index_position,
                key_type: key_type,
                lower_bound: lower_bound,
                upper_bound: upper_bound,
                limit: item_num_per_page,
                reverse: false,
                show_payer: false
            });
            let results   = '';
            let next_page = '';
            let i = 0;
            let len = resp.rows.length;
            let onclick_str               = '';
            let selling_price_str         = '';
            let active_purchase_price_str = '';
            // 以下逐个生成名称的信息
            for (i = 0; i < len; i++) {
                if (level === 1 || level === 2) {
                    let child_level = level + 1;
                    onclick_str = ' onclick="get_names_of_market(' + resp.rows[i].id32 + ', ' + child_level + ');"';
                } else {
                    onclick_str = '';
                }
                if (resp.rows[i].status === 1) {
                    selling_price_str = resp.rows[i].selling_price;
                } else {
                    selling_price_str = '&nbsp;';
                }
                if (resp.rows[i].active_buyer === current_my_contract) {
                    active_purchase_price_str = '&nbsp;';
                } else {
                    active_purchase_price_str = resp.rows[i].active_purchase_price + '<br />buyer: ' + resp.rows[i].active_buyer;
                }
                results = results + '<tr>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;"><a href="##"' + onclick_str   + '>' + resp.rows[i].meta_name + '<br />owner: ' + resp.rows[i].owner + '</a></td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + selling_price_str         + '</td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + active_purchase_price_str + '</td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">';
                results = results + '<div class="am-dropdown am-dropdown-down" id="names_of_market_dropdown_' + resp.rows[i].id32 + '" data-am-dropdown>';
                results = results + '<button class="am-btn am-btn-success am-round am-dropdown-toggle" onclick="show_names_of_market_dropdown(' + resp.rows[i].id32 + ');" data-am-dropdown-toggle>' + $("#operations").html() + ' <span class="am-icon-caret-down"></span></button><ul class="am-dropdown-content">';
                results = results + '<li><a href="##" onclick="direct_buy('            + resp.rows[i].id32 + ', \'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\', \'' + resp.rows[i].owner + '\', \'' + resp.rows[i].selling_price + '\');">' + $("#direct_buy").html() + '</a></li>';
                results = results + '<li><a href="##" onclick="make_purchase_order('   + resp.rows[i].id32 + ', \'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\');">' + $("#make_purchase_order").html() + '</a></li>';
                results = results + '<li><a href="##" onclick="create_sub_name('       + resp.rows[i].id32 + ', \'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\');">' + $("#create_sub_name").html() + '</a></li>';
                results = results + '</ul>';
                results = results + '</div>';
                results = results + '</td>';
                results = results + '</tr>';
            }
            // 如果 当前有数据 并且 有下一页
            if (len > 0 && resp.more === true) {
                results   = '<table width="100%" border="1"><tr><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#selling_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#purchase_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td></tr>' + results + '</table>';
                next_page = '<table width="100%" border="0"><tr><td align="center"><a href="##" onclick="do_get_names_of_market(' + id32_of_upper_level + ', ' + level + ', ' + index_position + ', \'' + key_type + '\', \'' + resp.next_key + '\', \'' + upper_bound + '\');">' + $("#next_page").html() + '</a></td></tr></table>';
            }
            // 如果 当前有数据 并且 没有下一页
            else if (len > 0 && resp.more === false) {
                results   = '<table width="100%" border="1"><tr><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#selling_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#purchase_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td></tr>' + results + '</table>';
                next_page = '<table width="100%" border="0"><tr><td align="center">' + $("#this_is_the_last_page").html() + '</td></tr></table>';
            }
            // 如果 当前无数据 并且 有下一页
            else if (len === 0 && resp.more === true) {
                results   = '&nbsp;';
                next_page = '<table width="100%" border="0"><tr><td align="center"><a href="##" onclick="do_get_names_of_market(' + id32_of_upper_level + ', ' + level + ', ' + index_position + ', \'' + key_type + '\', \'' + resp.next_key + '\', \'' + upper_bound + '\');">' + $("#next_page").html() + '</a></td></tr></table>';
            }
            // 如果 当前无数据 并且 没有下一页
            else if (len === 0 && resp.more === false) {
                results   = '<table width="100%" border="0"><tr><td align="center">' + $("#no_data_found").html() + '</td></tr></table>';
                next_page = '&nbsp;';
            }
            // 向目标容器赋值
            $("#name_market_level_" + level + "_names_list_div").html(results);
            $("#name_market_level_" + level + "_names_next_page_div").html(next_page);
            // 完成
            $("#my_modal_loading").modal('close');
            if (level === 1) {
                window.scrollTo(0, 0);
            }
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();
}

function get_system_statistics_info()
{
    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            const resp1 = await rpc.get_table_rows({
                json:  true,
                code:  current_my_contract,
                scope: current_my_contract,
                table: 'globalvars',
                index_position: 1,
                key_type: 'i64',
                lower_bound: 1,
                upper_bound: 2,
                limit: 1,
                reverse: false,
                show_payer: false
            });
            const resp2 = await rpc.get_table_rows({
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
            // 向目标容器赋值
            $("#system_statistics_info_contents_globalvars_pre").html(   JSON.stringify(resp1, null, 1) );
            $("#system_statistics_info_contents_globalparams_pre").html( JSON.stringify(resp2, null, 1) );
            // 完成
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();
}

function get_my_names(user_account)
{
    let index_position = 7;
    let key_type       = 'i128';

    let lower_bd  = new BigNumber( my_eos_name_to_uint64t(user_account) );
    let upper_bd  = new BigNumber( lower_bd.plus(1) );

    lower_bd      = lower_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
    lower_bd      = lower_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。

    upper_bd      = upper_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
    upper_bd      = upper_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。

    do_get_my_names(index_position, key_type, lower_bd.toFixed(), upper_bd.toFixed());
}

function do_get_my_names(index_position, key_type, lower_bound, upper_bound)
{
    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            const resp = await rpc.get_table_rows({
                json:  true,
                code:  current_my_contract,
                scope: current_my_contract,
                table: 'metanames',
                index_position: index_position,
                key_type: key_type,
                lower_bound: lower_bound,
                upper_bound: upper_bound,
                limit: item_num_per_page,
                reverse: false,
                show_payer: false
            });
            let results   = '';
            let next_page = '';
            let i = 0;
            let len = resp.rows.length;
            let selling_price_str         = '';
            let active_purchase_price_str = '';
            // 以下逐个生成名称的信息
            for (i = 0; i < len; i++) {
                if (resp.rows[i].status === 1) {
                    selling_price_str = resp.rows[i].selling_price;
                } else {
                    selling_price_str = '&nbsp;';
                }
                if (resp.rows[i].active_buyer === current_my_contract) {
                    active_purchase_price_str = '&nbsp;';
                } else {
                    active_purchase_price_str = resp.rows[i].active_purchase_price + '<br />buyer: ' + resp.rows[i].active_buyer;
                }
                results = results + '<tr>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + resp.rows[i].meta_name + '<br />owner: ' + resp.rows[i].owner + '</td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + selling_price_str         + '</td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + active_purchase_price_str + '</td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">';
                results = results + '<div class="am-dropdown am-dropdown-down" id="my_names_dropdown_' + resp.rows[i].id32 + '" data-am-dropdown>';
                results = results + '<button class="am-btn am-btn-success am-round am-dropdown-toggle" onclick="show_my_names_dropdown(' + resp.rows[i].id32 + ');" data-am-dropdown-toggle>' + $("#operations").html() + ' <span class="am-icon-caret-down"></span></button><ul class="am-dropdown-content">';
                results = results + '<li><a href="##" onclick="make_sale_order('            + resp.rows[i].id32 + ', \'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\');">' + $("#make_sale_order").html() + '</a></li>';
                results = results + '<li><a href="##" onclick="cancel_sale_order('          + resp.rows[i].id32 + ', \'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\');">' + $("#cancel_sale_order").html() + '</a></li>';
                results = results + '<li><a href="##" onclick="direct_deal_purchase_order(' + resp.rows[i].id32 + ', \'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\', \'' + resp.rows[i].active_buyer + '\', \'' + resp.rows[i].active_purchase_price + '\');">' + $("#direct_deal_purchase_order").html() + '</a></li>';
                results = results + '<li><a href="##" onclick="manage_resolution_records('  + resp.rows[i].id32 + ', \'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\');">' + $("#manage_resolution_records").html() + '</a></li>';
                results = results + '</ul>';
                results = results + '</div>';
                results = results + '</td>';
                results = results + '</tr>';
            }
            // 如果 当前有数据 并且 有下一页
            if (len > 0 && resp.more === true) {
                results   = '<table width="100%" border="1"><tr><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#selling_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#purchase_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td></tr>' + results + '</table>';
                next_page = '<table width="100%" border="0"><tr><td align="center"><a href="##" onclick="do_get_my_names(' + index_position + ', \'' + key_type + '\', \'' + resp.next_key + '\', \'' + upper_bound + '\');">' + $("#next_page").html() + '</a></td></tr></table>';
            }
            // 如果 当前有数据 并且 没有下一页
            else if (len > 0 && resp.more === false) {
                results   = '<table width="100%" border="1"><tr><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#selling_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#purchase_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td></tr>' + results + '</table>';
                next_page = '<table width="100%" border="0"><tr><td align="center">' + $("#this_is_the_last_page").html() + '</td></tr></table>';
            }
            // 如果 当前无数据 并且 有下一页
            else if (len === 0 && resp.more === true) {
                results   = '&nbsp;';
                next_page = '<table width="100%" border="0"><tr><td align="center"><a href="##" onclick="do_get_my_names(' + index_position + ', \'' + key_type + '\', \'' + resp.next_key + '\', \'' + upper_bound + '\');">' + $("#next_page").html() + '</a></td></tr></table>';
            }
            // 如果 当前无数据 并且 没有下一页
            else if (len === 0 && resp.more === false) {
                results   = '<table width="100%" border="0"><tr><td align="center">' + $("#no_data_found").html() + '</td></tr></table>';
                next_page = '&nbsp;';
            }
            // 向目标容器赋值
            $("#my_names_list_div").html(results);
            $("#my_names_next_page_div").html(next_page);
            // 完成
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();
}

function get_names_of_my_bidding(user_account)
{
    let index_position = 9;
    let key_type       = 'i128';

    let lower_bd  = new BigNumber( my_eos_name_to_uint64t(user_account) );
    let upper_bd  = new BigNumber( lower_bd.plus(1) );

    lower_bd      = lower_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
    lower_bd      = lower_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。

    upper_bd      = upper_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
    upper_bd      = upper_bd.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。

    do_get_names_of_my_bidding(index_position, key_type, lower_bd.toFixed(), upper_bd.toFixed());
}

function do_get_names_of_my_bidding(index_position, key_type, lower_bound, upper_bound)
{
    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            const resp = await rpc.get_table_rows({
                json:  true,
                code:  current_my_contract,
                scope: current_my_contract,
                table: 'metanames',
                index_position: index_position,
                key_type: key_type,
                lower_bound: lower_bound,
                upper_bound: upper_bound,
                limit: item_num_per_page,
                reverse: false,
                show_payer: false
            });
            let results   = '';
            let next_page = '';
            let i = 0;
            let len = resp.rows.length;
            let selling_price_str         = '';
            let active_purchase_price_str = '';
            // 以下逐个生成名称的信息
            for (i = 0; i < len; i++) {
                if (resp.rows[i].status === 1) {
                    selling_price_str = resp.rows[i].selling_price;
                } else {
                    selling_price_str = '&nbsp;';
                }
                if (resp.rows[i].active_buyer === current_my_contract) {
                    active_purchase_price_str = '&nbsp;';
                } else {
                    active_purchase_price_str = resp.rows[i].active_purchase_price + '<br />buyer: ' + resp.rows[i].active_buyer;
                }
                results = results + '<tr>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + resp.rows[i].meta_name + '<br />owner: ' + resp.rows[i].owner + '</td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + selling_price_str         + '</td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + active_purchase_price_str + '</td>';
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">';
                results = results + '<div class="am-dropdown am-dropdown-down" id="names_of_my_bidding_dropdown_' + resp.rows[i].id32 + '" data-am-dropdown>';
                results = results + '<button class="am-btn am-btn-success am-round am-dropdown-toggle" onclick="show_names_of_my_bidding_dropdown(' + resp.rows[i].id32 + ');" data-am-dropdown-toggle>' + $("#operations").html() + ' <span class="am-icon-caret-down"></span></button><ul class="am-dropdown-content">';
                results = results + '<li><a href="##" onclick="cancel_purchase_order(' + resp.rows[i].id32 + ', \'' + CryptoJS.enc.Base64.stringify(CryptoJS.enc.Utf8.parse(resp.rows[i].meta_name)) + '\', \'' + resp.rows[i].active_purchase_price + '\');">' + $("#cancel_purchase_order").html() + '</a></li>';
                results = results + '</ul>';
                results = results + '</div>';
                results = results + '</td>';
                results = results + '</tr>';
            }
            // 如果 当前有数据 并且 有下一页
            if (len > 0 && resp.more === true) {
                results   = '<table width="100%" border="1"><tr><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#selling_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#purchase_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td></tr>' + results + '</table>';
                next_page = '<table width="100%" border="0"><tr><td align="center"><a href="##" onclick="do_get_names_of_my_bidding(' + index_position + ', \'' + key_type + '\', \'' + resp.next_key + '\', \'' + upper_bound + '\');">' + $("#next_page").html() + '</a></td></tr></table>';
            }
            // 如果 当前有数据 并且 没有下一页
            else if (len > 0 && resp.more === false) {
                results   = '<table width="100%" border="1"><tr><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#selling_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#purchase_price").html() + '</td><td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">&nbsp;</td></tr>' + results + '</table>';
                next_page = '<table width="100%" border="0"><tr><td align="center">' + $("#this_is_the_last_page").html() + '</td></tr></table>';
            }
            // 如果 当前无数据 并且 有下一页
            else if (len === 0 && resp.more === true) {
                results   = '&nbsp;';
                next_page = '<table width="100%" border="0"><tr><td align="center"><a href="##" onclick="do_get_names_of_my_bidding(' + index_position + ', \'' + key_type + '\', \'' + resp.next_key + '\', \'' + upper_bound + '\');">' + $("#next_page").html() + '</a></td></tr></table>';
            }
            // 如果 当前无数据 并且 没有下一页
            else if (len === 0 && resp.more === false) {
                results   = '<table width="100%" border="0"><tr><td align="center">' + $("#no_data_found").html() + '</td></tr></table>';
                next_page = '&nbsp;';
            }
            // 向目标容器赋值
            $("#names_of_my_bidding_list_div").html(results);
            $("#names_of_my_bidding_next_page_div").html(next_page);
            // 完成
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();
}

function show_my_names_dropdown(id)
{
    $("#my_names_dropdown_" + id).dropdown('open');
}

function show_names_of_market_dropdown(id)
{
    $("#names_of_market_dropdown_" + id).dropdown('open');
}

function show_names_of_my_bidding_dropdown(id)
{
    $("#names_of_my_bidding_dropdown_" + id).dropdown('open');
}

function query_resolution_record()
{
    $("#query_resolution_record_name_input").val("");
    $("#query_resolution_record_content_span").html("");
    $("#query_resolution_record_target_select").html("");

    $('#div_query_resolution_record').modal({
        relatedTarget: this,
        onCancel: function() {},
        onConfirm: function() {}
    });

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
                    $("#query_resolution_record_target_select").append('<option value="' + resp.rows[i].target + '">' + resp.rows[i].target + '</option>');
                }
                more = resp.more;
                if (more) {
                    lower_bd = resp.next_key;
                }
            }
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();
}

function do_query_resolution_record()
{
    let meta_name = $("#query_resolution_record_name_input").val().trim();

    if (meta_name === '') {
        if (get_cookie('i18n_lang') === "zh") { alert("错误：名称不能为空。"); }
        else                                  { alert("Error: Name can not be empty."); }
        return;
    }

    let meta_name_sha256_hash = CryptoJS.SHA256(meta_name).toString();

    $("#my_modal_loading").modal('open');
    const rpc = new eosjs_jsonrpc.JsonRpc(current_endpoint);
    (async () => {
        try {
            let meta_name_existing = false;
            let id32_of_meta_name  = 0;
            var resp;
            let lower_bd = meta_name_sha256_hash;
            let len      = 0;
            let more     = true;
            // 查询名称
            while (more) {
                resp = await rpc.get_table_rows({
                    json:  true,
                    code:  current_my_contract,
                    scope: current_my_contract,
                    table: 'metanames',
                    index_position: 6,
                    key_type: 'sha256',
                    lower_bound: lower_bd,
                    limit: 1,
                    reverse: false,
                    show_payer: false
                });
                len = resp.rows.length;
                if (len === 1) {
                    if (resp.rows[0].meta_name === meta_name) {
                        meta_name_existing = true;
                        id32_of_meta_name  = resp.rows[0].id32;
                        break;
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
            if (meta_name_existing === false) {
                if (get_cookie('i18n_lang') === "zh") { $("#query_resolution_record_content_span").html("查询的名称不存在。"); }
                else                                  { $("#query_resolution_record_content_span").html("The name of the query does not exist."); }
                $("#my_modal_loading").modal('close');
                return;
            }
            // 查询并显示解析记录
            let target_object = $("#query_resolution_record_target_select").val();
            let resolution_record_existing = false;
            let lower_bound = new BigNumber( id32_of_meta_name );
            lower_bound     = lower_bound.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
            lower_bound     = lower_bound.multipliedBy(4294967296); // 4294967296 = 2的32次方，相当于左移32位。
            let right_value = new BigNumber( my_eos_name_to_uint64t(target_object) );
            lower_bound     = lower_bound.plus(right_value);
            lower_bd        = lower_bound.toFixed();
            len      = 0;
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
                    limit: 1,
                    reverse: false,
                    show_payer: false
                });
                len = resp.rows.length;
                if (len === 1) {
                    if (resp.rows[0].id32_of_meta_name === id32_of_meta_name && resp.rows[0].target_object === target_object) {
                        resolution_record_existing = true;
                        $("#query_resolution_record_content_span").html(my_escapeHTML(resp.rows[0].target_content));
                        break;
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
            if (resolution_record_existing === false) {
                if (get_cookie('i18n_lang') === "zh") { $("#query_resolution_record_content_span").html("解析记录不存在。"); }
                else                                  { $("#query_resolution_record_content_span").html("Resolution record does not exist."); }
            }
            // 完成
            $("#my_modal_loading").modal('close');
        } catch (e) {
            $("#my_modal_loading").modal('close');
            alert(e);
        }
    })();
}

function show_system_instructions()
{
}
