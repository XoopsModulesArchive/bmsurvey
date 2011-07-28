<?php
include '../../../include/cp_header.php';
include_once('../class/bmsurveyUtils.php');

if(
	(!defined('XOOPS_ROOT_PATH')) || 
	(!is_object($xoopsUser)) || 
	(!$xoopsUser->isAdmin()) ){
	exit();
}

xoops_cp_header();
include 'adminmenu.php';
	echo "<div style='float: left; width:100%;'>";
	/*
	**	Question Section
	*/
	echo "<center>";
	$url = XOOPS_URL."/modules/".$xoopsModule->dirname()."/pop.php";
	echo "<a href='".$url."?admin=1'>"._AM_BMSURVEY_CHECKRESPONSENOW."</a>";
	echo "<p>";
	echo sprintf(_AM_BMSURVEY_CHECKRESPONSEUSAGE,$url);
	echo "</center>";
	xoops_cp_footer();
?>
