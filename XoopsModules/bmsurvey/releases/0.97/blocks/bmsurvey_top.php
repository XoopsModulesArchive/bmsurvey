<?php
// $Id$
if(!defined('XOOPS_ROOT_PATH')){
	exit();
}

include_once XOOPS_ROOT_PATH.'/modules/bmsurvey/class/bmsurveyUtils.php';

function b_bmsurvey_wait_appl($options){
	global $xoopsUser;
	$result = array();
	bmsurveyUtils::assign_message($result);
	if($xoopsUser && ($xoopsUser->isAdmin())){
		$result['bmsurvey_applicationNum'] = bmsurveyUtils::getApplicationNum();	
	}
	return $result;
}

function b_bmsurvey_show($options) {
	global $xoopsUser;
	$result = array();
	bmsurveyUtils::assign_message($result);
	$result['survey'] = bmsurveyUtils::get_survey_list();
	/*
	$result['show_rss'] = ($options[0] == 1) ? 1 : 0;
	$result['blogTitle'] = _MB_POPNUPBLOG_BLOG_TITLE;
	$result['unameTitle'] = _MB_POPNUPBLOG_BLOGGER_NAME;
	$result['lastUpdateTitle'] = _MB_POPNUPBLOG_UPDATE_DATE;
	*/
	return $result;
}

function b_bmsurvey_edit($options){
	$checked = array();
	$checked[0] = ($options[0] == 1) ? ' selected' : '';
	$checked[1] = ($checked[0] == '') ? ' selected' : '';
	$form = '';
	/*
	$form .= _MB_POPNUPBLOG_SHOW_RSS_LINK." :";
	$form .= "<select name='options[0]'>\n";
	$form .= "<option value='1'".$checked[0].">"._MB_POPNUPBLOG_YES."</option>\n";
	$form .= "<option value='0'".$checked[1].">"._MB_POPNUPBLOG_NO."</option>\n";
	$form .= "</select>\n";
	*/
	return $form;
}
?>
