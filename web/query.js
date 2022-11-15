
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
                results = results + '<td style="width:25%; vertical-align:middle; text-align:center; word-wrap:break-word; word-break:break-all;">' + $("#operations").html()   + '</td>';
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
