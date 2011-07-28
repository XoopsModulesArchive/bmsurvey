


<?php

# $Id: purge.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	if($_SESSION['acl']['superuser'] != 'Y') {
		echo(mkerror(_('Only superusers allowed.'))."<br>\n");
		echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n");
		return;
	}

	$sids = array();
	if(!empty($_POST['list'])) {
		while(list(,$sid) = each($_POST['list'])) {
			array_push($sids, intval($sid));
		}
		survey_purge($sids);
	}

	$sql = "SELECT s.id,s.name,s.title,s.status,s.owner,s.realm,COUNT(q.id) as count FROM ".
		TABLE_SURVEY." s LEFT JOIN ".TABLE_QUESTION.
		" q ON s.id = q.survey_id AND q.deleted = 'N' GROUP BY s.id ORDER BY s.id DESC";
	$result = mysql_query($sql);

	$bg = $ESPCONFIG['bgalt_color2'];
?>
<h2><?php mb_echo('Purge Surveys'); ?></h2>

<p><?php mb_echo('This page is not directly on the main menu because it is dangerous. This <b>completely</b> removes everything about a survey from the database <b>forever</b>. All question info, general info, results, etc. are purged from the database. Do not do anything here that you are not completely certain about. There is no confirmation, there is no turning back.'); ?></p>

<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="<?php echo($bg); ?>">
		<td>&nbsp;</td>
		<th align="left"><?php mb_echo('ID'); ?></th>
		<th align="left"><?php mb_echo('Name'); ?></th>
		<th align="left"><?php mb_echo('Title'); ?></th>
		<th align="left"><?php mb_echo('Owner'); ?></th>
		<th align="left"><?php mb_echo('Group'); ?></th>
		<th align="left"><?php mb_echo("# Q's"); ?></th>
		<th align="left"><?php mb_echo('Status'); ?></th>
	</tr>
<?php
	while(list($sid,$name,$title,$status,$owner,$realm,$qnum) = mysql_fetch_row($result)) {
		if($status & STATUS_DELETED) {
			$stat = mb_('Archived');
		} elseif($status & STATUS_DONE) {
			$stat = mb_('Ended');
		} elseif($status & STATUS_ACTIVE) {
			$stat = mb_('Active');
		} elseif($status & STATUS_TEST) {
			$stat = mb_('Testing');
		} else {
			$stat = mb_('Editing');
		}
		
		if($bg != $ESPCONFIG['bgalt_color1'])
			$bg = $ESPCONFIG['bgalt_color1'];
		else
			$bg = $ESPCONFIG['bgalt_color2'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
		<td><input type="checkbox" name="list[]" value="<?php echo($sid); ?>"></td>
		<td><?php echo($sid); ?></td>
		<td><?php echo($name); ?></td>
		<td><?php echo($title); ?></td>
		<td><?php echo($owner); ?></td>
		<td><?php echo($realm); ?></td>
		<td><?php echo($qnum); ?></td>
		<td><?php echo($stat); ?></td>
	</tr>
<?php
	}
	
	if($bg != $ESPCONFIG['bgalt_color1'])
		$bg = $ESPCONFIG['bgalt_color1'];
	else
		$bg = $ESPCONFIG['bgalt_color2'];
?>
    <tr bgcolor="<?php echo($bg); ?>">
		<td colspan="8" align="center">
			<input type="hidden" name="where" value="purge">
			<input type="reset" name="reset" value="<?php mb_echo('Clear Checkboxes'); ?>">
			<input type="submit" name="submit" value="<?php mb_echo('Purge Surveys'); ?>">
		</td>
	</tr>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . mb_('Go back to Management Interface') . "</a>\n"); ?>
