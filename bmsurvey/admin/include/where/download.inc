<?php

# $Id: download.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Download patch submitted by Matthew Gregg <greggmc@musc.edu>
// 2003/01/30 Modified by James Flemer <jflemer@alum.rpi.edu>
// 2005/05/22 XOOPS version by Yoshi Sakai <webmaster@bluemooninc.biz>

	$sid = -1;
	if (!empty($_GET['sid']))
		$sid = intval($_GET['sid']);
	else if(!empty($_POST['sid']))
		$sid = intval($_POST['sid']);

	$sql = "SELECT name FROM ".TABLE_SURVEY." WHERE id = $sid";
	$result = mysql_query($sql);
	if (mysql_num_rows($result) < 1) {
		mysql_free_result($result);
		echo mkerror(_('Invalid survey ID.'));
		return;
	}
	$name = mysql_result($result, 0, 0);
	mysql_free_result($result);

	/* check ACLs for permissions */
	$srealm = auth_get_survey_realm($sid);
	if (isset($_GET['test'])) {
		/* check ACL to see if user is allowed to test
		 * _this_ survey */
		if($_SESSION['acl']['superuser'] != 'Y' &&
				!auth_is_owner($sid, $xoopsUser->uid()) &&
				!in_array($srealm, array_intersect(
						$_SESSION['acl']['pdesign'],
						$_SESSION['acl']['pall'])) &&
				!auth_no_access(_('to access this survey'))) {
			return;
		}
	} else {
		/* check ACL to see if user is allowed to export
		 * _this_ survey */
		if($_SESSION['acl']['superuser'] != 'Y' &&
				!auth_is_owner($sid, $xoopsUser->uid()) &&
				!in_array($srealm, array_intersect(
						$_SESSION['acl']['pdata'],
						$_SESSION['acl']['pall'])) &&
				!auth_no_access(_('to access this survey'))) {
			return;
		}
	}

	$type = isset($_GET['type']) ? $_GET['type'] : 'csv';
	
	// Try DBF first, default to CSV
	if($type == 'dbf') {
		$file = "$name.dbf";
		if(extension_loaded('dbase')) {
			echo mkerror(_('DBF download not yet implemented.'));
		} else {
			echo mkerror(_('The PHP dBase Extension is not installed.'));
		}
		return;
	}
	if($type == 'html') {
		header("Content-Disposition: attachment; filename=$name.html");
		echo("<html>\n");
		$ret = survey_results($sid);
		echo("</html>\n");
		return;
	}
	// CSV
	$csv_charset = $GLOBALS['ESPCONFIG']['csv_charset'];
	//$data = survey_generate_csv($sid,$csv_charset);
	header("Pragma: private");
	header("Cache-Control: public");
	header("Content-Transfer-Encoding: ".$csv_charset);
	header("Content-Disposition: attachment; filename=$name.csv");
	header("Content-Type: text/comma-separated-values");
	$output = survey_generate_csv($sid,$csv_charset);
	foreach ($output as $row ) {
		echo(join(',', $row) . "\n");
	}
	return;
?>
