<?php

# $Id: edit.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

    $bg = '';

	/* load names and titles of all surveys available to
	 * _this_ user */
	if($xoopsUser->isAdmin()) {
		$sql = 'SELECT s.id, s.name, s.title, s.owner, s.realm
		FROM '.TABLE_SURVEY.' s ORDER BY s.id DESC';
		//FROM '.TABLE_SURVEY.' s WHERE s.status = 0 ORDER BY s.id DESC';
	} else {
		$realms = $xoopsModuleConfig['PSTATUS'] ? array_to_insql(
			array_intersect(
				$_SESSION['acl']['pall'],
				array_merge(
					$_SESSION['acl']['pall'],
					$_SESSION['acl']['pdesign']))) : NULL;
		$sql = "SELECT s.id, s.name, s.title, s.owner, s.realm FROM "
			.TABLE_SURVEY." s WHERE (s.owner ='".$xoopsUser->uid()."'";
		$sql .= is_null($realms) ? "" : " || s.realm $realms";
		$sql .= ") ORDER BY s.id DESC";
	}
	$result = mysql_query($sql);

?>
<h2><?php echo(_GT_Edit_a_Survey); ?></h2>
<?php echo(_GT_Pick_Survey_to_Edit); ?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="#dddddd">
		<th align="left"><?php echo(_GT_ID); ?></th>
		<th align="left"><?php echo(_GT_Name); ?></th>
		<th align="left"><?php echo(_GT_Title); ?></th>
		<th align="left"><?php echo(_GT_Owner); ?></th>
		<th align="left"><?php echo(_GT_Group); ?></th>
	</tr>
<?php
	while(list($sid,$name,$title,$owner,$realm) = mysql_fetch_row($result)) {
		if($bg != $ESPCONFIG['bgalt_color1'])
			$bg = $ESPCONFIG['bgalt_color1'];
		else
			$bg = $ESPCONFIG['bgalt_color2'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
		<td><?php echo($sid); ?></td>
		<td>
			<a href="<?php echo($GLOBALS['ESPCONFIG']['ME'] ."?where=tab&newid=${sid}"); ?>"><?php echo($name); ?></a>
		</td>
		<td><?php echo($title); ?></td>
		<td><?php echo($owner); ?></td>
		<td><?php echo($realm); ?></td>
	</tr>
<?php
	}
	mysql_free_result($result);
?>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>