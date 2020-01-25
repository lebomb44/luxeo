mw.loader.implement("jquery.checkboxShiftClick",function($){(function($){$.fn.checkboxShiftClick=function(text){var prevCheckbox=null;var $box=this;$box.click(function(e){if(prevCheckbox!==null&&e.shiftKey){$box.slice(Math.min($box.index(prevCheckbox),$box.index(e.target)),Math.max($box.index(prevCheckbox),$box.index(e.target))+1).prop('checked',e.target.checked?true:false);}prevCheckbox=e.target;});return $box;};})(jQuery);;},{},{});mw.loader.implement("jquery.makeCollapsible",function($){(function($,mw){$.fn.makeCollapsible=function(){return this.each(function(){var _fn='jquery.makeCollapsible> ';var $that=$(this).addClass('mw-collapsible'),that=this,collapsetext=$(this).attr('data-collapsetext'),expandtext=$(this).attr('data-expandtext'),toggleElement=function($collapsible,action,$defaultToggle,instantHide){if(!$collapsible.jquery){return;}if(action!='expand'&&action!='collapse'){return;}if(typeof $defaultToggle=='undefined'){$defaultToggle=null;}if($defaultToggle!==null&&!(
$defaultToggle instanceof $)){return;}var $containers=null;if(action=='collapse'){if($collapsible.is('table')){$containers=$collapsible.find('>tbody>tr');if($defaultToggle){$containers.not($defaultToggle.closest('tr')).stop(true,true).fadeOut();}else{if(instantHide){$containers.hide();}else{$containers.stop(true,true).fadeOut();}}}else if($collapsible.is('ul')||$collapsible.is('ol')){$containers=$collapsible.find('> li');if($defaultToggle){$containers.not($defaultToggle.parent()).stop(true,true).slideUp();}else{if(instantHide){$containers.hide();}else{$containers.stop(true,true).slideUp();}}}else{var $collapsibleContent=$collapsible.find('> .mw-collapsible-content');if($collapsibleContent.length){if(instantHide){$collapsibleContent.hide();}else{$collapsibleContent.slideUp();}}else{if($collapsible.is('tr')||$collapsible.is('td')||$collapsible.is('th')){$collapsible.fadeOut();}else{$collapsible.slideUp();}}}}else{if($collapsible.is('table')){$containers=$collapsible.find('>tbody>tr');if(
$defaultToggle){$containers.not($defaultToggle.parent().parent()).stop(true,true).fadeIn();}else{$containers.stop(true,true).fadeIn();}}else if($collapsible.is('ul')||$collapsible.is('ol')){$containers=$collapsible.find('> li');if($defaultToggle){$containers.not($defaultToggle.parent()).stop(true,true).slideDown();}else{$containers.stop(true,true).slideDown();}}else{var $collapsibleContent=$collapsible.find('> .mw-collapsible-content');if($collapsibleContent.length){$collapsibleContent.slideDown();}else{if($collapsible.is('tr')||$collapsible.is('td')||$collapsible.is('th')){$collapsible.fadeIn();}else{$collapsible.slideDown();}}}}},toggleLinkDefault=function(that,e){var $that=$(that),$collapsible=$that.closest('.mw-collapsible.mw-made-collapsible').toggleClass('mw-collapsed');e.preventDefault();e.stopPropagation();if(!$that.hasClass('mw-collapsible-toggle-collapsed')){$that.removeClass('mw-collapsible-toggle-expanded').addClass('mw-collapsible-toggle-collapsed');if($that.find('> a').
length){$that.find('> a').text(expandtext);}else{$that.text(expandtext);}toggleElement($collapsible,'collapse',$that);}else{$that.removeClass('mw-collapsible-toggle-collapsed').addClass('mw-collapsible-toggle-expanded');if($that.find('> a').length){$that.find('> a').text(collapsetext);}else{$that.text(collapsetext);}toggleElement($collapsible,'expand',$that);}return;},toggleLinkPremade=function($that,e){var $collapsible=$that.eq(0).closest('.mw-collapsible.mw-made-collapsible').toggleClass('mw-collapsed');if($(e.target).is('a')){return true;}e.preventDefault();e.stopPropagation();if(!$that.hasClass('mw-collapsible-toggle-collapsed')){$that.removeClass('mw-collapsible-toggle-expanded').addClass('mw-collapsible-toggle-collapsed');toggleElement($collapsible,'collapse',$that);}else{$that.removeClass('mw-collapsible-toggle-collapsed').addClass('mw-collapsible-toggle-expanded');toggleElement($collapsible,'expand',$that);}return;},toggleLinkCustom=function($that,e,$collapsible){if(e){e.
preventDefault();e.stopPropagation();}var action=$collapsible.hasClass('mw-collapsed')?'expand':'collapse';$collapsible.toggleClass('mw-collapsed');toggleElement($collapsible,action,$that);};if(!collapsetext){collapsetext=mw.msg('collapsible-collapse');}if(!expandtext){expandtext=mw.msg('collapsible-expand');}var $toggleLink=$('<a href="#"></a>').text(collapsetext).wrap('<span class="mw-collapsible-toggle"></span>').parent().prepend('&nbsp;[').append(']&nbsp;').bind('click.mw-collapse',function(e){toggleLinkDefault(this,e);});if($that.hasClass('mw-made-collapsible')){return;}else{$that.addClass('mw-made-collapsible');}if(($that.attr('id')||'').indexOf('mw-customcollapsible-')===0){var thatId=$that.attr('id'),$customTogglers=$('.'+thatId.replace('mw-customcollapsible','mw-customtoggle'));mw.log(_fn+'Found custom collapsible: #'+thatId);if($customTogglers.length){$customTogglers.bind('click.mw-collapse',function(e){toggleLinkCustom($(this),e,$that);});}else{mw.log(_fn+'#'+thatId+
': Missing toggler!');}if($that.hasClass('mw-collapsed')){$that.removeClass('mw-collapsed');toggleLinkCustom($customTogglers,null,$that);}}else{if($that.is('table')){var $firstRowCells=$('tr:first th, tr:first td',that),$toggle=$firstRowCells.find('> .mw-collapsible-toggle');if(!$toggle.length){$firstRowCells.eq(-1).prepend($toggleLink);}else{$toggleLink=$toggle.unbind('click.mw-collapse').bind('click.mw-collapse',function(e){toggleLinkPremade($toggle,e);});}}else if($that.is('ul')||$that.is('ol')){var $firstItem=$('li:first',$that),$toggle=$firstItem.find('> .mw-collapsible-toggle');if(!$toggle.length){var firstval=$firstItem.attr('value');if(firstval===undefined||!firstval||firstval=='-1'){$firstItem.attr('value','1');}$that.prepend($toggleLink.wrap('<li class="mw-collapsible-toggle-li"></li>').parent());}else{$toggleLink=$toggle.unbind('click.mw-collapse').bind('click.mw-collapse',function(e){toggleLinkPremade($toggle,e);});}}else{var $toggle=$that.find('> .mw-collapsible-toggle');
if(!$that.find('> .mw-collapsible-content').length){$that.wrapInner('<div class="mw-collapsible-content"></div>');}if(!$toggle.length){$that.prepend($toggleLink);}else{$toggleLink=$toggle.unbind('click.mw-collapse').bind('click.mw-collapse',function(e){toggleLinkPremade($toggle,e);});}}}if($that.hasClass('mw-collapsed')&&($that.attr('id')||'').indexOf('mw-customcollapsible-')!==0){$that.removeClass('mw-collapsed');toggleElement($that,'collapse',$toggleLink.eq(0),true);$toggleLink.eq(0).click();}});};})(jQuery,mediaWiki);;},{"all":".mw-collapsible-toggle{float:right} li .mw-collapsible-toggle{float:none} .mw-collapsible-toggle-li{list-style:none}\n\n/* cache key: nfctools-wiki-db:resourceloader:filter:minify-css:7:4250852ed2349a0d4d0fc6509a3e7d4c */\n"},{"collapsible-expand":"Expand","collapsible-collapse":"Collapse"});mw.loader.implement("jquery.mw-jump",function($){jQuery(function($){$('.mw-jump').delegate('a','focus blur',function(e){if(e.type==="blur"||e.type==="focusout"){$(this).
closest('.mw-jump').css({height:'0'});}else{$(this).closest('.mw-jump').css({height:'auto'});}});});;},{},{});mw.loader.implement("jquery.placeholder",function($){(function($){$.fn.placeholder=function(){return this.each(function(){if(this.placeholder&&'placeholder'in document.createElement(this.tagName)){return;}var placeholder=this.getAttribute('placeholder');var $input=$(this);if(this.value===''||this.value===placeholder){$input.addClass('placeholder').val(placeholder);}$input.blur(function(){if(this.value===''){this.value=placeholder;$input.addClass('placeholder');}}).bind('focus drop keydown paste',function(e){if($input.hasClass('placeholder')){if(e.type=='drop'&&e.originalEvent.dataTransfer){try{this.value=e.originalEvent.dataTransfer.getData('text/plain');}catch(exception){this.value=e.originalEvent.dataTransfer.getData('text');}e.preventDefault();}else{this.value='';}$input.removeClass('placeholder');}});if(this.form){$(this.form).submit(function(){if($input.hasClass(
'placeholder')){$input.val('').removeClass('placeholder');}});}});};})(jQuery);;},{},{});mw.loader.implement("mediawiki.page.ready",function($){jQuery(document).ready(function($){if(!('placeholder'in document.createElement('input'))){$('input[placeholder]').placeholder();}$('.mw-collapsible').makeCollapsible();if($('table.sortable').length){mw.loader.using('jquery.tablesorter',function(){$('table.sortable').tablesorter();});}$('input[type=checkbox]:not(.noshiftselect)').checkboxShiftClick();mw.util.updateTooltipAccessKeys();});;},{},{});mw.loader.implement("mediawiki.user",function($){(function($){function User(options,tokens){var that=this;this.options=options||new mw.Map();this.tokens=tokens||new mw.Map();function generateId(){var id='';var seed='0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz';for(var i=0,r;i<32;i++){r=Math.floor(Math.random()*seed.length);id+=seed.substring(r,r+1);}return id;}this.name=function(){return mw.config.get('wgUserName');};this.anonymous=
function(){return that.name()?false:true;};this.sessionId=function(){var sessionId=$.cookie('mediaWiki.user.sessionId');if(typeof sessionId=='undefined'||sessionId===null){sessionId=generateId();$.cookie('mediaWiki.user.sessionId',sessionId,{'expires':null,'path':'/'});}return sessionId;};this.id=function(){var name=that.name();if(name){return name;}var id=$.cookie('mediaWiki.user.id');if(typeof id=='undefined'||id===null){id=generateId();}$.cookie('mediaWiki.user.id',id,{'expires':365,'path':'/'});return id;};this.bucket=function(key,options){options=$.extend({'buckets':{},'version':0,'tracked':false,'expires':30},options||{});var cookie=$.cookie('mediaWiki.user.bucket:'+key);var bucket=null;var version=0;if(typeof cookie==='string'&&cookie.length>2&&cookie.indexOf(':')>0){var parts=cookie.split(':');if(parts.length>1&&parts[0]==options.version){version=Number(parts[0]);bucket=String(parts[1]);}}if(bucket===null){if(!$.isPlainObject(options.buckets)){throw'Invalid buckets error. Object expected for options.buckets.'
;}version=Number(options.version);var range=0,k;for(k in options.buckets){range+=options.buckets[k];}var rand=Math.random()*range;var total=0;for(k in options.buckets){bucket=k;total+=options.buckets[k];if(total>=rand){break;}}if(options.tracked){mw.loader.using('jquery.clickTracking',function(){$.trackAction('mediaWiki.user.bucket:'+key+'@'+version+':'+bucket);});}$.cookie('mediaWiki.user.bucket:'+key,version+':'+bucket,{'path':'/','expires':Number(options.expires)});}return bucket;};}mw.user=new User(mw.user.options,mw.user.tokens);})(jQuery);;},{},{});

/* cache key: nfctools-wiki-db:resourceloader:filter:minify-js:7:03f66ae45dc581697c081e354fd25198 */
