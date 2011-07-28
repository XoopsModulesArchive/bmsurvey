
<?php

# $Id: survey_render.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

// Modify by Yoshi Sakai
// For Attach File
// <webmaster@bluemooninc.biz>
// 2004/05/23 Change form style more compact.
// 2004/05/26 Add Attach File.

function maxbyte_str($maxbyte){
	if ($maxbyte>=1000000){
		$maxbyte_str=sprintf("%d M",$maxbyte/1000000);
	} elseif ($maxbyte>=1000){
		$maxbyte_str=sprintf("%d K",$maxbyte/1000);
	} else {
		$maxbyte_str=sprintf("%d ",$maxbyte);
	}
	return $maxbyte_str;
}
/* {{{ proto bool survey_render(int survey_id, int section, string message)
   Reads current form variables from _POST.
   Builds HTML for the survey (prefixed w/ message). */
function survey_render_smarty($sid, $section = 1, $message = '') {
	global $xoopsTpl,$maxfilesize;
	global $_POST,$xoopsModuleConfig;
	@reset($_POST);

	if(empty($section)) $section = 1;

	$has_choices = esp_type_has_choices();

	// load survey title (and other globals)
	$sql = "SELECT * FROM ".TABLE_SURVEY." WHERE id='${sid}'";
	$result = mysql_query($sql);
	if(mysql_num_rows($result) != 1) return(false);
	$survey = mysql_fetch_array($result,MYSQL_ASSOC);
	$defval = "";
	if (!isset($_POST['submit'])){
		$rid = $survey['response_id'];
		$defval = response_select_defval($sid,$rid);
	}
	mysql_free_result($result);
//	var_dump($defval);

	$sections = survey_get_sections($sid);
	$num_sections = count($sections);
	if($section-- > $num_sections) return(false);	// invalid section

    $section_questions = survey_get_section_questions($sid);

	// load survey questions
	$sec_sql = array_to_insql($sections[$section]);
	$sql = "SELECT * FROM ".TABLE_QUESTION." WHERE id $sec_sql ORDER BY position,id";
	$questions_result = mysql_query($sql);
	if(mysql_num_rows($questions_result) < 1) return(false);
	// check to see if there are required questions
	$sql = "SELECT COUNT(*) FROM ".TABLE_QUESTION." WHERE id $sec_sql AND required='Y'";
	$has_required = mysql_result(mysql_query($sql),0,0);

	// find out what question number we are on $i
	$i=0;
	for($j=0;$j<$section;$j++) $i += count($section_questions[$j]);
	$xoopsTpl->assign('survey',$survey);
	$xoopsTpl->assign('message',$message);
	if($num_sections>1) {
		$xoopsTpl->assign('surveyPage',
			printf(_GT_Page_d_of_d, number_format($section+1), number_format($num_sections)) );
	}
	if($has_required) {
    	$xoopsTpl->assign('has_required',_MD_ASTERISK_REQUIRED);
	}
	$formbody = array();
	$i = $qnum = 1;
	while($question = mysql_fetch_array($questions_result,MYSQL_ASSOC)) {
		$res = array();
		// process each question
		$qid  = &$question['id'];
		$tid  = &$question['type_id'];
		$size = &$question['length'];
		$prec = &$question['precise'];
		// Section Text
		if ($tid == 100) {
			$res['number'] = '';
			$res['content'] = $question['content'];
			$res['body'] ='';
			$formbody[$i] = $res;
			++$i;
			continue;
		}
		if($has_choices[$tid]) {
			$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='$qid' AND content NOT LIKE '!other%' ORDER BY id";
			$choices_result = mysql_query($sql);
			$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='$qid' AND content LIKE '!other%' ORDER BY id";
			$others_result = mysql_query($sql);
			$others = mysql_num_rows($others_result);
		} else {
			$choices_result = ''; 
		}
		$qname = "Q" . $qid;
		$res['number']='';
		if($question['required']=='Y') { 
			$res['number']='<font color="#FF0000">*</font>'; 
		}
		$res['number'] = $res['number'].'<A NAME="Q'.$qnum.'">'.$qnum.'</A>';
		$res['content'] = $question['content'];
		$res['body'] ='';
		//echo "qid[".$qid."]tid[".$tid."]value[".$defval[$qname]."]<BR>";
		switch($tid) {
			case '1':	// Yes/No
				$res['body'] = "<label>".mkradio($qname,'Y',$defval)." "._GT_Yes."</label><label>"
					.mkradio($qname,'N',$defval)." "._GT_No."</label>";//by makinosuke
				if ($xoopsModuleConfig['RESET_RB']) $res['body'] .= '&nbsp;<input class="bms-bttn-uncheck" type="button" name="Button" value="' . _MD_BMSURVEY_CHECKRESET . '" onclick="uncheckRadio(\''.$qname.'\')" />';//by makinosuke
				break;
			case '2':	// single text line
				$res['body'] = mktext($qname, $size, $prec,$defval);
				break;
			case '3':	// essay
				$res['body'] = mktextarea($qname, $prec, $size, 'VIRTUAL',$defval);
				break;
			case '4':	// radio
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {
					$res['body'] = $res['body']."<label>".mkradio($qname,$choice['id'])." ".$choice['content']."</label>";//by makinosuke
				}
				$j=0;
				while($other = mysql_fetch_array($others_result,MYSQL_ASSOC)) {
					$cid = $other['id'];
					$other_text = preg_replace(
							array("/^!other=/","/^!other/"),
							array('',_MD_QUESTION_OTHER),
							$other['content']);
					$res['body'] .= mkradio($qname,"other_$cid");
					$cid = "Q${qid}_${cid}";
					$res['body'] .= "$other_text <input type=\"text\" size=\"20\" name=\"$cid\" onKeyPress=\"other_check(this.name)\"";
					if (isset($GLOBALS['_POST'][$cid])) $res['body'] .= "value=\"". htmlspecialchars($GLOBALS['_POST'][$cid])."\"";
					$res['body'] .= ">";
					$j++;
				}
				if ($xoopsModuleConfig['RESET_RB']) $res['body'] .= '&nbsp;<input class="bms-bttn-uncheck" type="button" name="Button" value="' . _MD_BMSURVEY_CHECKRESET . '" onclick="uncheckRadio(\''.$qname.'\')" />';//by makinosuke
				break;
			case '5':	// check boxes
				if ( $prec==0 ) $res['content'] .= _MD_BMSURVEY_CHECKANY; else  $res['content'] .= sprintf(_MD_BMSURVEY_CHECKLIMIT,$prec);
				$num_rows = mysql_num_rows($choices_result); 
				$num_alt=0;
				if ($num_rows > 3) $num_alt = ceil( $num_rows / 2 );
				$num=0;
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {	
					if ( ($num_alt>0) && ($num==$num_alt) ){
					}
					$res['body'] = $res['body']."<label>".mkcheckbox($qname,$choice['id'])." ".$choice['content']."</label>";//by makinosuke
					$num++;
				}
				$j=0;
				while($other = mysql_fetch_array($others_result,MYSQL_ASSOC)) {
					$cid = $other['id'];
					$other_text = preg_replace(
							array("/^!other=/","/^!other/"),
							array('',_MD_QUESTION_OTHER),
							$other['content']);
							$res['body'] = $res['body'].mkcheckbox($qname,"other_$cid");
                    $cid = "Q${qid}_${cid}";
                    $res['body'] = $res['body'].
                    	"$other_text <input type=\"text\" size=\"20\" name=\"$cid\" onKeyPress=\"other_check(this.name)\"";
                	if (isset($GLOBALS['_POST'][$cid]))
                        $res['body'] = $res['body'].' value="'. htmlspecialchars($GLOBALS['_POST'][$cid]) .'"';
					$res['body'] .= ">";
					$j++;
				}
				break;
			case '6':	// dropdown box
				$options = array();
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {
					$options[$choice['id']] = $choice['content'];
				}
				$res['body'] = mkselect($qname,$options,$defval);
				break;
			case '7':	// rating
				$res['body'] = '<table border="0" cellspacing="0" cellpadding="0"><tr>';
				$res['body'] = $res['body'].'<td width="60">'.mkradio($qname,1)." "._GT_1.'</td>';
				$res['body'] = $res['body'].'<td width="60">'.mkradio($qname,2)." "._GT_2.'</td>';
				$res['body'] = $res['body'].'<td width="60">'.mkradio($qname,3)." "._GT_3.'</td>';
				$res['body'] = $res['body'].'<td width="60">'.mkradio($qname,4)." "._GT_4.'</td>';
				$res['body'] = $res['body'].'<td width="60">'.mkradio($qname,5)." "._GT_5.'</td>';
				$res['body'] = $res['body'].'<td width="60">'.mkradio($qname,'N/A')." "._GT_N/A.'</td></tr></table>';
				break;
			case '8':	// ranking
				$patterns = "/\[TH](.*)\[\/TH\]/sU";
				$replacements = '';
				$res['content'] = preg_replace($patterns, $replacements, $question['content']);
				if (preg_match($patterns, $question['content'], $thstr))
					$th_title = explode( "," , $thstr[1] );
				else
					$th_title = array(1,2,3,4,5);
				$bg = '#eeeeee';	// bgcolor
				$res['body'] = '<table border="0" cellspacing="1" cellpadding="0"><tr><td width="20%"></td>';
				for ($j = 0; $j < $size; $j++) {
					$res['body'] = $res['body'].
						'<td width="16%" align="center" bgcolor="'.$bg.'">'.$th_title[$j].'</td>';	//number_format($j+1)
					if ($bg == '#eeeeee')   $bg = '#dddddd';
					else                    $bg = '#eeeeee';
				}
				if ($prec) {
					$res['body'] = $res['body'].
						'<td width="20%" align="center" bgcolor="'.$bg.'">'._GT_N/A.'</td>';
				}
				$res['body'] = $res['body'].'</tr>';
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {
					$cid = $choice['id'];
					$str = "Q${qid}_$cid";
					
					$res['body'] .= '<tr><td width="20%" align="right">'.$choice['content'];
					if ($xoopsModuleConfig['RESET_RB'])
						$res['body'] .= '&nbsp;<input type="button" name="Button" value="' . _MD_BMSURVEY_CHECKRESET . '" onclick="uncheckRadio(\''.$str.'\')" />';//by makinosuke
					$res['body'] .= '</td>';
					$bg = '#eeeeee';
					for ($j = 0; $j < $size; $j++) {
						$res['body'] = $res['body'].
							'<td width="16%" align="center" bgcolor="'.$bg.'">'.mkradio($str,$j).'</td>';
						if ($bg == '#eeeeee')   $bg = '#dddddd';
						else                    $bg = '#eeeeee';
					}
					if ($prec) {
						$res['body'] = $res['body'].
							'<td width="20%" align="center" bgcolor="'.$bg.'">'.mkradio($str,'N/A').'</td>';
					}
					$res['body'] = $res['body']."</tr>";
				}
				$res['body'] = $res['body']."</table>";
				break;
			case '9':	// date
				$varr[$qname] = date(_SHORTDATESTRING, time());
				$res['body'] = mktext($qname, 10, 10,$varr,$defval).'<em>(e.g. '.date(_SHORTDATESTRING, time()).')</em>';
				break;
			case '10':	// numeric
				$size++; // for sign
				if($prec) $size += 1 + $prec;
				$res['body'] = mktext($qname, $size, $size,$defval);
				break;
			case '40':	// Attache
				$res['body'] = $res['body']."<INPUT TYPE='hidden' NAME='MAX_FILE_SIZE' VALUE='".$maxfilesize."'>".
					"<INPUT type='file' size='".$size."' name='".$qname."' /> Max ".maxbyte_str($maxfilesize)."Byte.";
				break;
		}
		// end of select
		$formbody[$i] = $res;
		$i++; $qnum++;
	}
	// end of questions
	if($num_sections>1) {
		$res['body'] = $res['body'].'<font size="-1" class="surveyPage">'._GT_Page
			.number_format($section+1)._GT_of.number_format($num_sections);
	}
	return $formbody;
}
/*
** For Preview
*/
function survey_render($sid, $section = 1, $message = '') {
	global $_POST,$maxfilesize;
	@reset($_POST);

	if(empty($section))
		$section = 1;

	$has_choices = esp_type_has_choices();

// load survey title (and other globals)
	$sql = "SELECT * FROM ".TABLE_SURVEY." WHERE id='${sid}'";
	$result = mysql_query($sql);
	if(mysql_num_rows($result) != 1)
		return(false);
	$survey = mysql_fetch_array($result,MYSQL_ASSOC);
	mysql_free_result($result);

	$sections = survey_get_sections($sid);
	$num_sections = count($sections);
	if($section-- > $num_sections)
		return(false);	// invalid section

    $section_questions = survey_get_section_questions($sid);

// load survey questions
	$sec_sql = array_to_insql($sections[$section]);
	$sql = "SELECT * FROM ".TABLE_QUESTION." WHERE id $sec_sql ORDER BY position,id";
	$questions_result = mysql_query($sql);
	if(mysql_num_rows($questions_result) < 1)
		return(false);

// check to see if there are required questions
	$sql = "SELECT COUNT(*) FROM ".TABLE_QUESTION." WHERE id $sec_sql AND required='Y'";
	$has_required = mysql_result(mysql_query($sql),0,0);

// find out what question number we are on $i
	$i=0;
	for($j=0;$j<$section;$j++)
		$i += count($section_questions[$j]);
?>
<table width="100%" class="outer" cellspacing="1" >
<tr><td class="image"></td></tr>
<tr><th colspan="3" align="center">
<?php echo($survey["title"]); ?>
<?php echo("&nbsp;&nbsp;".$survey["subtitle"]); ?>
<?php if($num_sections>1) { ?>
	<font size="-1" class="surveyPage"><?php printf(_GT_Page_d_of_d, $section+1, $num_sections); ?></font>
<?php } ?>
</th></tr>
<?php 
	if ($survey["info"] || $message){
		echo ('<tr><td colspan="3">');
		echo($survey["info"]."&nbsp;".$message);
		echo ('</td></tr>');
	}
?>
<tr><td class="footer" colspan="3">
<?php
	if($has_required) {
    	echo(_MD_ASTERISK_REQUIRED);
	}
?>
</td></tr>
<?php
	while($question = mysql_fetch_array($questions_result,MYSQL_ASSOC)) {
		// process each question
		$qid  = &$question['id'];
		$tid  = &$question['type_id'];
		$size = &$question['length'];
		$prec = &$question['precise'];

		if ($tid == 100) {
?>
<!--<tr>
    <td class="preQuestionBorder">&nbsp;</td>
</tr>-->
<tr>
    <td colspan=3 height="18" vAlign="top">
        <table class="qnOuterTable" width="100%" border="0" cellpadding="1" cellspacing="1">
        <tr>
            <td width="100%" height="18" valign="top">
                <table class="qnInnerTable" width="100%" border="0" cellpadding="0" cellspacing="0">
                <tr>
        	        <td class="addInfo" width="100%" height="18">&nbsp;
        	            <?php echo $question['content']; ?>
        	        </td>
                    </tr>
                </table>
            </td>
        </tr>
        </table>
    </td>
</tr>
<?php

			continue;
		}

		++$i;
		if($has_choices[$tid]) {
			$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='$qid' AND content NOT LIKE '!other%' ORDER BY id";
			$choices_result = mysql_query($sql);
			$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='$qid' AND content LIKE '!other%' ORDER BY id";
			$others_result = mysql_query($sql);
			$others = mysql_num_rows($others_result);
		} else { $choices_result = ''; }
?>
<tbody>
                 <tr>
                 <td class="even" align="right" width="5">
			<?php
			if($question['required']=='Y') { 
				echo('<font color="#FF0000">*</font>'); 
			}
			echo($i);
			?><td class="even" align="left">
				<?php echo($question['content']); ?></td>
				<!-- </tr><tr> -->
				<td class="odd" align="left">
				<!--<blockquote>-->
<?php
		switch($tid) {
			case '1':	// Yes/No
?>
				<table cellSpacing="0" cellPadding="0">
                        	<tbody>
                          	<tr>
                            		<td><?php echo(mkradio($qid,'Y')); ?></td>
                            		<td><?php echo(_GT_Yes); ?></td>
                          	</tr>
                          	<tr>
                            		<td><?php echo(mkradio($qid,'N')); ?></td>
                            		<td><?php echo(_GT_No); ?></td>
                          	</tr>
                        	</tbody>
                      		</table>
<?php
				break;
			case '2':	// single text line
?>
					<?php echo(mktext($qid, $size, $prec)); ?>
<?php
				break;
			case '3':	// essay
?>
					<?php echo(mktextarea($qid, $prec, $size, 'VIRTUAL')); ?>
<?php
				break;
			case '4':	// radio
?>
					<table border="0" cellspacing="0" cellpadding="0">
					<tbody>
<?php			while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {	?>
						<!--<tr>-->
							<td><?php echo(mkradio($qid,$choice['id'])); ?></td>
							<td class="qnType"><?php echo($choice['content']); ?>&nbsp;</td>
						<!--</tr>-->
<?php			}
				$j=0;
				while($other = mysql_fetch_array($others_result,MYSQL_ASSOC)) {
					$cid = $other['id'];
					$other_text = preg_replace(
							array("/^!other=/","/^!other/"),
							array('',_MD_QUESTION_OTHER),
							$other['content']);
?>
						<tr>
							<td><?php echo(mkradio($qid,"other_$cid")); ?></td>
							<td class="qnType">
<?php
                    $cid = "${qid}_${cid}";
                    echo("$other_text&nbsp;<input type=\"text\" size=\"20\" name=\"$cid\" onKeyPress=\"other_check(this.name)\"");
                	if (isset($GLOBALS['_POST'][$cid]))
                        echo(' value="'. htmlspecialchars($GLOBALS['_POST'][$cid]) .'"');
                    echo(" />");
?>
                            </td>
						</tr>
<?php
					$j++;
				}
?>
					</tbody>
					</table>
<?php
				break;
			case '5':	// check boxes
?>
					<table border="0" cellspacing="0" cellpadding="0">
					<tbody><tr>
<?php			$num_rows = mysql_num_rows($choices_result); 
				$num_alt=0;
				if ($num_rows > 3) $num_alt = ceil( $num_rows / 2 );
				$num=0;
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {	
					if ( ($num_alt>0) && ($num==$num_alt) ){
//						?></tr><tr><?php
					}
					echo(mkcheckbox($qid,$choice['id'])); 
					echo("&nbsp;".$choice['content']);
					$num++;
				}
				$j=0;
				while($other = mysql_fetch_array($others_result,MYSQL_ASSOC)) {
					$cid = $other['id'];
					$other_text = preg_replace(
							array("/^!other=/","/^!other/"),
							array('',_MD_QUESTION_OTHER),
							$other['content']);
?>
						<!--<tr>-->
							<td><?php echo(mkcheckbox($qid,"other_$cid")); ?></td>
							<td class="qnType">
<?php
                    $cid = "${qid}_${cid}";
                    echo("$other_text&nbsp;<input type=\"text\" size=\"20\" name=\"$cid\" onKeyPress=\"other_check(this.name)\"");
                	if (isset($GLOBALS['_POST'][$cid]))
                        echo(' value="'. htmlspecialchars($GLOBALS['_POST'][$cid]) .'"');
                    echo(" />");
?>
                            </td>

						<!--</tr>-->
<?php
					$j++;
				}
?>
					</tr>
					</tbody>
					</table>
<?php
				break;
			case '6':	// dropdown box
				$options = array();
?>
<?php				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {
					$options[$choice['id']] = $choice['content'];
				}
?>
					<?php echo(mkselect($qid,$options)); ?>
<?php
				break;
			case '7':	// rating
?>
					<table border="0" cellspacing="0" cellpadding="0">
					<tbody>
						<tr>
							<td width="60"><?php echo(mkradio($qid,1)); ?> <?php echo(_GT_1); ?></td>
							<td width="60"><?php echo(mkradio($qid,2)); ?> <?php echo(_GT_2); ?></td>
							<td width="60"><?php echo(mkradio($qid,3)); ?> <?php echo(_GT_3); ?></td>
							<td width="60"><?php echo(mkradio($qid,4)); ?> <?php echo(_GT_4); ?></td>
							<td width="60"><?php echo(mkradio($qid,5)); ?> <?php echo(_GT_5); ?></td>
							<td width="60"><?php echo(mkradio($qid,'N/A')); ?> <?php echo(_GT_N/A); ?></td>
						</tr>
					</tbody>
					</table>
<?php
				break;
			case '8':	// ranking
?>
					<table border="0" cellspacing="1" cellpadding="0">
						<tbody>
						<tr>
							<td></td>
<?php
				$bg = '#eeeeee';
				for ($j = 0; $j < $size; $j++) {
?>
							<td width="40" align="center" bgcolor="<?php echo($bg); ?>"><?php echo($j+1); ?></td>
<?php
					if ($bg == '#eeeeee')   $bg = '#dddddd';
					else                    $bg = '#eeeeee';
				}
				if ($prec) {
?>
							<td width="40" align="center" bgcolor="<?php echo($bg); ?>"><?php echo(_GT_N/A); ?></td>
<?php
				}
?>
						</tr>
<?php
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {
					$cid = $choice['id'];
					$str = "${qid}_$cid";
?>
						<tr>
							<td align="right"><?php echo($choice['content']); ?></td>
<?php
					$bg = '#eeeeee';
					for ($j = 0; $j < $size; $j++) {
?>
							<td width="40" align="center" bgcolor="<?php echo($bg); ?>"><?php echo(mkradio($str,$j)); ?></td>
<?php
						if ($bg == '#eeeeee')   $bg = '#dddddd';
						else                    $bg = '#eeeeee';
					}
					if ($prec) {
?>
							<td width="40" align="center" bgcolor="<?php echo($bg); ?>"><?php echo(mkradio($str,'N/A')); ?></td>
<?php
					}
?>
						</tr>
<?php			} ?>
					</tbody>
					</table>
<?php
				break;
			case '9':	// date
				echo mktext($qid, 10, 10).'  <em>(e.g. '.date(_SHORTDATESTRING, time()).')</em>';
				break;
			case '10':	// numeric
					$size++; // for sign
					if($prec)
						$size += 1 + $prec;
?>
					<?php echo(mktext($qid, $size, $size)); ?>
<?php
				break;
			case '40':	// Attache
				//echo(mktext($qid, $size, $prec));
				echo "<INPUT TYPE='hidden' NAME='MAX_FILE_SIZE' VALUE='".$maxfilesize."'>";
				echo "<INPUT type='file' size='".$size."' name='".$qid."' /> Max ".maxbyte_str($maxfilesize)."Byte.";
				break;
		}
		// end of select
?>
        </td></tr></tbody>
<?php
	}
	// end of questions
?>
</table>
</div>
<?php if($num_sections>1) { ?>
	<font size="-1" class="surveyPage"><?php echo(_GT_Page); ?>
	<?php echo($section+1); ?>
	<?php echo(_GT_of); ?>
	<?php echo($num_sections); ?></font><br>
<?php } ?>
<?php
	return;
}
/******************************************************************************
** For email
******************************************************************************/
function survey_render_email($sid, $section = 1, $message = '', $hide_question = 0) {
	global $_POST,$maxfilesize;
	@reset($_POST);
	$body='';

	if(empty($section)) $section = 1;

	$has_choices = esp_type_has_choices();

// load survey title (and other globals)
	$sql = "SELECT * FROM ".TABLE_SURVEY." WHERE id='${sid}'";
	$result = mysql_query($sql);
	if(mysql_num_rows($result) != 1)
		return(false);
	$survey = mysql_fetch_array($result,MYSQL_ASSOC);
	mysql_free_result($result);

	$sections = survey_get_sections($sid);
	$num_sections = count($sections);
	if($section-- > $num_sections)
		return(false);	// invalid section

    $section_questions = survey_get_section_questions($sid);

// load survey questions
	$sec_sql = array_to_insql($sections[$section]);
	$sql = "SELECT * FROM ".TABLE_QUESTION." WHERE id $sec_sql ORDER BY position,id";
	$questions_result = mysql_query($sql);
	if(mysql_num_rows($questions_result) < 1)
		return(false);

// check to see if there are required questions
	$sql = "SELECT COUNT(*) FROM ".TABLE_QUESTION." WHERE id $sec_sql AND required='Y'";
	$has_required = mysql_result(mysql_query($sql),0,0);

// find out what question number we are on $i
	$i=0;
	for($j=0;$j<$section;$j++) $i += count($section_questions[$j]);
	$title = $survey["title"];
	if ($survey["subtitle"]){
		$title .=" ".$survey["subtitle"];
	}
	if ($survey["info"] || $message){
		$title .= "\n".$survey["info"]." ".$message;
	}
/*
	if($has_required) {
	   	$title .= "\n"._MD_ASTERISK_REQUIRED;
	}
*/
	$body = "";
	//$i = $qnum = 1;
	while($question = mysql_fetch_array($questions_result,MYSQL_ASSOC)) {
		// process each question
		$qnum = &$question['position'];
		$qid  = &$question['id'];
		$tid  = &$question['type_id'];
		$size = &$question['length'];
		$prec = &$question['precise'];
		if ($tid == 100) {
			$body.= "// " . $question['content'] . "\r\n";
			continue;
		}
		++$i;
		if($has_choices[$tid]) {
			$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='$qid' AND content NOT LIKE '!other%' ORDER BY id";
			$choices_result = mysql_query($sql);
			$sql = "SELECT * FROM ".TABLE_QUESTION_CHOICE." WHERE question_id='$qid' AND content LIKE '!other%' ORDER BY id";
			$others_result = mysql_query($sql);
			$others = mysql_num_rows($others_result);
		} else {
			$choices_result = '';
		}
		if($question['required']=='Y') { 
			$body.='[*]'; 
		}
		if ($hide_question==0)
			$content = $question['content'];
		else
			$content = '';
		//'Q'.  $qnum + 1 . ':' . $question['content'];	//$qid 
		$body .= sprintf("Q%u:%s",$qnum+1 ,$content);
		switch($tid) {
			case '1':	// Yes/No
				$body.=" 1.()Yes";
				$body.=" 2.()No";
				break;
			case '2':	// single text line
				$body .= sprintf(" []M%uL%u",$size,$prec);
				break;
			case '3':	// essay
				$body .= sprintf(" []R%uC%u",$size,$prec);
				break;
			case '4':	// radio
				$j=1;
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {
					$body.=' '.$choice['id'].'.()';	//(mkradio($qid,$choice['id']));
					$body.=$choice['content'];
					$j++;
				}
				$j=0;
				while($other = mysql_fetch_array($others_result,MYSQL_ASSOC)) {
					$cid = $other['id'];
					$other_text = preg_replace(
						array("/^!other=/","/^!other/"),
						array('',_MD_QUESTION_OTHER),
						$other['content']);	//'Other:'
					$body.= "\r\n   o_${cid}:${other_text}[]";	//${qid}
//					$body.=mkradio($qid,"other_$cid");
//                    $cid = "${qid}_${cid}";
                    //$body.="$other_text <input type=\"text\" size=\"20\" name=\"$cid\" onKeyPress=\"other_check(this.name)\"";
//                	if (isset($GLOBALS['_POST'][$cid]))
//                        $body.=(' value="'. htmlspecialchars($GLOBALS['_POST'][$cid]) .'"');
//                    $body.=(" />");
					$j++;
				}
				break;
			case '5':	// check boxes
				$num=0;
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {	
					$num++;
					//$body.=(mkcheckbox($qid,$choice['id'])); 
					$body.=' '.$choice['id'].'.[]';
					$body.=$choice['content'];
				}
				$j=0;
				while($other = mysql_fetch_array($others_result,MYSQL_ASSOC)) {
					$cid = $other['id'];
					$other_text = preg_replace(
						array("/^!other=/","/^!other/"),
						array('',_MD_QUESTION_OTHER),
					$other['content']);
					$body.= "\r\n   o_${cid}:${other_text}[]";	//${qid}
/*
					$body.=(mkcheckbox($qid,"other_$cid"));
                    $cid = "${qid}_${cid}";
                    $body.=("$other_text <input type=\"text\" size=\"20\" name=\"$cid\" onKeyPress=\"other_check(this.name)\"");
                	if (isset($GLOBALS['_POST'][$cid]))
                        $body.=(' value="'. htmlspecialchars($GLOBALS['_POST'][$cid]) .'"');
                    $body.=(" />");
*/
					$j++;
				}
				break;
			case '6':	// dropdown box
				$options = array();
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {
//					$options[$choice['id']] = $choice['content'];
					$body.= ' '.$choice['id'].'.()';
					$body.= $choice['content'];
				}
				//$body.=(mkselect($qid,$options));
				break;
			case '7':	// rating
				$body.='[]1[]2[]3[]4[]5';
				$body.=('N/A');
				break;
			case '8':	// ranking
				while($choice = mysql_fetch_array($choices_result,MYSQL_ASSOC)) {
					$cid = $choice['id'];
					$body.="\r\n${qid}_${cid}:";
					$body.=$choice['content'];
					if ($prec) {
						$body .= 'N/A';
					}else{
						for ($j = 0; $j < $size; $j++) {
							$body .= sprintf(' %u.()',$j+1);
						}
					}
				}
				break;
			case '9':	// date
				$body .= sprintf(" [%s]M%uL%uN",date(_SHORTDATESTRING, time()),$size,$prec);
				break;
			case '10':	// numeric
				$body .= sprintf(" []M%uL%uN",$size,$prec);
				break;
			case '40':	// Attache
				//$body.=(mktext($qid, $size, $prec));
				$body.= "<INPUT TYPE='hidden' NAME='MAX_FILE_SIZE' VALUE='".$maxfilesize."'>";
				$body.= "<INPUT type='file' size='".$size."' name='".$qid."' /> Max ".maxbyte_str($maxfilesize)."Byte.";
				break;
		}
		// end of select
		$body .= "\r\n";
	}
	// end of questions
	$myts =& MyTextSanitizer::getInstance();
	$ret['title']=$title;
	$ret['body']=$myts->stripSlashesGPC($body);
	return $ret;
	//return $body;
}
?>