<?php
// $Id: survey_report.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// <jflemer@alum.rpi.edu>

/* {{{ proto string survey_report(int survey_id, bool details, string format)
    Build a description of a survey, including all unique ids.
	Rerturns an empty string on success, else an error message. */
function survey_report($sid, $details = 0, $format = '') {
	if(empty($sid)) return;
	$bg = '';
	// build associative array holding weather each question
	// type has answer choices or not and the table the answers are in
	$has_choices = array();
	$response_table = array();
	$sql = "SELECT id,has_choices,response_table FROM ".TABLE_QUESTION_TYPE." ORDER BY id";
	if(!($result = mysql_query($sql))) {
		$errmsg = mb_('Error system table corrupt.') ." [ ". mb_('Table') .": question_type ]";
		return($errmsg);
	}
	while($row = mysql_fetch_row($result)) {
		$has_choices[$row[0]] = $row[1];
		$response_table[$row[0]] = $row[2];
	}
	mysql_free_result($result);

	// load survey title (and other globals)
	$sql = "SELECT * FROM ".TABLE_SURVEY." WHERE id='$sid'";
	if(!($result = mysql_query($sql))) {
		$errmsg = mb_('Error opening survey.') ." [ ID:${sid} R:" . mysql_num_rows($result) ."]";
		return($errmsg);
	}
	$survey = mysql_fetch_array($result, MYSQL_ASSOC);
	mysql_free_result($result);

	// load survey questions
	$sql = "SELECT * FROM ".TABLE_QUESTION."
			 WHERE survey_id='$sid' AND deleted='N'
			 ORDER BY position";
	if(!($questions_result = mysql_query($sql))) {
		$errmsg = mb_('Error opening survey.') .' '. mb_('No questions found.') ." [ ID:${sid} ]";
		return($errmsg);
	}
?>
<h2><?php mb_echo('Report for'); echo (': '. $survey["title"] .' ['. mb_('ID') .': '. $survey['id'] .']'); ?></h2>
<h3><?php echo($survey["subtitle"]); ?></h3>
<blockquote><?php echo($survey["info"]); ?></blockquote>
<table border="0" cellspacing="2" cellpadding="0" width="100%">
<tr>
	<th align="left"><?php mb_echo('#'); ?></th>
	<th align="left"><?php mb_echo('ID'); ?></th>
	<th align="left"><?php mb_echo("Req'd"); ?></th>
	<th align="left"><?php mb_echo('Public'); ?></th>
	<th align="left" colspan="2"><?php mb_echo('Content'); ?></th>
</tr>
<?php
	$i = 0;
	while($question = mysql_fetch_array($questions_result, MYSQL_ASSOC)) {
		// process each question
		$qid   = &$question['id'];
		$tid   = &$question['type_id'];
		$reqd  = ($question['required'] == 'Y') ?
					mb_('Yes') : mb_('No');
		$publ  = ($question['public'] == 'Y') ?
					mb_('Yes') : mb_('No');
		$table = &$response_table[$tid];

		if ($tid == 99) {
			echo("<tr><td colspan=\"6\"><hr></td></tr>\n");
			continue;
		}

		if($bg != '#ffffff')	$bg = '#ffffff';
		else					$bg = '#eeeeee';

?>
<tr bgcolor="<?php echo($bg); ?>">
 	<td align="right"><?php if ($tid < 50) echo(++$i); ?></td>
	<th align="right"><?php echo($qid); ?></th>
	<td><?php echo($reqd); ?></td>
	<td><?php echo($publ); ?></td>
	<td colspan="2"><?php echo($question["content"]); ?></td>
</tr>
<?php
		if($has_choices[$tid]) {
			$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE."
					 WHERE question_id = $qid
					 ORDER BY id";
 			$choices_result = mysql_query($sql);
			while($choice = mysql_fetch_array($choices_result, MYSQL_ASSOC)) {
?>
<tr bgcolor="<?php echo($bg); ?>"><th align="left"></th>
	<td></td>
	<td>&nbsp;</td>
	<td>&nbsp;</td>
	<th align="right"><?php echo($choice['id']); ?></th>
	<td><?php echo($choice['content']); ?></td>
</tr>
<?php
			}
			mysql_free_result($choices_result);
		} // end if has_choices
	} // end while
	mysql_free_result($questions_result);
?>
</table>
<?php
	return;
}
/* }}} */

?>