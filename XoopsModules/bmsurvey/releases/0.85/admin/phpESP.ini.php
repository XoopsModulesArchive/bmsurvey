<?php
// # $Id$
//  ------------------------------------------------------------------------ //
//                Bluemoon.Multi-Survey                                      //
//                    Copyright (c) 2005 Yoshi.Sakai @ Bluemoon inc.         //
//                       <http://www.bluemooninc.biz/>                       //
// ------------------------------------------------------------------------- //
//  This program is free software; you can redistribute it and/or modify     //
//  it under the terms of the GNU General Public License as published by     //
//  the Free Software Foundation; either version 2 of the License, or        //
//  (at your option) any later version.                                      //
//                                                                           //
//  You may not change or alter any portion of this comment or credits       //
//  of supporting developers from this source code or any supporting         //
//  source code which is considered copyrighted (c) material of the          //
//  original comment or credit authors.                                      //
//                                                                           //
//  This program is distributed in the hope that it will be useful,          //
//  but WITHOUT ANY WARRANTY; without even the implied warranty of           //
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
//  GNU General Public License for more details.                             //
//                                                                           //
//  You should have received a copy of the GNU General Public License        //
//  along with this program; if not, write to the Free Software              //
//  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA //
//  ------------------------------------------------------------------------ //
// Original Written by James Flemer For eGrad2000.com <jflemer@alum.rpi.edu>
//
// Usage : Must have gettext libraly. mb-functions avaiable.
//
// Name of application
$ESPCONFIG['name'] = 'BM-Survey';
// Application version
$ESPCONFIG['version'] = 'v0.8 Irene';
/*
** Here are all the configuration options.
*/
// Base URL for phpESP
// The string $server['HTTP_HOST'] will be replaced by the server name
$ESPCONFIG['base_url'] = XOOPS_URL.'/modules/bmsurvey/';

// URL of the images directory (for <img src='...'> tags)
$ESPCONFIG['image_url'] = $ESPCONFIG['base_url'] . 'images/';

// URL of the automatic survey publisher
$ESPCONFIG['autopub_url'] = $ESPCONFIG['base_url'] . 'public/survey.php';

// URL of the CSS directory (for themes)
$ESPCONFIG['css_url'] = $ESPCONFIG['base_url'] . 'public/css/';

//
// Upload file Section
//

// Upload Folder. It work with XOOPS_ROOT_PATH or XOOPS_URL
$ESPCONFIG['attach_path'] = '/uploads/';

// Acceptable MIME Type
$ESPCONFIG['subtype'] = "gif|jpe?g|png|bmp|zip|lzh|pdf|excel|powerpoint|octet-stream|x-pmd|x-mld|x-mid|x-smd|x-smaf|x-mpeg";

// embedding image MIME Content-Type
$ESPCONFIG['imgtype'] = "gif|jpe?g|png|bmp|x-pmd|x-mld|x-mid|x-smd|x-smaf|x-mpeg";
	
// Reject Ext. name
$ESPCONFIG['viri'] = "cgi|php|jsp|pl|htm";


if (!defined('ESP_BASE')) define('ESP_BASE', dirname(dirname(__FILE__)) .'/');
if (isset($_SERVER))  $server =& $_SERVER;
else                  $server =& $_SERVER;

// Allow phpESP to send email 
$ESPCONFIG['allow_email'] =  1;		// 0=No mail 1=send responce mail
//$ESPCONFIG['special_addr'] = "";	// Work with 'allow_email'=7
//$ESPCONFIG['special_char'] = "b1,";	// Add a spesial char for mail subject. (Work with 'allow_email'=7)

// Send human readable email, rather than machine readable (BOOLEAN)
$ESPCONFIG['human_email'] = true;

// Use authentication for designer interface (BOOLEAN)
$ESPCONFIG['auth_design'] = false;

// Use authentication for survey responders (BOOLEAN)
$ESPCONFIG['auth_response'] = true;

// Choose authentication type: { 'default', 'ldap' }
$ESPCONFIG['auth_type'] = 'default';

// LDAP connection information
// (Set these values if you choose 'ldap' as the authentication type.)
$ESPCONFIG['ldap_server'] = 'ldap.example.com';
$ESPCONFIG['ldap_port']   = '389';
$ESPCONFIG['ldap_dn']     = 'dc=example,dc=com';
$ESPCONFIG['ldap_filter'] = 'uid=';

// Group to add responders to via the sign-up page
// (Set to "null", without quotes, to disable the sign-up page.)
$ESPCONFIG['signup_realm'] = 'auto';
$ESPCONFIG['anonymousname'] = 1;	// 0 = 'Anonymous' / 1 = IP Address

// Default number of option lines for new questions
$ESPCONFIG['default_num_choices'] = 10;

