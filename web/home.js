
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

    $("#make_purchase_order_href").on("click", function() {
        do_make_purchase_order();
    });

    $("#manage_resolution_records_query_fee").on("click", function() {
        do_manage_resolution_records_query_fee();
    });

    $("#manage_resolution_records_insert_or_update_one_resolv_record").on("click", function() {
        do_manage_resolution_records_insert_or_update_one_resolv_record();
    });

    $("#menu_query_resolution_record").on("click", function() {
        $("#menu_body").offCanvas('close');
        query_resolution_record();
    });

    $("#query_resolution_record_button").on("click", function() {
        do_query_resolution_record();
    });

    $("#menu_system_instructions").on("click", function() {
        $("#menu_body").offCanvas('close');
        show_system_instructions();
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
