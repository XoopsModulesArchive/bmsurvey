












<?php

# $Id: groups.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// <jflemer@alum.rpi.edu>
// <jflemer@acm.rpi.edu>

	/* load only accounts available to _this_ user */
	if($_SESSION['acl']['superuser'] != 'Y' &&
			!auth_no_access(_GT_to_access_this_form)) {
		return;
	}
	
	$errstr = '';
	if(!empty($_POST['r']) && !empty($_POST['t'])) {
		$sql = "INSERT INTO ".TABLE_REALM." (name, title)
		VALUES ('". _addslashes($_POST['r']) ."', '".
		_addslashes($_POST['t']) ."')";
		if(!mysql_query($sql))
			$errstr = _GT_Error_adding_group . ' (' . mysql_error() .')';
	}
	if(!empty($_GET['del'])) {
		if($_GET['del'] == 'superuser') {
			$errstr = _GT_Error_deleting_group;
		} else {
			$sql = "SELECT count(d.username) FROM ".TABLE_DESIGNER." d WHERE d.realm='". _addslashes($_GET['del']) ."'";
			$result = mysql_query($sql);
			$count = mysql_result($result, 0, 0);
			mysql_free_result($result);
			if ($count > 0) {
				$errstr = _GT_Group_is_not_empty;
			} else {
				$sql = "DELETE FROM ".TABLE_REALM." WHERE name='". _addslashes($_GET['del']) ."'";
				if(!mysql_query($sql))
					$errstr = _GT_Error_deleting_group;
			}
		}
	}

	$sql = "SELECT r.name, r.title, count(d.username) FROM ".TABLE_REALM." r LEFT JOIN ".TABLE_DESIGNER." d ON r.name=d.realm GROUP BY r.name ORDER BY r.name";
	$sql2 = "SELECT count(d.username) FROM ".TABLE_REALM." r LEFT JOIN ".TABLE_RESPONDENT." d ON r.name=d.realm GROUP BY r.name ORDER BY r.name";
	$result = mysql_query($sql);
	$result2 = mysql_query($sql2);
	
	$i = 0;
	$bg = $ESPCONFIG['bgalt_color2'];
?>
<h2><?php echo(_GT_Manage_Groups); ?></h2>
<?php if(!empty($errstr)) echo('<p>'. mkerror($errstr) ."</p>\n"); ?>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
<input type="hidden" name="where" value="groups">
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="<?php echo($bg); ?>">
		<th align="left"><?php echo(_GT_Group); ?></th>
		<th align="left"><?php echo(_GT_Description); ?></th>
		<th align="left"><?php echo(_GT_Members); ?></th>
		<td>&nbsp;</td>
	</tr>
<?php
	while(list($realm, $title, $count) = mysql_fetch_row($result)) {
		$count += mysql_result($result2, $i++, 0);

		if ($bg == $ESPCONFIG['bgalt_color1'])
			$bg =& $ESPCONFIG['bgalt_color2'];
		else
			$bg =& $ESPCONFIG['bgalt_color1'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
		<td><?php echo($realm);  ?></td>
		<td><?php echo($title);  ?></td>
		<td><?php echo($count);  ?></td>
		<td><?php echo ($count ?
			"&nbsp;" :
			"<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=groups&del=$realm\">". _GT_Delete ."</a>"); ?></td>
	</tr>
<?php
	}
	mysql_free_result($result);
	mysql_free_result($result2);
	
	if ($bg == $ESPCONFIG['bgalt_color1'])
		$bg =& $ESPCONFIG['bgalt_color2'];
	else
		$bg =& $ESPCONFIG['bgalt_color1'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
		<td><input type="text" size="16" maxlength="16" name="r"></td>
		<td><input type="text" size="32" maxlength="64" name="t"></td>
		<td>&nbsp;</td>
		<td><input type="submit" name="add" value="<?php echo(_GT_Add); ?>"</td>
	</tr>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
