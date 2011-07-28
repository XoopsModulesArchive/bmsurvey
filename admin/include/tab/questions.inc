<?php

# $Id: questions.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	if(empty($_SESSION['curr_q']))
		$_SESSION['curr_q'] = 0;
    if(empty($_POST['id']))
        $_POST['id'] = 0;

	$curr_q =& $_SESSION['curr_q'];
	$sid =& $_SESSION['survey_id'];
   	$id = intval($_POST['id']);

    if (isset($_POST['type_id']))
        $_POST['type_id'] = intval($_POST['type_id']) ?
                $_POST['type_id'] : '';
    else
        $_POST['type_id'] = '';

	// build array of question IDs
	$sql = "SELECT id,type_id FROM ".TABLE_QUESTION." WHERE survey_id='$sid' AND deleted='N' AND type_id != 99 ORDER BY position";
	$result = mysql_query($sql);
	$total_num_q = mysql_num_rows($result);
	$q_ids = array();
	$qn_ids = array();
	while(list($qi,$tid) = mysql_fetch_row($result)){
		array_push($q_ids, $qi);
		if ($tid<50) array_push($qn_ids, $qi);
	}
	mysql_free_result($result);

	// update failed, stay on same question
	if(!$updated || isset($_POST['extra_choices'])) {
		for ( $q = 0; $q < $total_num_q && $q_ids[$q] != $id ; $q++);
		$_POST['q'] = ++$q;
		unset($q);
	} else {
		if(empty($_POST['q']))
			$_POST['q'] = $curr_q;
		if($_POST['q'] == mb_('New Field'))
			$_POST['q'] = 0;
		if($_POST['q']<0 || $_POST['q']>$total_num_q)
			$_POST['q'] = 1;
	}
	$curr_q = $_POST['q'];
	if($curr_q && isset($q_ids[$curr_q-1])){
		$curr_q_id = $q_ids[$curr_q-1];
		$curr_n = array_search($curr_q_id,$qn_ids);
		if (!is_null($curr_n)) $curr_n++;
	}else{
		$curr_q_id = 0;
		$curr_n = null;
	}
	$fields = array('name','type_id','length','precise','required','content','position');
	if($updated && $total_num_q > 0 && $curr_q) {
		// survey questions exist already
		// load values from DB
		$sql = "SELECT * FROM ".TABLE_QUESTION." WHERE id='${curr_q_id}' AND deleted='N' ORDER BY position";
		$result = mysql_query($sql);
		$question = mysql_fetch_array($result,MYSQL_ASSOC);
		mysql_free_result($result);
		foreach($fields as $f) {
			$_POST[$f] =& $question[$f];
		}
	} else if ($updated && $copy_question==0) {
		// adding a new question (possibly because there are no questions yet)
		$curr_q = 0;
		$curr_q_id = '';
		$num_choices = isset($_POST['num_choices']) ? intval($_POST['num_choices']) : 0;
		for($i=1; $i<$num_choices+1; ++$i) {
			unset($_POST["choice_content_$i"]);
		}
		foreach(array('name','length','precise','content','position','num_choices') as $f) {
			$_POST[$f] = NULL;
		}
	}else{
		// for copy question from other surveys
		foreach($fields as $f) {
            if(!empty($_POST[$f]))
			    $_POST[$f] = _stripslashes($_POST[$f]);
            else
                $_POST[$f] = '';
		}
	}
?>
<script language="javascript">
<!-- // comment
function clearTextInputs() {
	var i = 1;
	while (document.phpesp.elements["choice_content_" + i]) {
		document.phpesp.elements["choice_content_" + i].value = "";
		i++;

        }
}
// comment -->
</script>
<center>
	<input type="hidden" name="id" value="<?php echo($curr_q_id); ?>">
	<p>
<?php 
	mb_echo('Edit this field, or click the number of the field you would like to edit:'); 
	echo "</p>";
	$qnumbers = "";
	for($i=1; $i<$total_num_q+1; ++$i) {
		$qnumbers .= ' <input type="submit" name="q" value="'.$i.'">';
	}
	$qnumbers .= ' <input type="submit" name="q" value="'.mb_('New Field').'">';
	echo $qnumbers . "<hr><b>";
	if(!$curr_q) {
		mb_echo('New Field');
	} else {
		mb_echo('Field'." $curr_q");
	}
	echo '</b><table border="0"><tr>';
	printf("<th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th><th>%s</th></tr>",
			_AM_BMSURVEY_QUESTIONNUMBER,mb_('Field Name')._AM_BMSURVEY_FILEDNAME_DESC,mb_('Type'),mb_('Length'),mb_('Precision'),mb_('Required'));
	echo '<tr><td align="center">'.$curr_n.'</td><td align="center">'.mktext('name',12).'</td>';
	echo '<td align="center">';
	if($updated && empty($_POST['type_id'])) $_POST['type_id'] = 2;
	$sql = "SELECT id, type FROM ".TABLE_QUESTION_TYPE." WHERE id != 99";
	$result = mysql_query($sql);
	$arr = array();
	while(list($key, $val) = mysql_fetch_row($result)) {
		$arr["$key"] = mb_($val);
	}
	echo(mkselect('type_id',$arr));
?>
		</td>
			<td align="center"><?php
				if(empty($_POST['length'])) $_POST['length'] = 0;
				echo(mktext("length",6));
			?></td>
			<td align="center"><?php
				if(empty($_POST['precise'])) $_POST['precise'] = 0;
				echo(mktext("precise",6));
			?></td>
			<td align="center"><?php
				if(empty($_POST['required'])) $_POST['required'] = 'N';
				echo(mkselect("required",array(
					"Y" => mb_('Yes') . '               ',
					"N" => mb_('No')
				))); ?></td>
		</tr>
		<tr>
			<th align="left" valign="top">Text</th>
			<td colspan="5" valign="top"><?php
				echo(mktextarea("content",4,60,"VIRTUAL"));
			?></td>
		</tr>
	</table>
<?php
	// has answer options ... so show that part of the form
	if($curr_q == 0 || empty($_POST['type_id']) ||
		mysql_result(mysql_query("SELECT has_choices FROM ".TABLE_QUESTION_TYPE." WHERE id='" . $_POST['type_id'] ."'"),0,0)=='Y')
	{
		include($GLOBALS['ESPCONFIG']['include_path']."/tab/questions_options".$GLOBALS['ESPCONFIG']['extension']);
	}
	echo "<hr>" . $qnumbers;
?>
