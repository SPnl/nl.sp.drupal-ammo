(function ($) {

	$(document).ready(function(){
		$('.text-limit').each(function(){
			var index = $(this).html().lastIndexOf(' ', 300);
			if ($(this).html().length > 350) {
				$(this).html("<span>" + $(this).html().substr(0, index) + "</span><span style='display:none'>" + $(this).html().substr(index) + "</span> <span>... </span><a href='#' class='text-limit-toggle'>&#9658; Meer</a>"); 
		  }
		});

		$('.text-limit-toggle').bind('click',function(e){
			e.preventDefault();
			$(this).prev().prev().toggle();
			var html = $(this).html();
			$(this).html( html == "► Meer" ? "◄ Minder" : "► Meer"); 
			var html = $(this).prev().html();
			$(this).prev().html( html == "... " ? " " : "... "); 

		});
	}); // doc-ready

})(jQuery);
