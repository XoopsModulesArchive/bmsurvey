<?php
// $Id$
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
/*-----------------*/
include_once('../../mainfile.php');
include_once('pop.ini.php');
include_once('./class/bmsurveyUtils.php');
/*-----------------*/
include_once('./admin/include/function/survey_render.inc');
include_once('./admin/include/lib/espsql.inc');
include_once('./admin/include/lib/esphtml.forms.inc');

$debug = 1;	// When you debugging this source set to 1, 0 is off.

global $xoopsDB, $_GET;

//
// Check _GET
//
if (!isset($_GET['key']) || !isset($_GET['sid'])){
	echo "Usage:<br> cast.php?key=[keycode]&sid=[Survey ID]<br>";
	echo "ex)cast.php?key=111&sid=1\n";
	return false;
}
//
// Check cast_key
//
$key = $_GET['key'];
$cast_key = bmsurveyUtils::getXoopsModuleConfig('CAST_KEY');
$add_usage = bmsurveyUtils::getXoopsModuleConfig('ADD_USAGE');
$cast_from = bmsurveyUtils::getXoopsModuleConfig('MAILADDR');

if(!$cast_key){
	echo "Set Key code at preferences!";
	exit;
}
if(!eregi($cast_key,$key)){
	echo "Key code error!";
	exit;
}
if(!$cast_from){
	$cast_from=$xoopsConfig['adminmail'];
}
//
// Check Parameter
//
$sid = $_GET['sid'];
if (preg_match("/^([0-9])/",$sid)){
	$wstr = "and survey_id=".$sid." ";
}elseif(eregi("all",$sid)){
	$wstr = "";
}else{
	echo "Survey id error!";
	return false;
}
//
// Get Respondent rec
//
if (isset($_GET['username'])) {
	$wstr = "and username='".htmlspecialchars( $_GET['username'], ENT_QUOTES )."' ";
}
$sql = 'select username,email,survey_id from '.TABLE_RESPONDENT.' where disabled=2 '.$wstr.' order by survey_id asc';
if(!$db_result = $xoopsDB->query($sql)){
	return false;
}
//
// Send Mail to Respondent
//
$i = 0;
$cur_sid=0;
srand(time());
$hide_question = isset($_GET['hide']) ? $_GET['hide'] : 0;
while(list($uname, $mailto, $sid) = $xoopsDB->fetchRow($db_result)){
	if($cur_sid<>$sid){
		$ret = survey_render_email($sid,1,'',$hide_question);
		//$body = str_replace("\r\n", "<br>", $body);
		//echo $sid.$body;
		$cur_sid=$sid;
	}
	$ticket = rand(10, 99).rand(10, 99);
	if ($ret){
		$body = $add_usage ? _MD_POP_QINP_HEADER.$ret['body']: $ret['body'];
		cast_mail($uname, $mailto, $sid, $ret['title'], $body, $ticket,$cast_from);
		$sql = 'UPDATE '.TABLE_RESPONDENT.' SET response_id=0, password="'.$ticket.'" WHERE username="'.$uname.'"';
		//echo $sql;
		$xoopsDB->queryF($sql);
	}
}
//
// Return to admin
//
$admin = isset($_GET['admin']) ? $_GET['admin'] : 0;
if ($admin)
	redirect_header($SurveyCNF['admin'],3,"Survey sent.");

/*
** Display form body. Call Render with Smarty
*/
function cast_mail($username,$mailto,$sid,$title,$body,$ticket,$cast_from) {
	global $xoopsConfig;

	$subj = "Q,INP;".$title;
	$xoopsMailer =& getMailer();
	$xoopsMailer->useMail();
	$xoopsMailer->setTemplateDir(XOOPS_ROOT_PATH."/modules/bmsurvey/language/".$xoopsConfig['language']."/mail_template/");
	$xoopsMailer->setTemplate("mail_cast.tpl");
	$xoopsMailer->setToEmails($mailto);
	$xoopsMailer->assign('USERNAME', $username);
	$xoopsMailer->assign('SURVEYID', $sid);
	$xoopsMailer->assign('TICKET', $ticket);
	$xoopsMailer->assign('FORMBODY',$subj . "\r\n" . $body);
	$xoopsMailer->assign("SITENAME", $xoopsConfig['sitename']);
	$xoopsMailer->assign("ADMINMAIL", $xoopsConfig['adminmail']);
	$xoopsMailer->assign("SITEURL", XOOPS_URL);
	$xoopsMailer->setFromEmail($cast_from);
	$xoopsMailer->setFromName($xoopsConfig['sitename']);
	if ( function_exists('mb_encode_mimeheader') )
		$subj = mb_encode_mimeheader( $subj, bmsurveyUtils::get_mailcode(), "B" );
	$xoopsMailer->setSubject($subj);
	if (!$xoopsMailer->send()) {
		echo $xoopsMailer->getErrors();
		return false;
	}
}
?>
