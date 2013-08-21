$().ready(function(){//显示隐藏
    $(".navbox .title").each(function(i, v){
        $(this).bind("click", function(){
            $(this).toggleClass("open");
            $($(".navbox .content")[i]).slideToggle("normal");
        })
    })
})

$().ready(function(){//折叠
    $("#fold").bind("click", function(){
        $("#fold a").toggleClass("close");
        $("#sidebar").animate({
            width: 'toggle',
            opacity: 'toggle'
        }, "fast", function(){
            $("#content").toggleClass("clear-margin");
        });
    })
})
