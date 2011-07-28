<?php

# $Id: espauth.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

//  phpESP Authentication Library Chooser
//  By: James Flemer <jflemer@alum.rpi.edu>

	$auth_type = isset($GLOBALS['ESPCONFIG']['auth_type']) ? $GLOBALS['ESPCONFIG']['auth_type'] : "default";

	if (!file_exists($GLOBALS['ESPCONFIG']['include_path']."/lib/espauth-$auth_type".$ESPCONFIG['extension'])) {
		echo("<b>FATAL: Unable to set up authentication for type $auth_type. Aborting.</b>");
		exit;
	}

	require($ESPCONFIG['include_path']."/lib/espauth-$auth_type".$ESPCONFIG['extension']);

?>
