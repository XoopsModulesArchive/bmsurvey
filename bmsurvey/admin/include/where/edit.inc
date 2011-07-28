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
		$sql = "SELECT s.id, s.name, s.title, s.owner, s.realm FROM "
			.TABLE_SURVEY." s WHERE s.owner =".$xoopsUser->uid()." ORDER BY s.id DESC";
	}
	$result = mysql_query($sql);

?>
<h2><?php mb_echo('Edit a Survey'); ?></h2>
<?php mb_echo('Pick Survey to Edit'); ?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="#dddddd">
		<th align="left"><?php mb_echo('ID'); ?></th>
		<th align="left"><?php mb_echo('Name'); ?></th>
		<th align="left"><?php mb_echo('Title'); ?></th>
		<th align="left"><?php mb_echo('Owner'); ?></th>
		<th align="left"><?php mb_echo('Group'); ?></th>
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
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>