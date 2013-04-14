(function ($) {

// beware: have to load it every time, not just only for admin ones
// as the overlay has the code in the parent window, which is ordinary node/nid page
	

/**
 * Reset the active class on links in displaced elements according to
 * given path.
 * 
 * Overloaded CUSTOM version
 * that adds checks for query string params, so project pages view that differentiate
 * just on the tid query string param are not highlighted en masse
 *
 * @param activePath
 *   Path to match links against.
 */

if (Drupal.overlay) { // just when in overlayParent, not for a overlayChild
Drupal.overlay.resetActiveClass = function(activePath) {
  var self = this;
  var windowDomain = window.location.protocol + window.location.hostname;

  $('.overlay-displace-top, .overlay-displace-bottom')
  .find('a[href]')
  // Remove active class from all links in displaced elements.
  .removeClass('active')
  // Add active class to links that match activePath.
  .each(function () {
    var linkDomain = this.protocol + this.hostname;
    var linkPath = self.getPath(this);

    // A link matches if it is part of the active trail of activePath, except
    // for frontpage links.
    if (linkDomain == windowDomain && (activePath + '/').indexOf(linkPath + '/') === 0 && (linkPath !== '' || activePath === '')) {
      // drupal projects modifications start
      if (this.search) {
        var overlay_url = '/' + $.bbq.getState('overlay');
        var link_url    = this.pathname + this.search; 
        if (overlay_url == link_url) {
          $(this).addClass('active');
        }
      } else {
        $(this).addClass('active');
      }
      //$(this).addClass('active');
      // drupal projects modifications end
    }
  });
};
//var match = new RegExp('([?&])q=(.+)([&#]|$)').exec(link.search);
//if (match && match.length == 4) {
//  path = match[2];
//}

}

})(jQuery);