// Colors used by phpESP
$ESPCONFIG['main_bgcolor']      = '#FFFFFF';
$ESPCONFIG['link_color']        = '#0000CC';
$ESPCONFIG['vlink_color']       = '#0000CC';
$ESPCONFIG['alink_color']       = '#0000CC';
$ESPCONFIG['table_bgcolor']     = '#0099FF';
$ESPCONFIG['active_bgcolor']    = '#FFFFFF';
$ESPCONFIG['dim_bgcolor']       = '#3399CC';
$ESPCONFIG['error_color']       = '#FF0000';
$ESPCONFIG['warn_color']        = '#00FF00';
$ESPCONFIG['reqd_color']        = '#FF00FF';
$ESPCONFIG['bgalt_color1']      = '#FFFFFF';
$ESPCONFIG['bgalt_color2']      = '#EEEEEE';

/*******************************************************************
 * Most users will not need to change anything below this line.    *
 *******************************************************************/

// Enable debugging code (BOOLEAN)
$ESPCONFIG['DEBUG'] = false;

// Extension of support files
$ESPCONFIG['extension'] = '.inc';

// Survey handler to use
$ESPCONFIG['handler']        = ESP_BASE . 'public/handler.php';
$ESPCONFIG['handler_prefix'] = ESP_BASE . 'public/handler-prefix.php';

// Valid tabs when editing surveys
$ESPCONFIG['tabs'] = array('general', 'questions', 'preview', 'order', 'finish');

// Copy of PHP_SELF for later use
$ESPCONFIG['ME'] = $server['PHP_SELF'];

// CSS stylesheet to use for designer interface
$ESPCONFIG['style_sheet'] = null;

// Status of gettext extension
$ESPCONFIG['gettext'] = extension_loaded('gettext');

// HTML page title
$ESPCONFIG['title'] = $ESPCONFIG['name'] .', v'. $ESPCONFIG['version'];

// phpESP include path
$ESPCONFIG['include_path'] = ESP_BASE . '/admin/include/';

// phpESP css path
$ESPCONFIG['css_path'] = ESP_BASE . '/public/css/';

// phpESP locale path
$ESPCONFIG['locale_path'] = ESP_BASE . '/locale/';

// Load I18N support
// i18n language set
switch (_LANGCODE){
	case "da": $ESPCONFIG['default_lang'] = 'da_DK'; $ESPCONFIG['mail_charset'] = 'ISO-8859-1'; break;
	case "de": $ESPCONFIG['default_lang'] = 'da_DE'; $ESPCONFIG['mail_charset'] = 'ISO-8859-1'; break;
	case "el": $ESPCONFIG['default_lang'] = 'el_GR'; $ESPCONFIG['mail_charset'] = 'ISO-8859-7'; break;
	case "en": $ESPCONFIG['default_lang'] = 'en_US'; $ESPCONFIG['mail_charset'] = 'us-ascii'; break;
	case "es": $ESPCONFIG['default_lang'] = 'es_ES'; $ESPCONFIG['mail_charset'] = 'ISO-8859-1'; break;
	case "fr": $ESPCONFIG['default_lang'] = 'fr_FR'; $ESPCONFIG['mail_charset'] = 'ISO-8859-1'; break;
	case "it": $ESPCONFIG['default_lang'] = 'it_IT'; $ESPCONFIG['mail_charset'] = 'ISO-8859-1'; break;
	case "ja": $ESPCONFIG['default_lang'] = 'ja_JP'; $ESPCONFIG['mail_charset'] = 'iso-2022-jp'; break;
	case "nl": $ESPCONFIG['default_lang'] = 'nl_NL'; $ESPCONFIG['mail_charset'] = 'ISO-8859-1'; break;
	case "pt": $ESPCONFIG['default_lang'] = 'pt_PT'; $ESPCONFIG['mail_charset'] = 'ISO-8859-1'; break;
	case "sv": $ESPCONFIG['default_lang'] = 'sv_SE'; $ESPCONFIG['mail_charset'] = 'ISO-8859-1'; break;
}
require_once($ESPCONFIG['include_path'] . '/lib/espi18n' . $ESPCONFIG['extension']);
esp_setlocale_ex($ESPCONFIG['default_lang']);

if (!file_exists($ESPCONFIG['include_path']. '/funcs'. $ESPCONFIG['extension'])) {
    printf('<b>'. mb_('Unable to find the phpESP %s directory.
			Please check %s to ensure that all paths are set correctly.') .
			'</b>', 'include', 'phpESP.ini.php');
    exit;
}
if (!file_exists($ESPCONFIG['css_path'])) {
    printf('<b>'. mb_('Unable to find the phpESP %s directory.
			Please check %s to ensure that all paths are set correctly.') .
			'</b>', 'css', 'phpESP.ini.php');
    exit;
}

if (isset($GLOBALS)) {
    $GLOBALS['ESPCONFIG'] = $ESPCONFIG;
} else {
    global $ESPCONFIG;
}

require_once($ESPCONFIG['include_path'].'/funcs'.$ESPCONFIG['extension']);
?>
