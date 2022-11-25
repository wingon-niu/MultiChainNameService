
// utf8编码

function page_control_init()
{
    $("#name_market").on("click", function() {
        show_name_market();
    });

    $("#my_names").on("click", function() {
        show_my_names();
    });

    $("#names_of_my_bidding").on("click", function() {
        show_names_of_my_bidding();
    });

    $("#system_statistics_info").on("click", function() {
        show_system_statistics_info();
    });
}

function hide_all_pages()
{
    $("#name_market_div").hide();
    $("#my_names_div").hide();
    $("#names_of_my_bidding_div").hide();
    $("#system_statistics_info_div").hide();
}

function show_name_market()
{
    $("#menu_body").offCanvas('close');
    hide_all_pages();
    current_page = "name_market";
    $("#name_market_div").show();
    get_names_of_market(0, 1);
}

function show_my_names()
{
    $("#menu_body").offCanvas('close');
    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }
    hide_all_pages();
    current_page = "my_names";
    $("#my_names_div").show();
    get_my_names(current_user_account);
}

function show_names_of_my_bidding()
{
    $("#menu_body").offCanvas('close');
    if(current_user_account === "") {
        alert($("#please_login").html());
        return;
    }
    hide_all_pages();
    current_page = "names_of_my_bidding";
    $("#names_of_my_bidding_div").show();
    get_names_of_my_bidding(current_user_account);
}

function show_system_statistics_info()
{
    $("#menu_body").offCanvas('close');
    hide_all_pages();
    current_page = "system_statistics_info";
    $("#system_statistics_info_div").show();
    get_system_statistics_info();
}
