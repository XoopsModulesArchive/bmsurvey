<?php

# $Id: cancel.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	//session_register('survey_id');
	//session_register('survey_realm');
	if(ini_get('register_globals')) {
		$_SESSION['survey_id']    = &$survey_id;
		$_SESSION['survey_realm'] = &$survey_realm;
	}

	if(empty($_SESSION['survey_id'])) {
		// no survey INSERTed yet, so just go back to management interface
		$_SESSION['survey_id']    = '';
		$_SESSION['survey_realm'] = '';
		include(esp_where('index'));
		return;
	}

	$sql = "SELECT status FROM ".TABLE_SURVEY." WHERE	id='".$_SESSION['survey_id']."'";
	$result = mysql_query($sql);
	if(mysql_num_rows($result) < 1) {
		mysql_free_result($result);
		$_SESSION['survey_id']    = '';
		$_SESSION['survey_realm'] = '';
		include(esp_where('index'));
		return;
	}
	$status = mysql_result($result,0,0);
	mysql_free_result($result);
	$status |= STATUS_DELETED;
	$sql = "UPDATE ".TABLE_SURVEY." SET status='${status}' WHERE id='".$_SESSION['survey_id']."'";
	mysql_query($sql);
	$_SESSION['survey_id']    = '';
	$_SESSION['survey_realm'] = '';
	include(esp_where('index'));
	return;
?>
