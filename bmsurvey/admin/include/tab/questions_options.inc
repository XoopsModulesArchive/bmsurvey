<?php
# $Id: questions_options.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $
// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>
//---------------------------------------------------------------------
// New Interface Design by Kon Angelopoulos
// for the phpESP project.
// <angekproductions@iprimus.com.au>
//---------------------------------------------------------------------
// Add Copy Question by Yoshi Sakai
// for bmsurvey project.
// <webmaster@bluemooninc.biz>
//---------------------------------------------------------------------
	// do not need to import session vars here
	// it is done by questions.inc before this code executes

	// this type of question has answer options
	if ($updated && $curr_q) {
		$sql = "SELECT id,content FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='${curr_q_id}' ORDER BY id";
		$result = mysql_query($sql);
		$c = mysql_num_rows($result);
	} else {
		if (isset($_POST['num_choices']))
			$c = intval($_POST['num_choices']);
		else
			$c = $GLOBALS['ESPCONFIG']['default_num_choices'];
	}
	if(isset($_POST['extra_choices']))
		$num_choices = max($c, $_POST['num_choices']) + 1;
	else
		$num_choices = $c;
?>
<table border="0" cellspacing="0" width="100%">
	<tr><td colspan="2"><hr></td></tr>
	<tr>
		<td valign=TOP colspan="2">
			<?php printf(mb_('Enter the possible answers (if applicable). Enter %s on an line by itself to create a fill-in-the-blank answer at the end of this question. Any blank lines will be suppressed.'), '<tt>!other</tt>'); ?>
			<input type="hidden" name="num_choices" value="<?php echo($num_choices); ?>">
		</td>
	</tr>
<?php
		for($i=1; $i<$num_choices+1; ++$i) {
			if ($updated && $curr_q) {
				list($choice_id, $choice_content) = mysql_fetch_row($result);
			} else {
				if ( $curr_q && isset($_POST["choice_id_$i"]) ) {
					$choice_id = intval($_POST["choice_id_$i"]);
				}
				if(isset($_POST["choice_content_$i"]))
					$choice_content = _stripslashes($_POST["choice_content_$i"]);
			}
?>
	<tr>
		<td class="numbered"><?php echo($i); ?>.</td>
		<td valign=middle align="left">
			<input type="hidden" name="choice_id_<?php echo($i); ?>"
				value="<?php if(isset($choice_id)) echo($choice_id); ?>">
			<input type="text" size="60" name="choice_content_<?php echo($i); ?>"
				value="<?php if(isset($choice_content)) echo(htmlspecialchars($choice_content)); ?>">
		</td>
	</tr>
<?php	} ?>
	<tr>
		<td>&nbsp;</td>
		<td>
		<input type="submit" name="extra_choices" value="<?php mb_echo('Add another answer line'); ?>"> 
		<input type="button" name="clear" value="Clear all answer lines" onClick="clearTextInputs()"> 
		</td>
	</tr>
</table>
<HR>
<?php
//*****************************************************************************
// Copy question from other survey
//*****************************************************************************
	echo(_AM_BMSURVEY_COPYQUESTION."&nbsp;");

	/* load names and titles of all surveys available to
	 * _this_ user */
	if($xoopsUser->isAdmin()) {
		$sql = 'SELECT s.id, s.name, s.title, s.owner, s.realm, s.status
		FROM '.TABLE_SURVEY.' s ORDER BY s.id DESC';
	} else {
		$sql = "SELECT s.id, s.name, s.title, s.owner, s.realm, s.status FROM "
		.TABLE_SURVEY." s WHERE s.owner =".$xoopsUser->uid()." ORDER BY s.id DESC";
	}
	$result = mysql_query($sql);
	$copy_qid = isset($_POST['copy_qid']) ? intval($_POST['copy_qid']) : 0 ;
	$js_key = $js_val = $selections = $statuses = array();
	while(list($copy_sid,$name,$title,$owner,$realm,$status) = mysql_fetch_row($result)) {
		$sql = "SELECT q.id,q.name,q.content FROM " . TABLE_QUESTION 
			. " q WHERE survey_id='" . $copy_sid . "' AND deleted='N' ORDER BY position,id";
		$resultq = mysql_query($sql);
		$selections[$name]=array();
		$s_str = $k_str = null;
		$i = 1;
		while($row=mysql_fetch_array($resultq)){
			if ( $row['id'] && $row['name'] ){
				array_push($selections[$name],$row);
				$k_str .= $k_str ? "," : "'',";
				$k_str .= $row['id'];
				$s_str .= $s_str ? "," : "'---- " . $name . " ----',";
				//$s_str .= sprintf( "'%u: %s'",$i++,$row['name']);
				$content = strip_tags( $row['content']);
				$content = preg_replace("/(\n|\r|\t|\s)/","",$content);
				if (strlen($content)>48) $content = xoops_substr($content,0,48);
				$s_str .= sprintf( "'%u: %s[%s]'", $i++, $row['name'], $content);
			}
		}
		if ( isset($js_key[$status]) )
			$js_key[$status] .= "," . $k_str;
		else
			$js_key[$status] = $k_str;
		if ( isset($js_val[$status]) )
			$js_val[$status] .= "," . $s_str;
		else
			$js_val[$status] = $s_str;
		$statuses[]=$status;
	}
	$statusList = array(
		5=>_AM_BMSURVEY_ARCHIVED,
		8=>_AM_BMSURVEY_TEST,
		3=>_AM_BMSURVEY_EXPIRATION,
		1=>_AM_BMSURVEY_ACTIVE,
		0=>_AM_BMSURVEY_EDIT);
	$selstat = "";
	foreach($statusList as $k=>$v){
		if (in_array($k,$statuses)) $selstat .= sprintf('<OPTION value="%u">%s</option>',$k,$v) . "\r\n";
	}
	printf('<SELECT onchange="setSelect2Options(value)" name="select_status">
    	<OPTION value="">%s</option>%s</SELECT>&nbsp;',_AM_BMSURVEY_SELECTSTATUS,$selstat);
	echo '<select name="copy_qid">'; 
	/*
	foreach($selections as $name => $val){
		echo '<optgroup label="' . $name . '">';
		foreach($val as $s){
			$checked = $copy_qid == $s['qid'] ? "selected='selected'" : "";
			if ($s['content']){
				echo "<option value='" . $s['qid'] . "'" . $checked .">" . $s['name'] .": " .$s['content'] . "</option>\r\n"; 
			}
		}
		echo '</optgroup>'; 
	}
	*/
	echo '</select>&nbsp;';
	echo '<input type="submit" name="copy_question" value="'._SUBMIT.'">';
/*
** Java Script Section
*/
	echo '<script type="text/javascript">';
	echo "var optlist = new Array();";
	echo "var optvkey = new Array();";
	foreach($js_val as $k =>$v ){
		echo "optlist['$k'] = new Array($v);";
	} 
	foreach($js_key as $k =>$v ){
		echo "optvkey['$k'] = new Array($v);";
	} 
echo "
function setSelect2Options(val){
  var selecter = document.phpesp.copy_qid;
  var list = optlist[val];
  var vkey = optvkey[val];
 
  selecter.options.length = list.length;
  for(i=0; i<list.length; i++){
    selecter.options[i].value = vkey[i];
    selecter.options[i].text = list[i];
  }
  selecter.options[0].selected=true;
}
";
echo '</script>';

?>
