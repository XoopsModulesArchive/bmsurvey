


<?php

# $Id: purge.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	if($_SESSION['acl']['superuser'] != 'Y') {
		echo(mkerror(_GT_Only_superusers_allowed)."<br>\n");
		echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n");
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
<h2><?php echo(_GT_Purge Surveys); ?></h2>

<p><?php echo(_GT_This_page_is_not_directly); ?></p>

<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="<?php echo($bg); ?>">
		<td>&nbsp;</td>
		<th align="left"><?php echo(_GT_ID); ?></th>
		<th align="left"><?php echo(_GT_Name); ?></th>
		<th align="left"><?php echo(_GT_Title); ?></th>
		<th align="left"><?php echo(_GT_Owner); ?></th>
		<th align="left"><?php echo(_GT_Group); ?></th>
		<th align="left"><?php echo(_GT_Qs); ?></th>
		<th align="left"><?php echo(_GT_Status); ?></th>
	</tr>
<?php
	while(list($sid,$name,$title,$status,$owner,$realm,$qnum) = mysql_fetch_row($result)) {
		if($status & STATUS_DELETED) {
			$stat = _GT_Archived;
		} elseif($status & STATUS_DONE) {
			$stat = _GT_Ended;
		} elseif($status & STATUS_ACTIVE) {
			$stat = _GT_Active;
		} elseif($status & STATUS_TEST) {
			$stat = _GT_Testing;
		} else {
			$stat = _GT_Editing;
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
			<input type="reset" name="reset" value="<?php echo(_GT_Clear_Checkboxes); ?>">
			<input type="submit" name="submit" value="<?php echo(_GT_Purge Surveys); ?>">
		</td>
	</tr>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
