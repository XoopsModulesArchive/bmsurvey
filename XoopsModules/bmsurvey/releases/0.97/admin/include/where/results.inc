<?php

# $Id: results.inc,v 1.1.1.1 2005/08/10 12:14:03 yoshis Exp $

// Written by James Flemer
// For eGrad2000.com
// <jflemer@alum.rpi.edu>

	$bg = '';
	require ($ESPCONFIG['include_path']."where/crosstab.inc");
	// see if a survey has been selected
	$sid = '';
	if (!empty($_POST['sid']))
		$sid = intval($_POST['sid']);
	elseif (!empty($_GET['sid']))
		$sid = intval($_GET['sid']);
    
    if (!empty($sid)) {
		$srealm = auth_get_survey_realm($sid);
		if(!empty($_GET['test'])) {
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
			/* check ACL to see if user is allowed to get results to
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
if (isset($_GET['type'])) {
	if ($_GET['type'] == "cross") {
		echo('<h2>'. _GT_Cross_Tabulation ."</h2>\n");
	}
}
?>
<font size="+2">
<a href="<?php echo($GLOBALS['ESPCONFIG']['ME']."?where=help"); ?>" target="_blank">Help</a>
</font>
<br /><br />
<table border="0" cellpadding="4" cellspacing="0" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
<tr><td>
<?php
		$precision = '';
		if(!empty($_GET['precision']))
			$precision = intval($_GET['precision']);
		$totals = '';
		if(!empty($_GET['totals']))
			$totals = intval($_GET['totals']);
		$qid = '';
        $cids = '';
        $rid = '';
        $qidc = '';
        $qidr = '';
        $guicross = '';
        $deny = '';
        $defset = '';
		if(!empty($_GET['qid']))
			$qid = intval($_GET['qid']);
        if(!empty($_GET['cids']))
            $cids = _addslashes($_GET['cids']);
	if (!empty($_POST['cids'])){
		$cids = array_map('_addslashes', $_POST['cids']);
	}
	if (!empty($_POST['qid']))
		$qid = intval($_POST['qid']);

	if (!empty($_POST['qidc']))
		$qidc = intval($_POST['qidc']);
	if (!empty($_POST['qidr']))
		$qidr = intval($_POST['qidr']);
		if(!empty($_POST['rid']))
			$rid = intval($_POST['rid']);
		elseif(!empty($_GET['rid']))
			$rid = intval($_GET['rid']);
		if(!empty($_GET['type']))
			$guicross = $_GET['type'];
		if(!empty($_GET['deny']))
			$deny = $_GET['deny'];
		if(!empty($_GET['defset']))
			$defset = $_GET['defset'];
		if(!empty($rid)) {
            $ret = survey_results($sid, $precision, 0, '', '', $rid, '', $deny, $defset);
		} else if (!empty($guicross)){
			$ret = survey_results($sid, $precision, $totals, '','','', $guicross);
		} else if (!empty($qidc) || !empty($qidr)){
			cross_tabulation($sid, $qidc, $qidr, $qid, $cids);
        	} else {
		    	$ret = survey_results($sid, $precision, $totals, $qid, $cids);
        	}
		if(!empty($ret))
			echo("<font color=\"". $ESPCONFIG['error_color'] ."\">$ret</font>\n");
?>
<?php
		if(!empty($_GET['test'])) {
			echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=test&sid=". $_GET['sid'] ."\">". _GT_Test_Survey ."</a><br>\n");
		}


        if (!empty($_GET['type'])) {
?>
</td></tr>
<tr><td align="center">
<input type="reset" value="<?php echo _GT_Reset; ?>" />
<?php echo mksubmit('cross', _GT_Cross_Tabulate); ?>
<br />
<?php
        }
?>
</td></tr></table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
<?php
		return;
	}

// No SurveyId specified, so build a table of
// surveys to choose from ...

	/* load names and titles of all surveys available to
	 * _this_ user */
	$statusok = (STATUS_ACTIVE | STATUS_DONE);
	$statusno = STATUS_DELETED;
	if($xoopsUser->isAdmin()) {
		$sql = "SELECT id,name,title,owner,realm FROM ".TABLE_SURVEY."
		WHERE NOT (status & $statusno) AND (status & $statusok) ORDER BY id DESC";
	} else {
		$realms = array_to_insql(
			array_intersect(
				$_SESSION['acl']['pall'],
				$_SESSION['acl']['pdata']));
		//print_r($realms);
		$sql = "SELECT id,name,title,owner,realm
			FROM ".TABLE_SURVEY." WHERE NOT (status & $statusno) AND (status & $statusok) AND (owner = '".
			$xoopsUser->uid() ."' || realm $realms) ORDER BY id DESC";
	}
	$result = mysql_query($sql);

    if (empty($_GET['type'])) {
?>
<h2><?php echo(_GT_View_Survey_Results); ?></h2>
<?php echo(_GT_Pick_Survey_to_View); ?>

<input type="hidden" name="rid" value="" />
<input type="hidden" name="sid" value="" />
<input type="hidden" name="where" value="results" />
<?php
    } else {
        echo('<h2>'. _GT_Cross_Tabulation ."</h2>\n");
        echo(_GT_Pick_Survey_to_Cross_Tabulate);
    }
?>
<table border="0" cellspacing="0" cellpadding="4" align="center" bgcolor="<?php echo($ESPCONFIG['active_bgcolor']); ?>" width="95%">
	<tr bgcolor="#dddddd">
		<th align="left"><?php echo(_GT_ID); ?></th>
		<?php if (empty($_GET['type'])) { ?>
			<th align="left"><?php echo(_GT_Respondent); ?></th>
		<?php }
		      else { ?>
			<th align="left"></th>
		<?php }?>
		<th align="left"><?php echo(_GT_Name); ?></th>
		<th align="left"><?php echo(_GT_Title); ?></th>
		<th align="left"><?php echo(_GT_Owner); ?></th>
		<th align="left"><?php echo(_GT_Group); ?></th>
		<th align="left"><?php echo(_GT_Resp); ?></th>
	</tr>
<?php
    $count = 0;
	while(list($sid,$name,$title,$owner,$realm) = mysql_fetch_row($result)) {	
		//let's see how many respondents we have for each survey.
		$sql2 = "SELECT count(*) FROM ".TABLE_RESPONSE." where survey_id = $sid";
		$result2 = mysql_query($sql2);
		list($resp) = mysql_fetch_row($result2);
    	mysql_free_result($result2);

		if($bg != $ESPCONFIG['bgalt_color1'])
			$bg = $ESPCONFIG['bgalt_color1'];
		else
			$bg = $ESPCONFIG['bgalt_color2'];
?>
	<tr bgcolor="<?php echo($bg); ?>">
        <td><?php echo($sid); ?></td>
<?php
	if (empty($_GET['type'])) {
        	// grab all the rid values for the particular survey
        	$sql = "SELECT R.id FROM ".TABLE_RESPONSE." R ".
                	"WHERE R.survey_id = '${sid}' AND R.complete = 'Y' ".
                	"ORDER BY R.id";
        	$rid_result = mysql_query($sql);
        	$count++;
	}
?>
        <td>
	<?php 
		if (empty($_GET['type'])) {
	?>
            <select name="rid<?php echo $count; ?>" onChange="javascript:this.form.rid.value=this.form.rid<?php echo $count; ?>.options[this.form.rid<?php echo $count; ?>.selectedIndex].value; this.form.sid.value=<?php echo $sid; ?>; this.form.submit(); return false;">
            <option value=""></option>
<?php
        $i = 0;
        while (list($rid) = mysql_fetch_row($rid_result)) {
?>
            <option value="<?php echo $rid; ?>"><?php echo ++$i; ?></option>
<?php
        }
?>
            </select> 
<?php
		}
?>
        </td>
		<td>
		<?php
		if (!empty($_GET['type'])){
		?>	
			<a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=results&sid=${sid}&type=cross"); ?>">
	<?php   }
		else {
	?>
			<a href="<?php echo("". $GLOBALS['ESPCONFIG']['ME'] ."?where=results&sid=${sid}"); ?>">
	<?php   } ?>
				<?php echo($name); ?>
			</a>
		</td>
		<td><?php echo($title); ?></td>
		<td><?php echo($owner); ?></td>
		<td><?php echo($realm); ?></td>
		<td><?php echo($resp); ?></td>
	</tr>
<?php
	}
	mysql_free_result($result);
?>
</table>
<?php echo("<a href=\"". $GLOBALS['ESPCONFIG']['ME'] ."?where=manage\">" . _GT_Go_back_to_Management_Interface . "</a>\n"); ?>
