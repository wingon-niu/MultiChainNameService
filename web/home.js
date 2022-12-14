
// utf8编码

$(document).ready(function () {
    if ( get_cookie('i18n_lang') == undefined || get_cookie('i18n_lang') == null || get_cookie('i18n_lang') == "" ) {  
        change_lang("en");
    } else {
        change_lang( get_cookie('i18n_lang') );
    }

    $("#icon_cn").on("click", function() {
        change_lang("zh");
    });

    $("#icon_en").on("click", function() {
        change_lang("en");
    });

    $("#login").on("click", function() {
        $("#menu_body").offCanvas('close');
        my_login();
    });

    $("#logoff").on("click", function() {
        $("#menu_body").offCanvas('close');
        my_logoff();
    });

    $("#check_and_query_fee").on("click", function() {
        check_and_query_fee();
    });

    $("#create_name_button").on("click", function() {
        create_name();
    });

    $("#create_name_href").on("click", function() {
        $("#new_name_fee_value_span").html("");
        $("#name_belong_to_value_span").html("");
        create_name_show_modal();
    });

    $("#my_names_href").on("click", function() {
        show_my_names();
    });

    $("#make_sale_order_href").on("click", function() {
        do_make_sale_order();
    });

    //

    doc_scroll_top        = 0;
    current_page          = "name_market";
    page_control_init();
    hide_all_pages();
    $("#my_messages_div").hide();
    $("#name_market_div").show();

    init_wallet();
    restore_session();

    setTimeout(
        function(){
            get_names_of_market(0, 1)
        }, 1000
    );
});
