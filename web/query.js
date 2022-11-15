
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
            let onclick_str = '';
            // 以下逐个生成名称的信息
            for (i = 0; i < len; i++) {
                if (level === 1 || level === 2) {
                    let child_level = level + 1;
                    onclick_str = ' onclick="get_names_of_market(' + resp.rows[i].id32 + ', ' + child_level + ');"';
                } else {
                    onclick_str = '';
                }
                results = results + '<tr>';
                results = results + '<td width="25%" valign="middle" style="word-wrap:break-word; word-break:break-all;"><a href="##"' + onclick_str + '>' + resp.rows[i].meta_name + '</a></td>';
                results = results + '<td width="25%" valign="middle">&nbsp;</td>';
                results = results + '<td width="25%" valign="middle">&nbsp;</td>';
                results = results + '<td width="25%" valign="middle">&nbsp;</td>';
                results = results + '</tr>';
                //let f = cate + '<a href="##" onclick="alert(\'' + $("#head_hash").html() + storage_locations[resp.rows[i].storage_location] + '{' + resp.rows[i].article_hash + '}\');">id' + resp.rows[i].article_id + '</a>&nbsp;&nbsp;&nbsp;';
                //if (resp.rows[i].forward_article_id > 0) {
                //    f = f + '<span>' + $("#forward_article").html() + '</span>&nbsp;<a href="##" onclick="show_article_content_div(' + resp.rows[i].forward_article_id + ');">id' + resp.rows[i].forward_article_id + '</a>';
                //}
                //articles = articles + '<div><table width="100%" border="0">';
                //articles = articles + '<tr>' + '<td rowspan="3" width="71" align="center" valign="top"><a href="##" onclick="query_user_profile(\'' + resp.rows[i].user + '\');">' + '<div class="div_user_avatar_' + convert_dot_to_underline(resp.rows[i].user) + '" style="width:64px; height:64px;"></div></a></td>' + '<td><a href="##" onclick="query_user_profile(\'' + resp.rows[i].user + '\');">' + resp.rows[i].user + '</a>&nbsp;&nbsp;' + timestamp_trans_full(resp.rows[i].post_time) + '</td>' + '</tr>';
                //articles = articles + '<tr>' + '<td>' + f + '<pre class="preview_of_article_' + resp.rows[i].article_id + '" onclick="show_article_content_div(' + resp.rows[i].article_id + ');">&nbsp;</pre></td>' + '</tr>';
                //articles = articles + '<tr>' + '<td align="right"><a href="##" onclick="copy_article_link_to_clipboard(' + resp.rows[i].article_id + ');">' + $("#copy_link_only").html() + '</a>&nbsp;&nbsp;<span class="am-icon-share"></span>&nbsp;' + resp.rows[i].forwarded_times + '&nbsp;&nbsp;<span class="am-icon-comment"></span>&nbsp;' + resp.rows[i].replied_times + '&nbsp;&nbsp;</td>' + '</tr>';
                //articles = articles + '</table></div><hr />';
            }
            // 如果 当前有数据 并且 有下一页
            if (len > 0 && resp.more === true) {
                results   = '<table width="100%" border="1"><tr><td width="25%" align="center">&nbsp;</td><td width="25%" align="center">' + $("#selling_price").html() + '</td><td width="25%" align="center">' + $("#purchase_price").html() + '</td><td width="25%" align="center">&nbsp;</td></tr>' + results + '</table>';
                next_page = '<table width="100%" border="0"><tr><td align="center"><a href="##" onclick="do_get_names_of_market(' + id32_of_upper_level + ', ' + level + ', ' + index_position + ', \'' + key_type + '\', \'' + resp.next_key + '\', \'' + upper_bound + '\');">' + $("#next_page").html() + '</a></td></tr></table>';
            }
            // 如果 当前有数据 并且 没有下一页
            else if (len > 0 && resp.more === false) {
                results   = '<table width="100%" border="1"><tr><td width="25%" align="center">&nbsp;</td><td width="25%" align="center">' + $("#selling_price").html() + '</td><td width="25%" align="center">' + $("#purchase_price").html() + '</td><td width="25%" align="center">&nbsp;</td></tr>' + results + '</table>';
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
