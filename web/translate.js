
// utf8编码

function set_lang_cookie(lang_type)
{
    var exdate = new Date();
    exdate.setDate(exdate.getDate() + 30000);
    document.cookie = "i18n_lang=" + escape(lang_type) + "^;expires=" + exdate.toGMTString() + ";path=/";
}

// 尽管document.cookie看上去就像一个属性，可以赋不同的值。但它和一般的属性不一样，改变它的赋值并不意味着丢失原来的值，例如连续执行下面两条语句： 
// document.cookie="userId=828"; 
// document.cookie="userName=hulk";
// 这时浏览器将维护两个cookie，分别是userId和userName，因此给document.cookie赋值更像执行类似这样的语句： 
// document.addCookie("userId=828"); 
// document.addCookie("userName=hulk");
// 事实上，浏览器就是按照这样的方式来设置cookie的，如果要改变一个cookie的值，只需重新赋值，例如： 
// document.cookie="userId=929"; 
// 这样就将名为userId的cookie值设置为了929。

function set_cookie(c_name, c_value)
{
    var exdate = new Date();
    exdate.setDate(exdate.getDate() + 30000);
    document.cookie = c_name + "=" + escape(c_value) + "^;expires=" + exdate.toGMTString() + ";path=/";
}

function get_cookie(c_name)
{
    if (document.cookie && document.cookie != '') {
        let c_start = document.cookie.indexOf(c_name + "=");
        if (c_start != -1) { 
            c_start = c_start + c_name.length + 1;
            let c_end = document.cookie.indexOf("^", c_start);
            if (c_end == -1) c_end = document.cookie.length;
            return unescape(document.cookie.substring(c_start, c_end));
        }
    }
    return "";
}

function change_lang(lang_type)
{
    set_lang_cookie(lang_type);

    jQuery.i18n.properties({
        name : "strings",
        path : "/",
        mode : 'map',
        language : lang_type,
        cache : false,
        encoding : 'UTF-8',
        async : false,
        callback : function() {
            $("#header_title").html($.i18n.prop('header_title'));
            $("#login").html($.i18n.prop('login'));
            $("#logoff").html($.i18n.prop('logoff'));
            $(".my_cancel").html($.i18n.prop('my_cancel'));
            $(".my_confirm").html($.i18n.prop('my_confirm'));
            $(".my_close").html($.i18n.prop('my_close'));
            $("#please_login").html($.i18n.prop('please_login'));
            $("#next_page").html($.i18n.prop('next_page'));
            $("#name_market").html($.i18n.prop('name_market'));
            $("#name_market_info_span").html($.i18n.prop('name_market'));
            $("#my_names").html($.i18n.prop('my_names'));
            $("#my_names_info_span").html($.i18n.prop('my_names'));
            $("#my_names_span").html($.i18n.prop('my_names'));
            $("#names_of_my_bidding").html($.i18n.prop('names_of_my_bidding'));
            $("#names_of_my_bidding_info_span").html($.i18n.prop('names_of_my_bidding'));
            $("#create_name_span").html($.i18n.prop('create_name_span'));
            $("#system_statistics_info").html($.i18n.prop('system_statistics_info'));
            $("#system_statistics_info_info_span").html($.i18n.prop('system_statistics_info'));
            $(".level_1_names_info_span").html($.i18n.prop('level_1_names_info_span'));
            $(".level_2_names_info_span").html($.i18n.prop('level_2_names_info_span'));
            $(".level_3_names_info_span").html($.i18n.prop('level_3_names_info_span'));
            $("#no_data_found").html($.i18n.prop('no_data_found'));
            $("#this_is_the_last_page").html($.i18n.prop('this_is_the_last_page'));
            $("#selling_price").html($.i18n.prop('selling_price'));
            $("#purchase_price").html($.i18n.prop('purchase_price'));
            $("#operations").html($.i18n.prop('operations'));
            $("#system_variables_span").html($.i18n.prop('system_variables_span'));
            $("#system_parameters_span").html($.i18n.prop('system_parameters_span'));
            $("#new_name_span").html($.i18n.prop('new_name_span'));
            $("#new_name_fee_span").html($.i18n.prop('new_name_fee_span'));
            $("#check_and_query_fee").html($.i18n.prop('check_and_query_fee'));
            $("#create_name_button").html($.i18n.prop('create_name_button'));
            $("#name_belong_to_span").html($.i18n.prop('name_belong_to_span'));
            $("#allowed_num_of_bytes_of_level_1_name_span").html($.i18n.prop('allowed_num_of_bytes_of_level_1_name_span'));
            $("#allowed_num_of_bytes_of_level_2_name_span").html($.i18n.prop('allowed_num_of_bytes_of_level_2_name_span'));
            $("#allowed_num_of_bytes_of_level_3_name_span").html($.i18n.prop('allowed_num_of_bytes_of_level_3_name_span'));
            $("#max_num_of_bytes_of_name_span").html($.i18n.prop('max_num_of_bytes_of_name_span'));

            //

            if (current_user_account != '') {
                let s = $("#login").html();
                $("#login").html( s + ' ' + current_user_account );
            }
        }
    });
}
