
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

    $("#write_an_article_href").on("click", function() {
        if(current_user_account === "") {
            alert($("#please_login").html());
            return;
        }
        $("#forward_article_id").val("0");
        write_an_article_show_modal();
    });

    //

    $("#my_messages_div").hide();
    page_control_init();

    setTimeout(
        function(){
            //get_home_page_articles()
        }, 1000
    );
});
