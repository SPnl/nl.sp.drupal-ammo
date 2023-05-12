(function ($) {

  Drupal.behaviors.ammo = {
    attach: function (context, settings) {

        $('.tablinks').bind('click',function(e){
          e.preventDefault();
          $('.tabcontent').removeClass('active');
          $('.tablinks.active').removeClass('active');
          $(this).addClass('active');
          var tabId = $(this).attr('Id');
          var contentId = '#tabcontent-' + tabId.substring(4);
          $(contentId).addClass('active');
        });

    }
  };

})(jQuery);
