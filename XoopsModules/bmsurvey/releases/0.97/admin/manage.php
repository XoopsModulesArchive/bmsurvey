<?php
// $Id: manage.php,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $
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
// Original phpESP by James Flemer For eGrad2000.com <jflemer@alum.rpi.edu>
require('../../../mainfile.php');
include '../../../include/cp_header.php';
if(!$xoopsUser || !is_object($xoopsUser)){
	redirect_header(XOOPS_URL.'/modules/bmsurvey/',2,_MD_BMSURVEY_CAN_WRITE_USER_ONLY);
	exit();
}
include '../conf.php';
include_once('../class/bmsurveyUtils.php');
/*
** Get parameter as command.
*/
$where = '';
if(isset($_POST['where'])) $where = htmlspecialchars ( $_POST['where'] , ENT_QUOTES );
elseif(isset($_GET['where'])) $where = htmlspecialchars ( $_GET['where'] , ENT_QUOTES );
$sid = 0;
if(isset($_POST['sid'])) $sid = intval($_POST['sid']);
elseif(isset($_GET['sid'])) $sid = intval($_GET['sid']);


	if ($where == 'test' && $sid) 
		redirect_header(XOOPS_URL.'/modules/bmsurvey/manage.php' . );

	if (!defined('ESP_BASE'))
		define('ESP_BASE', dirname(dirname(__FILE__)) .'/');

 	$CONFIG = ESP_BASE . '/admin/phpESP.ini.php';

	$ESPCONFIG['csv_charset']  = bmsurveyUtils::getXoopsModuleConfig('CSV_CHARSET');

	if(!file_exists($CONFIG)) {
		echo("<b>FATAL: Unable to open $CONFIG. Aborting.</b>");
		exit;
	}
	if(!extension_loaded('mysql')) {
		echo('<b>FATAL: Mysql extension not loaded. Aborting.</b>');
		exit;
	}
	require_once($CONFIG);
	
	//esp_init_db();
	
	//session_register('acl');
	if(get_cfg_var('register_globals')) {
		$_SESSION['acl'] = &$acl;
	}
	if($ESPCONFIG['auth_design']) {
		if(!manage_auth(
				_addslashes(@$_SERVER['PHP_AUTH_USER']),
				_addslashes(@$_SERVER['PHP_AUTH_PW'])))
			exit;
	} else {
		$_SESSION['acl'] = array (
			'username'  => $xoopsUser->uid(),
			'pdesign'   => array('none'),
			'pdata'     => $member_handler->getGroupsByUser( $xoopsUser->uid() ),
			'pstatus'   => array('none'),
			'pall'      => $member_handler->getGroupsByUser( $xoopsUser->uid() ),
			'pgroup'    => array('none'),
			'puser'     => array('none'),
			'superuser' => 'Y',
			'disabled'  => 'N',
			'home'  => XOOPS_ROOT_PATH."/cache"
		);
		//$member_handler->getGroupsByUser( $xoopsUser->uid() )
	}
	$where = '';
	if(isset($_POST['where']))
		$where = $_POST['where'];
	elseif(isset($_GET['where']))
		$where = $_GET['where'];

	if ($where == 'download') {
		include(esp_where($where));
		exit;
	}else{
		xoops_cp_header();
		include 'adminmenu.php';
		echo "<div style='float: left; width:100%;'>";
	}
	if($ESPCONFIG['DEBUG']) {
		include($ESPCONFIG['include_path']."/debug".$ESPCONFIG['extension']);
	}

	if(!empty($ESPCONFIG['style_sheet'])) {
		echo("<link href=\"". $ESPCONFIG['style_sheet'] ."\" rel=\"stylesheet\" type=\"text/css\" />\n");
	}
	echo('<meta http-equiv="Content-Type" content="text/html; charset='. _CHARSET ."\" />\n");

	if(file_exists($ESPCONFIG['include_path']."/head".$ESPCONFIG['extension']))
		include($ESPCONFIG['include_path']."/head".$ESPCONFIG['extension']);

	// This is the body admin
	//echo "manage include:".esp_where($where);
	include(esp_where($where));
	//echo $_SESSION['survey_id'];
	if(file_exists($ESPCONFIG['include_path']."/foot".$ESPCONFIG['extension']))
		include($ESPCONFIG['include_path']."/foot".$ESPCONFIG['extension']);
	
	xoops_cp_footer();
?>
