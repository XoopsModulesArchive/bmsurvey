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
include '../../../include/cp_header.php';
if(
	(!defined('XOOPS_ROOT_PATH')) || 
	(!is_object($xoopsUser)) || 
	(!$xoopsUser->isAdmin()) ){
	exit();
}
xoops_cp_header();
checkPermit();

include 'adminmenu.php';

$url = XOOPS_URL."/modules/".$xoopsModule->dirname()."/language/".$xoopsConfig['language']."/";
echo '<div style="float:left; width:100%;">';
echo "<li><a href='".$url."doc_updateinfo.php'><span>"._AM_BMSURVEY_DOC_UPDATEINFO."</span></a></li>";
echo "<p>";
echo "<li><a href='".$url."doc_popnupblog.php'><span>"._AM_BMSURVEY_DOC_POPNUPBLOG."</span></a></li>";
echo "<p>";
echo "<li><a href='".$url."doc_mailto.php'><span>"._AM_BMSURVEY_DOC_MAILTO."</span></a></li>";
echo '</div>';

//include 'test.php';

xoops_cp_footer();

function checkPermit(){
	global $xoopsModule;

	$modpath = XOOPS_ROOT_PATH."/modules/".$xoopsModule->dirname();
	$permit_err = array();
	$_check_list = array(
		XOOPS_ROOT_PATH."/uploads/bmsurvey/",
		XOOPS_ROOT_PATH."/uploads/bmsurvey/thumbs",
		$modpath."/log/"
		);

	if ($dir = @opendir($modpath."/log/")) {
		while($file = readdir($dir)) {
			if($file == ".." || $file == "." || eregi("^\.(.*)",$file)) continue;
			array_push($_check_list, $modpath."/log/".$file);
		}
		closedir($dir);
	}

	foreach($_check_list as $dir){
		if(!is_writable($dir)){
			$permit_err[] = _AM_BMSURVEY_ERROR01."=> ".$dir;
		}
	}

	$_alert_icon = "<img src='../images/alert.gif'>&nbsp;</img>";
//	$_alert_icon = "<img src='../caution.gif' height='15' width='50'>&nbsp;";
	foreach($permit_err as $er_msg){
//		echo "<img src='$_alert_icon' height='15' width='50'>&nbsp;$er_msg<br />";
		echo "$_alert_icon$er_msg<br />";
	}
}
