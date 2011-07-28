<?php

# $Id: export.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// <jflemer@alum.rpi.edu>

if(!empty($_POST['sid']))
	$sid = intval($_POST['sid']);
elseif(!empty($_GET['sid']))
	$sid = intval($_GET['sid']);
else
	$sid = '';
$bg = '';

?>
<h2><?php echo(_GT_Export_Data); ?></h2>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
<table border="0" align="center" cellspacing="0" cellpadding="4" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
<?php

/* If the Survey ID is not given, then build
 * a menu of available surveys to chose from.
 */
if(!$sid) {
?>
	<tr bgcolor="#dddddd">
		<th align="left"><?php echo(_GT_ID); ?></th>
		<th align="left"><?php echo(_GT_Name); ?></th>
		<th align="left"><?php echo(_GT_Title); ?></th>
		<th align="left"><?php echo(_GT_Owner); ?></th>
		<th align="left"><?php echo(_GT_Group); ?></th>
		<th align="left"><?php echo(_GT_Status); ?></th>
		<th align="left" colspan="2"><?php echo(_GT_Format); ?></td>
	</tr>
<?php
	/* load names and titles of all surveys available to
	 * _this_ user */
	$statusok = (STATUS_ACTIVE | STATUS_DONE & ~STATUS_DELETED);
	if($xoopsUser->isAdmin()) {
		$sql = "SELECT id,name,title,status,owner,realm FROM ".TABLE_SURVEY."
		WHERE (status & $statusok) ORDER BY id DESC";
	} else {
		$realms = array_to_insql(
			array_intersect(
				$_SESSION['acl']['pall'],
				$_SESSION['acl']['pdata']));
		$sql = "SELECT id,name,title,status,owner,realm
			FROM ".TABLE_SURVEY." WHERE (status & $statusok) AND (owner = '".
			$xoopsUser->uid() ."' || realm $realms) ORDER BY id DESC";
	}
	$result = mysql_query($sql);

	while(list($sid,$name,$title,$status,$owner,$realm) = mysql_fetch_row($result)) {
		$stat = _GT_Editing;

		if($status & STATUS_DELETED) {
			$stat = _GT_Archived;
			continue;
		} elseif($status & STATUS_DONE) {
			$stat = _GT_Ended;
		} elseif($status & STATUS_ACTIVE) {
			$stat = _GT_Active;
		} elseif($status & STATUS_TEST) {
			$stat = _GT_Testing;
		}

		if($bg != $ESPCONFIG['bgalt_color1'])
			$bg = $ESPCONFIG['bgalt_color1'];
		else
			$bg = $ESPCONFIG['bgalt_color2'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
		<td><?php echo($sid); ?></td>
		<td><?php echo($name); ?></td>
		<td><?php echo($title); ?></td>
		<td><?php echo($owner); ?></td>
		<td><?php echo($realm); ?></td>
		<td><?php echo($stat); ?></td>
		<td><?php
			echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=export&type=csv&sid=$sid\">" . _GT_CSV . "</a>"); ?>
			(<?php
			echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=download&type=csv&sid=$sid\">" . _GT_download . '</a>'); ?>)
			<!--</td><td>-->
			&nbsp;
			<!-- <?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=export&type=dbf&sid=$sid\">" . _GT_DBF . "</a>"); ?>  -->
			<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=download&type=html&sid=$sid\">" . _GT_HTML . "</a>"); ?>
		</td>
	</tr>
<?php
	}
?>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
<?php
	return;
	}

	/* sid supplied ... continue */
	$sql = "SELECT name FROM ".TABLE_SURVEY." WHERE id = $sid";
	$result = mysql_query($sql);
	$name = mysql_result($result, 0, 0);
	mysql_free_result($result);

	/* check ACLs for permissions */
	$srealm = auth_get_survey_realm($sid);
	if(isset($_GET['test'])) {
		/* check ACL to see if user is allowed to test
		 * _this_ survey */
		if($_SESSION['acl']['superuser'] != 'Y' &&
				!auth_is_owner($sid, $xoopsUser->uid()) &&
				!in_array($srealm, array_intersect(
						$_SESSION['acl']['pdesign'],
						$_SESSION['acl']['pall'])) &&
				!auth_no_access(_GT_to_access_this_survey)) {
			return;
		}
		echo("<p><b>". _GT_Testing_Survey ."</b> (". _GT_SID ." = $sid)</p>\n");
	} else {
		/* check ACL to see if user is allowed to export
		 * _this_ survey */
		if($_SESSION['acl']['superuser'] != 'Y' &&
				!auth_is_owner($sid, $xoopsUser->uid()) &&
				!in_array($srealm, array_intersect(
						$_SESSION['acl']['pdata'],
						$_SESSION['acl']['pall'])) &&
				!auth_no_access(_GT_to_access_this_survey)) {
			return;
		}
	}

	$type = isset($_GET['type']) ? $_GET['type'] : 'csv';

	// Try DBF first, default to CSV
	if($type == 'dbf') {
		$file = $_SESSION['acl']['home'] . "/$name.dbf";
		if(extension_loaded('dbase')) {
			$ret = survey_export_dbf($sid, $file);
		} else {
			$ret = 0;
			echo("<tr><td>" . mkerror(_GT_The_PHP_dBase) . "</td></tr>");
		}
	} else {
		$csv_charset = $GLOBALS['ESPCONFIG']['csv_charset'];
		$file = $_SESSION['acl']['home'] . "/$name.csv";
		$ret = survey_export_csv($sid, $file,$csv_charset);
	}

	echo("<tr><td>");
	if(isset($_GET['test'])) {
		echo("<p><b>". _GT_Testing_Survey ."</b> (". _GT_SID ." = $sid)</p>\n");
	}
	if($ret) {
		echo(_GT_Survey_exported_as . " <tt>$file</tt>");
	} else {
		echo(mkwarn(_GT_Error_exporting_survey_as . " <tt>$file</tt>"));
	}
	echo("</td></tr></table>\n");
	echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n");

?>
