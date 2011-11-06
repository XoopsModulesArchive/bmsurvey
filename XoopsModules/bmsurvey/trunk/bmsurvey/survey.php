<?php
# $Id$

// Matthew Gregg
// <greggmc at musc.edu>
require('../../mainfile.php');
require(XOOPS_ROOT_PATH.'/header.php');
require('./conf.php');
include_once('./class/bmsurveyUtils.php');
if($xoopsTpl){
	bmsurveyUtils::assign_message($xoopsTpl);
}
	if (!defined('ESP_BASE')){
		$dirname = $xoopsModule->getvar("dirname");
		define('ESP_BASE', XOOPS_ROOT_PATH.'/modules/'.$dirname.'/');
	}
	$CONFIG = ESP_BASE . 'admin/phpESP.ini.php';
	if(!file_exists($CONFIG)) {
		echo("<b>FATAL: Unable to open config file Aborting.</b>");
		exit;
	}
	if(!extension_loaded('mysql')) {
		echo('<b>FATAL: Mysql extension not loaded. Aborting.</b>');
		exit;
	}
	require_once($CONFIG);	
	
//	esp_init_db();	
	
	$_name = '';
	$_title = '';
	$_css = '';
	if (isset($_GET['name'])) {
		$_name = _addslashes($_GET['name']);
		unset($_GET['name']);
		$_SERVER['QUERY_STRING'] =
			ereg_replace('(^|&)name=[^&]*&?', '', $_SERVER['QUERY_STRING']);
	}
	if (!empty($_name)) {
        $_sql = "SELECT id,title,theme FROM ".TABLE_SURVEY." WHERE name = '$_name'";
        if ($_result = mysql_query($_sql)) {
           	if (mysql_num_rows($_result) > 0) list($sid, $_title, $_css) = mysql_fetch_row($_result);
           	mysql_free_result($_result);
        }
       	unset($_sql);
       	unset($_result);
	}

    // call the handler-prefix once $sid is set to handle
    // authentication / authorization
	include($ESPCONFIG['handler_prefix']);

	if (empty($_name) && isset($sid) && $sid) {
        $_sql = "SELECT title,theme FROM ".TABLE_SURVEY." WHERE id = '$sid'";
        if ($_result = mysql_query($_sql)) {
            if (mysql_num_rows($_result) > 0){
                list($_title, $_css) = mysql_fetch_row($_result);
            }
            mysql_free_result($_result);
        }
        unset($_sql);
        unset($_result);
	}

?>
<?php
/*
    if (!empty($_css)) {
	    echo('<link rel="stylesheet" href="'. $GLOBALS['ESPCONFIG']['css_url'].$_css ."\" type=\"text/css\">\n");
    }
    unset($_css);
*/
?>
<?php
	unset($_name);
	unset($_title);

	include($ESPCONFIG['handler']);
	$xoopsOption['template_main'] = 'bmsurvey_survey.html';

	include(XOOPS_ROOT_PATH.'/footer.php');
?>
