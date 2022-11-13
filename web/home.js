
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
    });

    $("#logoff").on("click", function() {
        $("#menu_body").offCanvas('close');
    });

    $("#create_name_href").on("click", function() {
        if(current_user_account === "") {
            alert($("#please_login").html());
            return;
        }
        //$("#forward_article_id").val("0");
        //write_an_article_show_modal();
    });

    $("#my_names_href").on("click", function() {
        show_my_names();
    });

    //

    doc_scroll_top        = 0;
    current_page          = "name_market";
    page_control_init();
    hide_all_pages();
    $("#my_messages_div").hide();
    $("#name_market_div").show();

    setTimeout(
        function(){
            get_names_of_market(0, 1)
        }, 1000
    );
});
